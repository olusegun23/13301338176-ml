#ifndef QROBOT_BUS_MT_H
#define QROBOT_BUS_MT_H


#include "QROBOT_QUEUE_MT_H.hpp"


namespace qrobot

{

template<typename T>
class bus {

public:


// Publisher advertise(const std::string& topic, uint32_t queue_size, bool latch = false)



int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {  
	std::unique_lock<std::mutex> mlock_pubs(_mutex_pubs);
	  //std::cout  <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")......  \n ";
	// need check the queue already exists ?  it is smart pointer
	std::unique_ptr<queue_mt<T>> q(new queue_mt<T>(queuesize));		
	auto ret= _pubs.insert(std::pair<std::string, std::unique_ptr<queue_mt<T>>>(topic+threadname, std::move(q)));
  
        if (ret.second==false) {
                //std::cout << "Error element  already existed  with a value of :" << topic+threadname << '\n';
                 errorcode="Error element  already existed  with a value of :";
                return -1;
        } ;   // end if
	//std::cout << topic+threadname<< "[_pubs]  advertise success..." << __FUNCTION__  << "\n ";
        return 0;
}


int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T&)>
 callback,std::string& errorcode)  {
	std::unique_lock<std::mutex> mlock_topic_sub_threads(_mutex_topic_sub_threads);
        std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks); 
     //std::cout  <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")......  \n ";
	// need check the queue already exists ?  it is smart pointer
	std::unique_ptr<queue_mt<T>> q(new queue_mt<T>(queuesize));
	auto ret1= _subs.insert(std::pair<std::string, std::unique_ptr<queue_mt<T>>>(topic+threadname, std::move(q)));
        auto ret3=_callbacks.insert(std::pair<std::string, std::function<void(T&)>>(topic+threadname, callback));
	std::tuple<std::string, std::string> k(topic,threadname);
	auto ret2= _topic_sub_threads.insert(k);


	
	if (ret1.second==false || ret3.second==false || ret2.second==false) {
                //std::cout << "Error element  already existed  with a value of :" << topic+threadname << '\n';
                 errorcode="Error element  already existed  with a value of :";
                return -1;
        } ;   // end 

     //std::cout << topic+threadname<< "[_topic_sub_threads,_subs, _callbacks]  subscribe success..." << __FUNCTION__  << "\n ";
      return 0;
     }



//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
// 	publish (const M &message) const 
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {
//	std::unique_lock<std::mutex> mlock_pubs(_mutex_pubs);
//        std::unique_lock<std::mutex> mlock_topic_sub_threads(_mutex_topic_sub_threads);
//        std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
      //std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<< topic+threadname<<")...... msg=()   \n ";
      auto  it_pubs = _pubs.find(topic+threadname);
      if (it_pubs != _pubs.end()) {
            //std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second)->safe_size() << "\n";
         } else {
            //std::cout << "Not found in [ _pubs]       \n ";
            errorcode="Error Not found in [ _pubs]";
            return -1;

       };    

// loop _topic_sub_threads (if topic==item.first)  and push msg 
       for(auto const& item : _topic_sub_threads) {
             if (topic==std::get<0>(item)) {  
   
		//std::cout << "...Found topic ("<< std::get<0>(item)<< ") in _topic_sub_threads with  [" <<std::get<0>(item)+std::get<1>(item)<< "] \n";
		auto it_subs = _subs.find(std::get<0>(item)+std::get<1>(item));
        	if (it_subs != _subs.end()) {
		//std::cout<<"==BUS push msg("<< ")===into==_subs:["<<std::get<0>(item)+std::get<1>(item)<<"] Queuesize="<<(it_subs->second)->safe_size() << "\n";	
           	(it_subs->second)->check_push(msg);  
		//#############dump_queue_qrobot_msg_type1(it_subs->second); 
           	//std::cout<<"=BUS push msg("<<")==into==_subs:[" <<std::get<0>(item)+std::get<1>(item)<<"] Queuesize= "<<(it_subs->second)->safe_size() <<"\n";
         	} else {
			//std::cout<<"==BUS NOT push msg("<<")===not found _subs:["<<std::get<0>(item)+std::get<1>(item)<<"]\n"; 
        	}; // end if it_subs != _subs.end()
//	//std::cout << "*******************************************loop topic_sub_threads*************if topic==item.first*****  \n ";
		
             } else {
		//std::cout<<"==BUS NOT push msg("<<")===not found in _topic_sub_threads:["<< topic<<"]\n"; 
               
            };   // end if topic==item.first 

 //           //std::cout << "*******************************************loop topic_sub_threads******************  \n ";
       	};  // end for  auto const& item : _topic_sub_threads

    return 0;

}

//  
int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
	assert (fetchnum >=0);
	assert (isblock >=0);
	assert (isblock <2);
	assert (!(isblock ==1 && fetchnum==0));
//	assert (!(isblock ==1 ));
//	std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
//        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks);    
	//std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")..." << "   \n ";
	auto it_callback = _callbacks.find(topic+threadname);
        auto it_subs = _subs.find(topic+threadname);
	if (it_callback != _callbacks.end() && it_subs != _subs.end()) {
		std::function<void(T&)> f=it_callback->second;
		//std::cout << "...Found  _callbacks[" <<  topic+threadname  << "]   \n ";
		// //std::cout << "...Found _subs[" << topic+threadname <<  "]    Queue size : " <<it_subs->second.size() << "\n";	
                //std::cout << "*************************************   \n ";
		
		T t{nullptr};
		if (isblock==0 && fetchnum==0){
			while (!it_subs->second->safe_empty()){
				it_subs->second->try_pop(t);
				if (t!=nullptr)	f(t);				
			};


		};


		if (isblock==0 && fetchnum>0){
                        for (int i=0;i<fetchnum;i++){
				if  (!it_subs->second->safe_empty()){
                        		it_subs->second->try_pop(t);
					if (t!=nullptr) f(t);
				};
                        };


                }; 


		if (isblock==1 && fetchnum>0){

                        for (int i=0;i<fetchnum;i++){
                                it_subs->second->wait_and_pop(t);
				//std::unique_ptr<queue_mt<T>> q(it_subs->second);	
				//mlock_subs.unlock();	
				//q->wait_and_pop(t);	
				if (t!=nullptr) f(t);
                        };

                };

	}  else {
               //std::cout << "...Not found  _callbacks or _subs  [" << topic+threadname  << "]   \n" << std::endl ;
	} ; // end  


  //std::cout <<"typeid(*this).name() =["<< typeid(*this).name()<<"] \n";
	return 0;
} 

std::map<std::string, std::unique_ptr<queue_mt<T>>>  _pubs;
//std::multimap<std::string, std::string> _topic_sub_threads; 
std::set<std::tuple<std::string, std::string>> _topic_sub_threads;
std::map<std::string, std::unique_ptr<queue_mt<T>>>  _subs;
std::map<std::string,std::function<void(T&)>>  _callbacks;
std::mutex _mutex_pubs;
std::mutex _mutex_topic_sub_threads;
std::mutex _mutex_subs;
std::mutex _mutex_callbacks;


};


}




#endif // QROBOT_BUS_MT_H
