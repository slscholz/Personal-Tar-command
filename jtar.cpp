#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <sstream>
#include "file.h"
using namespace std;

bool testFile(string name);
vector<string> execute(string command, string name, vector<string> &vec,int count);
bool isDir(string name);
File fillFile(string name);
void makeTar(string name,vector<string> files);         //line 153
void printTarNames(string name);                        //line 179
void extractTar(string name);                           //line 203

int main(int argc, char const *argv[])
{
  string temp=argv[1];
  if(temp.compare("-cf")==0 && argc>3)                  //make tar file using named directories and files
  {
    string name=argv[2];
    vector<string> files;
    for(int i=3;i<argc;i++)                             //get file and directory names
    {
      string temp2=argv[i];
      if(!testFile(temp2))                              //test if valid file name
      {
        cerr<<"file or directory does not exist"<<endl;
        return 1;
      }
      execute("ls ",temp2,files,0);                     //do ls to get all files in directory if directory
    }
    makeTar(name,files);
  }
  else if(temp.compare("-tf")==0 && argc>=3)            //list names in a tar file
  {
    string name=argv[2];
    printTarNames(name);
  }
  else if(temp.compare("-xf")==0 && argc>=3)            //read and recreate files from a tar file
  {
    string name=argv[2];
    if(!testFile(name))                              //test if valid file name
    {
      cerr<<"file does not exist"<<endl;
      return 1;
    }
    extractTar(name);
  }
  else if(temp.compare("--help")==0)                    //display help options
  {
    cout<<"-cf  makes a tar file based on the files or directories following the name of the tar file"<<endl;
    cout<<"-tf  lists the names of all files that have packed into a tar file."<<endl;
    cout<<"-xf  reads a tar file, and recreates all the files saved in that tar file"<<endl;
  }
  else
  {
    cerr<<"Command Line Error-invalid option or format"<<endl;
  }

  return 0;
}
//////////////////////////////////////////////////////////////
bool testFile(string name) //checks if file exists
{
  fstream file(name.c_str(), ios::in);
  if(file)
  {
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////
vector<string> execute(string command, string name, vector<string> &vec, int count) //executes ls command to retrieve all files
{
  if(!isDir(name))
  {
    vec.push_back(name);
    return vec;
  }
  vec.push_back(name + "/");

  string tempFile="result" + to_string(count) + ".txt";                            //read system command output
  system( ((command + name) + " > " + tempFile).c_str());
  ifstream in( tempFile.c_str() );

  string temp;
  while(in>>temp)
  {
    if(isDir(name + "/" + temp))                                                  //if directory: recursive ls
    {
      execute("ls ",name + "/" + temp,vec,count+1);
    }
    else
    {
      vec.push_back(name + "/" + temp);
    }
  }
  in.close();
  for(int i=0;i<count;i++)                                                        //delete temporary text files
  {
    string remFile="result" + to_string(i) + ".txt";
    system( ("rm " + remFile).c_str() );
  }
  return vec;
}
///////////////////////////////////////////////////////////
bool isDir(string name) //check if file is a directory
{
  system( (("stat --format=\"%F\" " + name) + " > results.txt").c_str());
  ifstream in("results.txt");

  string temp;
  in>>temp;
  system("rm results.txt");
  if(temp.compare("directory")==0)
  {
    return true;
  }
  return false;
}
////////////////////////////////////////////////////////
File fillFile(string name) //read in file protection mode, size, and date, record file information in File object
{
  system( (("stat --format=\"%a %s\" " + name) + " > answer.txt").c_str());
  ifstream in("answer.txt");
  char Pmode[5];
  in>>Pmode;
  char Size[7];
  in>>Size;
  system( (("stat --format=\"%y\" " + name) + " > answer.txt").c_str());
  ifstream in2("answer.txt");
  string d, date;
  in2>>d;
  date=d.substr(0,4)+d.substr(5,2)+d.substr(8);
  in2>>d;
  system("rm answer.txt");
  date=date+d.substr(0,2)+d.substr(3,2)+"."+d.substr(6,2);
  char array1[81];
  strcpy(array1,name.c_str());
  char array2[16];
  strcpy(array2,date.c_str());
  File f(array1,Pmode,Size,array2);
  if(isDir(name))
  {
    f.flagAsDir();
  }
  return f;
}
//////////////////////////////////////////////////////////////
void makeTar(string name, vector<string> files)             //create tar file - cf command
{
  fstream tarFile(name.c_str(), ios::out | ios::binary);
  int totalSize=0;
  tarFile.write((char*) &totalSize, sizeof(int));
  for(int i=0;i<files.size();i++)                           //all directories and files
  {
    File f=fillFile(files[i]);
    //cout<<f.getName()<<" "<<f.getPmode()<<" "<<f.getSize()<<" "<<f.getStamp()<<endl;
    tarFile.write((char*) &f, sizeof(File));
    if(!f.isADir())
    {
      fstream inStream(files[i],ios::in);
      char ch;
      while(inStream.get(ch))                               //write out text
      {
        tarFile.put(ch);
        totalSize++;
      }
    }
    totalSize+=sizeof(File);
  }
  tarFile.seekg(0);
  tarFile.write((char*) &totalSize, sizeof(int));
}
//////////////////////////////////////////////////////////////////
void printTarNames(string name) //print all names in a tar file - tf command
{
  fstream tarFile(name.c_str(), ios::in | ios::binary);
  int tarSize;
  tarFile.read((char*) &tarSize, sizeof(tarSize));
  File f;
  while(tarFile.read ((char *) &f, sizeof(File)))
  {
    if(!f.isADir())             //skip over text file contents
    {
      string size=f.getSize();
      stringstream s(size);
      int Size=0;
      s>>Size;
      tarFile.seekg(Size, ios::cur);
      cout<<f.getName()<<endl;
    }
    else
    {
      cout<<f.getName()<<endl;
    }
  }
}
///////////////////////////////////////////////////////////////////
void extractTar(string name)//extract files from a tar file - xf command
{
  fstream tarFile(name.c_str(), ios::in | ios::binary);
  int tarSize;
  tarFile.read((char*) &tarSize, sizeof(tarSize));
  File f;
  while(tarFile.read ((char *) &f, sizeof(File)))
  {
    if(!f.isADir())                                       //make text files with their contents
    {
      fstream writeOut(f.getName(), ios::out);
      char ch;

      string size=f.getSize();
      stringstream s(size);
      int Size=0;
      s>>Size;

      while(Size>0 && tarFile.read ((char *) &ch, sizeof(char)))
      {
        writeOut<<ch;
        Size--;
      }
    }
    else if( !testFile(f.getName()) )                     //create directories if needed
    {
      system( ("mkdir "+f.getName()).c_str() );
    }
    //use touch and chmod to change timestamp and protection mode
    system( ( "touch -t " + f.getStamp() + " " + f.getName() ).c_str() );
    system( ( "chmod " + f.getPmode() + " " + f.getName() ).c_str() );
  }
}
