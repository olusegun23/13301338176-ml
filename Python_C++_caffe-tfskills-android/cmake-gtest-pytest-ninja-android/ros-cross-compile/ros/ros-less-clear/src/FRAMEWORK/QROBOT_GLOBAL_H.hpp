#ifndef QROBOT_GLOBAL_H
#define QROBOT_GLOBAL_H


#include <QROBOT_HUB_MT_H.hpp>
#include <QROBOT_MESSAGE_H.hpp>
#include <QROBOT_UTILS_H.hpp> 
namespace qrobot

{
hub mhub;
utils tools;

std::vector<std::shared_ptr<qrobot_msg_type1>> g_algo1_out,g_algo1_in_1,g_algo2_in_1,g_algo3_in_1,g_main_in_1;
std::vector<std::shared_ptr<qrobot_msg_type2>> g_algo2_out,g_algo1_in_2,g_algo2_in_2,g_algo3_in_2;
std::vector<std::shared_ptr<qrobot_msg_type3>> g_algo3_out,g_algo1_in_3,g_algo2_in_3,g_algo3_in_3;



std::vector<int>   g_algo1_callbacknum; 
int g_callbacknum=0;



int g_runs=10000000;
int g_runs_spinonce=100000;




int g_sleep=30000;// us 
int g_buffer_wait=10; // s  wait for all queue clear when thread stop
int g_thread_wait=2; // s wait for all thread sync when start





int g_isblock=0;
int g_fetchnum=0;


/*
int g_isblock=0;
int g_fetchnum=1;
*/


/*int g_isblock=1;
int g_fetchnum=1;
*/


int g_subs_queue_size=10;


int g_vector_1=20;
int g_vector_2=20;







}


#endif
