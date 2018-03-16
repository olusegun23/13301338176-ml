
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

}

#endif // QROBOT_MESSAGE_H
