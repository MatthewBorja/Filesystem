#ifndef TABLE_H
#define TABLE_H
#include "filesys.h"

class Table : public Filesys
{
	public:
		Table(string, int, int, string, string);
		int buildTable(string);
		int search(string);
	private:
		string flatfile;
		string indexfile;
		int indexSearch(string);
};
#endif	/* TABLE_H */
