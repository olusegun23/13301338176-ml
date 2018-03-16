#ifndef DFS_QROBOT_TASK_H
#define DFS_QROBOT_TASK_H



#include <QROBOT_HUB_MT_H.hpp>
#include <QROBOT_MESSAGE_H.hpp>
#include <QROBOT_UTILS_H.hpp>
#include <QROBOT_GLOBAL_H.hpp>
#include <QROBOT_TASK_H.hpp>

namespace qrobot {



class  algo_dfs:public qrobot_task {
public:
	algo_dfs(std::string str):TASK_NAME(str){};

	template<typename T>
	void callback_dfs_vslam4(T msg1) {
		std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
      //  g_algo1_in_1.push_back(msg1);
	
	}




	void operator() () {

		std::string error("   ok           ");
		mhub.advertise<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"DFS",2,0,error); 
 		std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(&algo_dfs::callback_dfs_vslam4<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1);
		mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"VSLAM4",g_subs_queue_size,0,f1,error);

		sleep(g_thread_wait);
		int i=1;
		while (i<g_runs) {
			std::shared_ptr<qrobot_msg_type2> msg2(new  qrobot_msg_type2());
        		msg2->frameid=i;

	//======================feed dynamic vector of vector data========================================
        		std::random_device r;
        // Choose a random mean between 1 and 6
        		std::default_random_engine e1(r());
        		std::uniform_int_distribution<int> uniform_dist_value(1, 10000);
        		std::uniform_int_distribution<int> uniform_dist_dim1(1,g_vector_1);
        		std::uniform_int_distribution<int> uniform_dist_dim2(g_vector_1,g_vector_2);
			int dim1=uniform_dist_dim1(e1);
			int dim2=uniform_dist_dim2(e1);
        		float mean = uniform_dist_value(e1)+0.0001;

        		for (int k=0;k<dim1;k++) {
              			msg2->map2d.push_back(std::vector<float>(dim2, mean));
        		};



	//======================feed dynamic vector of vector data========================================

			mhub.publish<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"DFS",msg2,0,error);

			if (i>g_fetchnum){ 
				mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"VSLAM4",g_fetchnum,g_isblock,error);
			};
			std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg->frameid ["<<  msg2->frameid<<"]" << std::endl;
	
        		usleep(g_sleep*3);
        		i++;
        	}; 


		sleep(g_buffer_wait);



	}	


public:
	std::string TASK_NAME;


};



} // namespace



#endif // DFS_QROBOT_TASK_H
