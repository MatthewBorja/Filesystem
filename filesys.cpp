//filesys.cpp
#include "filesys.h"
#include <iostream>
#include <sstream>
using namespace std;

Filesys::Filesys(string diskname, int numberofblocks, int blocksize) : Sdisk(diskname, numberofblocks, blocksize){
	//Filename: s.char, blocknumber s digits
	rootsize = getBlockSize() / 12;
	fatsize = (getNumberOfBlocks() * 6 / getBlockSize()) + 1; //Add in extra block in case we need it.
	string buffer;
	getBlock(1, buffer);		//Get first block and put it in the buffer. Supposed to be the root directory.
	if(buffer[0] == '#'){		//If '#', there is no root directory!
		int code = buildFs();	//Build the file system. Capture return value in code
	} else {	//Read file system.
		int code = readFs();
	}
}

//This module writes FAT and ROOT to the sdisk (closing the sdisk).
int Filesys::fsClose(){
	fsSynch();
	return 0;
}

//This function adds an entry for the string file in ROOT with an initial first block of 0 (empty)
//It returns error codes of 1 if successful and 0 otherwise (no room or file already exists).
int Filesys::newFile(string file){
	int firstblock1 = getFirstBlock(file);
	if (firstblock1 == -1){
		for (int i = 0; i < filename.size(); i++){
			if (filename[i] == "XXXXX"){
				filename[i] = file;
				firstblock[i] = 0;
				fsSynch();
				return 1;
			}
		}
	} else {
		cout << "File: '" << file << "' already exists!" << endl;
		return 0;
	}
	return 0;
}

//This function removes the entry file from ROOT if the file is empty (first block is 0).
//It returns error codes of 1 if successful and 0 otherwise (not empty or file does not exist).
int Filesys::rmFile(string file){
	for (int i = 0; i < rootsize; i++){
		if(filename[i] == file){
			if(firstblock[i] != 0){
				cout << "File not empty!" << endl;
				return 0;
			} else {
				filename[i] = "XXXXX";
				fsSynch();
				return 1;
			}
		}
	}
	return 0;
}

//This function returns the block number of the first block in file.
//It return the first block of the given filename. Return -1 if file is not found.
int Filesys::getFirstBlock(string file){
	for (int i = 0; i < filename.size(); i++){
		if(filename[i] == file){
			return firstblock[i];
		}
	}
	return -1;	//File not found. Return -1.
}

//This function adds a block of data stored in the string buffer to the end of file F and
//returns the block number. It returns error code 0 if the file does not exist, and returns -1
//if there are no available blocks (file system is full!).
int Filesys::addBlock(string file, string buffer){
	int block = getFirstBlock(file);
	if (block == -1){
		cout << "Error: File exists!" << endl;
		return 0;
	}
	//File exists:
	int allocate = fat[0];
	if (allocate == 0){
		cout << "Error: No space!" << endl;
		return 0;
	}
	//Update fat[0] to next free block after allocated block.
	fat[0] = fat[fat[0]];
	fat[allocate] = 0;
	//Update the links:
	if (block == 0){//Update the ROOT
		for (int i = 0; i < filename.size(); i++){
			if (filename[i] == file){
				//Found the index:
				firstblock[i] = allocate;
				break;			//Break out of For loop and If
			}
		}
	}
	else {//Go through the links and update the list.
		while (fat[block] != 0){
			block = fat[block];
		}
		fat[block] = allocate;
	}
	putBlock(allocate, buffer);	//Writes data block to the disk at blocknumber(allocate).
	fsSynch();
	return allocate;
}

//This function removes block numbered blocknumber from file and returns an error code of 1 if successful,
//0 otherwise.
int Filesys::delBlock(string file, int blocknumber){
	if (checkFileBlock(file, blocknumber)){
		int deallocate = blocknumber;
		if (blocknumber == getFirstBlock(file))
		{	//Update the ROOT
			for (int i = 0; i < filename.size(); i++)
			{
				if (file == filename[i]) //Found the file
				{
					firstblock[i] = fat[blocknumber];
					break;
				}
			}
		} else {//blocknumber != firstblock
			int b = getFirstBlock(file);
			while (fat[b] != blocknumber){
				b = fat[b];
			}
			fat[b] = fat[blocknumber];	//fat[blocknumber] block that comes after blocknumber
		}
		fat[blocknumber] = fat[0];
		fat[0] = blocknumber;
		fsSynch();
		return 1;
	} else {//blocknumber does not belong to file
		return 0;
	}
}

