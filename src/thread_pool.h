#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <functional>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <assert.h>

class thread_pool{
public:
    typedef std::function<void ()> task;
    thread_pool(size_t number_of_threads)
        : _n_of_threads(number_of_threads),
        _running(false),
        _mutex(),
        _unique_lock(_mutex),
        _has_task_cv(),
        _deque_full_cv(),
        _tasks(),
        _threads(number_of_threads){
    }
    //will we have multi threads to add_tasks?
    void add_task(const task& t){
        //lock the deque
        _unique_lock.lock();
        //put the check inside the lock??
        while(deque_full()){
            _deque_full_cv.wait(_unique_lock);
        }
        //if has space for the new task
        _tasks.push_back(t);
        //if the deque is empty, we need to notify one thread when we add one task
        _has_task_cv.notify_one();
        _unique_lock.unlock();
    }
    void start(){
        initialize();
    }
    void stop(){
        _unique_lock.lock();
        _running = false;
        _has_task_cv.notify_all();
        _deque_full_cv.notify_all();
        for(auto t : _threads){
            std::bind(&std::thread::join, t);
        }
    }
private:
    //allocate threads
    void initialize(){
        for(size_t i = 0; i < _n_of_threads; i++){
            //construct the new thread, let it go the default routine
            std::shared_ptr<std::thread> thread_ptr{new std::thread{std::bind(&thread_pool::default_routine, this)}};
            _threads.push_back(thread_ptr);
        }
    }
    //only invoked in the add_task 
    //so it will always be locked
    inline bool has_task(){
        return !_tasks.empty();
    }
    inline bool deque_full(){
        return _tasks.size() > _n_of_threads;
    }
    //to each thread
    void default_routine(){
        while(_running){
            _unique_lock.lock();
            //no task so wait
            while(!has_task()){
                _has_task_cv.wait(_unique_lock);//unlock and block
            }
            //someone added a task to the tasks
            task t = _tasks.front();
            _tasks.pop_front();
            _deque_full_cv.notify_one();
            _unique_lock.unlock();
            t();
        }
    }
private:
    size_t                                          _n_of_threads;
    bool                                            _running;
    std::mutex                                      _mutex;
    std::unique_lock<std::mutex>                    _unique_lock;
    std::condition_variable                         _has_task_cv;
    std::condition_variable                         _deque_full_cv;
    std::deque<task>                                _tasks;
    std::vector<std::shared_ptr<std::thread>>       _threads;
};

#endif // _THREAD_POOL_H_