#include <iostream>
#include <chrono>



/*
ratio<3600, 1>                hours
ratio<60, 1>                    minutes
ratio<1, 1>                      seconds
ratio<1, 1000>               microseconds
ratio<1, 1000000>         microseconds
ratio<1, 1000000000>    nanosecons
*/
 
int main()
{
    typedef std::chrono::duration<int, std::ratio<1, 100000000>> shakes;
    typedef std::chrono::duration<int, std::centi> jiffies;
    typedef std::chrono::duration<float, std::ratio<12096,10000>> microfortnights;
    typedef std::chrono::duration<float, std::ratio<3155,1000>> nanocenturies;
 
    std::chrono::seconds sec(1);
 
    std::cout << "1 second is:\n";
 
    std::cout << std::chrono::duration_cast<shakes>(sec).count()
              << " shakes\n";
    std::cout << std::chrono::duration_cast<jiffies>(sec).count()
              << " jiffies\n";
    std::cout << std::chrono::duration_cast<microfortnights>(sec).count()
              << " microfortnights\n";
    std::cout << std::chrono::duration_cast<nanocenturies>(sec).count()
              << " nanocenturies\n";



std::chrono::duration<int>twentySeconds(20);
std::chrono::duration<double,std::ratio<60>>halfAMinute(0.5);
std::chrono::duration<long,std::ratio<1,1000>> oneMillisecond(1);











}
