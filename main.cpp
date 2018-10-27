#include <iostream>
#include "src/thread_pool.h"
using namespace std;

void print(){
    cout << 123 << endl;
}

void default_cb(){
    cout << "default cb ... " << endl;
}

int main(){
    thread_pool pool{1};
    pool.add_task(print);
    pool.start();
    // pool.join();
}
