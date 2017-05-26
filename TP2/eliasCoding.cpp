#include "eliasCoding.hpp"

using namespace std;

void eliasCoding::encode(int num, bool* bitCode){
  if (num == 0){
    bitCode[0] = 0;
    bitCode[1] = 0;
    return;
  }
  else if (num == 1){
    bitCode[0] = 0;
    bitCode[1] = 1;
    return;
  }
  int n = floor(log2(num));
  int n1 = n+1;
  int n2 = num - pow(2,n);
  // cout << "n2: " << n2 << "\n";
  for (int i=0; i<n; i++){
    bitCode[i] = 1;
  }
  bitCode[n] = 0;
  int ci = n-1;
  for (int i=(n+1); i<=(2*n); i++){
    bitCode[i] = ((n2 & (1 << ci)) != 0);
    ci--;
  }
}

void eliasCoding::decode(int* num, vector<bool> bitCode){
  int n = 0;
  if (bitCode.size() == 2){
    *num = (int) bitCode[1];
    return;
  }
  while (bitCode[n] != 0){
    n++;
  }
  int n2 = 0;
  int ci = n-1;
  for (int i=(n+1); i<=(2*n); i++){
    n2 |= ((1 << ci) & ((int) bitCode[i] << ci));
    // int asd = (int) ((1 << ci) & ((int) bitCode[i] << ci));
    // cout << "bitcode: " << ((int) bitCode[i]) << " 1 shiftado: " << (1 << ci) << " asd " << asd << "\n";
    ci--;
  }
  // cout << "ns: " << n << " " << n2 << "\n";
  *num = pow(2,n)+n2;
}

void eliasCoding::encodeAndWrite(int a, int b, int c, string fileName, bool writeTerm){
  // cout << "aqui!" << a << " " << b << " " << c <<"\n";
  int sizA=2*floor(log2(a)) + 1, sizB=2*floor(log2(b)) + 1, sizC=2*floor(log2(c)) + 1;
  if ((a == 0) || (a == 1)) sizA = 2;
  if ((b == 0) || (b == 1)) sizB = 2;
  if ((c == 0) || (c == 1)) sizC = 2;
  // if ()
  // cout << sizA << " " << sizB << " " << sizC << " aeho\n\n\n";
  bool bA[sizA];
  bool bB[sizB];
  bool bC[sizC];
  int nbytes;
  if (writeTerm){
    nbytes = ceil((float) (sizA+sizB+sizC)/BYTESIZE);
  }
  else{
    nbytes = ceil((float) (sizB+sizC)/BYTESIZE);
  }
  // cout << "nbytes: " << nbytes << "\n";
  char bytes[nbytes];
  memset(bytes,(char) 0,sizeof(char)*nbytes);
  encode(b,bB);
  encode(c,bC);
  int numBit = 0;
  if (writeTerm){
    encode(a,bA);
    bytes[0] |= 1 << 7;
    for (int i=0; i<sizA; i++){
      bytes[numBit/BYTESIZE] |= (bA[i] << (numBit%BYTESIZE));
      // cout << (int) bA[i];
      numBit++;
    }
  }
  // cout << "\nbB: ";
  for (int i=0; i<sizB; i++){
    bytes[numBit/BYTESIZE] |= (bB[i] << (numBit%BYTESIZE));
    // cout << (int) bB[i];
    numBit++;
  }
  // cout << "\nbC: ";
  for (int i=0; i<sizC; i++){
    bytes[numBit/BYTESIZE] |= (bC[i] << (numBit%BYTESIZE));
    // cout << (int) bC[i];// << "\n";
    numBit++;
  }
  // cout << "\n";
  // cout << sizA << "  " << sizB <<  "  " << sizC << "  " << nbytes <<"\n";
  ofstream runFile;
  // stringstream ss;
  // ss << runNum;
  // string fileName = FILERUN + ss.str();
  runFile.open(fileName, ios::app | ios::binary);
  // cout << "trying to save...\n";
  //for (int i=0; i<nbytes; i++){
    // unsigned char cc= bytes[i];
  runFile.write(bytes,nbytes);
    // cout << (int) bytes[i] << " ";
  //}
  runFile.close();
  // cout << "final: \n";
  // cout << "\n";
}


// int main(){
//   ifstream f1,f2;
//   f1.open("compressed");
//   f2.open("uncompressed");
//   int num = 5;
//   int n = log2(num);
//   bool bitCode[2*n + 1];
//   code(num, bitCode);
//   for (int i=0; i<(2*n+1); i++) cout  << bitCode[i];
//   int asd;
//   decode(&asd, bitCode);
//   f1.close();
//   f2.close();
//   // cout << "\n" << asd;
//   return 0;
// }
