#include "eliasCoding.hpp"

using namespace std;

void eliasCoding::encode(int num, bool* bitCode){
  if (num == 0){
    bitCode[0] = 0;
    bitCode[1] = 0;
    return;
  }
  else if (num == 1){ //trata exclusivamente os números 0 e 1
    bitCode[0] = 0;
    bitCode[1] = 1;
    return;
  }
  int n = floor(log2(num));
  int n1 = n+1;
  int n2 = num - pow(2,n);
  for (int i=0; i<n; i++){ //calcula a primeira parte em código unário
    bitCode[i] = 1;
  }
  bitCode[n] = 0;
  int ci = n-1;
  for (int i=(n+1); i<=(2*n); i++){ //calcula a segunda parte em código binário
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
  while (bitCode[n] != 0){ //reconhece o código unário
    n++;
  }
  int n2 = 0;
  int ci = n-1;
  for (int i=(n+1); i<=(2*n); i++){ //lê o código binário
    n2 |= ((1 << ci) & ((int) bitCode[i] << ci));
    ci--;
  }
  *num = pow(2,n)+n2;
}

void eliasCoding::encodeAndWrite(int a, int b, int c, string fileName, bool writeTerm){
  int sizA=2*floor(log2(a)) + 1, sizB=2*floor(log2(b)) + 1, sizC=2*floor(log2(c)) + 1;
  if ((a == 0) || (a == 1)) sizA = 2;
  if ((b == 0) || (b == 1)) sizB = 2;
  if ((c == 0) || (c == 1)) sizC = 2;
  bool bA[sizA];
  bool bB[sizB];
  bool bC[sizC];
  int nbytes;
  // cout << "Escrevendo " << a << ", " << b << " e " << c << ". EscreveTermo: ";
  // if (writeTerm)  cout << "SIM\n";
  // else  cout << "NAO\n";

  if (writeTerm){ //calcula o tamanho da escrita, em bytes
    nbytes = ceil((float) (sizA+sizB+sizC)/BYTESIZE);
  }
  else{
    nbytes = ceil((float) (sizB+sizC)/BYTESIZE);
  }
  // cout << "Nbytes: " << nbytes << "\n";
  char bytes[nbytes];
  for (int i=0; i<nbytes; i++)  bytes[i] = (char) 0;
  //memset(bytes,(char) 0,sizeof(char)*nbytes); //seta todos os bits para zero
  if (writeTerm) bytes[0] |= (char) 1 << 7;
  encode(a,bA); //caso vá escrever o termo, codifica A
  // cout << "Coded A: \n";
  // for (int i=0; i<sizA; i++)  cout << bA[i] << " ";
  // cout << "\n";
  encode(b,bB);
  encode(c,bC); //codifica B e C
  int numBit = 0;
  if (writeTerm){
    for (int i=0; i<sizA; i++){
      // cout << "inserindo:\n" << (int) (bA[i] << (numBit%BYTESIZE)) << " no byte " << numBit/BYTESIZE << "\n";
      bytes[numBit/BYTESIZE] |= (char) (bA[i] << (numBit%BYTESIZE)); //insere A, bit por bit
      numBit++;
    }
  }
  for (int i=0; i<sizB; i++){
    bytes[numBit/BYTESIZE] |= (bB[i] << (numBit%BYTESIZE));//insere B, bit por bit
    numBit++;
  }
  for (int i=0; i<sizC; i++){
    bytes[numBit/BYTESIZE] |= (bC[i] << (numBit%BYTESIZE));//insere C, bit por bit
    numBit++;
  }
  ofstream runFile;
  // for (int i=0; i<nbytes; i++){
  //   if ((i > 0) && ((int) bytes[i] < 0)){
  //     cout << (int) bytes[i] << " ";
  //   }
  // }
  // cout << "\n";
  runFile.open(fileName, ios::app | ios::binary); //escreve o resultado em arquivo binário
  runFile.write(bytes,nbytes);
  // for (int i=0; i<nbytes; i++){
  //   char *c = new char;
  //   *c = bytes[i];
  //   delete[] c;
  // }
  runFile.close();
}
