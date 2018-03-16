

// g++ -g  -o 1 test_QROBOT_HUB_MT_H_msg2-1-3-shared_ptr.cpp  -std=c++11 -pthread  2> log
//valgrind --tool=massif  ./1    ms_print massif.out.12345
//http://valgrind.org/docs/manual/ms-manual.html
// valgrind --leak-check=full ./1 > log  2>&1

#include "QROBOT_HUB_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;

//===============================control-test=================================================

//std::shared_ptr<std::shared_ptr<qrobot_msg_type1>> msg(new  std::shared_ptr<qrobot_msg_type1>());

hub mhub;

utils tools;


std::vector<std::shared_ptr<qrobot_msg_type1>> g_algo1_out,g_algo1_in_1,g_algo2_in_1,g_algo3_in_1;
std::vector<std::shared_ptr<qrobot_msg_type2>> g_algo2_out,g_algo1_in_2,g_algo2_in_2,g_algo3_in_2;
std::vector<std::shared_ptr<qrobot_msg_type3>> g_algo3_out,g_algo1_in_3,g_algo2_in_3,g_algo3_in_3;



std::vector<int>   g_algo1_callbacknum; 
int g_callbacknum=0;


int g_runs=100000000;
int g_runs_spinonce=1000;
int g_sleep=10000;// us 
int g_buffer_wait=10; // s  wait for all queue clear when thread stop
int g_thread_wait=2; // s wait for all thread sync when start



int g_isblock=0;
int g_fetchnum=0;


/*
int g_isblock=0;
int g_fetchnum=1;
*/


/*int g_isblock=1;
int g_fetchnum=1;
*/


int g_subs_queue_size=20;


int g_vector_1=10;
int g_vector_2=20;





//===============================control-test==================================================



class  algo_task1 {
public:
   algo_task1(std::string str):TASK_NAME(str){};
	template<typename T>
	void callback_1_1(T msg1)
     {
  std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
        g_algo1_in_1.push_back(msg1);
	 g_callbacknum=g_callbacknum+1;
	
     }
	template<typename T>	
	void callback_1_2(T msg2)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg2->frameid<<"]" << std::endl;
        g_algo1_in_2.push_back(msg2);
     }
	template<typename T>
	void callback_1_3(T msg3)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg3->frameid<<"]" << std::endl;
        g_algo1_in_3.push_back(msg3);
     }

   void operator() () {
	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	std::string error("   ok           ");
	mhub.advertise<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",1,0,error);
	mhub.advertise<std::shared_ptr<qrobot_msg_type4>>(TASK_NAME,"topic4",1,0,error); // test bus<> memory leak


 	std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(&algo_task1::callback_1_1<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_subs_queue_size,0,f1,error);

	std::function<void(std::shared_ptr<qrobot_msg_type2>)> f2=std::bind(&algo_task1::callback_1_2<std::shared_ptr<qrobot_msg_type2>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_subs_queue_size,0,f2,error);

	std::function<void(std::shared_ptr<qrobot_msg_type3>)> f3=std::bind(&algo_task1::callback_1_3<std::shared_ptr<qrobot_msg_type3>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_subs_queue_size,0,f3,error);



	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_subs_queue_size,0,std::bind(&algo_task1::callback_1_1<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1),error);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_subs_queue_size,0,std::bind(&algo_task1::callback_1_2<std::shared_ptr<qrobot_msg_type2>>,this,std::placeholders::_1),error);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_subs_queue_size,0,std::bind(&algo_task1::callback_1_3<std::shared_ptr<qrobot_msg_type3>>,this,std::placeholders::_1),error);



	sleep(g_thread_wait);
	int i=1;
	while (i<g_runs) {
	std::shared_ptr<qrobot_msg_type1> msg1(new  qrobot_msg_type1());
        msg1->frameid=i;
	mhub.publish<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",msg1,0,error);
	g_algo1_out.push_back(msg1);	



	if (i>g_fetchnum){ 


		mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);

		std::cout<<"\n" << "g_callbacknum("<<g_callbacknum <<")  " << std::endl;
                if (g_callbacknum==g_fetchnum){
                } else {
                        std::cout<<"\n" << "Task("<< TASK_NAME<<")  [g_callbacknum NOT match fetchnum ]" << std::endl;
                };


		g_algo1_callbacknum.push_back(g_callbacknum);
                g_callbacknum=0;
	};
	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg->frameid ["<<  msg1->frameid<<"]" << std::endl;
	
        usleep(g_sleep);
        i++;


	/*for (auto i:g_algo1_in_2) {
		tools.dump_vector_of_vector(i->map2d);
	};*/



        }; 


	sleep(g_buffer_wait);

