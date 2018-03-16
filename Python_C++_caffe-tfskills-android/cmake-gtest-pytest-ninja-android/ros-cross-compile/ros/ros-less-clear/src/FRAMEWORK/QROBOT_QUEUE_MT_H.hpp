
#ifndef QROBOT_QUEUE_MT_H
#define QROBOT_QUEUE_MT_H

#include "QROBOT_COMM_H.hpp"

namespace qrobot

{

template<typename T>
class queue_mt{
private:
	std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
	int maxsize=10;
public:
    
    //std::queue<T> data_queue;
	std::queue<T> snap_queue;
	queue_mt() {}

	queue_mt(int size): maxsize(size){
	       DEBUG_T("construct queue_mt  created",T);	
	}
     
	queue_mt(const queue_mt& other){
        std::lock_guard<std::mutex> lock(other.mut);
        data_queue=other.data_queue;
	}


	queue_mt& operator=(const queue_mt) = delete;





/*    crd_queue(const crd_queue &orig):mut(std::move(orig.mut)),data_queue(orig.data_queue),data_cond(std::move(orig.data_cond)),max
size(orig.maxsize),snap_queue(orig.snap_queue)
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
	
	
	void check_push(T new_value){
       	//assert (std::is_pod<T>::value); 
/*	std::cout << "T is POD" << std::is_pod<T>::value<< std::endl;
	std::cout << "T is integral" << std::is_integral<T>::value<< std::endl; 
	std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";*/
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.size()  == maxsize){
			data_queue.pop();
        	}; 
        	data_queue.push(new_value);
        	//data_cond.notify_one();
		data_cond.notify_all();
	}

	void wait_and_pop(T& value){
        	std::unique_lock<std::mutex> lk(mut);
        	data_cond.wait(lk,[this]{return !data_queue.empty();});
		value=data_queue.front();
        	data_queue.pop();
		//lk.unlock();
   	}

	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> lk(mut);
        	data_cond.wait(lk,[this]{return !data_queue.empty();});
        	std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        	data_queue.pop();
        	return res;
	}


	bool try_pop(T& value){
        	std::lock_guard<std::mutex> lk(mut);
        	if(data_queue.empty()) return false;
        	value=data_queue.front();
        	data_queue.pop();
	}

	std::shared_ptr<T> try_pop(){
        	std::lock_guard<std::mutex> lk(mut);
        	if(data_queue.empty()) return std::shared_ptr<T>();
        	std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        	data_queue.pop();
        	return res;
	}



	bool safe_empty(){
        	std::lock_guard<std::mutex> lk(mut);
        	return data_queue.empty();
	}

	int safe_size(){
		std::lock_guard<std::mutex> lk(mut);		
        	return data_queue.size();
	}


	void snapshot(){
        	std::lock_guard<std::mutex> lk(mut);
		snap_queue=data_queue;
	
	}


    ~queue_mt(){
	DEBUG_T("destruct queue_mt   destroyed",T);
	}


};


}




#endif // QROBOT_QUEUE_MT_H
