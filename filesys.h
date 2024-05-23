#ifndef FILESYS_H
#define FILESYS_H

#include "sdisk.h"
using namespace std;
class Filesys : public Sdisk {
public:
	Filesys(string, int, int);
	int fsClose();
	int newFile(string);
	int rmFile(string);
	int getFirstBlock(string);
	int addBlock(string, string);
	int delBlock(string, int);
	int readBlock(string, int, string &);
	int writeBlock(string, int, string);
	int nextBlock(string, int);
	vector<string> ls();
private:
	int rootsize;			//Maximum number of entries in ROOT.
	int fatsize;			//Number of blocks occupied by FAT.
	vector<string> filename;	//Filenames in ROOT.
	vector<int> firstblock;		//first blocks in ROOT
	vector<int> fat;		//FAT
	bool checkFileBlock(string, int);//Returns 
	int buildFs();			//Builds the file system.
	int readFs();			//Reads the file system.
	int fsSynch();			//Writes the FAT and ROOT to the sdisk.
};
#endif /* FILESYS_H */
