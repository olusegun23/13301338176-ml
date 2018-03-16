
// g++ -o 1 test_perf_2_QROBOT_HUB_MT_H_msg2-1-3-shared_ptr_block_fetchnum_frequceny.cpp  -std=c++11 -pthread
//valgrind --tool=massif  ./1    ms_print massif.out.12345
//http://valgrind.org/docs/manual/ms-manual.html
// valgrind --leak-check=full ./1 > log  2>&1


#include "../../src/FRAMEWORK/QROBOT_HUB_MT_H.hpp"
#include "../../src/FRAMEWORK/QROBOT_MESSAGE_H.hpp"
#include "../../src/FRAMEWORK/QROBOT_UTILS_H.hpp"


using namespace qrobot;

hub mhub;
utils tools;
std::vector<std::shared_ptr<qrobot_msg_type1>> g_algo1_out;
std::vector<std::shared_ptr<qrobot_msg_type2>> g_algo2_out;
std::vector<std::shared_ptr<qrobot_msg_type3>> g_algo3_out;

// (1,1) queue =1000 no lost message and fetch_number match 
// (1,1) queue =queue=g_runs/4 no lost message and fetch_number match 
// (1,1) queue =1 no lost message and fetch_number match 
// (1,1) queue =0 not work 
//(1,2) queue=2 not work dead lock
//(1,1) queue=10 no lost message and fetch_number match 
//no meaning(1,0)

//(0,0) queue=10  lost message 
//(0,0) queue=g_runs/3 no loss message  
//(0,0) queue=10 , will lost messgae (g_algo1_in_2(756))
//(0,0) queue=g_runs/5 , will lost messgae  g_algo1_in_2(946)
//(0,0) queue=g_runs/4 , no messaeg lost
//(0,0) all fetch num NOT match

//(0,1) queue=g_runs  no message lost
//(0,2) queue=g_runs  no message lost
//(0,2) queue=10  many(60%) message lost
//(0,2) queue=10  change to std::this_thread::yield(); std::this_thread::sleep_for  25%-30%  message lost; 
//===========the reason seem OS schedue pin one thread 

/*
std::chrono::milliseconds duration(1);
while (true)
   {
      if(!mutex.try_lock())
      {
           std::this_thread::yield();
           std::this_thread::sleep+for(duration);
           continue;
      }
      return;
   }



*/
//===============================control-test==================================================



