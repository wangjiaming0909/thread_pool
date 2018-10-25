#ifndef STAGE_H
#define STAGE_H
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>

class stage{
public:
    typedef void*(Func)(void *);
    stage(Func func){
		_thread_p = new std::thread(func);
    }
    ~stage(){}
private:
    std::shared_ptr<std::mutex>                  	_mutex_p;
   	std::shared_ptr<std::condition_variable>     	_available_p;
    std::shared_ptr<std::condition_variable>     	_ready_p;
    std::shared_ptr<std::thread>                 	_thread_p;
    void*                       					_data_ptr;
};


#endif /* STAGE_H */

