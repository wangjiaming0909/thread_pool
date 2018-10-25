#ifndef _PIPE_H_
#define _PIPE_H_

#include "stage.h"
#include <deque>

class pipe{
private:
    std::mutex              _mutex;
    std::deque<stage>       _stages;
    int                     _active;

public:
    pipe(){}
    ~pipe(){}
    
};

#endif
