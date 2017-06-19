#include <iostream>
#include <bitset>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

#define BYTESIZE 7 //um dos bits de cada byte é reservado como flag para indicar início de cada entrada

using namespace std;

namespace eliasCoding{
  void encode(int num, bool* bitCode); //codifica um inteiro em uma sequência de booleanos
  void decode(int* num, vector<bool> bitCode); //decodifica uma sequẽncia de booleanos em um inteiro
  void encodeAndWrite(int a, int b, int c, string fileName, bool writeTerm); //recebe uma tripla, codifica e escreve em 'fileName'.
}                                               //writeTerm define se o numero do termo será escrito
