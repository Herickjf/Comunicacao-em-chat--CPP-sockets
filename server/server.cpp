#include <iostream>
#include <thread>
#include <vector>
#include "../libs/libtslog.hpp"

using namespace std;

void threadFunc(int warning_type){
    string msg = "Thread " + to_string(warning_type);

    switch (warning_type)
    {
    case 1:
        LOG_INFO(msg);
        break;
    case 2:
        LOG_ERROR(msg);
        break;
    case 3:
        LOG_SUCCESS(msg);
        break;
    case 4:
        LOG_DEBUG(msg);
        break;
    case 5:
        LOG_WARN(msg);
        break;
    }
}

int main(){
    vector<thread> threads;

    for(int i = 1; i <= 5; i++){
        threads.emplace_back(threadFunc, i);
    }

    for(auto &thread: threads){
        thread.join();
    }

    LOG_INFO("Todas as threads finalizadas com sucesso!");

    return 0;
}