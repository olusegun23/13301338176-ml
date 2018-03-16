#include <thread>
#include <chrono>
#include <iostream>
template <typename Duration, typename Function>
void timer(Duration const & d, Function const & f)
{
    std::thread([d,f](){
        std::this_thread::sleep_for(d);
        f();
    }).detach();
}



void hello() {std::cout << "Hello!\n";}

int main()
{
    timer(std::chrono::seconds(5), &hello);
    std::cout << "Launched\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
}
