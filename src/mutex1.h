#include <pthread.h>

class __mutex_base
{
protected:
    typedef pthread_mutex_t			__native_type;

    __native_type  _M_mutex;

    __mutex_base() noexcept {
      pthread_mutex_init(&_M_mutex, nullptr);
    }

    ~__mutex_base() noexcept { pthread_mutex_destroy(&_M_mutex); }

    __mutex_base(const __mutex_base&) = delete;
    __mutex_base& operator=(const __mutex_base&) = delete;
};


class mutex : private __mutex_base
{
public:
    typedef __native_type* 			native_handle_type;

    mutex() noexcept = default;
    ~mutex() = default;

    mutex(const mutex&) = delete;//no copy constructor
    mutex& operator=(const mutex&) = delete; //no assign

    void lock() {
        int __e = pthread_mutex_lock(&_M_mutex);

        // EINVAL, EAGAIN, EBUSY, EINVAL, EDEADLK(may)
        // if (__e)
        // __throw_system_error(__e);
    }

    bool try_lock() noexcept {
      // XXX EINVAL, EAGAIN, EBUSY
      return !pthread_mutex_trylock(&_M_mutex);
    }

    void unlock() {
      // XXX EINVAL, EAGAIN, EPERM
      pthread_mutex_unlock(&_M_mutex);
    }

    native_handle_type native_handle() { return &_M_mutex; }
};