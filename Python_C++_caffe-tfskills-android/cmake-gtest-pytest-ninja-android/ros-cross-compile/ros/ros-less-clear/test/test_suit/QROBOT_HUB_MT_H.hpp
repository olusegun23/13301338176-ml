#ifndef QROBOT_HUB_MT_H
#define QROBOT_HUB_MT_H


#include "QROBOT_BUS_MT_H.hpp"


namespace qrobot

{

class hub {

public:


hub(){
 	DEBUG("construct hub created")	   

  }

template<typename T>
int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode){ 
	std::lock_guard<std::mutex> lock(_mutex_bus);
	auto  it = _bus_map.find(topic);
        if (it == _bus_map.end()){
                 _bus_map[topic]=new bus<T>();
		DEBUG_T_V("advertise topic CREATE  ",T,topic+"----"+threadname);
        } else {

		DEBUG_T_V("advertise topic already EXISTS",T,topic+"----"+threadname);
	};
        bus<T>*  mbus = static_cast<bus<T>*>(_bus_map[topic]);
        mbus->advertise(threadname,topic,queuesize,options,errorcode);
        return 0;
	}


template<typename T>
int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T)>
 callback,std::string& errorcode)  {
        std::lock_guard<std::mutex> lock(_mutex_bus);    
	auto  it = _bus_map.find(topic);
        if (it == _bus_map.end()){
                 _bus_map[topic]=new bus<T>();
		DEBUG_T_V("subscribe  topic CREATE  ",T,topic+"----"+threadname);
        } else {

		DEBUG_T_V("subscribe  topic already EXISTS",T,topic+"----"+threadname);
        };
        bus<T>*  mbus = static_cast<bus<T>*>(_bus_map[topic]);
	mbus->subscribe(threadname,topic,queuesize,options,callback,errorcode);
        return 0;
	}


//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
// 	publish (const M &message) const 

template<typename T>
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {
//	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
//static_assert(std::is_same<decltype(d), double>::value, "");
        auto  it = _bus_map.find(topic);
        if (it != _bus_map.end()) {
		 bus<T>* mbus =static_cast<bus<T>*>(it->second);
                 mbus->publish(threadname,topic,msg,isblock,errorcode);
		DEBUG_T_V("publish topic FOUND and PROXY in hub  ",T,topic+"----"+threadname);		

        } else {
        	errorcode="Error bus<T>  not existing ";
		DEBUG_T_V("publish topic NOT FOUND in hub  ",T,topic+"----"+threadname);
                return -1;
	} ;   // end if
	
	return 0;

}

//  

template<typename T>
int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
//	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
        auto  it = _bus_map.find(topic);
        if (it != _bus_map.end()) {
		bus<T>* mbus =static_cast<bus<T>*>(it->second);
		mbus->spinOne(threadname,topic,fetchnum,isblock,errorcode);
		DEBUG_T_V("spinOnce topic FOUND and PROXY in hub  ",T,topic+"----"+threadname); 

        } else {
                errorcode="Error bus<T> NOT  FOUND ";
		DEBUG_T_V("spinOnce topic NOT FOUND in hub  ",T,topic+"----"+threadname);                 
		return -1;
        } ;   // end if

	return 0;
}


template<typename T>
void release_T(std::string topic){
	DEBUG_T("release..T",T);
        bus<T>*  mbus = static_cast<bus<T>*>(_bus_map[topic]);
        delete mbus;

}


~hub(){
	  DEBUG("destruct hub destroyed");
}

std::map<std::string, void*>  _bus_map;
//std::map<std::string,std::shared_ptr<bus<void>>> _bus_map;
std::mutex _mutex_bus;

};


}




#endif // QROBOT_HUB_MT_H
