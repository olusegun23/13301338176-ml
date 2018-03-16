

//g++ -g  -o 1 test_DFS_QROBOT_TASK_H_change.hpp.cpp -I/home/linaro/ros-less/src/FRAMEWORK  -std=c++11  -pthread 
//valgrind --tool=massif  ./1    ms_print massif.out.12345
//http://valgrind.org/docs/manual/ms-manual.html
// valgrind --leak-check=full ./1 > log  2>&1



#include <QROBOT_HUB_MT_H.hpp>
#include <QROBOT_MESSAGE_H.hpp>
#include <QROBOT_UTILS_H.hpp>
#include <QROBOT_GLOBAL_H.hpp>
#include "DFS_QROBOT_TASK_H_change.hpp"


unsigned char g_cameraImage_Raw[480*640];
float g_vslam_pos_x;
float g_vslam_pos_y;
float g_vslam_pos_z;

int main(int argc, char **argv) {

	using namespace qrobot;
	algo_dfs  m_algo_task2("DFS");
        std::thread       prod2(m_algo_task2);
	std::string TASK_NAME("VSLAM-simulation");
	std::string error("   ok           ");
	mhub.advertise<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"VSLAM4",1,0,error);

        int count = 0;
        while (count<1000){
		

		double vm, rss;
        	tools.mem_usage(vm, rss);
        	std::cout << "VM: " << vm << "; RSS: " << rss << std::endl;		
		std::shared_ptr<qrobot_msg_type1> msg1(new  qrobot_msg_type1());
                msg1->frameid=count;
                msg1->pos[0]=g_vslam_pos_x;
                msg1->pos[1]=g_vslam_pos_y;
                msg1->pos[2]=g_vslam_pos_z;

		g_vslam_pos_x++;
		g_vslam_pos_y++;
		g_vslam_pos_z++;
		memcpy(msg1->image,g_cameraImage_Raw,480*640);
		g_cameraImage_Raw[100]=count;
		mhub.publish<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"VSLAM4",msg1,0,error);

		usleep(g_sleep);
		++count;
         };


        prod2.join();


        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM3");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM4");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("2DMAP");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("DFS");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("SF");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("AE_COMMAND");
	return 0;
}

