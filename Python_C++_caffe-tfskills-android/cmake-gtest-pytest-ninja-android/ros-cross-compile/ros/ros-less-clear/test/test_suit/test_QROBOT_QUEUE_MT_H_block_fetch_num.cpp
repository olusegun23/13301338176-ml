

//g++ -o 1 test_QROBOT_QUEUE_MT_H.cpp  -std=c++11 -pthread

#include "QROBOT_QUEUE_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;
int g_queue_size=10;
int g_vector_1=1000;
int g_vector_2=1000;

void test_T_as_shared_ptr()
{

	queue_mt<std::shared_ptr<qrobot_msg_type1>> q(g_queue_size);

	utils tools;
        int  i=1;
        for (int j=0;j<g_queue_size+2;j++) {

        
	std::shared_ptr<qrobot_msg_type1> msg(new  qrobot_msg_type1());
        msg->frameid=i;
        msg->name="/image-frame: frameid,description,image array, size";
        msg->image[8888]=i;
        q.check_push(msg);
        std::cout<<"\n" << "----------push-----------"<< msg->frameid << std::endl;
        q.snapshot();
        //tools.dump_snapshot_queue_mt(q.snap_queue);====>need ptr-version 
        i++;
        }


        for (int j=0;j<g_queue_size;j++) {

	std::shared_ptr<qrobot_msg_type1> msg(new  qrobot_msg_type1());
        q.wait_and_pop(msg);
        std::cout<<"\n" << "----------wait_and_pop-----------"<< msg->frameid << std::endl;
        }


        std::cout<<"\n" << "----------finish-----------"<<std::endl;

	return;


}

int main()
    {
	test_T_as_shared_ptr();
        return 0;
    }





