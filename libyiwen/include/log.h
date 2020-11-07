/*-
 A tool to put timestamp into file, file descriptor and string.
 Copyright © 2020 yiwen xue
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
 must display the following acknowledgement:
 This product includes software developed by the organization.
 4. Neither the name of the organization nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY yiwen xue ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yiwen xue BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#pragma once 
#include <stdio.h>
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

void timestamp_str(char *str, int len) { 
   time_t now; 
   struct tm *time_struct; 
   int length; 
   char time_buffer[40]; 

   time(&now);  // get number of seconds since epoch 
   time_struct = localtime((const time_t *)&now); // convert to tm struct 
   length = strftime(time_buffer, 40, "%m/%d/%Y %H:%M:%S > ", time_struct); 
   if (length < len)
       sprintf(str, "%s", time_buffer);
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
   length = strftime(time_buffer, 40, "%m/%d/%Y %H:%M:%S > ", time_struct); 
   write(fd, time_buffer, length); // write timestamp string to log 

} 
