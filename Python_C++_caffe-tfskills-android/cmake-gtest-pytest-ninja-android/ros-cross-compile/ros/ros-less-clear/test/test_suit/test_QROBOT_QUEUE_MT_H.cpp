

//g++ -o 1 test_QROBOT_QUEUE_MT_H.cpp  -std=c++11 -pthread

#include "QROBOT_QUEUE_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;


int g_queue_size=100;
int g_vector_1=1000;
int g_vector_2=1000;

void test_T_as_value()
{
	queue_mt<qrobot_msg_type1> q(5);
        utils tools;
        int  i=1;
        for (int j=0;j<20;j++) {

        qrobot_msg_type1 msg;
        msg.frameid=i;
        msg.name="/image-frame: frameid,description,image array, size";
        msg.image[8888]=i;
        q.check_push(msg);
        std::cout<<"\n" << "----------push-----------"<< msg.frameid << std::endl;
        q.snapshot();
        //tools.dump_snapshot_queue_mt(q.snap_queue);
        i++;
        }


        for (int j=0;j<10;j++) {

        qrobot_msg_type1 msg;
        q.wait_and_pop(msg);
        std::cout<<"\n" << "----------wait_and_pop-----------"<< msg.frameid << std::endl;
        }


        std::cout<<"\n" << "----------finish-----------"<<std::endl;


	return;



}



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






void test_T_as_shared_ptr_vector_of_vector()
{

        queue_mt<std::shared_ptr<qrobot_msg_type2>> q(g_queue_size);
        utils tools;
	std::random_device r;
    	// Choose a random mean between 1 and 6
    	std::default_random_engine e1(r());
    	std::uniform_int_distribution<int> uniform_dist(1, 10000);




        for (int i=0;i<g_queue_size;i++) {
       
	 	std::shared_ptr<qrobot_msg_type2> msg2(new  qrobot_msg_type2());
		msg2->frameid=i;
		float mean = uniform_dist(e1)+0.0001;
		for (int k=0;k<g_vector_1;k++) {
       			msg2->map2d.push_back(std::vector<float>(g_vector_2, mean));
		};
		q.check_push(msg2);
		std::cout<<"\n" << "----------push-----------"<< msg2->frameid << std::endl;
	        q.snapshot();
       		 //tools.dump_snapshot_queue_mt(q.snap_queue);====>need ptr-version 
	};


        for (int i=0;i<g_queue_size;i++) {
        std::shared_ptr<qrobot_msg_type2> msg2(new  qrobot_msg_type2());
        q.try_pop(msg2);
        std::cout<<"\n" << "-----try_pop---"<< msg2->frameid<<"--dim 1="<<msg2->map2d.size()<<"--dim2="<<msg2->map2d[1].size() << std::endl;
	tools.dump_vector_of_vector(msg2->map2d);

        };

        std::cout<<"\n" << "----------finish-----------"<<std::endl;

//      std::unique_ptr<queue_mt<T>> q(new queue_mt<T>(queuesize));

}











int main()
    {

	//test_T_as_value();
	// test_T_as_shared_ptr();
	test_T_as_shared_ptr_vector_of_vector();

        return 0;
    }