//Gets block numbered blocknumber from file and stores the data in the string buffer. It returns
//an error code of 1 if successful and 0 otherwise.
int Filesys::readBlock(string file, int blocknumber, string& buffer){
	if (checkFileBlock(file, blocknumber)){
		getBlock(blocknumber, buffer);
		return 1;
	} else {//Didn't work. Blocknumber did not belong to file.
		cout << "Error! Blocknumber does not belong to file!" << endl;
		return 0;
	}
}

//Writes the buffer to the block numbered blocknumber in file. It returns an appropriate error code.
int Filesys::writeBlock(string file, int blocknumber, string buffer){
	if(checkFileBlock(file, blocknumber)){
		return putBlock(blocknumber, buffer);
	}
	return 0;
}

//Returns the number ofthe block that follows blocknumber in file. It will return 0 if blocknumber
//is the last block and -1 if some other error has occurred (such as file is not in the root directory,
//or blocknumber is not a block in file.)
int Filesys::nextBlock(string file, int blocknumber){
	if (checkFileBlock(file, blocknumber)){
		return fat[blocknumber];
	} else {
		return -1;
	}
}

//Returns true if b is a block in file.
bool Filesys::checkFileBlock(string file, int b){
	int block = getFirstBlock(file);
	while (block != 0){
		if (block == b){
			return true;
		}
		block = fat[block];
	}
	return false;
}

// builds the file system
int Filesys::buildFs(){
	for(int i = 0; i < rootsize; i++){
		filename.push_back("XXXXX");	//Create root directory.
		firstblock.push_back(0);	//Placeholder to signify open slot.
	}
	fat.push_back(fatsize + 2);		//fat[0] = fatsize + 2;
	fat.push_back(-1);			//Reserved for root.
	for(int i = 0; i < fatsize; i++){
		fat.push_back(-1);
	}
	for(int i = fatsize + 2; i < getNumberOfBlocks(); i++){
		fat.push_back(i + 1);
	}
	fat[getNumberOfBlocks() - 1] = -1;
	fsSynch();				//Now it will be on the disk.
	return 1;				//End of buildFs()
}

// reads the file system
int Filesys::readFs(){
	istringstream instream1;
	istringstream instream2;
	string buffer1, buffer2, b;
	getBlock(1, buffer1);			//Buffer1 contains root directory now.
	for(int i = 0; i < fatsize; i++){
		getBlock(2+i, b);			//FAT begins in block #2 and goes up through (2+fatsize-1)
		buffer2 += b;				//Entire FAT in buffer2.
	}
	instream1.str(buffer1);			//Input device, reading from buffer 1.
	instream2.str(buffer2);
	//Reading in the root directory.
	for(int i = 0; i < rootsize; i++){
		string s;			//Read in file name.
		int k;				//Read in first block.
		instream1 >> s >> k;
		filename.push_back(s);
		firstblock.push_back(k);
	}
	for(int i = 0; i < getNumberOfBlocks(); i++){
		int k;
		instream2 >> k;			//Read in another value
		fat.push_back(k);		//Push value to FAT.
	}
	return 1;				//End of readFs()
}

//Writes the FAT and ROOT to the Sdisk.
int Filesys::fsSynch(){
	ostringstream ostream1;	//Root
	for(int i = 0; i < filename.size(); i++){
		ostream1 << filename[i] << " " << firstblock[i] << " ";	//blanks separate two data.
	}
	ostringstream ostream2;	//FAT
	for(int i = 0; i < fat.size(); i++){
		ostream2 << fat[i] << " ";
	}
	//Convert both ostreams to single string.
	string buffer1 = ostream1.str();
	string buffer2 = ostream2.str();
	//Block both buffers. Gives us blocks:
	vector<string> blocks1 = block(buffer1, getBlockSize());	//One block
	vector<string> blocks2 = block(buffer2, getBlockSize());	//many blocks
	putBlock(1, blocks1[0]);
	for(int i = 0; i < blocks2.size(); i++){
		putBlock(2 + i, blocks2[i]);
	}
	return 1;
}

//List the files in the file system.
vector<string> Filesys::ls(){
	vector<string> flist;
	for (int i=0; i<filename.size(); i++){
        if (filename[i] != "XXXXX"){
			flist.push_back(filename[i]);
        }
    }
	return flist;
}

