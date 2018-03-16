/*===============================================================================================


g++ -o 1 1.cpp  -std=c++11 -pthread

How to test :
 (1) sender , (2)(3) receier  : 

1.    ./1  | grep message  > log
2. 
 grep algo_task3  log  >log_algo_task3 
 grep algo_task1  log  >log_algo_task1
 grep algo_task2  log  >log_algo_tas2k


3. wc -l log_algo*
   4647 log_algo_task1
   4647 log_algo_task2
   4647 log_algo_task3

==========================================================================*/
#include <string>  
#include <iostream>  
#include <map>  
#include <utility>
#include <stdio.h>
#include <queue>    
#include <functional>

#include <stdio.h>
#include <functional>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <atomic>
#include<cstring>
#include <cstdlib>
#include <mutex>  
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <assert.h>




template<typename T>
class crd_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    int maxsize=10;
public:
    
    //std::queue<T> data_queue;

    crd_queue()
    {}

    crd_queue(int size): maxsize(size)
    {
    }

    void check_push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
	if (data_queue.size()  == maxsize)  {
	data_queue.pop();
        }; 
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
    }


    bool safe_empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

   void snapshot() 
    {
        std::lock_guard<std::mutex> lk(mut);
	snap_queue=data_queue;
	
    }
	

std::queue<T> snap_queue; 

};

/*
template<typename T>
class crd_queue : public std::queue<T>
{
public:
        int maxsize=10;
        crd_queue(int inputsize): maxsize(inputsize)
        {
        }


        void check_push(T new_value)
        {
              std::cout << "\n================"<<this->size()<<"==================\n"<<std::endl;

                 if (this->size() == maxsize)  {
                        this->pop();
                };
                this->push(new_value);
        }

};

*/

template<typename T>
class bus {

public:

void dump_queue_qrobot_msg_type1(crd_queue<T> q)
{
	std::cout << "\n";
	std::cout << "class  qrobot_msg_type1 { public: int frameid; std::string name; std::vector<char> image; int len=640*480; }";
	std::cout << "\n";
	std::cout << "{";
	std::cout << "\n";
	while (!q.empty()) {
	std::cout << "(";
        //std::cout<q.front().frameid<<", "<<q.front().name<<", "<<q.front().image[q.front().len-1] <<", ";
	std::cout<<q.front().frameid<<", "<<q.front().name<<", "<<q.front().image[q.front().len-1];
	std::cout << ")";
	std::cout << "\n";
        q.pop();
       };
	 std::cout << "}\n";

}

void dump_map_queue_qrobot_msg_type1(std::string name,std::map<std::string, crd_queue<T>> map)
{
        std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}       \n";        
        for(auto const& item : map) {
                crd_queue<T> q=item.second;
                dump_queue_qrobot_msg_type1(q); 
       };   

}




void dump_general_multimap(std::string name,std::multimap<std::string, std::string> map)
{
    std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
    std::cout << "\n<";
    for(auto const& item : map) {
      std::cout << "  [" << item.first << ", " << item.second << "]" << std::endl; 
      };
std::cout << ">\n";
}




void dump_map_function(std::string name,std::map<std::string,std::function<void(T&)>> map)

{
	 std::cout << "\n===========   "<<__FUNCTION__<< "   map_name= {" <<  name  << "}   \n";
  	 std::cout << "\n<";
    	for(auto const& item : map) {
         std::function<void(T&)> f=item.second;
  
		std::cout << "  [" << item.first << ", " << typeid(f).name() << "]" << std::endl;
      	};
	std::cout << ">\n";


}


// Publisher advertise(const std::string& topic, uint32_t queue_size, bool latch = false)

int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {  
	std::unique_lock<std::mutex> mlock_pubs(_mutex_pubs);
	  std::cout  <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")......  \n ";
          crd_queue<T> q(queuesize); 
        auto ret= _pubs.insert(std::pair<std::string, crd_queue<T>>(topic+threadname, &q));  
        if (ret.second==false) {
                std::cout << "Error element  already existed  with a value of :" << topic+threadname << '\n';
                 errorcode="Error element  already existed  with a value of :";
                return -1;
        } ;   // end if

         return 0;
}



