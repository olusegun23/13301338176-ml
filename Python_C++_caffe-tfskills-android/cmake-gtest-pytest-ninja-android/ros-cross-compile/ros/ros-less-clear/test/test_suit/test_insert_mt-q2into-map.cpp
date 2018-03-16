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
    std::mutex mut;
    std::queue<T> data_queue;
    std::queue<T> snap_queue;
    std::condition_variable data_cond;
    int maxsize=10;
public:
    
    //std::queue<T> data_queue;

    crd_queue()
    {}

    crd_queue(int size): maxsize(size)
    {
    }

/*    crd_queue(const crd_queue &orig):mut(std::move(orig.mut)),data_queue(orig.data_queue),data_cond(std::move(orig.data_cond)),maxsize(orig.maxsize),snap_queue(orig.snap_queue)
    {
    }
*/
    /*crd_queue& operator=(const crd_queue &rhs)
    {
	mut=std::move(std::ref(rhs.mut));
	data_queue=rhs.data_queue;
	data_cond=std::move(rhs.data_cond);
	maxsize=std::move(rhs.maxsize);
	snap_queue=std::move(rhs.snap_queue);	
	return *this;
    }*/
	
	
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


    bool safe_empty() 
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

   int safe_size() 
    {
	std::lock_guard<std::mutex> lk(mut);		
        return data_queue.size();
    }


   void snapshot() 
    {
        std::lock_guard<std::mutex> lk(mut);
	snap_queue=data_queue;
	
    }

    ~crd_queue()
    {
    }

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
	//std::cout<<q.front().frameid<<", "<<q.front().name<<", "<<q.front().image[q.front().len-1];
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

std::map<std::string, std::unique_ptr<crd_queue<T>>>  _pubs;
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
 char image[640*480];

};

void myf(const std::mutex& mtx)

{
    while(true)
    {
        // lock 
        // do something
        // unlock
    }
}

// =======================================================global================================================================
bus<qrobot_msg_type1>  mbus;
//std::mutex g_mut_log;


int main()
    {

//	crd_queue<qrobot_msg_type1> q(10); 



/*  solution 1 work*========================solution2 not work, how to assign mutex and pass into map===================*/
	std::unique_ptr<crd_queue<qrobot_msg_type1>> q(new crd_queue<qrobot_msg_type1>(10));  
	qrobot_msg_type1 msg1;
	msg1.frameid=88;
  
        // q->wait_and_pop(msg1);
        q->try_pop(msg1);
	q->check_push(msg1);
        auto ret= mbus._pubs.insert(std::pair<std::string, std::unique_ptr<crd_queue<qrobot_msg_type1>>>("topic+threadname", std::move(q)));

auto  it_pubs = mbus._pubs.find("topic+threadname");
      if (it_pubs != mbus._pubs.end()) {
		qrobot_msg_type1 msg2;
		msg2.frameid=99;
           	(it_pubs->second)->check_push(msg2); 
		std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second)->safe_size() << "\n";
		qrobot_msg_type1 t;
		(it_pubs->second)->wait_and_pop(t);
		std::cout << "Found in [ _pubs]  after publish Queue size : " <<t.frameid << "\n";
         } else {
            std::cout << "Not found in [ _pubs]       \n ";
            return -1;

       };    


  std::cout << "  done       \n ";
//============================================================================================
	  crd_queue<qrobot_msg_type1> p(10);


//         crd_queue<qrobot_msg_type1> q(p);
	//	p=q;

//	auto ret1= mbus._subs.insert(std::pair<std::string, crd_queue<qrobot_msg_type1>>("topic+threadname",std::move(p)));


    std::mutex mtx1;
    std::mutex mtx2;
	//mtx2=std::ref(mtx1);
	myf(mtx1);


/*std::unique_ptr<Foo> p1(new Foo);  // p1 owns Foo
    if (p1) p1->bar();

    {
        std::unique_ptr<Foo> p2(std::move(p1));  // now p2 owns Foo
        f(*p2);

        p1 = std::move(p2);  // ownership returns to p1
        std::cout << "destroying p2...\n";
    }


*/



        return 0;
    }





