#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

/*
 * you can think of-and use-a lock_guard<BankAccount> object as a permit. 
 * Owning a lock_guard<BankAccount> gives you rights to do certain things. 
 * The lock_guard<BankAccount> object should not be copied or aliased 
 * (it's not a transmissible permit).
 */
template <typename Lockable>
class strict_lock  {
public:
    typedef Lockable lockable_type;

    explicit strict_lock(lockable_type& obj) : obj_(obj) {
        obj.lock(); // locks on construction
    }

    strict_lock() = delete;
    strict_lock(strict_lock const&) = delete;
    strict_lock& operator=(strict_lock const&) = delete;

    ~strict_lock() { obj_.unlock(); } //  unlocks on destruction 

    bool owns_lock(mutex_type const* l) const noexcept // strict lockers specific function 
    {
      return l == &obj_;
    }

private:
    lockable_type& obj_;
};

BankAccount myAccount("John Doe", "123-45-6789");
strict_lock<BankAccount> myLock(myAccount); // ok

class BankAccount
: public basic_lockable_adapter<boost::mutex>
{
    int balance_;
public:
    void Deposit(int amount, strict_lock<BankAccount>&) {
        // Externally locked
        balance_ += amount;
    }
    void Deposit(int amount) {
        strict_lock<BankAccount> guard(*this); // Internally locked
        Deposit(amount, guard);
    }
    void Withdraw(int amount, strict_lock<BankAccount>&) {
        // Externally locked
        balance_ -= amount;
    }
    void Withdraw(int amount) {
        strict_lock<BankAccount> guard(*this); // Internally locked
        Withdraw(amount, guard);
    }
};

void ATMWithdrawal(BankAccount& acct, int sum) {
    strict_lock<BankAccount> guard(acct);
    acct.Withdraw(sum, guard);
    acct.Withdraw(2, guard);
}

class BankAccount {
: public basic_lockable_adapter<boost::mutex>
    int balance_;
public:
    void Deposit(int amount, strict_lock<BankAccount>& guard) {
        // Externally locked
        // 检查锁是不是上在正确的对象上
        if (!guard.owns_lock(*this))
            throw "Locking Error: Wrong Object Locked";
        balance_ += amount;
    }
// ...
};







































