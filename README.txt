Project 2: jtar
Samantha Scholz
Completed March 11, 2021

File:
      -records name, protection mode, size, time stamp, and directory or file 
       information
      -flagAsDir method allows user to set private boolean directory
      -getters for each variable
      
jtar:
      -creates, reads, and extracts tar files
      
      -methods:
      
        -main:
          -takes in commmand line arguments
          -checks for errors in file names and command line input
          
          Possible commands:
            -"cf" : calls makeTar function
            -"tf" : calls printTarNames function
            -"xf" : calls extractTar function
            
        -testFile:
          -takes in a string file name
          -creates an input stream using the file name
          -if the file exists (can be read) it returns true
          
        -execute:
          -runs a recursive "ls" command on the given file or directory name
          -if the file is not a directory it returns the given file
          -if the file is a directory is adds that value to a vector and retrieves the
           children of the directory with a recursive call using the directory path as            the new file name
            
        -isDir:
          -uses the stat command to determine if the given file is a directory
          
        -fillFile:
          -uses stat to obtain a file's protection mode, file size, and date
          -creates a File object with the name, protection mode, file size, date and
           directory status associated with the given file name
          -uses file constructor with 4 arguments and method flagAsDir;
          
        -makeTar:
          -creates Tar file
          -reads through a vector of file names
          -calls fillFile to get create file objects
          -uses fstream to send the file objects to a binary file
          -if the given file is a text file the information it contains is also sent to
           the binary file 
        
        -printTarNames:
          -reads in a binary file
          -it reads in the file size followed by File objects
          -if the file is a text file it ignores the text of the file
          -prints the names of all the File objects contained in the binary file
        
        -extractTar:
          -reads in a binary file
          -if the file is not a directory an fstream object is used to create and fill
           the file
          -if the file is a directory
            -if the directory does not exist then the it is created using "mkdir"
            -otherwise it is ignored
          -changes the time stamp of the created files to their original using "touch"
          -changes the protection mode of the files to their original using "chmod"
