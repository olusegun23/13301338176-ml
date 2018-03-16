//g++ -o 1 test_std::shared_ptr_void_.cpp  -std=c++11

#include "QROBOT_COMM_H.hpp"

//https://www.justsoftwaresolutions.co.uk/threading/why-do-we-need-atomic_shared_ptr.html



//http://stackoverflow.com/questions/5913396/why-do-stdshared-ptrvoid-work
/*

class MyClass;
void thread_func(std::shared_ptr<MyClass> sp){
    sp->do_stuff();
    std::shared_ptr<MyClass> sp2=sp;
    do_stuff_with(sp2);
}
int main(){
    std::shared_ptr<MyClass> sp(new MyClass);
    std::thread thread1(thread_func,sp);
    std::thread thread2(thread_func,sp);
    thread2.join();
    thread1.join();

return;

}*/


#include <memory>
#include <iostream>
#include <vector>

class test {
public:
  test() {
    std::cout << "Test created" << std::endl;
  }
  ~test() {
    std::cout << "Test destroyed" << std::endl;
  }
};

int main() {
  std::cout << "At begin of main.\ncreating std::vector<std::shared_ptr<void>>" 
            << std::endl;
  std::vector<std::shared_ptr<void>> v;
  {
    std::cout << "Creating test" << std::endl;
    v.push_back( std::shared_ptr<test>( new test() ) );
    std::cout << "Leaving scope" << std::endl;
  }
  std::cout << "Leaving main" << std::endl;
  return 0;
}

