#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <vector>
#include "block.h"
using namespace std;

class Sdisk{
     public:
	     Sdisk(string, int, int);		//Constructor
	     int getBlock(int, string&);
	     int putBlock(int, string);		//Setter Function
	     int getNumberOfBlocks();		//Accessor Function
	     int getBlockSize();		//Accessor Function
	     bool checkDisk();
     private:
	     string diskname;			//File name of software-disk
	     int numberofblocks;		//Number of blocks on disk.
	     int blocksize;			//Block size in bytes -- industry standard: 512
};

#endif /* SDISK_H */