/*
Subscriber 	subscribe (const std::string &topic, uint32_t queue_size, void(*fp)(M), const TransportHints &transport_hints=TransportHints())
*/

int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T&)> callback,std::string& errorcode)  {
	std::unique_lock<std::mutex> mlock_topic_sub_threads(_mutex_topic_sub_threads);
        std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks); 
     std::cout  <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")......  \n ";
      crd_queue<T> q(queuesize);
      _topic_sub_threads.insert(std::pair<std::string, std::string>(topic,threadname));
      _subs.insert(std::pair<std::string, crd_queue<T>>(topic+threadname, q));
      _callbacks.insert(std::pair<std::string, std::function<void(T&)>>(topic+threadname, callback));
     //  _callbacks.insert(std::pair<std::string,std::forward<std::function<void(T)>>(threadname+topic,callback));
     std::cout << topic+threadname<< "..insert[_topic_sub_threads,  _subs,   _callbacks ]subscribe done...." << __FUNCTION__  << "   \n ";
      return 0;
     }


//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
// 	publish (const M &message) const 
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {
	std::unique_lock<std::mutex> mlock_pubs(_mutex_pubs);
        std::unique_lock<std::mutex> mlock_topic_sub_threads(_mutex_topic_sub_threads);
        std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
      std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<< topic+threadname<<")...... msg=()   \n ";
      auto  it_pubs = _pubs.find(topic+threadname);
      if (it_pubs != _pubs.end()) {
            std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second).size() << "\n";
         } else {
            std::cout << "Not found in [ _pubs]       \n ";
            errorcode="Error Not found in [ _pubs]";
            return -1;

       };    


// loop _topic_sub_threads (if topic==item.first)  and push msg 

       for(auto const& item : _topic_sub_threads) {
             if (topic==item.first) {  
        
		std::cout << "...Found topic ("<< item.first<< ")   in _topic_sub_threads  with  [" <<item.first+item.second<< "]   \n ";
		auto it_subs = _subs.find(item.first+item.second);
        	if (it_subs != _subs.end()) {
		//std::cout <<"==push msg("<< msg.frameid<< ")===into======.Found (item.first+item.second) in    _subs:[" <<item.first+item.second<< "] after publish Queue size : " <<(it_subs->second).size() << "\n";	
           	(it_subs->second).check_push(msg);  
		//#############dump_queue_qrobot_msg_type1(it_subs->second); 
           	//std::cout <<"==push msg("<< msg.frameid<< ")===into======.Found (item.first+item.second) in    _subs:[" <<item.first+item.second<< "] after publish Queue size : " <<(it_subs->second).size() << "\n";
         	} else {
            		std::cout << "NOT Found(item.first+item.second)  in    _subs:[" <<item.first+item.second<< "]  msg=()   \n ";
        	}; // end if it_subs != _subs.end()

//	std::cout << "*******************************************loop topic_sub_threads*************if topic==item.first*****  \n ";		

             } else {
		std::cout << "...Not Found topic ("<< item.first<< ")   in _topic_sub_threads   \n ";
               
            };   // end if topic==item.first 


 //           std::cout << "*******************************************loop topic_sub_threads******************  \n ";



       	};  // end for  auto const& item : _topic_sub_threads

    return 0;

}

// ros::spinOnce(); 

