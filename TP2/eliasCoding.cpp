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
  cout << "ns: " << n << " " << n2 << "\n";
  *num = pow(2,n)+n2;
}

void eliasCoding::encodeAndWrite(int a, int b, int c, int runNum){
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
  int nbytes = ceil((float) (sizA+sizB+sizC)/BYTESIZE);
  // cout << "nbytes: " << nbytes << "\n";
  char bytes[nbytes];
  memset(bytes,(char) 0,sizeof(char)*nbytes);
  encode(a,bA);
  encode(b,bB);
  encode(c,bC);
  int numBit = 0;
  bytes[0] |= 1 << 7;
  // cout << "\nbA: ";
  for (int i=0; i<sizA; i++){
    bytes[numBit/BYTESIZE] |= (bA[i] << (numBit%BYTESIZE));
    // cout << (int) bA[i];
    numBit++;
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
  stringstream ss;
  ss << runNum;
  string fileName = FILERUN + ss.str();
  runFile.open(fileName, ios::app | ios::binary);
  // cout << "trying to save...\n";
  for (int i=0; i<nbytes; i++){
    // unsigned char cc= bytes[i];
    runFile.write(bytes+i,1);
    // cout << (int) bytes[i] << " ";
  }
  runFile.close();
  // cout << "final: \n";
  // cout << "\n";
}

vector<int> eliasCoding::readAndDecode(ifstream runFile){
  // ifstream runFile;
  vector<int> ret;
  // stringstream ss;
  // ss << runNum;
  // string fileName = FILERUN + ss.str();
  // runFile.open(fileName, ios::binary);
  char *c = new char;
  //shared_ptr<char> c(new char);
  vector<char> cVec;
  runFile.read(c,1);
  // cout << fileName << "\n";
  cout << "bytes lidos:\n" ;//<< (int) *c << "\n";
  char c2 = *c;
  cout << (int) c2 << "\n";
  cVec.push_back(c2);
  runFile.read(c,1);
  c2=*c;
  // cout << "depois: " << (int) *c << "\n";
  // cout << ((*c & (1 << 7)) == 0) << "\n";
  while (((*c & (1 << 7)) == 0) && !runFile.eof()){
    cout << (int) c2 << "\n";
    cVec.push_back(c2);
    // cout << (int) *c << "\n";
    runFile.read(c,1);
    c2=*c;
  }
  if (!runFile.eof()) runFile.unget();
  delete[] c;
  // bool *bA, *bB, *bC;
  int nums[3], i=0;
  //for (int ii=0; ii<cVec.size(); ii++)  cout<<(int) cVec[ii]<<"\n";
  // while ()
  int by = 0;
  char c1 = cVec[0];
  // if ((c1 & 1) == 0)
  //i=0;
  for (int k=0; k<3; k++){
    int pos=0;
    vector<bool> boolvec;
    int aux=i;
    while ((c1 & (1 << i)) != 0){
      boolvec.push_back(true);
      i++;
      if (i == 7){
        by++;
        c1 = cVec[by];
        i = 0;
      }
      pos++;
    }
    boolvec.push_back(false);
    if(((c1 & (1 << aux)) == 0)){
      pos++;
    }
    i++;
    int n=pos;
    cout << "aqui! " << "pos: " << pos << " i: " << i << "\n";
    for (int count=0; count<n; count++){
      boolvec.push_back((c1 & (1 << i)) != 0);
      // cout << "pushado: " << (int) ((c1 & (1 << i)) != 0) << "  " << count << "\n";
      i++;
      if (i == 7){
        by++;
        c1 = cVec[by];
        i = 0;
      }
      pos++;
    }
    cout << "vector of size " << boolvec.size() << ":\n";
    for (vector<bool>::iterator it=boolvec.begin(); it != boolvec.end(); ++it){
      cout << (int) *it;
    }
    cout  << "\n";
    int auxx = 0;
    decode(&auxx,boolvec);
    ret.push_back(auxx);
//    nums[k] = auxx;
//    cout << "\nnum: " << auxx << "\n";
  }
  // else{
  //   cout << "error!\n"
  // }
  //cout << c << "\n";
  // runFile.close();
  return ret;
}//to be continued...
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
