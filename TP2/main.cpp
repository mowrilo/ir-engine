#include <iostream>
#include "fileReader.hpp"
#include "parser.hpp"
#include <fstream>
#include "triple.hpp"
#include <algorithm>
#include "eliasCoding.hpp"
#include <bitset>
#include <cmath>

#define NTHREADS 7
#define PATH_HTMLS "/media/murilo/hdext/collection/"//"/home/murilo/Documentos/ri/TP2/"//
#define FILERUN "runs/run"
#define BYTESIZE 7

int runNum;

using namespace std;

void encodeAndWrite(int a, int b, int c, int runNum){
  cout << "aqui!" << a << " " << b << " " << c <<"\n";
  int sizA=2*floor(log2(a)) + 1, sizB=2*floor(log2(b)) + 1, sizC=2*floor(log2(c)) + 1;
  if ((a == 0)) sizA = 1;
  // if ()
  cout << sizA << " " << sizB << " " << sizC << " aeho\n\n\n";
  bool bA[sizA];
  bool bB[sizB];
  bool bC[sizC];
  int nbytes = ceil((float) (sizA+sizB+sizC)/BYTESIZE);
  cout << "nbytes: " << nbytes << "\n";
  char bytes[nbytes];
  memset(bytes,(char) 0,sizeof(char)*nbytes);
  eliasCoding::encode(a,bA);
  eliasCoding::encode(b,bB);
  eliasCoding::encode(c,bC);
  int numBit = 0;
  bytes[0] |= 1 << 7;
  cout << "\nbA: ";
  for (int i=0; i<sizA; i++){
    bytes[numBit/BYTESIZE] |= (bA[i] << (numBit%BYTESIZE));
    cout << bA[i];
    numBit++;
  }
  cout << "\nbB: ";
  for (int i=0; i<sizB; i++){
    bytes[numBit/BYTESIZE] |= (bB[i] << (numBit%BYTESIZE));
    cout << bB[i];
    numBit++;
  }
  cout << "\nbC: ";
  for (int i=0; i<sizC; i++){
    bytes[numBit/BYTESIZE] |= (bC[i] << (numBit%BYTESIZE));
    cout << bC[i];// << "\n";
    numBit++;
  }
  cout << "\n";
  // cout << sizA << "  " << sizB <<  "  " << sizC << "  " << nbytes <<"\n";
  ofstream runFile;
  stringstream ss;
  ss << runNum;
  string fileName = FILERUN + ss.str();
  runFile.open(fileName, ios::app | ios::binary);
  // cout << "trying to save...\n";
  runFile.write(bytes,nbytes);
  runFile.close();
  cout << "final: \n";
  for (int i=0; i<nbytes; i++){
    unsigned char cc= bytes[i];
    cout << (int) cc << " ";
  }
  cout << "\n";
}

// void readAndDecode()//to be continued...


int main(){
  encodeAndWrite(0,14,1,4);
  fileReader fr(PATH_HTMLS, 1, NTHREADS);
  parser ps;
  int docNum = 1;
  ofstream docs;
  docs.open("docs", ios::out);
  for (int i=0; i<1; i++){
    runNum=i;
    fr.openNextFile();
    vector<string> test = fr.getNextHtml();
    string url = test[0];
    string htmlCode = test[1];
    docs << docNum << " " << url << "\n";
    docNum++;
    int tamanho = htmlCode.size();
    ps.normalizeText(htmlCode);
    unordered_map<int,int> freqs = ps.parse(htmlCode);
    vector<triple> run;
    while (tamanho){
      test = fr.getNextHtml();
      url = test[0];
      htmlCode = test[1];
      docs << docNum << " " << url << "\n";
      docNum++;
      ps.normalizeText(htmlCode);
      if (url.size() > 0) freqs = ps.parse(htmlCode);
      for (unordered_map<int,int>::iterator it=freqs.begin(); it != freqs.end(); it++){
        triple aux(it->first, docNum, it->second);
        run.push_back(aux);
      }
      tamanho = htmlCode.size();
    }
    cout << "beginning sorting...\n";
    sort(run.begin(),run.end());
    cout << "ended sorting!\n";
    cout << "ntriples: " << run.size() << "\n";

    int diffTerm=0, diffDoc=0;
    int prevTerm=0, prevDoc=0;
    for (vector<triple>::iterator it=run.begin(); it != run.end(); ++it){
      diffTerm = it->nterm - prevTerm;
      if (diffTerm == 0){
        diffDoc = it->ndoc - prevDoc;
      }
      else{
        diffDoc = it->ndoc;
      }
      cout << it->nterm << " " << it->ndoc << " " << it->freq << "\n";
      encodeAndWrite(diffTerm,diffDoc,it->freq,runNum);
      prevTerm = it->nterm;
      prevDoc = it->ndoc;
    }

    // cout << "maxDiff: " << maxDiff << "\n";
    // ps.printVoc();
    fr.closeFile();
  }
  docs.close();
  return 0;
}
