default: 
	make clean
	make fileSys
	
test: test.o fs.o utils.o
	g++ -o testBench test.o fs.o utils.o
	
fileSys: utils.o function.o fileSys.o fs.o
	g++ -g -o fsTerm fileSys.o fs.o utils.o function.o

test.o:
	g++ -c test.cpp

function.o:
	g++ -g -c function.cpp
	
fileSys.o:
	g++ -g -c fileSys.cpp

fs.o:
	g++ -c fs.cpp

utils.o:
	g++ -c utils.cpp
	
clean:
	rm *.o
