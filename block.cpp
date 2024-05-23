//block.cpp
#include <string>
#include <vector>
#include "block.h"

vector<string> block(string s, int b)		//Blocks the buffer into a list of blocks of size b.
{
	string tempblock;			//Temporary block data.
	vector<string> blocks;			//Vector to hold blocks of data.
	int numberofblocks = 0;			//Initialize variable to 0.
	if(s.length() % b == 0)			//If buffer is perfectly divisible
		numberofblocks = s.length()/b;	//Set numberofblocks to that many blocks.
	else					//Otherwise,
		numberofblocks = s.length()/b + 1;	
	for(int i = 0; i < numberofblocks; i++)
	{
		tempblock = s.substr(i*b, b);
		blocks.push_back(tempblock);
	}
	int lastblock = blocks.size() - 1;
	//Add on a block of '#' signs to last block.
	for(int i = blocks[lastblock].length(); i < b; i++)
	{
		blocks[lastblock] += '#';
	}
	return blocks;
}
