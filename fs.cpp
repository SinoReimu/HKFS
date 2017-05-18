#include <string>
#include <fstream>

using namespace std;

fstream init_disk(string fileName) {
	return fstream(fileName, ios::binary|ios::in|ios::out);
}
