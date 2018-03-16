

//g++ -o 1 test_QROBOT_QUEUE_MT_H.cpp  -std=c++11 -pthread

#include "QROBOT_BUS_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;

bus<qrobot_msg_type1> mbus;
utils tools;




int g_callback_num=0;



void callback_1(qrobot_msg_type1& msg)
     {
     std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   \n ";
     std::cout<<"\n" << "------callback1--message["<<msg.frameid<<","<<msg.name <<"]  " << std::endl;
     }









std::vector<int> g_algo1_out,g_algo1_in_1,g_algo1_in_2,g_algo1_in_3;
std::vector<int> g_algo2_out,g_algo2_in_1,g_algo2_in_2,g_algo2_in_3;
std::vector<int> g_algo3_out,g_algo3_in_1,g_algo3_in_2,g_algo3_in_3;
int g_runs=1000;
int g_sleep=1000;
int g_buffer_wait=50;

class  algo_task1 {
public:
   algo_task1(std::string str):TASK_NAME(str){};

	void callback_1_1(qrobot_msg_type1& msg)
     {
  std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo1_in_1.push_back(msg.frameid);
     }
	
	void callback_1_2(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo1_in_2.push_back(msg.frameid);
     }

	void callback_1_3(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo1_in_3.push_back(msg.frameid);
     }

   void operator() () {

	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	std::string error("   ok           ");
	mbus.advertise(TASK_NAME,"topic1",1,0,error);
	mbus.subscribe(TASK_NAME,"topic1",1000,0,std::bind(&algo_task1::callback_1_1,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic2",1000,0,std::bind(&algo_task1::callback_1_2,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic3",1000,0,std::bind(&algo_task1::callback_1_3,this,std::placeholders::_1),error);
	int i=1;
	while (i<g_runs) {
        qrobot_msg_type1 msg;
        msg.frameid=i;
	{
	// g_mut_log.lock();
	mbus.publish(TASK_NAME,"topic1",msg,0,error);
	g_algo1_out.push_back(msg.frameid);	
  	mbus.spinOne(TASK_NAME,"topic2",0,0,error);
	mbus.spinOne(TASK_NAME,"topic3",0,0,error);
	mbus.spinOne(TASK_NAME,"topic1",0,0,error);

	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg.frameid ["<<  msg.frameid<<"]" << std::endl;
	// g_mut_log.unlock();
	}
        usleep(g_sleep);
       // q.dump_queue();
       // dump_queue_qrobot_msg_type1(q.snap_queue);      
        i++;
        } 

	sleep(g_buffer_wait);

  }	
public:
std::string TASK_NAME;
};





class  algo_task2 {
public:
   algo_task2(std::string str):TASK_NAME(str){};

	void callback_2_1(qrobot_msg_type1& msg)
     {
  std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo2_in_1.push_back(msg.frameid);
     }
	
	void callback_2_2(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo2_in_2.push_back(msg.frameid);
     }

	void callback_2_3(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo2_in_3.push_back(msg.frameid);
     }

   void operator() () {

	std::string error("   ok           ");
	mbus.advertise(TASK_NAME,"topic2",1,0,error);
	mbus.subscribe(TASK_NAME,"topic1",1000,0,std::bind(&algo_task2::callback_2_1,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic2",1000,0,std::bind(&algo_task2::callback_2_2,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic3",1000,0,std::bind(&algo_task2::callback_2_3,this,std::placeholders::_1),error);
	int i=1;
	while (i<g_runs) {
        qrobot_msg_type1 msg;
        msg.frameid=i;
	{
	// g_mut_log.lock();
	mbus.publish(TASK_NAME,"topic2",msg,0,error);
	g_algo2_out.push_back(msg.frameid);	
  	mbus.spinOne(TASK_NAME,"topic1",0,0,error);
	mbus.spinOne(TASK_NAME,"topic3",0,0,error);
	mbus.spinOne(TASK_NAME,"topic2",0,0,error);

	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg.frameid ["<<  msg.frameid<<"]" << std::endl;
	// g_mut_log.unlock();
	}
        usleep(g_sleep);
       // q.dump_queue();
       // dump_queue_qrobot_msg_type1(q.snap_queue);      
        i++;
        } 

	sleep(g_buffer_wait);

  }	
public:
std::string TASK_NAME;
};





class  algo_task3 {
public:
   algo_task3(std::string str):TASK_NAME(str){};

	void callback_3_1(qrobot_msg_type1& msg)
     {
  std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo3_in_1.push_back(msg.frameid);
     }
	
