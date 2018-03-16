
//g++ -o -1 test_print.cpp -std=c++11

#include "QROBOT_COMM_H.hpp"

class Date_P
{
    int mo, da, yr;
public:
    Date_P(int m, int d, int y)
    {
        mo = m; da = d; yr = y;
    }
    friend std::ostream& operator<<(std::ostream& os, const Date_P& dt); 
};





std::ostream& operator<<(std::ostream& os, const Date_P& dt)  
{  
    os << dt.mo << '/' << dt.da << '/' << dt.yr;  
    return os;  
}  



  int main()

{

	 Date_P dt(5, 6, 92);  
	 std::cout << dt;

}
