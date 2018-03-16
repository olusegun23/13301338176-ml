
#ifndef QROBOT_UTILS_H
#define QROBOT_UTILS_H

#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>


#include "QROBOT_QUEUE_MT_H.hpp"


namespace qrobot

{








class utils 
{

public:


//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}




template<typename T>
void dump_vector_of_vector(std::vector <std::vector<T>> map2d )
{
        std::cout << "\n";
        std::cout << "class  qrobot_msg_type { public: int frameid .........  }";
        std::cout << "\n";
        std::cout << "{";
        std::cout << "\n";
	for (int i=0;i<map2d.size();i++){
		std::cout << "\n[";
		for (int j=0;j<map2d[i].size();j++){
			std::cout<<map2d[i][j]<<", ";
		};
		std::cout << "]\n";
	};
        std::cout << "\n";
        std::cout << "}\n";
	return;
}



template<typename T>
void dump_snapshot_queue_mt(std::queue<T> q)
{
        std::cout << "\n";
        std::cout << "class  qrobot_msg_type1 { public: int frameid; std::string name; image[8888]; int len=640*480; }";
        std::cout << "\n";
        std::cout << "{";
        std::cout << "\n";
        while (!q.empty()) {
        std::cout << "(";
        std::cout<<q.front().frameid<<", "<<q.front().name<<", "<<q.front().image[8888];
        std::cout << ")";
        std::cout << "\n";
        q.pop();
       };
         std::cout << "}\n";
}


template<typename T>
void dump_vector(std::vector<T> v)
{
	std::cout << "\n";
	std::cout << "(";
	for (auto i : v) std::cout<<","<<i;
	std::cout << ")";
	std::cout << "\n";

}


/*void dump_map_queue_qrobot_msg_type1(std::string name,std::map<std::string, crd_queue<T>> map)
{
        std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}       \n";        
        for(auto const& item : map) {
                crd_queue<T> q=item.second;
                dump_queue_qrobot_msg_type1(q); 
       };   

}*/




void dump_set_tuple(std::string name,std::set<std::tuple<std::string, std::string>> value)
{
    std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
    std::cout << "\n<";
    for(auto const& item : value) {
      std::cout << "  [" << std::get<0>(item) << ", " << std::get<1>(item) << "]" << std::endl; 
      };
std::cout << ">\n";
}

template<typename T>
void dump_map_keyandsize(std::string name,std::map<std::string, std::unique_ptr<queue_mt<T>>>   map)
{
    std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
    std::cout << "\n<";
    for(auto const& item : map) {
      //std::cout << "  [" << item.first << ", " << item.second->safe_size() << "]" << std::endl;
	std::cout << "  [" << item.first << "]" << std::endl;	
      };
std::cout << ">\n";
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


}  //namespace

#endif // QROBOT_UTILS_H
