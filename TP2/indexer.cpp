#include "indexer.hpp"

#define NTHREADS 7

using namespace std;

ofstream indexer::docs;
int indexer::docNum;
mutex indexer::docsFile;

indexer::indexer(){
}

void indexer::encodeAndWrite(int a, int b, int c, int runNum){
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
  eliasCoding::encode(a,bA);
  eliasCoding::encode(b,bB);
  eliasCoding::encode(c,bC);
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

void indexer::start(string path_to_collection){
  docs.open("docs", ios::out);
  docNum=1;
  vector<thread> thrds;
  for (int i=0; i<NTHREADS; i++){ //Inicia as threads sem semente
    thrds.push_back(thread(index, path_to_collection, i));
  }
  for (int i=0; i<NTHREADS; i++){
    thrds[i].join();
    cout << "joining thread " << i << "...\n";
  }
  docs.close();
}

void indexer::readAndDecode(int runNum){
  ifstream runFile;
  stringstream ss;
  ss << runNum;
  string fileName = FILERUN + ss.str();
  runFile.open(fileName, ios::binary);
  char *c = new char;
  //shared_ptr<char> c(new char);
  vector<char> cVec;
  runFile.read(c,1);
  cout << fileName << "\n";
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
    eliasCoding::decode(&auxx,boolvec);
    nums[k] = auxx;
    cout << "\nnum: " << auxx << "\n";
  }

  // else{
  //   cout << "error!\n"
  // }
  //cout << c << "\n";
  runFile.close();
}//to be continued...


void indexer::index(string path_to_collection,int threadid){
  // encodeAndWrite(0,114,1,4);
  // encodeAndWrite(0,14,1,4);
  // readAndDecode(4);
  // readAndDecode(4);
  fileReader fr(path_to_collection, threadid, NTHREADS);
  parser ps;
  int runNum = threadid;
  while(fr.openNextFile()){
    vector<string> test = fr.getNextHtml();
    string url = test[0];
    string htmlCode = test[1];
    docsFile.lock();
    docs << docNum << " " << url << "\n";
    docNum++;
    docsFile.unlock();
    int tamanho = htmlCode.size();
    ps.normalizeText(htmlCode);
    unordered_map<int,int> freqs = ps.parse(htmlCode);
    vector<triple> run;
    while (tamanho){
      test = fr.getNextHtml();
      url = test[0];
      htmlCode = test[1];
      docsFile.lock();
      docs << docNum << " " << url << "\n";
      docNum++;
      docsFile.unlock();
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
    runNum+=NTHREADS;
  }
  // docs.close();
  // return 0;
}
