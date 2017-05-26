#include <iostream>
#include <bitset>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <memory>

#define BYTESIZE 7

using namespace std;

namespace eliasCoding{
  void encode(int num, bool* bitCode);
  void decode(int* num, vector<bool> bitCode);
  void encodeAndWrite(int a, int b, int c, string fileName, bool writeTerm);
  // vector<int> readAndDecode(ifstream* runFile);
}
