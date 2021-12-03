using namespace std;
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>

class File
{
   public :
       File (const File& otherFile); //copy constuctor (deep copy)
       File (const char name[], const char pmode[], 
             const char size[], const char stamp[]); //explicit constuctor, takes all the private var info
       File ();	//default no args constructor
       File & operator = (const File& otherFile); //operator overloading with a deep copy
       string getSize() const; //getters
       string getName() const;
       string getPmode() const;
       string getStamp() const;
       int recordSize() const;
       void flagAsDir(); //marks it as a directory not a file
       bool isADir() const; //getter

   private :
       char name[81], pmode[5], size[7], stamp[16];
       bool ADir;
};
