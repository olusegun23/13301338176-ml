#ifndef QROBOT_BUS_MT_H
#define QROBOT_BUS_MT_H


#include "QROBOT_QUEUE_MT_H.hpp"
#include "QROBOT_UTILS_H.hpp"

namespace qrobot

{

template<typename T>
class bus {

public:


bus() {
	DEBUG_T("construct mbus created",T);
  }

// Publisher advertise(const std::string& topic, uint32_t queue_size, bool latch = false)
int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {  
	std::unique_lock<std::mutex> mlock_pubs(_mutex_pubs);
	auto  it = _pubs.find(topic+threadname);
        if (it == _pubs.end()){
		 std::unique_ptr<queue_mt<T>> q(new queue_mt<T>(queuesize));
		_pubs[topic+threadname]=std::move(q);
		
                DEBUG_T_V("mbus advertise topic queue CREATE  ",T,topic+"----"+threadname);
        } else {
		errorcode="mbus advertise topic queue already EXISTS:"+topic+"----"+threadname;
                DEBUG_T_V("mbus advertise topic queue already EXISTS",T,topic+"----"+threadname);
		return -1;
        };
        return 0;
}


int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T)>
 callback,std::string& errorcode)  {
	std::unique_lock<std::mutex> mlock_topic_sub_threads(_mutex_topic_sub_threads);
        std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks); 
	auto  it = _subs.find(topic+threadname);
        if (it == _subs.end()){
		std::unique_ptr<queue_mt<T>> q(new queue_mt<T>(queuesize));
		std::tuple<std::string, std::string> k(topic,threadname);
		DEBUG_T_V("mbus subscribe queue and tuple<topic,threadname> CREATE  ",T,topic+"----"+threadname);
		auto ret1= _subs.insert(std::pair<std::string, std::unique_ptr<queue_mt<T>>>(topic+threadname, std::move(q)));
        	auto ret3=_callbacks.insert(std::pair<std::string, std::function<void(T)>>(topic+threadname, callback));
        	auto ret2= _topic_sub_threads.insert(k);
		if (ret1.second==false || ret3.second==false || ret2.second==false) {
                	errorcode="mbus subscribe ADD  (_subs, _callbacks,_topic_sub_threads) FAIL"+topic+"----"+threadname;
			DEBUG_T_V("mbus subscribe ADD  (_subs, _callbacks,_topic_sub_threads) FAIL",T,topic+"----"+threadname);
                	return -1;
        	} ;   // end
                DEBUG_T_V("mbus subscribe ADD  (_subs, _callbacks,_topic_sub_threads) SUCCESS ",T,topic+"----"+threadname);
        } else {
                errorcode="mbus subscribe topic queue already EXISTS:"+topic+"----"+threadname;
                DEBUG_T_V("mbus subscribe topic queue already EXISTS",T,topic+"----"+threadname);
                return -1;
        };

        return 0;

     }