class  algo_task {
public:
   algo_task(std::string str,int duration_p,int duration_s,int runs_p, int runs_s, int thread_wait, int isblock, int fetchnum, int subs_queue_size, int vector_1, int vector_2):TASK_NAME(str),duration_p(duration_p),duration_s(duration_s),runs_p(runs_p),runs_s(runs_s),thread_wait(thread_wait),isblock(isblock),fetchnum(fetchnum),subs_queue_size(subs_queue_size),vector_1(vector_1),vector_2(vector_2){};
	template<typename T>
	void callback_1(T msg1)
     {
  std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg1->frameid<<"]" << std::endl;
        algo_in_1.push_back(msg1);
	spinonce_callback_1=spinonce_callback_1+1;
	
     }
	template<typename T>	
	void callback_2(T msg2)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg2->frameid<<"]" << std::endl;
        algo_in_2.push_back(msg2);
	spinonce_callback_2=spinonce_callback_2+1;
     }
	template<typename T>
	void callback_3(T msg3)
     {
     std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<").FUCTION(" << __FUNCTION__  << ")-msg->frameid["<<msg3->frameid<<"]" << std::endl;
        algo_in_3.push_back(msg3);
	spinonce_callback_3=spinonce_callback_3+1;
     }

   void operator() () {
	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	std::string error("   ok           ");


	if (TASK_NAME=="THREAD1") {
		mhub.advertise<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",1,0,error);
	};


	if (TASK_NAME=="THREAD2") {
		mhub.advertise<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",1,0,error);

        };


	if (TASK_NAME=="THREAD3") {
                mhub.advertise<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",1,0,error);
        };


	mhub.subscribe<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",subs_queue_size,0,std::bind(&algo_task::callback_1<std::shared_ptr<qrobot_msg_type1>>,this,std::placeholders::_1),error);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",subs_queue_size,0,std::bind(&algo_task::callback_2<std::shared_ptr<qrobot_msg_type2>>,this,std::placeholders::_1),error);
	mhub.subscribe<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",subs_queue_size,0,std::bind(&algo_task::callback_3<std::shared_ptr<qrobot_msg_type3>>,this,std::placeholders::_1),error);

	sleep(thread_wait);

        std::cout<<"\n ======test parameters ======================   TASK_NAME("<< TASK_NAME<<") runs_p=("<< runs_p<< ")  runs_s=("<< runs_s<<") sleep_duration_p="<<duration_p <<"  ,sleep_duration_s="<< duration_s << ",   thread_wait="<< thread_wait<<"   isblock="<<isblock<< "  fetchnum="<< fetchnum<<" subs_queue_size="<<subs_queue_size<< ",   vector_1="<< vector_1<<",   vector_2="<<vector_2<<std::endl; 

	std::chrono::milliseconds _duration_p(duration_p);
	std::chrono::milliseconds _duration_s(duration_s);

	

	std::thread pub([&](){
        	std::cout <<"\n" <<"TASK_NAME("<< TASK_NAME<<")  pub thread function\n";
		std::string error("   ok           ");
		int i=1;
        	while (i<runs_p) {

			utils tools;
			double vm, rss;
                        tools.mem_usage(vm, rss);
                        std::cout <<"\n" <<"TASK_NAME("<< TASK_NAME<<") VM: " << vm << "; RSS: " << rss << std::endl;

			if (TASK_NAME=="THREAD1") {
        			std::shared_ptr<qrobot_msg_type1> msg1(new  qrobot_msg_type1());
        			msg1->frameid=i;
        			mhub.publish<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",msg1,0,error);
				g_algo1_out.push_back(msg1);
				std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<") loop in pub Task("<< TASK_NAME<<") msg->frameid ["<<  msg1->frameid<<"]" << std::endl;

			};


			 if (TASK_NAME=="THREAD2") {    
				std::shared_ptr<qrobot_msg_type2> msg2(new  qrobot_msg_type2());
        			msg2->frameid=i;
        			std::random_device r;
        // Choose a random mean between 1 and 6
        			std::default_random_engine e1(r());
        			std::uniform_int_distribution<int> uniform_dist_value(1, 10000);
        			std::uniform_int_distribution<int> uniform_dist_dim1(1,vector_1);
        			std::uniform_int_distribution<int> uniform_dist_dim2(vector_1,vector_2);
        			int dim1=uniform_dist_dim1(e1);
        			int dim2=uniform_dist_dim2(e1);
        			float mean = uniform_dist_value(e1)+0.0001;

        			for (int k=0;k<dim1;k++) {
              				msg2->map2d.push_back(std::vector<float>(dim2, mean));
        			};

        //======================feed dynamic vector of vector data========================================
			        mhub.publish<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",msg2,0,error);
      				g_algo2_out.push_back(msg2);
                                std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<") loop in pub Task("<< TASK_NAME<<") msg->frameid ["<<  msg2->frameid<<"]" << std::endl;

                        };


			 if (TASK_NAME=="THREAD3") {    
                                std::shared_ptr<qrobot_msg_type3> msg3(new  qrobot_msg_type3());
                                msg3->frameid=i;
                                mhub.publish<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",msg3,0,error);
                                g_algo3_out.push_back(msg3);
                                std::cout<<"\n" << "TASK_NAME("<< TASK_NAME<<") loop in pub Task("<< TASK_NAME<<") msg->frameid ["<<  msg3->frameid<<"]" << std::endl;

                        };

        		//std::this_thread::yield();
        		std::this_thread::sleep_for(_duration_p);
        		i++;
		};


	});
	




	std::thread sub([&](){

		std::cout <<"\n" <<" TASK_NAME("<< TASK_NAME<<")  sub thread function\n";
		std::string error("   ok           ");

                int i=1;
                while (i<runs_s) {
			


			if (TASK_NAME=="THREAD1") {
				mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",fetchnum,0,error);
                        	spinonce_callback_1_hist.push_back(spinonce_callback_1);
                        	spinonce_callback_1=0;
				mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",fetchnum,isblock,error);
                		spinonce_callback_2_hist.push_back(spinonce_callback_2);
                		spinonce_callback_2=0;
                		mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",fetchnum,0,error);
                		spinonce_callback_3_hist.push_back(spinonce_callback_3);
                		spinonce_callback_3=0;

			};



			if (TASK_NAME=="THREAD2") {
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",fetchnum,0,error);
                                spinonce_callback_1_hist.push_back(spinonce_callback_1);
                                spinonce_callback_1=0;
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",fetchnum,isblock,error);
                                spinonce_callback_2_hist.push_back(spinonce_callback_2);
                                spinonce_callback_2=0;
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",fetchnum,0,error);
                                spinonce_callback_3_hist.push_back(spinonce_callback_3);
                                spinonce_callback_3=0;

                        };



			if (TASK_NAME=="THREAD3") {
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type1>>(TASK_NAME,"topic1",fetchnum,0,error);
                                spinonce_callback_1_hist.push_back(spinonce_callback_1);
                                spinonce_callback_1=0;
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type2>>(TASK_NAME,"topic2",fetchnum,isblock,error);
                                spinonce_callback_2_hist.push_back(spinonce_callback_2);
                                spinonce_callback_2=0;
                                mhub.spinOne<std::shared_ptr<qrobot_msg_type3>>(TASK_NAME,"topic3",fetchnum,0,error);
                                spinonce_callback_3_hist.push_back(spinonce_callback_3);
                                spinonce_callback_3=0;

                        };



                	std::cout<<"\n"  << "TASK_NAME("<< TASK_NAME<<")loop in sub Task(i="<<i<<"   "<< TASK_NAME<< std::endl;
        		//std::this_thread::yield();
        		std::this_thread::sleep_for(_duration_s);
			i=i+1;
                };


        });


	

	pub.join();
	std::cout <<"\n" <<" TASK_NAME("<< TASK_NAME<<")  pub join done\n";
	sub.join();
	std::cout <<"\n" <<" TASK_NAME("<< TASK_NAME<<")  sub join done\n";

	if (miss_check()) {std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<")===miss_check() PASSED"<<std::endl;};
	if  (isblock==1) { 
		if (fetch_number_check()) {std::cout<<"\n" <<" TASK_NAME("<< TASK_NAME<<")===fetch_number_check() PASSED"<<std::endl;};
	};


	 std::cout <<"\n" <<" TASK_NAME("<< TASK_NAME<<")  rundone\n";

	
	return;

  }


