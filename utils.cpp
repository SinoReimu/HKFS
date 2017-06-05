#include <string>
#include <vector>

using namespace std;

void split(string& s, const string& delim, vector<string>* ret) {
	size_t  last = 0;
	size_t index=s.find_first_of(delim,last);
	int size = s.length();
	if(index==0) {
		index=s.find_first_of(delim, 1);
		last+=1;	
	}
	while (index!=string::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (size-last>0)
		ret->push_back(s.substr(last,size-last));
}
