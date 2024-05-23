//shell.cpp
#include <iostream>
#include "shell.h"

using namespace std;

Shell::Shell(string diskname, int numberofblocks, int blocksize) : Filesys(diskname, numberofblocks, blocksize){

}

//This function will list all the files in the root directory.
int Shell::dir(){
	vector<string> flist=ls();
	for (int i = 0; i < flist.size(); i++)
	{
		cout << flist[i] << endl;
	}
	return 1;
}

//Add a new file using buffer as data.
int Shell::add(string file, string buffer){
	int fb = getFirstBlock(file);	//Get the first block in the file.
	if (fb >= 0)
	{
		cout << "File exists!" << endl;
		return 0;	//Function failed.
	}
	int code = newFile(file);
	if (code == 0)	//Checks to see if any space left in ROOT.
	{
		cout << "No space left!" << endl;
		return 0;
	}
	vector<string> blocks = block(buffer, getBlockSize());
	for (int i = 0; i < blocks.size(); i++)
	{	//Add buffer to file one block at a time.
		int c = addBlock(file, blocks[i]);
	}
	return 1;
}


//Delete a file.
int Shell::del(string file){
	//Check to see if file exists.
	if (getFirstBlock(file) == -1)
	{
		cout << "File doesn't exist!" << endl;
		return 0;
	}
	//Delete first block of file, check if next block is 0. Keep deleting until last block.
	while (getFirstBlock(file) != 0){
		delBlock(file, getFirstBlock(file));
	}
	rmFile(file);
	return 1;
}

//Lists the contents of a file.
int Shell::type(string file){
	int b = getFirstBlock(file);
	if (b == -1)
	{
		cout << "File doesn't exist!" << endl;
		return 0;
	}
	if (b == 0)
	{
		return 1;
	}
	while (b != 0)
	{
		string buffer;
		readBlock(file, b, buffer);
		cout << buffer << endl;
		b = nextBlock(file, b);
	}
	cout << endl;
	return 1;
}

//Copies file 1 to file 2.
int Shell::copy(string file1, string file2){
	//Check file 1 exists and file 2 does not exist.
	int code = newFile(file2);
	if (code == 0)
	{
		cout << "Error: No space left in ROOT is likely!" << endl;
		return 0;
	}
	int b = getFirstBlock(file1);
	while (b != 0)	//While b is not the last block in the file.
	{
		string buffer;
		//Read block from file1,
		readBlock(file1, b, buffer);
		//Add block to file2.
		addBlock(file2, buffer);
		//Assign the next block.
		b = nextBlock(file1, b);
	}
	return 1;
}
