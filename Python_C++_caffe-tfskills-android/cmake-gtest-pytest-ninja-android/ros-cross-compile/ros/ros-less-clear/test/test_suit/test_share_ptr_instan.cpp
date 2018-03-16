

//g++ -o 1 test_QROBOT_BUS_MT_H_msg2-1-3-shared_ptr.cpp  -std=c++11 -pthread

#include "QROBOT_BUS_MT_H.hpp"
#include "QROBOT_MESSAGE_H.hpp"
#include "QROBOT_UTILS_H.hpp"
using namespace qrobot;

//===============================control-test=================================================

//std::shared_ptr<std::shared_ptr<qrobot_msg_type1>> msg(new  std::shared_ptr<qrobot_msg_type1>());


int main()
    {



	std::shared_ptr<qrobot_msg_type3> t{nullptr};

std::cout <<"TASK3=========callback in 3"<<"\n";

	return 0;
    }





