#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void print(const char *str, std::chrono::microseconds delay)
{
    while (*str)
    {
        std::cout << *str << std::endl;
        ++str;
        std::this_thread::sleep_for(delay);
    }
}

void doubleAndAppend(int number, std::string &result)
{
    result += std::to_string(number * 2);
}

int main()
{
    using namespace std::chrono_literals;

    std::string result;

    /*std::thread t{print, "Hello, World", 500ms};
    std::thread t2{print, "Cau kamo", 500ms};
    t.join();
    t2.join();*/

    std::thread thread1{doubleAndAppend, 50, std::ref(result)};
    thread1.join();
}