//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {
// loop _topic_sub_threads (if topic==item.first)  and push msg 
	DEBUG_T_V("mbus publish===  loop start (_topic_sub_threads) with [topic] ",T,topic+"----"+threadname+" msg_id=("+std::to_string(msg->frameid)+")");
        for(auto const& item : _topic_sub_threads) {
             if (topic==std::get<0>(item)) {  
		DEBUG_T_V("mbus publish",T,topic +"FOUND in _topic_sub_threads["+topic+"] msg_id=("+std::to_string(msg->frameid)+")");
		auto it_subs = _subs.find(std::get<0>(item)+std::get<1>(item));
        	if (it_subs != _subs.end()) {
		// std::cout<<"==BUS before push msg("<< ")===into==_subs:["<<std::get<0>(item)+std::get<1>(item)<<"] Queuesize="<<(it_subs->second)->safe_size() << "\n";	

			DEBUG_T_V("mbus publish before push msg into ",T,topic +"   _subs["+std::get<0>(item)+std::get<1>(item)+"]  queue size("+std::to_string((it_subs->second)->safe_size())+") msg_id=("+std::to_string(msg->frameid)+")"); 
           		(it_subs->second)->check_push(msg);  
			//#############dump_queue_qrobot_msg_type1(it_subs->second); 
           		//std::cout<<"=BUS after push msg("<<")==into==_subs:[" <<std::get<0>(item)+std::get<1>(item)<<"] Queuesize= "<<(it_subs->second)->safe_size() <<"\n";
        	 	DEBUG_T_V("mbus publish after push msg into ",T,topic +"   _subs["+std::get<0>(item)+std::get<1>(item)+"]  queue size("+std::to_string((it_subs->second)->safe_size())+") msg_id=("+std::to_string(msg->frameid)+")"); 
		} else {
			DEBUG_T_V("mbus publish NOT push",T,topic +"NOT found   _subs["+std::get<0>(item)+std::get<1>(item)+"] msg_id=("+std::to_string(msg->frameid)+")");
			errorcode="mbus publish NOT push, NOT found   _subs["+std::get<0>(item)+std::get<1>(item)+"] msg_id=("+std::to_string(msg->frameid)+")";	
			return -1;
	
        	}; // end if it_subs != _subs.end()
		
             } else {
			DEBUG_T_V("mbus publish NOT push",T,topic +"NOT found  in _topic_sub_threads["+topic+"] msg_id=("+std::to_string(msg->frameid)+")");	
			errorcode="mbus publish NOT push, NOT found  in _topic_sub_threads["+topic+"] msg_id=("+std::to_string(msg->frameid)+")";
              		return -1; 
            };   // end if topic==item.first 

	DEBUG_T_V("mbus publish===  loop end (_topic_sub_threads) with [topic] ",T,topic+"----"+threadname+" msg_id=("+std::to_string(msg->frameid)+")");
       	};  // end for  auto const& item : _topic_sub_threads

    return 0;

}

//  
int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
	assert (fetchnum >=0);
	assert (isblock >=0);
	assert (isblock <2);
	assert (!(isblock ==1 && fetchnum==0));
	//tools.dump_map_function("_callbacks",_callbacks);
//	assert (!(isblock ==1 ));
//	std::unique_lock<std::mutex> mlock_subs(_mutex_subs);
//        std::unique_lock<std::mutex> mlock_callbacks(_mutex_callbacks);    
	//std::cout <<"...FUCTION=(" << __FUNCTION__  <<  ")   topic+threadname=("<<topic+threadname<<")..." << "   \n ";
	auto it_callback = _callbacks.find(topic+threadname);
        auto it_subs = _subs.find(topic+threadname);
	if (it_callback != _callbacks.end() && it_subs != _subs.end()) {
		std::function<void(T)> f=it_callback->second;
		DEBUG_T_V("mbus spinOnce ===  FOUND  _callbacks[] and _subs[] ",T,topic+"----"+threadname);
	
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
              		DEBUG_T_V("mbus spinOnce ===  NOT FOUND  _callbacks[] and _subs[] ",T,topic+"----"+threadname);	
			errorcode="mbus spinOnce ===  NOT FOUND  _callbacks[] and _subs[] ";
                        return -1;
 
			//std::cout << "...Not found  _callbacks or _subs  [" << topic+threadname  << "]   \n" << std::endl ;
	} ; // end  


	return 0;
} 

~bus(){
     DEBUG_T("destruct mbus  destroyed",T);
}



std::map<std::string, std::unique_ptr<queue_mt<T>>>  _pubs;
//std::multimap<std::string, std::string> _topic_sub_threads; 
std::set<std::tuple<std::string, std::string>> _topic_sub_threads;
std::map<std::string, std::unique_ptr<queue_mt<T>>>  _subs;
std::map<std::string,std::function<void(T)>>  _callbacks;
std::mutex _mutex_pubs;
std::mutex _mutex_topic_sub_threads;
std::mutex _mutex_subs;
std::mutex _mutex_callbacks;
//utils tools;


};


}




#endif // QROBOT_BUS_MT_H
