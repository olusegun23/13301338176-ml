#ifndef QROBOT_HUB_MT_H
#define QROBOT_HUB_MT_H


#include "QROBOT_BUS_MT_H_value.hpp"


namespace qrobot

{

class hub {

public:


template<typename T>
int advertise(const std::string threadname,const std::string topic,const int queuesize,const int options,std::string& errorcode) {  

	std::map<std::string,std::shared_ptr<bus<T>>> _bus_map_T=static_cast<std::map<std::string,std::shared_ptr<bus<T>>>>(_bus_map);

	auto  it = _bus_map_T.find(topic);
	//auto  it = _bus_map.find(typeid(T).name());
	//static_assert(std::is_same<decltype(d), double>::value, "");

	if (it != _bus_map_T.end()){ 
		it->second->advertise(threadname,topic,queuesize,options,errorcode);	
            	//std::cout << "Found in [ _pubs]  after publish Queue size : " <<(it_pubs->second)->safe_size() << "\n";
         } else {
		std::shared_ptr<bus<T>> mbus(new bus<T>());
        	mbus->advertise(threadname,topic,queuesize,options,errorcode);
		std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
		auto ret= _bus_map.insert(std::pair<std::string, std::shared_ptr<bus<T>>>(topic, mbus));
		//auto ret= _bus_map.insert(std::pair<std::string, bus<T>*>(typeid(mbus).name, mbus));	
		if (ret.second==false) {
                	errorcode="Error bus<T>  already existed  with a value of :";
                	return -1;
		} ;   // end if */ 
       };

        return 0;
}



template<typename T>
int  subscribe(const std::string& threadname, const std::string& topic,const int queuesize,const int options,std::function<void(T)>
 callback,std::string& errorcode)  {

	std::map<std::string,std::shared_ptr<bus<T>>> _bus_map_T=static_cast<std::map<std::string,std::shared_ptr<bus<T>>>>(_bus_map);

        auto  it = _bus_map_T.find(topic);
	//auto  it = _bus_map.find(typeid(T).name());
        if (it != _bus_map_T.end()) {
		 it->second->subscribe(threadname,topic,queuesize,options,callback,errorcode);

         } else {
                std::shared_ptr<bus<T>> mbus(new bus<T>());
                mbus->subscribe(threadname,topic,queuesize,options,callback,errorcode);
		std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
		auto ret= _bus_map.insert(std::pair<std::string, std::shared_ptr<bus<T>>>(topic, mbus));
		//auto ret= _bus_map.insert(std::pair<std::string, bus<T>*>(typeid(mbus).name, mbus));

       };
    
	return 0;
     }






//template<typename M > void 	publish (const boost::shared_ptr< M > &message) const 
// 	publish (const M &message) const 

template<typename T>
int publish(std::string threadname,std::string topic, const T msg,const int isblock,std::string& errorcode) {
//	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);
	//auto  it = _bus_map.find(typeid(T).name());
	std::map<std::string,std::shared_ptr<bus<T>>> _bus_map_T=static_cast<std::map<std::string,std::shared_ptr<bus<T>>>>(_bus_map);
        auto  it = _bus_map_T.find(topic);
        if (it != _bus_map_T.end()) {
                 it->second->publish(threadname,topic,msg,isblock,errorcode);
		 std::cout<<"\n==HUB push msg find ("<< ")===topic+threadname["<<topic+threadname<<"]\n";
		

        } else {
        	errorcode="Error bus<T>  not existing ";
		std::cout<<"\n==HUB not found push msg("<< ")===topic+threadname["<<topic+threadname<<"]\n";
                return -1;
	} ;   // end if
	
	return 0;

}

template<typename T>
int  spinOne(std::string threadname, const std::string topic,const int fetchnum, const int isblock,std::string& errorcode) {
//	std::unique_lock<std::mutex> mlock_bus(_mutex_bus);

	 std::map<std::string,std::shared_ptr<bus<T>>> _bus_map_T=static_cast<std::map<std::string,std::shared_ptr<bus<T>>>>(_bus_map);
        auto  it = _bus_map_T.find(topic);
	//auto  it = _bus_map.find(typeid(T).name());
        if (it != _bus_map_T.end()) {
		it->second->spinOne(threadname,topic,fetchnum,isblock,errorcode);

        } else {
                errorcode="Error bus<T>  not existing ";
                return -1;
        } ;   // end if

	return 0;
} 

/*

~hub(){

	for ( auto i : _bus_map ){
    		//delete i.second;  // ? or is abc an iterator?
	}


}
*/

//std::map<std::string, void*>  _bus_map;
std::map<std::string,std::shared_ptr<bus<void>>> _bus_map;
std::mutex _mutex_bus;

};


}




#endif // QROBOT_HUB_MT_H
