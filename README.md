# Advanced C programming
Several years have passed since I first learned c programming language. I use it to analysis data, simulate and create applications. But C is not good at these applications, instead, it's more popular in low level world, such as operating systems, and embedded systems. To write code for embedded system, one need more advanced c knowledges. While that is something you teachers will not teach you.

## Data Structure
All data structure are implemented with c MACROs.
You will find them similar with the source code of linux or freebsd. Yes.

### Double Likned List

### Single Linked List

### Binary Tree

## program Argus
Sometimes we need to change parameters to run the program, but it's stupid to compile the code every time you need to make changes. Some would type the perameters to programs when running with _scanf_ or _getc_. But there is a more convinent way for us to choose. There exists a special data in c programs, it's _argc_ and _argv_. That's a method to pass string to you program once you run your programs. But it requires some special modifications on the declearation of main function. 
```C
#int main(int argc, char **argv)
``` 
_argc_ records the number of arguments you pass to program, while _**argv_ stores a set of pointers to arguments (strings).   
To read parameters in a elegant way, we can use the libs of get_opt(), and get_opt_loog().
## Config files
It's not easy if you need to modify many parameters at startup. To solve this problem, one can write the arguments with their vallues into a file, and read this file at startup, to load the settings. 
In the folder, there is a simple implement of config read header. For details, see the source codes, which are easy to understand.
## C callback function
Sometimes we have many different ways to solve a problem, but don't know which function we will use. We want the user to choose it. A convinent way is to use pointer to functions, you just change the pointer. But use the pointer directly will make the code messy. We can use a method called 'call_back' function. The idea if that, you can just pass the pointer to a function, and the pointer of input structure and output pointers (usually no output). This function is just a 'method' to this problem.  
## MultiThread -- POSIX and OpenMP

## C classlike programming