//if noblock, more spinOnce to clear queue
	if (g_isblock==0)  {

                for (int i=0;i<g_runs_spinonce;i++) {
			mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
			mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);
                        mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);
			usleep(g_sleep);
                };
        }






  }	
public:
std::string TASK_NAME;
};




class  algo_task2 {
public:
   algo_task2(std::string str):TASK_NAME(str){};
	template<typename T>
	void callback_2_1(T msg1)
     {
  std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
        g_algo2_in_1.push_back(msg1);
	 g_callbacknum=g_callbacknum+1;
	
     }
	template<typename T>	
	void callback_2_2(T msg2)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg2->frameid<<"]" << std::endl;
        g_algo2_in_2.push_back(msg2);
     }
	template<typename T>
	void callback_2_3(T msg3)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg3->frameid<<"]" << std::endl;
        g_algo2_in_3.push_back(msg3);
     }

   void operator() () {
	


	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	std::string error("   ok           ");
	mhub.advertise<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",2,0,error);

 	std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(&algo_task2::callback_2_1<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_subs_queue_size,0,f1,error);

	std::function<void(std::shared_ptr<qrobot_msg_type2>)> f2=std::bind(&algo_task2::callback_2_2<std::shared_ptr<qrobot_msg_type2>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_subs_queue_size,0,f2,error);

	std::function<void(std::shared_ptr<qrobot_msg_type3>)> f3=std::bind(&algo_task2::callback_2_3<std::shared_ptr<qrobot_msg_type3>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_subs_queue_size,0,f3,error);

	sleep(g_thread_wait);
	int i=1;
	while (i<g_runs) {
	std::shared_ptr<qrobot_msg_type2> msg2(new  qrobot_msg_type2());
        msg2->frameid=i;

	//======================feed dynamic vector of vector data========================================
	utils tools;
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

	mhub.publish<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",msg2,0,error);
	g_algo2_out.push_back(msg2);	

	double vm, rss;
   	tools.mem_usage(vm, rss);
   	std::cout << "VM: " << vm << "; RSS: " << rss << std::endl;


	if (i>g_fetchnum){ 

		mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);

		std::cout<<"\n" << "g_callbacknum("<<g_callbacknum <<")  " << std::endl;
                if (g_callbacknum==g_fetchnum){
                } else {
                        std::cout<<"\n" << "Task("<< TASK_NAME<<")  [g_callbacknum NOT match fetchnum ]" << std::endl;
                };


		g_algo1_callbacknum.push_back(g_callbacknum);
                g_callbacknum=0;
	};
	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg->frameid ["<<  msg2->frameid<<"]" << std::endl;
	
        usleep(g_sleep);
        i++;
        }; 


	sleep(g_buffer_wait);

//if noblock, more spinOnce to clear queue
	if (g_isblock==0)  {

                for (int i=0;i<g_runs_spinonce;i++) {
			mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
			mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);
                        mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);
			usleep(g_sleep);
                };
        }






  }	
public:
std::string TASK_NAME;
};









