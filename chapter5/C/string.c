#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main (){
    char h[20] = "hello";
    char w[] = {'w', 'o', 'r', 'l', 'd', '\0'};
    char ww[] = "world";
    
    //printf("h[0]=%c\n", h[0]);
    //printf("w=%s\n", w);
    
    //printf("&h = %p, h=%s\n", &h, h);
    char *c = strcat(h,w);
    
    //printf("&h = %p, h=%s\n", &h, h);
    //printf("&c = %p, c=%s\n", &c, c);
    //printf("&c = %p\n", &c);
    printf("c=%s\n", c);
    
    char *p = strchr(c, 'l');
    *p = ' ';
    printf("p=%s\n", p);
    printf("c=%s\n", c);
    
   char str[] = "This is - www.tutorialspoint.com - website";
   const char s[2] = "-";
   
   /* get the first token */
   char *token = strtok(str, " ");
   
   /* walk through other tokens */
   while( token != NULL ) 
   {
      printf( " %s\n", token );
    
      token = strtok(NULL, " ");
   }
    
}