int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
	assert (fetchnum >=0);
	std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks);    
	std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")..." << "   \n ";
	auto it_callback = _callbacks.find(topic+threadname);
        auto it_subs = _subs.find(topic+threadname);
	if (it_callback != _callbacks.end() && it_subs != _subs.end()) {
		std::function<void(T&)> f=it_callback->second;
		std::cout << "...Found  _callbacks[" <<  topic+threadname  << "]   \n ";
		// std::cout << "...Found _subs[" << topic+threadname <<  "]    Queue size : " <<it_subs->second.size() << "\n";	
                std::cout << "*************************************   \n ";
		switch (fetchnum) {
      				case 0:
      					{
						//while (!it_subs->second.empty()) {
						while (!it_subs->second.safe_empty()) {
                                        		//f(it_subs->second.front());
							T t;	
							it_subs->second.wait_and_pop(t);	
							f(t);
								
                                        		/*if (!it_subs->second.empty()){
								f(it_subs->second.front());
					///////#####		dump_queue_qrobot_msg_type1(it_subs->second);
                                        			it_subs->second.pop();
				///####				dump_queue_qrobot_msg_type1(it_subs->second);
                                        			std::cout << "..fetch==0...after callback and pop : " <<  " q Queue size : " <<it_subs->second.size() << "  \n";
                                        		} else {
							};*/

						};
	
    						break;
					}
      
      				default:
      					{
         					for (int i=1;i<=fetchnum;i++) {
                                       			//if (!it_subs->second.empty()){
							if (!it_subs->second.safe_empty()){
								T t;
                                                        	it_subs->second.wait_and_pop(t);
                                                        	f(t);
                                        			/*f(it_subs->second.front());
                                       ///###  		 	dump_queue_qrobot_msg_type1(it_subs->second);	
								it_subs->second.pop();
				///###				dump_queue_qrobot_msg_type1(it_subs->second);
								 std::cout << "..fetch>0....after callback and pop : " <<  " q Queue size : " <<it_subs->second.size() << "  \n";

                       	                		} else {*/
							};
                       	                	} ; //end for 

      					}
		};  // end switch


	}  else {
               std::cout << "...Not found  _callbacks or _subs  [" << topic+threadname  << "]   \n" << std::endl ;
	} ; // end  
	return 0;
} 
std::map<std::string, crd_queue<T>>  _pubs;
std::multimap<std::string, std::string> _topic_sub_threads; 
std::map<std::string, crd_queue<T>>  _subs;
std::map<std::string,std::function<void(T&)>>  _callbacks;
std::mutex _mutex_pubs;
std::mutex _mutex_topic_sub_threads;
std::mutex _mutex_subs;
std::mutex _mutex_callbacks;


};

struct  qrobot_msg_type1 
{
 int frameid;
 std::string name;
 float pos[6]={0.0,0.0,0.0,0.0,0.0,0.0};
 std::vector<char> image;
 const int len=640*480;

};



// =======================================================global================================================================
bus<qrobot_msg_type1>  mbus;
//std::mutex g_mut_log;

static void callback_topic_qrobot_msg_type1(qrobot_msg_type1& msg)
     {
        std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   \n ";
        std::cout<<"\n" << "----------callback_topic_qrobot_msg_type1 in  (main)----------message["<<msg.frameid<<"]   " << std::endl;
     }



//===================================================global==================================================================


class   qrobot_background_task
{
public:

     virtual void operator()() = 0;
};



class  algo_task1: public qrobot_background_task {
public:
   algo_task1(){};

    void callback_topic_qrobot_msg_type_algo_task1(qrobot_msg_type1& msg)
     {
        std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   \n ";
     std::cout<<"\n" << "------callback_topic_qrobot_msg_type1 in  (algo_task2)-------message["<<msg.frameid<<"]  " << std::endl;
     }

	
   void operator() () {

       std::string error("   ok           ");
       mbus.advertise("algo_task1","qrobot_msg_type1_topic_",1,0,error);
       int i=1;
	while (1) {
        qrobot_msg_type1 msg;
        msg.frameid=i;
        msg.name="/image-frame: frameid,description,image array, size";
        std::vector<char> stoneImage(msg.len,'m'); 
        stoneImage[msg.len-1]=i;
        msg.image=stoneImage; 
        //q.push(msg); 
	{
	// g_mut_log.lock();
	mbus.publish("algo_task1","qrobot_msg_type1_topic_",msg,1,error);
  	std::cout<<"\n" << "---publish  topic_qrobot_msg_type1 in  (algo_task11)-------message["<<msg.frameid<<"]  " << std::endl;
	// g_mut_log.unlock();
	}
        usleep(1);
       // q.dump_queue();
       // dump_queue_qrobot_msg_type1(q.snap_queue);      
        i++;
        } 
  }	
public:
const std::string taskname{"algo_task1"};
};



