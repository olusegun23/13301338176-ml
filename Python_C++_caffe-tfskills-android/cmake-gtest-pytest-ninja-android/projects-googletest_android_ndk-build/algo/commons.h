#ifndef CMC_COMMONS_H
#define CMC_COMMONS_H

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




#endif
