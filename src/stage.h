#ifndef STAGE_H
#define STAGE_H
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <functional>

class stage{
public:
    //Task type
    using Task = std::function<void ()>;
    //if you want to pass one task into the constructor with parameters
    //you have to use bind
    //one stage has one thread doing one task
    stage(Task task){
               
    }
    //run the task
    void run(){
        _thread_p = std::shared_ptr<std::thread>(new std::thread(_task));
    }
    ~stage(){}
private:
    std::shared_ptr<std::mutex>                  	_mutex_p;
   	std::shared_ptr<std::condition_variable>     	_available_p;
    std::shared_ptr<std::condition_variable>     	_ready_p;
    std::shared_ptr<std::thread>                 	_thread_p;
    Task                                            _task;
    void*                       					_data_ptr;
};


#endif /* STAGE_H */