class  algo_task2: public qrobot_background_task {
public:
   algo_task2(){};

   static void callback_topic_qrobot_msg_type_algo_task2(qrobot_msg_type1& msg)
     {
        std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   \n ";
        std::cout<<"\n" << "------callback_topic_qrobot_msg_type1 in  (algo_task2)-------message["<<msg.frameid<<"]  " << std::endl;
     }

	
   void operator() () {

       std::string error("   ok           ");

       mbus.subscribe("algo_task2","qrobot_msg_type1_topic_",1,0,callback_topic_qrobot_msg_type_algo_task2,error);

        while (1) {


	 mbus.spinOne("algo_task2","qrobot_msg_type1_topic_",1,1,error); 

	  std::cout<<"\n" << "algo_task2----------spinOnce->callback----------" << std::endl;

        usleep(10);
        };

  }
public:
const std::string taskname{"algo_task2"};
};


class  algo_task3: public qrobot_background_task {
public:
   algo_task3(){};

   static void callback_topic_qrobot_msg_type_algo_task3(qrobot_msg_type1& msg)
     {
        std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   \n ";
        std::cout<<"\n" << "------callback_topic_qrobot_msg_type3 in  (algo_task3)-------message["<<msg.frameid<<"]  " << std::endl;
     }


   void operator() () {

       std::string error("   ok           ");

       mbus.subscribe("algo_task3","qrobot_msg_type1_topic_",1,0,callback_topic_qrobot_msg_type_algo_task3,error);

        while (1) {


         mbus.spinOne("algo_task3","qrobot_msg_type1_topic_",1,1,error);

          std::cout<<"\n" << "algo_task3----------spinOnce->callback----------" << std::endl;

        usleep(20);
        };

  }
public:
const std::string taskname{"algo_task3"};
};






int main()
    {

	algo_task1  m_algo_task1;
	std::thread     prod1(m_algo_task1);

	algo_task2  m_algo_task2;
        std::thread       prod2(m_algo_task2);

	algo_task3  m_algo_task3;
	std::thread       prod3(m_algo_task3);

	prod1.join();
	prod2.join();
	prod3.join();
 

/*	std::string error("   ok           ");

       mbus.advertise("main_","qrobot_msg_type1",10,0,error);
       mbus.subscribe("main_","qrobot_msg_type1",10,0,callback_topic_qrobot_msg_type1,error);

 	mbus.dump_map_function("mbus._callbacks",mbus._callbacks);
	mbus.dump_general_multimap("mbus._topic_sub_threads",mbus._topic_sub_threads);
        mbus.dump_map_queue_qrobot_msg_type1("mbus._subs",mbus._subs);
      int i=88;

       qrobot_msg_type1 msg;
       msg.name="/image-frame: frameid,description,image array, size";
       std::vector<char> stoneImage(msg.len,'m'); 
       msg.image=stoneImage; 

       while (1) { 
       msg.frameid=i;
	i++;
	std::cout <<"...FUCTION=(" << __FUNCTION__  << "================.msg(" <<msg.frameid   <<  ") publish ====  \n ";	
       mbus.publish("main_","qrobot_msg_type1",msg,1,error);
	 mbus.dump_map_queue_qrobot_msg_type1("mbus._subs",mbus._subs);
       mbus.spinOne("main_","qrobot_msg_type1",1,1,error); 
       sleep(1);
	usleep(1);
      
       };*/

        return 0;
    }