class  algo_task3 {
public:
   algo_task3(std::string str):TASK_NAME(str){};
	template<typename T>
	void callback_3_1(T msg1)
     {
  std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
        g_algo3_in_1.push_back(msg1);
	 g_callbacknum=g_callbacknum+1;
	
     }
	template<typename T>	
	void callback_3_2(T msg2)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg2->frameid<<"]" << std::endl;
        g_algo3_in_2.push_back(msg2);
     }
	template<typename T>
	void callback_3_3(T msg3)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg3->frameid<<"]" << std::endl;
        g_algo3_in_3.push_back(msg3);
     }

   void operator() () {
	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	std::string error("   ok           ");
	mhub.advertise<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic1",1,0,error);
	  usleep(g_sleep);

 	std::function<void(std::shared_ptr<qrobot_msg_type1>)> f1=std::bind(&algo_task3::callback_3_1<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_subs_queue_size,0,f1,error);

	std::function<void(std::shared_ptr<qrobot_msg_type2>)> f2=std::bind(&algo_task3::callback_3_2<std::shared_ptr<qrobot_msg_type2>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_subs_queue_size,0,f2,error);

	std::function<void(std::shared_ptr<qrobot_msg_type3>)> f3=std::bind(&algo_task3::callback_3_3<std::shared_ptr<qrobot_msg_type3>>,this,std::placeholders::_1); 
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_subs_queue_size,0,f3,error);

	int i=1;
	while (i<g_runs) {
	std::shared_ptr<qrobot_msg_type3> msg3(new  qrobot_msg_type3());
        msg3->frameid=i;
	mhub.publish<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",msg3,0,error);
	g_algo3_out.push_back(msg3);	


	if (i>g_fetchnum){ 


		mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);
		mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);

		std::cout<<"\n" << "g_callbacknum("<<g_callbacknum <<")  " << std::endl;
                if (g_callbacknum==g_fetchnum){
                } else {
                        std::cout<<"\n" << "Task("<< TASK_NAME<<")  [g_callbacknum NOT match fetchnum ]" << std::endl;
                };


		g_algo1_callbacknum.push_back(g_callbacknum);
                g_callbacknum=0;
	};
	std::cout<<"\n" << "loop in Task("<< TASK_NAME<<") msg->frameid ["<<  msg3->frameid<<"]" << std::endl;
	
        usleep(g_sleep);
        i++;
        }; 


	sleep(g_buffer_wait);

//if noblock, more spinOnce to clear queue
	if (g_isblock==0)  {

                for (int i=0;i<g_runs_spinonce;i++) {
			mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",g_fetchnum,g_isblock,error);
			mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",g_fetchnum,g_isblock,error);
                        mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",g_fetchnum,g_isblock,error);
			usleep(g_sleep);
                };
        }






  }	
public:
std::string TASK_NAME;
};




















