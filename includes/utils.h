#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

const string COMMAND_SPLIT = " ";
const string PATH_SPLIT = "/";

void split(string& s, const string& delim,vector<string >* ret);

#endif
