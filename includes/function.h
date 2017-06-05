#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

using namespace std;

void	format		();
void	mkdir		(string curDir, string dirName);
void	ls			(string curDir);
string	cd			(string curDir, string dirName);
void	touch		(string curDir, string fileName);
void	rm			(string curDir, string fileName);
void	write		(string curDir, string fileName, string content, string mode);
void	read		(string curDir, string fileName);

#endif
