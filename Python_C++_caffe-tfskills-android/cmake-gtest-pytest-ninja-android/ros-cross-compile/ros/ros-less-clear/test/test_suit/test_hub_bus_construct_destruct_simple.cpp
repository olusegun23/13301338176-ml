//g++ -o -g 1 test_hub_bus_construct_destruct_simple.cpp -std=c++11
//valgrind --leak-check=yes ./1
#include "QROBOT_COMM_H.hpp"

//https://www.justsoftwaresolutions.co.uk/threading/why-do-we-need-atomic_shared_ptr.html



//http://stackoverflow.com/questions/5913396/why-do-stdshared-ptrvoid-work
/*

class MyClass;
void thread_func(std::shared_ptr<MyClass> sp){
    sp->do_stuff();
    std::shared_ptr<MyClass> sp2=sp;
    do_stuff_with(sp2);
}
int main(){
    std::shared_ptr<MyClass> sp(new MyClass);
    std::thread thread1(thread_func,sp);
    std::thread thread2(thread_func,sp);
    thread2.join();
    thread1.join();

return;

}*/


#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "QROBOT_BUS_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
using namespace qrobot;
class test {
public:
  test() {
    std::cout << "Test created" << std::endl;
  }
  ~test() {
    std::cout << "Test destroyed" << std::endl;
  }

std::mutex _mutex_callb;
};



/*
template<typename T>
class bus {
public:
  bus() {
    std::cout << "bus created==="<<typeid(T).name() << std::endl;
  }
  ~bus() {
    std::cout << "bus destroyed=="<<typeid(T).name() << std::endl;
  }

std::mutex _mutex_callb;
//std::map<std::string,T> _subs;//work
//std::map<std::string,std::unique_ptr<std::queue<T>>>  _subs;// work
std::map<std::string,std::unique_ptr<queue_mt<T>>>  _subs;

};

*/

class hub {
public:
  hub() {
    std::cout << "hub created" << std::endl;
  }




template<typename T>
int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {   std::lock_guard<std::mutex> lock(_mutex_bus);	
	auto  it = _bus_map.find(topic);
	//auto  it = _bus_map.find(typeid(T).name());
	//static_assert(std::is_same<decltype(d), double>::value, "");

	if (it == _bus_map.end()){
		 _bus_map[topic]=new bus<T>();	
	};	

	bus<T>*  mbus = static_cast<bus<T>*>(_bus_map[topic]);	
	mbus->advertise(threadname,topic,queuesize,options,errorcode);	
        return 0;
}


template<typename T>
void release_T(std::string topic){
        std::cout << ".....release..T.." << std::endl;
	bus<T>*  mbus = static_cast<bus<T>*>(_bus_map[topic]);
	delete mbus;

}
  ~hub() {
	

    std::cout << "hub destroyed" << std::endl;
  }


std::mutex _mutex_bus;
std::map<std::string,void*> _bus_map;


};






int main() {
  std::cout << "At begin of main.\ncreating std::vector<std::shared_ptr<void>>" 
            << std::endl;
  std::vector<std::shared_ptr<void>> v;
  {
    v.push_back( std::shared_ptr<test>( new test() ) );
    std::cout << "Leaving scope" << std::endl;
  }
  std::cout << "=====================" << std::endl;



hub mhub;

/*mhub._bus_map["topic1"]=new bus<qrobot_msg_type1>();
mhub._bus_map["topic2"]=new bus<qrobot_msg_type2>();*/


std::string error("   ok           ");


		mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD1","topic1",1,0,error);
		mhub.advertise<std::shared_ptr<qrobot_msg_type2>>("THREAD1","topic2",1,0,error);
		mhub.advertise<std::shared_ptr<qrobot_msg_type2>>("THREAD1","topic3",1,0,error);



/*std::string topic("topic1");
auto  it = mhub._bus_map.find(topic);
        if (it == mhub._bus_map.end()){
                 mhub._bus_map[topic]=new bus<std::shared_ptr<qrobot_msg_type1>>();
        };
        bus<std::shared_ptr<qrobot_msg_type1>>*  mbus = static_cast<bus<std::shared_ptr<qrobot_msg_type1>>*>(mhub._bus_map[topic]);
//	std::shared_ptr<bus<std::shared_ptr<qrobot_msg_type1>>>  mbus(static_cast<std::shared_ptr<bus<std::shared_ptr<qrobot_msg_type1>>>>(mhub._bus_map[topic]);
        mbus->advertise("threadname",topic,10,1,error); 
	delete mbus;

*/

	mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
	mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
 	mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic3");	




//mhub.release();//dual free


//	mhub.releaseT<std::shared_ptr<qrobot_msg_type1>>(topic); 
//std::map<std::string,void*> mymap;

//mymap["Test"]=new test();


//mymap.clear();








  return 0;
}

