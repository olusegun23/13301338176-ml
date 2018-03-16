

// g++ -g  -o 1 test_QROBOT_HUB_MT_H_msg2-1-3-shared_ptr.cpp  -std=c++11 -pthread  2> log
//valgrind --tool=massif  ./1    ms_print massif.out.12345
//http://valgrind.org/docs/manual/ms-manual.html


#include "QROBOT_HUB_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;

//===============================control-test=================================================

//std::shared_ptr<std::shared_ptr<qrobot_msg_type1>> msg(new  std::shared_ptr<qrobot_msg_type1>());

hub mhub;

utils tools;


//void f(void* m){};
void f(void m){};

int main(){

//std::shared_ptr<qrobot_msg_type1> msg1(new  qrobot_msg_type1());
std::shared_ptr<bus<std::shared_ptr<qrobot_msg_type2>>>  mbus_T(new bus<std::shared_ptr<qrobot_msg_type2>>());
//std::shared_ptr<bus<void>>  mbus(new bus<void>());

//failed std::map<std::string,std::shared_ptr<bus<void>>> _bus_map;
typedef  std::shared_ptr<qrobot_msg_type2> M2;
std::map<std::string,std::shared_ptr<bus<M2>>> _bus_map_T;
// failed _bus_map_T = static_cast<std::map<std::string,std::shared_ptr<bus<M2>>>>(_bus_map);









//static_cast<std::map<std::string,std::shared_ptr<bus<std::shared_ptr<qrobot_msg_type2>>>>>(_bus_map);

/*        auto  it = _bus_map_T.find(topic);
        //auto  it = _bus_map.find(typeid(T).name());
        //static_assert(std::is_same<decltype(d), double>::value, "");

        if (it != _bus_map_T.end()){
                it->second->advertise(threadname,topic,queuesize,options,errorcode);
                //std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second)->safe_size() << "\n";
         } else {
                std::shared_ptr<bus<T>> mbus(new bus<T>());
                mbus->advertise(threadname,topic,queuesize,options,errorcode);
                std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
                auto ret= _bus_map.insert(std::pair<std::string, std::shared_ptr<bus<T>>>(topic, mbus));
                //auto ret= _bus_map.insert(std::pair<std::string, bus<T>*>(typeid(mbus).name, mbus));  
                if (ret.second==false) {
                        errorcode="Error bus<T>  already existed  with a value of :";
                        return -1;
                } ;   // end if */ 




















		std::string error("   ok           ");
//	mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("TASK_NAME","topic1",1,0,error);

/*
 	std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(&algo_task1::callback_1_1<std::shared_ptr<qrobot_msg_typ
e1>>,this,std::placeholders::_1);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_subs_queue_size,0,f1,error);

	std::function<void(std::shared_ptr<qrobot_msg_type2>)> f2=std::bind(&algo_task1::callback_1_2<std::shared_ptr<qrobot_msg_typ
e2>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_subs_queue_size,0,f2,error);

	std::function<void(std::shared_ptr<qrobot_msg_type3>)> f3=std::bind(&algo_task1::callback_1_3<std::shared_ptr<qrobot_msg_typ
e3>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_subs_queue_size,0,f3,error);

*/

/*std::map<std::string,std::shared_ptr<bus<void>>> _bus_map;



 bus<T>*  mbus = new bus<T>();
                mbus->advertise(threadname,topic,queuesize,options,errorcode);
                std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
                mhub._bus_map.insert(std::pair<std::string, bus<T>*>(topic, mbus));


*/



return 0;
}
