#include <iostream>
#include <fstream>
#include "sdisk.h"

Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize){
	this->diskname = diskname;
	this->numberofblocks = numberofblocks;
	this->blocksize = blocksize;
	//Check if diskname exists
	ifstream diskstream;
	diskstream.open(diskname.c_str());
	bool flag = diskstream.good();
	diskstream.close();
	if (flag){	//Option 1, Disk exists.
		ifstream instream;
		instream.open(diskname.c_str());
		char c;
		int count = 0;
		c = instream.get();		//Read a character.
		while(instream.good()){
			count++;
			c = instream.get();	//Read another character
		}
		if (count == numberofblocks * blocksize){
			cout << "Disk found. Byte count is correct." << endl;
			return;
		}
		else{
			cout << "Disk architecture is wrong." << endl;
		}
	}
	else{		//Option 2, Create Disk
		ofstream ostream;
		ostream.open(diskname.c_str());
		for (int i = 0; i < (numberofblocks*blocksize); i++){
			ostream.put('#');
		}
		cout << "Disk created." << endl;
		return;
	}
	return;
}

int Sdisk::getBlock(int blocknumber, string& buffer){
	if (blocknumber >= numberofblocks || blocknumber < 0){	//The operation failed.
		cout << "Bad block number!" << endl;
		return 0;
	}
	if (buffer.length() > blocksize){
		cout << "Wrong sized buffer!" << endl;
		return 0;
	}
	fstream iostream;
	iostream.open(diskname.c_str(), ios::in | ios::out);
	iostream.seekg(blocknumber * blocksize);	//Beginning of block.
	buffer.clear();			//Clear buffer to ensure it is empty.
	for(int i = 0; i < blocksize; i++){
		char c;
		iostream.get(c);	//Retrieve a character from diskname.
		buffer += c;		//Add character onto the buffer.
	}
	return 1;
}

int Sdisk::putBlock(int blocknumber, string buffer){
	if (blocknumber >= numberofblocks || blocknumber < 0){
		cout << "Bad block number!" << endl;
		return 0;
	}
	if (buffer.length() > blocksize){
		cout << "Wrong sized buffer!" << endl;
		return 0;
	}
	fstream iostream;
	iostream.open(diskname.c_str(), ios::in | ios::out);
	iostream.seekp(blocknumber * blocksize);	//Set the put pointer at the start of block.
	for(int i = 0; i < buffer.length(); i++){
		iostream.put(buffer[i]);	//Input buffer char by char.
	}
	return 1;
}

int Sdisk::getNumberOfBlocks(){
	return numberofblocks;
}

int Sdisk::getBlockSize(){
	return blocksize;
}

bool Sdisk::checkDisk(){
	return true;
}
