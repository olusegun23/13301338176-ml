#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
int main()
{
   char *str;
   char c;
   /* Initial memory allocation */
   printf("input a character to malloc  1.6G  memory\n");
   c=getchar();
   str = (char *) malloc(1.6*1024*1024*1024);
   
   if(str == NULL)
        {
        printf("out of memory\n");
        exit;
        };
   printf("malloc 1.6G success......\n");
     if(mlock(str,1.6*1024*1024*1024) == -1) {
                perror("mlock");
                printf("mlock fail \n");
                return (-1);
        };

   printf("mlock success......\n");
   memset(str,'a',1.3*1024*1024*1024);

   printf("memset 1.3G success......\n");
   for (;;) {
    memset(str,'a',1.3*1024*1024*1024);
    usleep (10);
    printf("memset success......\n");
    printf("page size = %d.\n",getpagesize());
   }
   printf("input a character to free memory\n");
   free(str); 
   c=getchar();
   printf("input a character to exit\n");
   c=getchar();
   
   return(0);
}
