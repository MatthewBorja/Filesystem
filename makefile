myproject: table.o shell.o filesys.o sdisk.o block.o  main.o
	c++ -o myproject table.o shell.o filesys.o sdisk.o block.o main.o
table.o: table.cpp
	c++ -c table.cpp
shell.o: shell.cpp
	c++ -c shell.cpp
filesys.o: filesys.cpp
	c++ -c filesys.cpp
sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp
block.o: block.cpp
	c++ -c block.cpp
main.o: main.cpp
	c++ -c main.cpp

clean:
	rm *.o
