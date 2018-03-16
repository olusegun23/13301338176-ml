// To build:
// g++ -o   test new_threads_within_callback_framework.cpp  -pthread -std=c++11
// To run:
// ./test
// framework for create new threads  in callback fucntion (caller)


#include <stdio.h>
#include <functional>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <atomic>
#include<cstring>
#include <cstdlib>
//------------------------------------------------------------------------
// Callback function.
typedef std::function<int(int,const unsigned char*)> CallbackFunction;

int g_frameid;
unsigned char* g_stonesImage = NULL;
std::atomic <bool> noThreadruning = {true};
std::atomic <bool> main_loop = {true};
std::atomic <bool> thread_loop = {true};
//------------------------------------------------------------------------
// "Caller" allows a callback to be connected.  It will call that callback.
class Caller
{
public:
    // Clients can connect their callback with this.
    void connectCallback(CallbackFunction cb)
    {
        m_cb = cb;
    }

    // Test the callback to make sure it works.
    void test(int frameid,const unsigned char* stonesImage)
    {
        int i = m_cb(frameid,stonesImage);

        // printf("test process  frameid ==: %d  return: %d\n", frameid,i);
        std::cout<<"***test process frameid = " <<frameid<<"  return: " << i << std::endl;
    }

private:
    // The callback provided by the client via connectCallback().
    CallbackFunction m_cb;
};

class Callee
{
public:
    int callbackFunction(int frameid,const unsigned char* stonesImage)
    {
         g_frameid=frameid;
	 memcpy(g_stonesImage,stonesImage, sizeof(unsigned char) * 640*480);		
         if (noThreadruning) {  
         std::thread thread_algorithm([&] {
                int current_frameid;
                unsigned char* current_stonesImage = (unsigned char*)  malloc(sizeof(unsigned char) * 640*480);
	 	
                while (thread_loop) {
                	current_frameid = g_frameid; 
			memcpy(current_stonesImage,g_stonesImage, sizeof(unsigned char) * 640*480);
		 	std::cout<<"===Begin algo Thread Executing: process current_frameid  = " <<current_frameid << "   current_stonesImage[0]=" << current_stonesImage[0] <<std::endl; 
                	usleep(300000);
			std::cout<<"===End algo Thread Executing:   process  current_frameid = "<<current_frameid<<"   current_stonesImage[0]=" << current_stonesImage[0] << std::endl;    
                };

		 free(current_stonesImage);         
            });

             noThreadruning=false;
             thread_algorithm.detach();
         };
        std::cout<<"+++++  Callee::callbackFunction() process global frameid  = " <<g_frameid<<std::endl;
        return 0;
    }

};


int main()
{
    Caller caller;
    Callee callee;
    int frameid {0};
    unsigned char* stonesImage = (unsigned char*)  malloc(sizeof(unsigned char) * 640*480);
    memset(stonesImage, '-', sizeof(unsigned char)*640*480);
    g_stonesImage = (unsigned char*)  malloc(sizeof(unsigned char) * 640*480);
 
    caller.connectCallback([&callee](int frameid, const unsigned char* stonesImage) { return callee.callbackFunction(frameid,stonesImage); }); 
    while (main_loop)
    {
    stonesImage[0]=frameid; 
    caller.test(frameid,stonesImage);
    usleep (30000);
    frameid++;

    }

    free(g_stonesImage);
    
    return 0;
}
