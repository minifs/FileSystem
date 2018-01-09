# Mini Filesystem(minifs)  
[![Build Status](https://api.travis-ci.org/minifs/FileSystem.svg?branch=master)](https://travis-ci.org/minifs/FileSystem/)  

### team 7 @ NCTU-OS  
<img src="https://i.imgur.com/v5BbSpX.png" width="120" height="50">  
Mini Filesystem is a easy way to simply implement filesystem on Linux OS.   
This minifs is the ext2 deformation.   
This is the final project of team 7 at NCTU CS OS class.   

# Installation  
```sh
$ git clone https://github.com/minifs/FileSystem.git
$ cd FileSystem
$ make
```

# Usage  
```sh
>help
**********Command List**********
1.ls [Dir]...                    list files under directories.
2.cd [Dir]                       change to [Dir].
3.cp [file]...[Dir]              copy files to [Dir].
4.rm [file]...                   remove files.
5.rmdir [Dir]...                 remove directories.
6.touch [file]...                create files.
7.mkdir [Dir]...                 create directories.
8.mv [file] [filename]           rename the file.
9.mvdir [Dir] [directoryname]    rename the directory.
10.cat [file]...                 read files.
11.pwd                           print the current directory path.
12.echo [content] > [file]       write content to the file.
13.find [Dir] [file/Dir]         find specific file or directory
```

---

# Development  
## Environment Requirement  
- Ubuntu 16.04  

## Tools Requirement  
- CMake >= 3.0.0  
- gcc >= 4.8

## Build  

## Coding Style
`$ astyle --style=kr <fliename>`

## Test  
Using `gtest`  