bool miss_check(){

	if (g_algo1_out.size()==algo_in_1.size() && g_algo2_out.size()==algo_in_2.size() &&  g_algo3_out.size()==algo_in_3.size()) return true;

	std::cout <<"\n TASK_NAME("<< TASK_NAME<<")==g_algo1_out("<< g_algo1_out.size()<<")algo_in_1("<< algo_in_1.size()<<") algo_in_2("<< algo_in_2.size()<<") algo_in_3("<<  algo_in_3.size()<<")===FUCTION=("<<__FUNCTION__<<  ")==\n";
	std::vector<int> out1,out2,out3;
	for (auto i:g_algo1_out) out1.push_back(i->frameid);
	for (auto i:g_algo2_out) out2.push_back(i->frameid);
	for (auto i:g_algo3_out) out3.push_back(i->frameid);
	std::vector<int> in1,in2,in3;
	for (auto i:algo_in_1) in1.push_back(i->frameid);
	for (auto i:algo_in_2) in2.push_back(i->frameid);
	for (auto i:algo_in_3) in3.push_back(i->frameid);
	std::vector<int> diff1(runs_s),diff2(runs_s),diff3(runs_s);
	//set_symmetric_difference  std::set_union 
	//std::set_intersection(out.begin(), out.end(), in.begin(), in.end() , diff.begin());
	std::cout <<"\n TASK_NAME("<< TASK_NAME<<")=callback in 1"<<"\n";	
	std::set_difference(out1.begin(), out1.end(), in1.begin(), in1.end() , diff1.begin());
	for (auto i:diff1) { if (i !=0) std::cout <<"["<<i<<"],"; }
	 std::cout <<"\n TASK_NAME("<< TASK_NAME<<")=callback in 2"<<"\n";       
        std::set_difference(out2.begin(), out2.end(), in2.begin(), in2.end() , diff2.begin());
        for (auto i:diff2) { if (i !=0) std::cout <<"["<<i<<"],"; }
	 std::cout <<"\n TASK_NAME("<< TASK_NAME<<")=callback in 3"<<"\n";       
        std::set_difference(out3.begin(), out3.end(), in3.begin(), in3.end() , diff3.begin());
        for (auto i:diff3) { if (i !=0) std::cout <<"["<<i<<"],"; }
	return false;
}


