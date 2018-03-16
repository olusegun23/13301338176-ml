
#ifndef QROBOT_MESSAGE_H
#define QROBOT_MESSAGE_H

#include "QROBOT_COMM_H.hpp"
namespace qrobot

{









class unifiedheader
 
{

public:
 int frameid;
 float timestamp;
 std::string  topicname;
 
};
 
 
 
 
class  qrobot_msg_type2 
{
public:
unifiedheader  header;
std::vector <std::vector<float>> map2d;
std::string name;
int frameid;
char image[640*480];

 ~qrobot_msg_type2() {
    std::cout << "qrobot_msg_type2  destroyed" << std::endl;
  }




};





class  qrobot_msg_type3
{
public:
unifiedheader  header;
std::string name;
int frameid;
char image[640*480];
};






class qrobot_msg_type1 
{

 public:

 int frameid;
 std::string name;
 float pos[6]={0.0,0.0,0.0,0.0,0.0,0.0};
 char image[640*480];


};


class  qrobot_msg_type4
{
public:
unifiedheader  header;
std::vector <std::vector<float>> map2d;
std::string name;
int frameid;
char image[640*480];

 ~qrobot_msg_type4() {
    std::cout << "qrobot_msg_type4  destroyed" << std::endl;
  }


};




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






}

#endif // QROBOT_MESSAGE_H
