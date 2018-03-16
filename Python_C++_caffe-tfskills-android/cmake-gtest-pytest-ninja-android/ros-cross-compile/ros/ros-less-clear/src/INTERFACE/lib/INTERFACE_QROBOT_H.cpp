

// g++ -g  -o 1 test_perf_2_QROBOT_HUB_MT_H_msg2-1-3-shared_ptr.cpp  -std=c++11 -pthread  2> log
//valgrind --tool=massif  ./1    ms_print massif.out.12345
//http://valgrind.org/docs/manual/ms-manual.html
// valgrind --leak-check=full ./1 > log  2>&1



#include <QROBOT_HUB_MT_H.hpp>
#include <QROBOT_MESSAGE_H.hpp>
#include <QROBOT_UTILS_H.hpp>
#include <QROBOT_GLOBAL_H.hpp>
#include <VSLAM_MV_QROBOT_TASK_H.hpp>
#include <DFS_QROBOT_TASK_H.hpp>
#include <MAP2D_QROBOT_TASK_H.hpp>
#include <SF_QROBOT_TASK_H.hpp>
#include <AE_QROBOT_TASK_H.hpp>
#include "INTERFACE_QROBOT_H.hpp"

using namespace qrobot;

std::mutex mut_callback_ros;
unsigned char g_ros_cameraImage_Raw[480*640];
float g_ros_vslam_pos_x;
float g_ros_vslam_pos_y;
float g_ros_vslam_pos_z;

template<typename T>
void callback_main_1(T msg1) {
        std::lock_guard<std::mutex> lkros(mut_callback_ros);
        std::cout<<"\n" <<" main  FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
   //     g_main_in_1.push_back(msg1);
        memcpy(g_ros_cameraImage_Raw,msg1->image,480*640);
        g_ros_vslam_pos_x=msg1->pos[0];
        g_ros_vslam_pos_y=msg1->pos[1];
        g_ros_vslam_pos_z=msg1->pos[2];
        printf("\n main  Robot pose is %f,  %f,  %f\n",g_ros_vslam_pos_x,g_ros_vslam_pos_y,g_ros_vslam_pos_z);
        return;
        }



int interface_qrobot::qrobot_start() {

	/*Date_P dt(99, 99, 77);
	DEBUG_T_V("MMMMMMMMMMM...",Date_P,dt);*/
	algo_task1  m_algo_task1("VSLAM");
	std::thread     prod1(m_algo_task1);
	std::string error("   ok           ");
	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>("main","topic1",g_subs_queue_size,0,std::bind(callback_main_1<std::shared_ptr<qrobot_msg_type1>>,std::placeholders::_1),error);

	algo_dfs  m_algo_task2("DFS");
        std::thread       prod2(m_algo_task2);

        algo_2dmap  m_algo_task3("2DMAP");
        std::thread       prod3(m_algo_task3);


        algo_sf  m_algo_task4("SF");
        std::thread       prod4(m_algo_task4);


        algo_ae  m_algo_task5("AE");
        std::thread       prod5(m_algo_task5);


        int count = 0;
        while (1){
		std::stringstream ss;
		ss << "main  msg" << count<<" vslam_ros_pose_x="<< std::to_string(g_ros_vslam_pos_x)<<" vslam_ros_pose_y="<< std::to_string(g_ros_vslam_pos_y)<<" vslam_ros_pose_z="<< std::to_string(g_ros_vslam_pos_z);
		//msg.data = ss.str();
		mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>("main","topic1",g_fetchnum,g_isblock,error);
		usleep(g_sleep);
		++count;
         };

	prod1.join();
        prod2.join();
        prod3.join();
        prod4.join();
        prod5.join();

	return 0;
}

void interface_qrobot::qrobot_getmap() {};

void interface_qrobot::qrobot_stop(){
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM3");
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("VSLAM4");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("2DMAP");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("DFS");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("SF");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("AE_COMMAND");

};


