#include <fstream>
#include "readFile.h"
#include <iostream>


using namespace std;

int parseFile(const char* filename)
{
  ifstream in;
  in.open(filename);
  if (!in.is_open()) {
    cerr << "Could not open file " << filename << endl;
    return 0;
  }
  string str;
  do {
    getline(in, str);
    cout << "Got line:  " << str << endl;
  } while (getline(in, str));
}