int main()
    {



	 Date_P dt(99, 99, 77);

	DEBUG_T_V("MMMMMMMMMMM...",Date_P,dt);

	algo_task1  m_algo_task1("THREAD1");
	std::thread     prod1(m_algo_task1);

	algo_task2  m_algo_task2("THREAD2");
        std::thread       prod2(m_algo_task2);

	algo_task3  m_algo_task3("THREAD3");
	std::thread       prod3(m_algo_task3);

	prod1.join();
	prod2.join();
	prod3.join();



std::cout <<"==dump====g_algo3_in_2==="<<"\n";

for (auto i:g_algo3_in_2) {
tools.dump_vector_of_vector(i->map2d);
};








std::cout <<"TASK1==g_algo1_out("<< g_algo1_out.size()<<")g_algo1_in_1("<< g_algo1_in_1.size()<<") g_algo1_in_2("<< g_algo1_in_2.size()<<") g_algo1_in_3("<<  g_algo1_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n";



std::vector<int> out;
std::vector<int> in;
std::vector<int> diff(g_runs-1);



std::cout <<"TASK1=========callback in 1"<<"\n";
for (auto i:g_algo1_out) out.push_back(i->frameid);
for (auto i:g_algo1_in_1) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK1=========callback in 1"<<"\n";





std::cout <<"TASK1=========callback in 2"<<"\n";
for (auto i:g_algo2_out) out.push_back(i->frameid);
for (auto i:g_algo1_in_2) in.push_back(i->frameid);


/*for (auto i:g_algo2_out) { std::cout <<"<"<<i->frameid<<">,"; }

std::cout <<"....for (auto i:g_algo2_out) ;"<<"\n";

for (auto i:g_algo1_in_2) { std::cout <<"("<<i->frameid<<"),"; }

std::cout <<"....for (auto i:g_algo1_in_2) "<<"\n";*/

//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"\nTASK1=========callback in 2"<<"\n";


std::cout <<"TASK1=========callback in 3"<<"\n";

std::cout <<"\nTASK1=========g_algo3_out.size=("<<g_algo3_out.size()<<"    g_algo1_in_3 size=(" <<g_algo1_in_3.size()<<")    \n";
for (auto i:g_algo3_out) out.push_back(i->frameid);
for (auto i:g_algo1_in_3) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"\nTASK1=========callback in 3"<<"\n";







std::cout <<"\nTASK2==g_algo2_out("<< g_algo2_out.size()<<")g_algo2_in_1("<< g_algo2_in_1.size()<<") g_algo2_in_2("<< g_algo2_in_2.size()<<") g_algo2_in_3("<<  g_algo2_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n";

std::cout <<"TASK2=========callback in 1"<<"\n";
for (auto i:g_algo1_out) out.push_back(i->frameid);
for (auto i:g_algo2_in_1) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK2=========callback in 1"<<"\n";



std::cout <<"TASK2=========callback in 2"<<"\n";
for (auto i:g_algo2_out) out.push_back(i->frameid);
for (auto i:g_algo2_in_2) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK2=========callback in 2"<<"\n";


std::cout <<"TASK2=========callback in 3"<<"\n";
for (auto i:g_algo3_out) out.push_back(i->frameid);
for (auto i:g_algo2_in_3) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK2=========callback in 3"<<"\n";






std::cout <<"\nTASK3==g_algo3_out("<< g_algo3_out.size()<<")g_algo3_in_1("<< g_algo3_in_1.size()<<") g_algo3_in_2("<< g_algo3_in_2.size()<<") g_algo3_in_3("<<  g_algo3_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n";

std::cout <<"TASK3=========callback in 1"<<"\n";
for (auto i:g_algo1_out) out.push_back(i->frameid);
for (auto i:g_algo3_in_1) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK3=========callback in 1"<<"\n";



std::cout <<"TASK3=========callback in 2"<<"\n";
for (auto i:g_algo2_out) out.push_back(i->frameid);
for (auto i:g_algo3_in_2) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK3=========callback in 2"<<"\n";


std::cout <<"TASK3=========callback in 3"<<"\n";
for (auto i:g_algo3_out) out.push_back(i->frameid);
for (auto i:g_algo3_in_3) in.push_back(i->frameid);
//set_symmetric_difference  std::set_union 
//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
std::set_difference(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
for (auto i:diff) { if (i !=0) std::cout <<"["<<i<<"],"; }
out.clear();
in.clear();
for (auto i:diff) { i=0; }
std::cout <<"TASK3=========callback in 3"<<"\n";


int sucesscount=0;
for (auto i:g_algo1_callbacknum) { 
	if (i ==g_fetchnum) {
		sucesscount=sucesscount+1;
	};
};
if (sucesscount==g_algo1_callbacknum.size()){
	std::cout <<"TASK1=========fetchnum==1 test PASS"<<"\n";
} else {
	std::cout <<"TASK1=========fetchnum==1 test FAIL"<<"\n";
};




mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
mhub.release_T<std::shared_ptr<qrobot_msg_type4>>("topic4");









	return 0;
}


