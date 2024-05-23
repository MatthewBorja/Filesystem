#ifndef SHELL_H
#define SHELL_H
#include "filesys.h"
class Shell : public Filesys
{
	public:
		Shell(std::string, int, int);
		int dir();			//Lists all files.
		int add(std::string, std::string);	//Add a new file using buffer as data.
		int del(std::string);		//Deletes the file.
		int type(std::string);		//Lists the contents of file.
		int copy(std::string, std::string);	//Copies file1 to file2.
};
#endif /* SHELL_H */
