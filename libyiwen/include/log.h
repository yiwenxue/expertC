#pragma once 
#include <time.h>
#include <unistd.h>

void timestamp_std(FILE *stream) { 
   time_t now; 
   struct tm *time_struct; 
   int length; 
   char time_buffer[40]; 

   time(&now);  // get number of seconds since epoch 
   time_struct = localtime((const time_t *)&now); // convert to tm struct 
   length = strftime(time_buffer, 40, "%m/%d/%Y %H:%M:%S", time_struct); 
   fprintf(stream, "%s", time_buffer);

} 

/* This function writes a timestamp string to the open file descriptor 
 * passed to it. 
 */ 
void timestamp(int fd) { 
   time_t now; 
   struct tm *time_struct; 
   int length; 
   char time_buffer[40]; 

   time(&now);  // get number of seconds since epoch 
   time_struct = localtime((const time_t *)&now); // convert to tm struct 
   length = strftime(time_buffer, 40, "%m/%d/%Y %H:%M:%S> ", time_struct); 
   write(fd, time_buffer, length); // write timestamp string to log 

} 
