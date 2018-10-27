#include <iostream>
#include "src/thread_pool.h"
#include <string>
using namespace std;

// void print(){
//     cout << 123 << endl;
// }
void print(int a, string s){
    cout << "thread id: " << this_thread::get_id() << " " << a << " " << s << endl;
}

void default_cb(){
    cout << "default cb ... " << endl;
}

int main(){
    thread_pool pool{5};
    pool.add_task(std::bind(print, 1, "asd"));
    // pool.add_task(default_cb);
    pool.start();
    string s;
    while(1){
        cin >> s;
        if(s == "q")
            exit(0);
        else if(s == "add"){
            for(int i = 0; i < 10; i++)
                pool.add_task(bind(print, i, "add"));
        }else
            cout << s << endl;
    }
}
