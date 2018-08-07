template <typename T>
class Singleton
{
public:
  static T& instance()
  {
    if (!t_) { 
      std::unique_lock lck1(mutex1_);
      if (!t_) {
       	T tmp = new T;
        // 1. malloc
        // 2. constructor
        // 3. assignment
      }
    }
    return *t_;
  }

private
  static T* t_;
  static std::mutex	mutex1_;
};

template <typename T>
T* Singleton<T>::t_ = nullptr;
