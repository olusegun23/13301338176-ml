
//g++ -o 1 customer.cpp    -L../build -lqrobot_static -pthread -L. -lcamera  -lmv  -Wl,-rpath,../build:.
//export LD_LIBRARY_PATH

#include "../INTERFACE_QROBOT_H.hpp"
int main(){

interface_qrobot qcom;

qcom.qrobot_start();
//qrobot_start();

return 0;
}
