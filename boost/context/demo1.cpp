#include <array>
#include <functional>

#include <boost/context/all.hpp>

class Coroutine {
public:
    Coroutine() :
        my_context(boost::context::make_fcontext(
            stack.data() + stack.size(),
            stack.size(),
            Coroutine::dispatch)) // 指定跳转(context switch)回调函数
    {}
    virtual ~Coroutine() {}

    void operator()() { // 最后一个参数相当于附带参数指针，类似于 pthread 线程函数参数
        boost::context::jump_fcontext(&yield_context, my_context, reinterpret_cast<intptr_t>(this));
    }

protected:
    void yield() {
        boost::context::jump_fcontext(my_context, &yield_context, 0);
    }

    virtual void call() = 0;

private:
    static void dispatch(intptr_t coroutine_ptr) {
        Coroutine *coroutine = reinterpret_cast<Coroutine *>(coroutine_ptr);
        coroutine->call();
        while (true) coroutine->yield();
    }

private:
    boost::context::fcontext_t    *my_context;
    boost::context::fcontext_t    yield_context; // 跳转到的context，鬼知道是什么，相当于临时变量，由系统决定
    std::array<intptr_t, 64*1024> stack;
};

struct A : public Coroutine {
    void call() {
        std::cerr << "A went to the store one day.\n";
        yield();
        std::cerr << "A was looking for groceries.\n";
        yield();
        std::cerr << "A finally found what she was looking for.\n";
    }
};

struct B : public Coroutine {
    void call() {
        std::cerr << "B went to the store one day.\n";
        yield();
        std::cerr << "B was looking for replacement tires.\n";
        yield();
        std::cerr << "B didn't find anything at all.\n";
        yield();
        std::cerr << "B went to another store.\n";
        yield();
        std::cerr << "B got the tires installed there.\n";
    }
};

struct C : public Coroutine {
    void call() {
        std::cerr << "C went to the store one day.\n";
        yield();
        std::cerr << "C was looking for USB drives.\n";
        yield();
        std::cerr << "C found several with competitive pricing.\n";
        yield();
        std::cerr << "C couldn't decide which to buy, so gave up.\n";
    }
};


int main() {
    std::cerr << "So, this is what happened.\n";
    A a;
    B b;
    C c;
    for (size_t i=0; i<10; ++i) {
        a();
        b();
        c();
    }
    std::cerr << "Then it all was done.\n";
}

// 都是在main线程中执行
// context 一个线程里执行多个任务, 实现时间片轮转，不是执行完一个再执行下一个
/*
 * So, this is what happened.
 * A went to the store one day.
 * B went to the store one day.
 * C went to the store one day.
 * A was looking for groceries.
 * B was looking for replacement tires.
 * C was looking for USB drives.
 * A finally found what she was looking for.
 * B didn't find anything at all.
 * C found several with competitive pricing.
 * B went to another store.
 * C couldn't decide which to buy, so gave up.
 * B got the tires installed there.
 * Then it all was done.
 */
