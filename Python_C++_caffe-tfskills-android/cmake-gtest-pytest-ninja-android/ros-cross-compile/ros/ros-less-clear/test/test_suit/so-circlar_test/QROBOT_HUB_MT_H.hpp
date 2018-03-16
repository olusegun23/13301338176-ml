#ifndef QROBOT_HUB_MT_H
#define QROBOT_HUB_MT_H


#include "QROBOT_BUS_MT_H.hpp"


namespace qrobot

{

class hub {

public:


template<typename T>
int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {  
	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
	auto  it = _bus_map.find(topic+threadname);
	if (it != _bus_map.end()) {
	//	std::shared_ptr<bus<T>> mbus =static_cast<std::shared_ptr<bus<T>>>(it->second);  static_cast<bus<T>*>(it->second);
		bus<T>*  mbus = static_cast<bus<T>*>(it->second);	
		mbus->advertise(threadname,topic,queuesize,options,errorcode);	
            	//std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second)->safe_size() << "\n";
         } else {
		//std::shared_ptr<bus<T>> mbus(new bus<T>());
		bus<T>*  mbus = new bus<T>();
        	mbus->advertise(threadname,topic,queuesize,options,errorcode);
        	// auto ret= _bus_map.insert(std::pair<std::string, std::shared_ptr<bus<T>>>(topic+threadname, mbus));
		auto ret= _bus_map.insert(std::pair<std::string, bus<T>*>(topic+threadname, mbus));	

		if (ret.second==false) {
                	errorcode="Error bus<T>  already existed  with a value of :";
                	return -1;
		} ;   // end if
       };

        return 0;
}

template<typename T>
int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T&)>
 callback,std::string& errorcode)  {
	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
        auto  it = _bus_map.find(topic+threadname);
        if (it != _bus_map.end()) {
		 bus<T>*  mbus = static_cast<bus<T>*>(it->second);
		 mbus->subscribe(threadname,topic,queuesize,options,callback,errorcode);

         } else {
                //std::unique_ptr<bus<T>> mbus(new bus<T>());
		bus<T>*  mbus = new bus<T>();	
                mbus->subscribe(threadname,topic,queuesize,options,callback,errorcode);
		auto ret= _bus_map.insert(std::pair<std::string, bus<T>*>(topic+threadname, mbus));

       };
    
	return 0;
     }



//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
// 	publish (const M &message) const 

template<typename T>
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {

	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
        auto  it = _bus_map.find(topic+threadname);
        if (it != _bus_map.end()) {
		 std::shared_ptr<bus<T>> mbus =static_cast<std::shared_ptr<bus<T>>>(it->second);
                 mbus->publish(threadname,topic,msg,isblock,errorcode);

        } else {
        	errorcode="Error bus<T>  not existing ";
                return -1;
	} ;   // end if
	
	return 0;

}

//  

template<typename T>
int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
        auto  it = _bus_map.find(topic+threadname);
        if (it != _bus_map.end()) {
		std::shared_ptr<bus<T>> mbus =static_cast<std::shared_ptr<bus<T>>>(it->second);
		mbus->spinOne(threadname,topic,fetchnum,isblock,errorcode);

        } else {
                errorcode="Error bus<T>  not existing ";
                return -1;
        } ;   // end if

	return 0;
} 



std::map<std::string, void*>  _bus_map;

//std::map<std::string,std::shared_ptr<bus<void>>> _bus_map;
std::mutex _mutex_bus;

};


}




#endif // QROBOT_HUB_MT_H