	void callback_3_2(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo3_in_2.push_back(msg.frameid);
     }

	void callback_3_3(qrobot_msg_type1& msg)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg.frameid["<<msg.frameid<<"]" << std::endl;
        g_algo3_in_3.push_back(msg.frameid);
     }

   void operator() () {

	std::string error("   ok           ");
	mbus.advertise(TASK_NAME,"topic3",1,0,error);
	mbus.subscribe(TASK_NAME,"topic1",1000,0,std::bind(&algo_task3::callback_3_1,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic2",1000,0,std::bind(&algo_task3::callback_3_2,this,std::placeholders::_1),error);
	mbus.subscribe(TASK_NAME,"topic3",1000,0,std::bind(&algo_task3::callback_3_3,this,std::placeholders::_1),error);
	int i=1;
	while (i<g_runs) {
        qrobot_msg_type1 msg;
        msg.frameid=i;
	{
	// g_mut_log.lock();
	mbus.publish(TASK_NAME,"topic3",msg,0,error);
	g_algo3_out.push_back(msg.frameid);	
  	mbus.spinOne(TASK_NAME,"topic2",0,0,error);
	mbus.spinOne(TASK_NAME,"topic3",0,0,error);
	mbus.spinOne(TASK_NAME,"topic1",0,0,error);

	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg.frameid ["<<  msg.frameid<<"]" << std::endl;
	// g_mut_log.unlock();
	}
        usleep(g_sleep);
       // q.dump_queue();
       // dump_queue_qrobot_msg_type1(q.snap_queue);      
        i++;
        } 

	sleep(g_buffer_wait);

  }	
public:
std::string TASK_NAME;
};












template<typename T>
int test_2pub_3sub_fetch_2_nobloack(){
	std::string test_case("mbus. test_2pub_3sub_fetch_2_nobloack");
	std::string error_msg("   ok           ");
	int msgnum1,msgnum2;
        




	if (msgnum1==4 && msgnum2==0 )  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout <<msgnum2<< "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;

}






template<typename T>
int test_spinOnce_fetch_2_block() {
	std::string test_case("mbus. test test_publish");
	std::string error_msg("   ok           ");
        
 	int msgnum1=0;
	int msgnum2=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");


        qrobot_msg_type1 msg;
			
	for (int i=3;i<runs;i++){
		mbus.spinOne(threadname,topic,1,1,error_msg);

	};

	for(auto const& item : mbus._topic_sub_threads) {
             if (topic==std::get<0>(item)) {
                auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                if (it_subs != mbus._subs.end()) {
			std::cout<<"== _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
	
		it_subs->second->snapshot();
                tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
		msgnum1=it_subs->second->safe_size();

                } else {
                        std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                }; // end if it_subs != _subs.end()

             } else {
                std::cout<<"==NOT topic:["<< topic<<"]\n";
            };   // end if topic==item.first 	
	};





	 for (int i=0;i<runs;i++){
               	mbus.spinOne(threadname,topic+std::to_string(i),2,1,error_msg); 
		for(auto const& item : mbus._topic_sub_threads) {
             		if ((topic+std::to_string(i))==std::get<0>(item)) {
                	auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                	if (it_subs != mbus._subs.end()) {
                        	std::cout<<"==in _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		it_subs->second->snapshot();
                		tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
                		msgnum2=msgnum2+it_subs->second->safe_size();

                		} else {
                        		std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		}; // end if it_subs != _subs.end()

            		 } else {
                		std::cout<<"==NOT topic:["<< topic<<"]\n";
            		};   // end if topic==item.first    
        	};

        };


	if (msgnum1==4 && msgnum2==0 )  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout <<msgnum2<< "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;

}






























template<typename T>
int test_spinOnce_fetch_2_noblock() {
	std::string test_case("mbus. test test_publish");
	std::string error_msg("   ok           ");
        
 	int msgnum1=0;
	int msgnum2=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");


        qrobot_msg_type1 msg;
			
	for (int i=4;i<runs;i++){
		mbus.spinOne(threadname,topic,2,0,error_msg);

	};

	for(auto const& item : mbus._topic_sub_threads) {
             if (topic==std::get<0>(item)) {
                auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                if (it_subs != mbus._subs.end()) {
			std::cout<<"== _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
	
		it_subs->second->snapshot();
                tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
		msgnum1=it_subs->second->safe_size();

                } else {
                        std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                }; // end if it_subs != _subs.end()

             } else {
                std::cout<<"==NOT topic:["<< topic<<"]\n";
            };   // end if topic==item.first 	
	};





	 for (int i=0;i<runs;i++){
               	mbus.spinOne(threadname,topic+std::to_string(i),2,0,error_msg); 
		for(auto const& item : mbus._topic_sub_threads) {
             		if ((topic+std::to_string(i))==std::get<0>(item)) {
                	auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                	if (it_subs != mbus._subs.end()) {
                        	std::cout<<"==in _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		it_subs->second->snapshot();
                		tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
                		msgnum2=msgnum2+it_subs->second->safe_size();

                		} else {
                        		std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		}; // end if it_subs != _subs.end()

            		 } else {
                		std::cout<<"==NOT topic:["<< topic<<"]\n";
            		};   // end if topic==item.first    
        	};

        };


	if (msgnum1==3 && msgnum2==0 )  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout <<msgnum2<< "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;

}


















template<typename T>
int test_spinOnce_fetch_all_noblock() {
	std::string test_case("mbus. test test_publish");
	std::string error_msg("   ok           ");
        
 	int msgnum1=0;
	int msgnum2=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");


        qrobot_msg_type1 msg;
			
	for (int i=0;i<runs;i++){
		mbus.spinOne(threadname,topic,0,0,error_msg);

	};

	for(auto const& item : mbus._topic_sub_threads) {
             if (topic==std::get<0>(item)) {
                auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                if (it_subs != mbus._subs.end()) {
			std::cout<<"== _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
	
		it_subs->second->snapshot();
                tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
		msgnum1=it_subs->second->safe_size();

                } else {
                        std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                }; // end if it_subs != _subs.end()

             } else {
                std::cout<<"==NOT topic:["<< topic<<"]\n";
            };   // end if topic==item.first 	
	};





	 for (int i=0;i<runs;i++){
               	mbus.spinOne(threadname,topic+std::to_string(i),0,0,error_msg); 
		for(auto const& item : mbus._topic_sub_threads) {
             		if ((topic+std::to_string(i))==std::get<0>(item)) {
                	auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                	if (it_subs != mbus._subs.end()) {
                        	std::cout<<"==in _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		it_subs->second->snapshot();
                		tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
                		msgnum2=msgnum2+it_subs->second->safe_size();

                		} else {
                        		std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		}; // end if it_subs != _subs.end()

            		 } else {
                		std::cout<<"==NOT topic:["<< topic<<"]\n";
            		};   // end if topic==item.first    
        	};

        };


	if (msgnum1==0 && msgnum2==0 && g_callback_num==10)  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout <<msgnum2<< "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;

}



















template<typename T>
int test_publish() {
	std::string test_case("mbus. test test_publish");
	std::string error_msg("   ok           ");
        
 	int msgnum1=0;
	int msgnum2=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");


        qrobot_msg_type1 msg;
			
	for (int i=0;i<runs;i++){
		msg.frameid=i;
        	msg.image[8888]=i;
		msg.name="/image-frame: topic=["+topic+"]";
        	mbus.publish(threadname,topic,msg,1,error_msg);
	};

	for(auto const& item : mbus._topic_sub_threads) {
             if (topic==std::get<0>(item)) {
                auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                if (it_subs != mbus._subs.end()) {
			std::cout<<"== _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
	
		it_subs->second->snapshot();
                tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
		msgnum1=it_subs->second->safe_size();

                } else {
                        std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                }; // end if it_subs != _subs.end()

             } else {
                std::cout<<"==NOT topic:["<< topic<<"]\n";
            };   // end if topic==item.first 	
	};





	 for (int i=0;i<runs;i++){
                msg.frameid=i;
                msg.image[8888]=i;
		msg.name="/image-frame: topic=["+topic+std::to_string(i)+"]";
                mbus.publish(threadname,topic+std::to_string(i),msg,1,error_msg);
		for(auto const& item : mbus._topic_sub_threads) {
             		if ((topic+std::to_string(i))==std::get<0>(item)) {
                	auto it_subs = mbus._subs.find(std::get<0>(item)+std::get<1>(item));
                	if (it_subs != mbus._subs.end()) {
                        	std::cout<<"==in _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		it_subs->second->snapshot();
                		tools.dump_snapshot_queue_mt(it_subs->second->snap_queue);
                		msgnum2=msgnum2+it_subs->second->safe_size();

                		} else {
                        		std::cout<<"==NOT found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n";
                		}; // end if it_subs != _subs.end()

            		 } else {
                		std::cout<<"==NOT topic:["<< topic<<"]\n";
            		};   // end if topic==item.first    
        	};

        };


	if (msgnum1==runs && msgnum2==runs)  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout <<msgnum2<< "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;

}






template<typename T>
int test_subscribe() {

	std::string test_case("mbus. test subscribe");
	std::string error_msg("   ok           ");
        
 	int error=0;
	int success=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");

			
	for (int i=0;i<runs;i++){
		int j=mbus.subscribe(threadname,topic,100,0,callback_1,error_msg);
		if (j==-1) error=error+1;
        	
		auto  it_pubs = mbus._pubs.find(topic+threadname);
        	if (it_pubs != mbus._pubs.end()) {
                	while (!it_pubs->second->safe_empty()) {
				it_pubs->second->snapshot();
        			tools.dump_snapshot_queue_mt(it_pubs->second->snap_queue);	
                	};
        	 } else {
            		std::cout << "Not found in [ _pubs]       \n ";
        	};
	};


	for (int i=0;i<runs;i++){
		int j=mbus.subscribe(threadname,topic+std::to_string(i),100,0,callback_1,error_msg);
                if (j==0) success=success+1;
		auto  it_pubs = mbus._pubs.find(topic+threadname);
                if (it_pubs != mbus._pubs.end()) {
                        while (!it_pubs->second->safe_empty()) {
                                it_pubs->second->snapshot();
                                tools.dump_snapshot_queue_mt(it_pubs->second->snap_queue);
                        };
                 } else {
                        std::cout << "Not found in [ _pubs]       \n ";
                };

        };


	if (error==runs-1 && success==runs)  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout << "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };

	tools.dump_set_tuple("_topic_sub_threads",mbus._topic_sub_threads);
	tools.dump_map_function("_callbacks",mbus._callbacks);


return 0;


}






template<typename T>
int test_unique_prt_map_pubs() {

	std::string test_case("mbus._pubs    :insert Queue and get back");
	std::unique_ptr<queue_mt<qrobot_msg_type1>> q(new queue_mt<qrobot_msg_type1>(100));  

	std::vector<int> a{1,2,3,6,7,88,44,66};
        std::vector<int> b;
        std::vector<int> c;
	for (auto i : a) { // access by value, the type of i is int
		qrobot_msg_type1 msg1;
        	msg1.frameid=i;
		q->check_push(msg1);
	}
	
	q->snapshot();
        tools.dump_snapshot_queue_mt(q->snap_queue);  
	
        auto ret= mbus._pubs.insert(std::pair<std::string, std::unique_ptr<queue_mt<qrobot_msg_type1>>>("topic+threadname", std::move(q)));
	
	auto  it_pubs = mbus._pubs.find("topic+threadname");
      	if (it_pubs != mbus._pubs.end()) {
		while (!it_pubs->second->safe_empty()) {
			T t;	
			t.frameid=1;
	                (it_pubs->second)->wait_and_pop(t);
       			
			b.push_back(t.frameid);
		};	
		
		tools.dump_vector<int>(b);

         } else {
            std::cout << "Not found in [ _pubs]       \n ";
            return -1;

	};    

	std::set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::inserter(c, c.begin()));
       		if (c.size()==a.size()) {
                    std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "]  -testcase ["<< test_case<< "]\n";
                } else {
			std::cout << "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "]  -testcase ["<< test_case<< "]\n";		
                };

//  std::cout << "  done       \n ";


return 0;


}


template<typename T>
int test_unique_prt_map_subs() {

	std::string test_case("mbus.map    :insert Queue and get back");
	std::unique_ptr<queue_mt<qrobot_msg_type1>> q(new queue_mt<qrobot_msg_type1>(100));  

	std::vector<int> a{1,2,3,6,7,8118,44,66};
        std::vector<int> b;
        std::vector<int> c;
	for (auto i : a) { // access by value, the type of i is int
		qrobot_msg_type1 msg1;
        	msg1.frameid=i;
		q->check_push(msg1);
	}
	
	q->snapshot();
        tools.dump_snapshot_queue_mt(q->snap_queue);  
	
        auto ret= mbus._subs.insert(std::pair<std::string, std::unique_ptr<queue_mt<qrobot_msg_type1>>>("topic+threadname", std::move(q)));
	
	auto  it_pubs = mbus._subs.find("topic+threadname");
      	if (it_pubs != mbus._pubs.end()) {
		while (!it_pubs->second->safe_empty()) {
			T t;	
			t.frameid=1;
	                (it_pubs->second)->wait_and_pop(t);
       			
			b.push_back(t.frameid);
		};	
		
		tools.dump_vector<int>(b);

         } else {
            std::cout << "Not found in [ _pubs]       \n ";
            return -1;

	};    

	std::set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::inserter(c, c.begin()));
       		if (c.size()==a.size()) {
                    std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "]  -testcase ["<< test_case<< "]\n";
                } else {
			std::cout << "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "]  - testcase ["<< test_case<< "]\n";		
                };