bool fetch_number_check() {

		bool pass=true;
		for (auto i:spinonce_callback_1_hist) {
        		if (i !=fetchnum) {
				std::cout<<"g_spinonce_callback_1_1_hist fetch_number test FAIL  i=["<<i<<"] not match g_fetchnum="<< fetchnum << std::endl;
				pass=false;
        		};
		};

		if (pass)  std::cout <<"_spinonce_callback_1_1_hist fetch_number test PASS match g_fetchnum="<< fetchnum << std::endl;
		for (auto i:spinonce_callback_2_hist) {
		        if (i !=fetchnum) {
				std::cout<<"g_spinonce_callback_1_2_hist fetch_number test FAIL  i=["<<i<<"] not match g_fetchnum="<< fetchnum << std::endl;
        			pass=false;
		        };
		};
		if (pass)  std::cout <<"_spinonce_callback_1_2_hist fetch_number test PASS match g_fetchnum="<< fetchnum << std::endl;
		for (auto i:spinonce_callback_3_hist) {
        		if (i !=fetchnum) {
				std::cout<<"g_spinonce_callback_1_3_hist fetch_number test FAIL  i=["<<i<<"] not match g_fetchnum="<< fetchnum << std::endl;
        			pass=false;
			};
		};
		
		if (pass)  std::cout <<"_spinonce_callback_1_3_hist fetch_number test PASS match g_fetchnum="<< fetchnum << std::endl;

		return pass;

};



	
public:
std::string TASK_NAME;
int duration_p;
int  duration_s;
std::vector<int>   spinonce_callback_1_hist;
int spinonce_callback_1=0;
std::vector<int>   spinonce_callback_2_hist;
int spinonce_callback_2=0;
std::vector<int>   spinonce_callback_3_hist;
int spinonce_callback_3=0;

std::vector<std::shared_ptr<qrobot_msg_type1>> algo_in_1;
std::vector<std::shared_ptr<qrobot_msg_type2>> algo_in_2;
std::vector<std::shared_ptr<qrobot_msg_type3>> algo_in_3;
int runs_p=1000;
int runs_s=1000;
int thread_wait=1; // s wait for all thread sync when start
int isblock=0;
int fetchnum=2;
int subs_queue_size=10;
int vector_1=100;
int vector_2=100;

};




