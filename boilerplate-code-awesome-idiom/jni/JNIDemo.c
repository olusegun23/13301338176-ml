// cc -o libJNIDemo.so -shared -fPIC -I/home/haijunz/QC_SW/jdk1.8.0_144/include -I/home/haijunz/QC_SW/jdk1.8.0_144/include/linux JNIDemo.c


#include "JNIDemo.h"
JNIEXPORT void JNICALL Java_JNIDemo_display(JNIEnv *env, jobject obj)
{
char name[30];
printf("What is your name?\n");
scanf("%s",name);
printf("Hello %s, you are running JNIDemo\n", name);
}
int main()
{
return  0;
}
