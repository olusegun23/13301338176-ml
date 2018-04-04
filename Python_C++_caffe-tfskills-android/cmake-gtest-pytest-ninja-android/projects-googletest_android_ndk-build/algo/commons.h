#ifndef COMMONS_H
#define COMMONS_H
#include <mutex>
#include<thread>
#include <functional>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <stdio.h>
#include <queue>
#include <functional>
#include <stdio.h>
#include <functional>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <atomic>
#include<cstring>
#include <cstdlib>
#include <mutex>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <assert.h>
#include <set>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <random>
#include <iomanip>
#include <cmath>
#include <typeinfo>
#include <stdio.h>
#include <sstream>


#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>

#include <functional>
#include <type_traits>


#define DEBUG(info)  std::cout << "\n debug::"<<info << "   -FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";

#define DEBUG_T(info,T)  std::cout << "\n debug::"<<info <<"  type["<<typeid(T).name()<<"]   -FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
#define DEBUG_T_V(info,T,value)  std::cout << "\n debug::"<<info <<"  type["<<typeid(T).name()<<"] value["<<value<< "] -FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";


/*#define DEBUG(info)
#define DEBUG_T(info,T)
#define DEBUG_T_V(info,T,value)
*/


 class utils
    {

    public:


        template<typename T>
        void dump_vector(std::vector<T> v)
        {
            std::cout << "\n";
            std::cout << "(";
            for (auto i : v) std::cout<<","<<i;
            std::cout << ")";
            std::cout << "\n";

        }


        template<typename T>
        void dump_map_function(std::string name,std::map<std::string,std::function<void(T)>> map)

        {
            std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
            std::cout << "\n<";
            for(auto const& item : map) {
                std::function<void(T)> f=item.second;

                std::cout << "  [" << item.first << ", " << typeid(f).name() << "]" << std::endl;
            };
            std::cout << ">\n";


        }




    };  // class utils





class BenchmarkSpeed
{
public:
    BenchmarkSpeed(const std::string &name) :
            mStart(std::chrono::high_resolution_clock::now()),
            mName(name)
    {}

    ~BenchmarkSpeed()
    {
        std::chrono::duration<double, std::milli> diff = std::chrono::high_resolution_clock::now() - mStart;
        std::cout << "...Benchmark" << mName << "' take " << diff.count() << " milliseconds" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    std::string mName;
};


#define BENCHMARKSPPED_START(functionname)  BenchmarkSpeed(functionname);
#define BENCHMARKSPPED_END(functionname);



class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable( const NonCopyable& );
    const NonCopyable& operator=( const NonCopyable& );
};



template <class T>
class Singleton : private NonCopyable {
public:
    template <typename... Args>
    static T& getInstance(Args&&... args) {

        /*call_once(
                get_once_flag(),
                [] (Args&&... args) {
                    instance_.reset(new T(std::forward<Args>(args)...));
                }, std::forward<Args>(args)...);
        */
        call_once(
          get_once_flag(),
          [&, args...] () {
            instance_.reset(new T(std::forward<Args>(args)...));
          });

        return *instance_.get();
    }





protected:
    explicit Singleton<T>() {}
    ~Singleton<T>() {}

private:
    static std::unique_ptr<T> instance_;
    static std::once_flag& get_once_flag() {
        static std::once_flag once_;
        return once_;
    }
};

template<class T> std::unique_ptr<T> Singleton<T>::instance_ = nullptr;





#endif
