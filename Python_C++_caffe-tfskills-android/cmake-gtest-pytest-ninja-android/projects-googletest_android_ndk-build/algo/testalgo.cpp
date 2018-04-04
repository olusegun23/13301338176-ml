

#include <gtest/gtest.h> // googletest header file



#include "algo.h"

// IndependentMethod is a test case - here, we have 2 tests for this 1 test case
TEST(TestCase1, test1) {

    BENCHMARKSPPED_START("foo");
    EXPECT_EQ(0, foo());
    BENCHMARKSPPED_END("foo");


    BENCHMARKSPPED_START("foo");
    foo();
    BENCHMARKSPPED_END("foo");


}




TEST(TestCase2Singleton, test1) {


class Foo : public Singleton<Foo> {
       friend class Singleton<Foo>;

 private:

       Foo(const std::string& hello) : hello_(hello)  {
           std::cout << hello << std::endl;
       }

       Foo() : hello_(nullptr) {
           std::cout << "Foo()" << std::endl;
      }

       const std::string& hello_;
  };

     const std::string hello("hello world");
     Foo::getInstance(hello);
     Foo::getInstance();
}