//  std::cout << "  done       \n ";


return 0;


}



template<typename T>
int test_advertise() {

	std::string test_case("mbus. test advertise");
	std::string error_msg("   ok           ");
        
 	int error=0;
	int success=0;
	int runs=5;
	std::string topic("topic_");
	std::string threadname("algo_task1");

			
	for (int i=0;i<runs;i++){
		int j=mbus.advertise(threadname,topic,1,0,error_msg);
		if (j==-1) error=error+1;
        	
		auto  it_pubs = mbus._pubs.find(topic+threadname);
        	if (it_pubs != mbus._pubs.end()) {
                	while (!it_pubs->second->safe_empty()) {
				it_pubs->second->snapshot();
        			tools.dump_snapshot_queue_mt(it_pubs->second->snap_queue);	
                	};
        	 } else {
            		std::cout << "Not found in [ _pubs]       \n ";
        	};
	};


	for (int i=0;i<runs;i++){
                int j=mbus.advertise(threadname,topic+std::to_string(i),1,0,error_msg);
                if (j==0) success=success+1;
		auto  it_pubs = mbus._pubs.find(topic+threadname);
                if (it_pubs != mbus._pubs.end()) {
                        while (!it_pubs->second->safe_empty()) {
                                it_pubs->second->snapshot();
                                tools.dump_snapshot_queue_mt(it_pubs->second->snap_queue);
                        };
                 } else {
                        std::cout << "Not found in [ _pubs]       \n ";
                };

        };


	if (error==runs-1 && success==runs)  {
		std::cout << "----(success)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                } else {
		std::cout << "----(fail)----FILE:["<<__FILE__<<"]  FUCTION:["<<__FUNCTION__<<"]  LINE:[" << __LINE__<< "] \n";
                };


return 0;


}







