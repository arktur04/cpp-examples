#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <cstdio>
const int cnt = 100;
std::mutex mtx;
void foo(int thread_num)
{
    for(int i = 0; i < cnt; i++)
    {
        mtx.lock();
        std::cout << "thread " << thread_num << " says: Hello N" << i << std::endl;
        mtx.unlock();
        std::this_thread::yield();
    }
}

int main()
{
    std::ios::sync_with_stdio(false); // disable a sychronization with printf for performance reasons
    std::promise<void> p;
    auto sf = p.get_future().share();
    const int thread_cnt = 1000;
    std::vector<std::unique_ptr<std::thread>> threads(thread_cnt);
    for(int i = 0; i < thread_cnt; i++)
    {
        threads[i] = std::make_unique<std::thread>(std::thread([sf, i]{sf.wait(); foo(i);}));
    }
    std::cout << "starting threads..." << std::endl;
    p.set_value();
    for(auto &thread: threads)
    {
        thread->join();
    }
    std::cout << "threads completed." << std::endl;
    return 0;
}
