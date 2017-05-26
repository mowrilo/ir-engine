#include "run.hpp"

run::run(int nRun){
  stringstream ss;
  ss << nRun;
  string fileName = FILERUN + ss.str();
  cout << "opens " << fileName << "\n";
  runFile.open(fileName,ios::in | ios::binary);
}

vector<int> run::readAndDecode(){
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
  // cout << "bytes lidos:\n" ;//<< (int) *c << "\n";
  char c2 = *c;
  cout << "pushing first: " <<  (int) c2 << "\n";
  cVec.push_back(c2);
  runFile.read(c,1);
  c2=*c;
  // cout << "depois: " << (int) *c << "\n";
  // cout << ((*c & (1 << 7)) == 0) << "\n";
  while (((c2 & (1 << 7)) == 0) && !runFile.eof()){
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
    // cout << "aqui! " << "pos: " << pos << " i: " << i << "\n";
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
    // cout << "vector of size " << boolvec.size() << ":\n";
    // for (vector<bool>::iterator it=boolvec.begin(); it != boolvec.end(); ++it){
    //   cout << (int) *it;
    // }
    // cout  << "\n";
    int auxx = 0;
    eliasCoding::decode(&auxx,boolvec);
    ret.push_back(auxx);
//    nums[k] = auxx;
  //  cout << "\nnum: " << auxx << "\n";
  }
  cout << "decoded: " << ret[0] <<" " << ret[1] << " " << ret[2] <<"\n";
  // else{
  //   cout << "error!\n"
  // }
  //cout << c << "\n";
  // runFile.close();
  return ret;
}//to be continued...

bool run::isEOF(){
  return runFile.eof();
}

void run::close(){
  runFile.close();
}
