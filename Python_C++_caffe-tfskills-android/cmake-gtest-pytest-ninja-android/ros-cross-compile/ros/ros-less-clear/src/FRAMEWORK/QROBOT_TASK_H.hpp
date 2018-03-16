
#ifndef QROBOT_TASK_H
#define QROBOT_TASK_H

#include "QROBOT_COMM_H.hpp"

namespace qrobot

{
	
	class	qrobot_task
	{
		public:
		virtual void operator()() = 0;	
	};


}


#endif //QROBOT_TASK_H 
