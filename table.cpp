//table.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include "table.h"
using namespace std;
//Create a new (empty) files flatfile and indexfile in the file system on the Sdisk using diskname.
Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile) : Filesys(diskname, numberofblocks, blocksize)
{
	this->flatfile = flatfile.substr(0, 5);
	this->indexfile = indexfile.substr(0, 5);
	if (newFile(flatfile) && newFile(indexfile)) {
		cout << "Building table..." << endl;
		buildTable("data.txt");
	}
}

//This function will read records from the input file, add the records to the flatfile and create
//index records consisting of the date and blocknumber, then add the index records to the index
//file.
int Table::buildTable(string input_file)
{
	//Check if files were created
	ifstream infile;
	infile.open(input_file.c_str(), ios::in);
	string rec;
	ostringstream outstream;
	getline(infile, rec);
	while(infile.good())
	{
		string key = rec.substr(0,5);
		cout << key << endl;
		//blocks[0] = rec#####
		vector<string> blocks = block(rec, getBlockSize());
		//Add block[0] into flat file
		int b = addBlock(flatfile, blocks[0]);
		cout << b << endl;
		//Output key and blocknumber.
		outstream << key << " " << b << " ";
		getline(infile, rec);
	}
	//Insert a fake record to tell us it is the last one.
	outstream << "Kangaroo" << " " << 0 << " ";	//No historical date matches kangaroo.
	string buffer = outstream.str();
	//For use in the indexfile
	vector<string> blocks = block(buffer, getBlockSize());
	for (int i = 0; i < blocks.size(); i++)
	{
		addBlock(indexfile, blocks[i]);
	}
	return 1;
}


//This function accepts a key value and searches the index file with a call to indexSearch for
//the record where the date matches the specified value. indexSearch returns the blocknumber
//of the block in the flatfile where the target record is located. This block should then be
//read and the record displayed.
int Table::search(string key)
{
	string buffer;
	int b = indexSearch(key);
	if (b != -1){
		readBlock(flatfile, b, buffer);	//Get record into buffer.
	}
	cout << buffer << endl;
	return 1;
}

//This function accepts a key value and searches the index file indexfile for the record where
//the date matches the specified value. indexSearch then returns the blocknumber key of the
//index record where the match occurs.
int Table::indexSearch(string key)
{
	istringstream instream;
	string buffer, buffer1;
	int b = getFirstBlock(indexfile);
	while (b != 0)
	{//Process block b
		readBlock(indexfile, b, buffer1);
		buffer += buffer1;
		buffer1.clear();	//Clear the temporary buffer.
		b = nextBlock(indexfile, b);
	}
	instream.str(buffer);		//indexfile now in istringstream or input stream.
	string tempkey;
	int date;
	instream >> tempkey >> date;	//Read first index record.
	while (tempkey != "Kangaroo")
	{
		if (tempkey == key)
		{
			return date;
		}
		instream >> tempkey >> date;
	}
	cout << "Error: " << key << " was not found!" << endl;
	return -1;		//Was 0.
}
