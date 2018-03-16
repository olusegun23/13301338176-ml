
#ifndef QROBOT_UTILS_H
#define QROBOT_UTILS_H

#include "QROBOT_QUEUE_MT_H.hpp"


namespace qrobot

{


class utils 
{

public:



template<typename T>
void dump_vector_of_vector(std::vector <std::vector<T>> map2d )
{
        std::cout << "\n";
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
void dump_map_function(std::string name,std::map<std::string,std::function<void(T&)>> map)

{
	 std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
  	 std::cout << "\n<";
    	for(auto const& item : map) {
         std::function<void(T&)> f=item.second;
  
		std::cout << "  [" << item.first << ", " << typeid(f).name() << "]" << std::endl;
      	};
	std::cout << ">\n";


}




};  // class utils


}  //namespace

#endif // QROBOT_UTILS_H
