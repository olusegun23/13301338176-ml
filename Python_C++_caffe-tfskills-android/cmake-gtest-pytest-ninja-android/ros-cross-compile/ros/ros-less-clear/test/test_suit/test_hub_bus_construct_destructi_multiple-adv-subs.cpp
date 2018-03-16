

//g++ -o 1 test_hub_bus_construct_destructi_multiple-adv-subs.cpp -std=c++11 -pthread

#include "QROBOT_HUB_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;

//===============================control-test=================================================

//std::shared_ptr<std::shared_ptr<qrobot_msg_type1>> msg(new  std::shared_ptr<qrobot_msg_type1>());

//bus<qrobot_msg_type2> mbus2;
hub mhub;
std::string error("   ok           ");



template<typename T>
void callback(T msg)
     {
	
     }




void test_5_same_topic_advertise(){

DEBUG("");
mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD1","topic1",1,0,error);
mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD1","topic1",1,0,error);
mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD2","topic1",1,0,error);
mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD3","topic1",1,0,error);
mhub.advertise<std::shared_ptr<qrobot_msg_type1>>("THREAD4","topic1",1,0,error);

return;
}








void test_1_topic_subs(){

DEBUG("");




std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(callback<std::shared_ptr<qrobot_msg_type1>>,std::placeholders::_1);
mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>("TASK_NAME","topic1",10,0,f1,error);

std::function<void(std::shared_ptr<qrobot_msg_type1>)> f2=std::bind(callback<std::shared_ptr<qrobot_msg_type1>>,std::placeholders::_1);
mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>("TASK_NAME","topic2",10,0,f2,error);



mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic2");

return;
}


int main()
    {
	test_5_same_topic_advertise();
	test_1_topic_subs();
	return 0;
}





