default:
	make clean
	make fs
	
fs: fileSys.o fs.o utils.o
	g++ -o fsTerm fileSys.o fs.o utils.o

fileSys.o:
	g++ -c fileSys.cpp

fs.o:
	g++ -c fs.cpp

utils.o:
	g++ -c utils.cpp
	
	
clean:
	rm fsTerm fileSys.o fs.o
