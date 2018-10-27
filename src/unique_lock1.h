template <typename _Mutex>
class unique_lock {
public:
    typedef _Mutex mutex_type;

    unique_lock() noexcept : _M_device(0), _M_owns(false) {}

    explicit unique_lock(mutex_type &__m)
        : _M_device(std::__addressof(__m)), _M_owns(false) {
        lock();
        _M_owns = true;
    }

    unique_lock(mutex_type &__m, defer_lock_t) noexcept
        : _M_device(std::__addressof(__m)), _M_owns(false) {}

    unique_lock(mutex_type &__m, try_to_lock_t)
        : _M_device(std::__addressof(__m)), _M_owns(_M_device->try_lock()) {}

    unique_lock(mutex_type &__m, adopt_lock_t)
        : _M_device(std::__addressof(__m)), _M_owns(true)
    { // XXX calling thread owns mutex }

        template <typename _Clock, typename _Duration>
        unique_lock(mutex_type & __m, const chrono::time_point<_Clock, _Duration> &__atime)
            : _M_device(std::__addressof(__m)), _M_owns(_M_device->try_lock_until(__atime)) {}

        template <typename _Rep, typename _Period>
        unique_lock(mutex_type & __m, const chrono::duration<_Rep, _Period> &__rtime)
            : _M_device(std::__addressof(__m)), _M_owns(_M_device->try_lock_for(__rtime)) {}

        ~unique_lock()
        {
            if (_M_owns)
                unlock();
        }

        unique_lock(const unique_lock &) = delete;
        unique_lock &operator=(const unique_lock &) = delete;

        unique_lock(unique_lock && __u) noexcept
            : _M_device(__u._M_device), _M_owns(__u._M_owns)
        {
            __u._M_device = 0;
            __u._M_owns = false;
        }

        unique_lock &operator=(unique_lock &&__u) noexcept
        {
            if (_M_owns)
                unlock();
            unique_lock(std::move(__u)).swap(*this);
            __u._M_device = 0;
            __u._M_owns = false;
            return *this;
        }

        void lock()
        {
            if (!_M_device)
                __throw_system_error(int(errc::operation_not_permitted));
            else if (_M_owns)
                __throw_system_error(int(errc::resource_deadlock_would_occur));
            else
            {
                _M_device->lock();
                _M_owns = true;
            }
        }

        bool try_lock()
        {
            if (!_M_device)
                __throw_system_error(int(errc::operation_not_permitted));
            else if (_M_owns)
                __throw_system_error(int(errc::resource_deadlock_would_occur));
            else
            {
                _M_owns = _M_device->try_lock();
                return _M_owns;
            }
        }

        template <typename _Clock, typename _Duration>
        bool try_lock_until(const chrono::time_point<_Clock, _Duration> &__atime)
        {
            if (!_M_device)
                __throw_system_error(int(errc::operation_not_permitted));
            else if (_M_owns)
                __throw_system_error(int(errc::resource_deadlock_would_occur));
            else
            {
                _M_owns = _M_device->try_lock_until(__atime);
                return _M_owns;
            }
        }

        template <typename _Rep, typename _Period>
        bool try_lock_for(const chrono::duration<_Rep, _Period> &__rtime)
        {
            if (!_M_device)
                __throw_system_error(int(errc::operation_not_permitted));
            else if (_M_owns)
                __throw_system_error(int(errc::resource_deadlock_would_occur));
            else
            {
                _M_owns = _M_device->try_lock_for(__rtime);
                return _M_owns;
            }
        }

        void unlock()
        {
            if (!_M_owns)
                __throw_system_error(int(errc::operation_not_permitted));
            else if (_M_device)
            {
                _M_device->unlock();
                _M_owns = false;
            }
        }

        void
            swap(unique_lock & __u) noexcept
        {
            std::swap(_M_device, __u._M_device);
            std::swap(_M_owns, __u._M_owns);
        }

        mutex_type *
        release() noexcept
        {
            mutex_type *__ret = _M_device;
            _M_device = 0;
            _M_owns = false;
            return __ret;
        }

        bool
        owns_lock() const noexcept
        {
            return _M_owns;
        }

        explicit operator bool() const noexcept
        {
            return owns_lock();
        }

        mutex_type *
        mutex() const noexcept
        {
            return _M_device;
        }

      private:
        mutex_type *_M_device;
        bool _M_owns; // XXX use atomic_bool
    };

    /// Swap overload for unique_lock objects.
    template <typename _Mutex>
    inline void
    swap(unique_lock<_Mutex> &__x, unique_lock<_Mutex> &__y) noexcept
    {
        __x.swap(__y);
    }

    template <typename _Lock>
    inline unique_lock<_Lock>
    __try_to_lock(_Lock &__l)
    {
        return unique_lock<_Lock>{__l, try_to_lock};
    }

    template <int _Idx, bool _Continue = true>
    struct __try_lock_impl
    {
        template <typename... _Lock>
        static void
        __do_try_lock(tuple<_Lock &...> &__locks, int &__idx)
        {
            __idx = _Idx;
            auto __lock = std::__try_to_lock(std::get<_Idx>(__locks));
            if (__lock.owns_lock())
            {
                constexpr bool __cont = _Idx + 2 < sizeof...(_Lock);
                using __try_locker = __try_lock_impl<_Idx + 1, __cont>;
                __try_locker::__do_try_lock(__locks, __idx);
                if (__idx == -1)
                    __lock.release();
            }
        }
    };

    template <int _Idx>
    struct __try_lock_impl<_Idx, false>
    {
        template <typename... _Lock>
        static void
        __do_try_lock(tuple<_Lock &...> &__locks, int &__idx)
        {
            __idx = _Idx;
            auto __lock = std::__try_to_lock(std::get<_Idx>(__locks));
            if (__lock.owns_lock())
            {
                __idx = -1;
                __lock.release();
            }
        }
    };
