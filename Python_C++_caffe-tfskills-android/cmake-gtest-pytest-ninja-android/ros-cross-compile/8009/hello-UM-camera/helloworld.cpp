#include  <iostream>
#include<array>
template <typename T, int N>
struct my_array {
T data[N];
};


int main(void)
{
	std::cout<<"Hello world!\n";
        my_array<int,10> a;
	return 0;
}