int main()
    {

	/*test_unique_prt_map_pubs<qrobot_msg_type1>();
	test_unique_prt_map_subs<qrobot_msg_type1>();        
	test_advertise<qrobot_msg_type1>();
	test_subscribe<qrobot_msg_type1>();
	test_publish<qrobot_msg_type1>();
	test_spinOnce_fetch_all_noblock<qrobot_msg_type1>();
	test_publish<qrobot_msg_type1>();
	std::cout <<"==================================================================================...FUCTION=(" << __FUNCTION__  <<  ")===================   \n ";
	test_spinOnce_fetch_2_noblock<qrobot_msg_type1>();
	test_2pub_3sub_fetch_2_nobloack<qrobot_msg_type1>();*/
        //test_2pub_3sub_fetch_ALL_nobloack<qrobot_msg_type1>(); 

//	test_spinOnce_fetch_2_block<qrobot_msg_type1>();



	algo_task1  m_algo_task1("THREAD1");
	std::thread     prod1(m_algo_task1);

	algo_task2  m_algo_task2("THREAD2");
        std::thread       prod2(m_algo_task2);

	algo_task3  m_algo_task3("THREAD3");
	std::thread       prod3(m_algo_task3);


	prod1.join();
	prod2.join();
	prod3.join();


std::cout <<"TASK1==g_algo1_out("<< g_algo1_out.size()<<")g_algo1_in_1("<< g_algo1_in_1.size()<<") g_algo1_in_2("<< g_algo1_in_2.size()<<") g_algo1_in_3("<<  g_algo1_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n ";

std::cout <<"TASK2==g_algo2_out("<< g_algo2_out.size()<<")g_algo2_in_1("<< g_algo2_in_1.size()<<") g_algo2_in_2("<< g_algo2_in_2.size()<<") g_algo2_in_3("<<  g_algo2_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n ";

std::cout <<"TASK3==g_algo3_out("<< g_algo3_out.size()<<")g_algo3_in_1("<< g_algo3_in_1.size()<<") g_algo3_in_2("<< g_algo3_in_2.size()<<") g_algo3_in_3("<<  g_algo3_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n ";



	return 0;
    }