int test_receiver_faster_block_fetch_1_2unblock_inc_queue(void) {

        algo_task  m_algo_task1("THREAD1",200,100,1000,1000,1,1,1,1000,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",200,100,1000,1000,1,0,0,1000,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",200,100,1000,1000,1,0,0,1000,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;


}



int test_receiver_faster_block_fetch_1_2unblock_inc_spin_queue(void) {

        algo_task  m_algo_task1("THREAD1",2,1,1000,3000,1,1,1,1000,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,3000,1,0,0,1000,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,3000,1,0,0,1000,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;


}








int test_receiver_faster_block_fetch_1_2unblock(void) {

	algo_task  m_algo_task1("THREAD1",2,1,1000,1000,1,1,1,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,1000,1,0,0,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,1000,1,0,0,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;


}
	







int test_receiver_faster_block_fetch_1() {
	algo_task  m_algo_task1("THREAD1",2,1,1000,1000,1,1,1,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,1000,1,1,1,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,1000,1,1,1,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}




int test_receiver_slower_block_fetch_1() {

        algo_task  m_algo_task1("THREAD1",1,2,1000,1000,1,1,1,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",1,2,1000,1000,1,1,1,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",1,2,1000,1000,1,1,1,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}





int test_receiver_slower_noblock_fetch_all() {

  	algo_task  m_algo_task1("THREAD1",1,2,1000,1000,1,0,0,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",1,2,1000,1000,1,0,0,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",1,3,1000,1000,1,0,0,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}





int test_receiver_faster_noblock_fetch_all() {
	algo_task  m_algo_task1("THREAD1",2,1,1000,1000,1,0,0,10,100,100);
	std::thread     prod1(m_algo_task1);
	algo_task  m_algo_task2("THREAD2",2,1,1000,1000,1,0,0,10,100,100);
        std::thread     prod2(m_algo_task2);
	algo_task  m_algo_task3("THREAD3",2,1,1000,1000,1,0,0,10,100,100);
        std::thread     prod3(m_algo_task3);
	prod1.join();
	prod2.join();
	prod3.join();
	mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
	mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
	mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
	return 0;

}




int test_receiver_faster_noblock_fetch_all_inc_queue(void) {

        algo_task  m_algo_task1("THREAD1",2,1,1000,1000,1,0,0,1000,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,1000,1,0,0,1000,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,1000,1,0,0,1000,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}

int test_receiver_faster_noblock_fetch_all_inc_spin(void) {

        algo_task  m_algo_task1("THREAD1",2,1,1000,3000,1,0,0,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,3000,1,0,0,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,3000,1,0,0,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}

int test_receiver_faster_noblock_fetch_1_inc_spin(void) {

        algo_task  m_algo_task1("THREAD1",2,1,1000,3000,1,0,1,10,100,100);
        std::thread     prod1(m_algo_task1);
        algo_task  m_algo_task2("THREAD2",2,1,1000,3000,1,0,1,10,100,100);
        std::thread     prod2(m_algo_task2);
        algo_task  m_algo_task3("THREAD3",2,1,1000,3000,1,0,1,10,100,100);
        std::thread     prod3(m_algo_task3);
        prod1.join();
        prod2.join();
        prod3.join();
        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;
}



int test_receiver_faster_noblock_fetch_all_inc_spin_queue() {
/*       Date_P dt(99, 99, 77);
        DEBUG_T_V("MMMMMMMMMMM...",Date_P,dt);*/
        /*algo_task1(std::string str,std::chrono::milliseconds duration_p,std::chrono::milliseconds duration_s,int runs_p, int runs_s, int thread_wait, int isblock, int fetchnum, int subs_queue_size, int vector_1, int vector_2):TASK_NAME(str),duration_p(duration_p),duration_s(duration_s),runs_p(runs_p),runs_s(runs_s),thread_wait(thread_wait),isblock(isblock),fetchnum(fetchnum),subs_queue_size(subs_queue_size),vector_1(vector_1),vector_2(vector_2){};*/

        algo_task  m_algo_task1("THREAD1",2,1,1000,4000,1,0,0,1000,100,100);
        std::thread     prod1(m_algo_task1);

        algo_task  m_algo_task2("THREAD2",2,1,1000,4000,1,0,0,1000,100,100);
        std::thread     prod2(m_algo_task2);

        algo_task  m_algo_task3("THREAD3",2,1,1000,4000,1,0,0,1000,100,100);
        std::thread     prod3(m_algo_task3);

        prod1.join();
        prod2.join();
        prod3.join();

//      std::cout <<"==dump====g_algo3_in_2[100]==="<<"\n"; 
//tools.dump_vector_of_vector(g_algo3_in_2[100]->map2d);

        mhub.release_T<std::shared_ptr<qrobot_msg_type1>>("topic1");
        mhub.release_T<std::shared_ptr<qrobot_msg_type2>>("topic2");
        mhub.release_T<std::shared_ptr<qrobot_msg_type3>>("topic3");
        return 0;

}




int main() {

	//test_receiver_faster_noblock_fetch_all(); //lost message
	//test_receiver_faster_noblock_fetch_all_inc_queue();// still lost message
	//test_receiver_faster_noblock_fetch_all_inc_spin(); // Pass
	//test_receiver_faster_noblock_fetch_1_inc_spin();  //Pass
	//test_receiver_slower_noblock_fetch_all();	//Pass

	//test_receiver_slower_block_fetch_1();  //hang up

	test_receiver_faster_block_fetch_1();// hang up

        // test_receiver_faster_block_fetch_1_2unblock();//no hang up , lost message 
	//test_receiver_faster_block_fetch_1_2unblock_inc_queue();// no hang up , less lost message  
	// test_receiver_faster_block_fetch_1_2unblock_inc_spin_queue(); hang on
	return 0;

}


