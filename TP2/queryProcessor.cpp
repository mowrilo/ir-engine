#include "queryProcessor.hpp"

queryProcessor::queryProcessor(void){}

int queryProcessor::getTermID(string termToFind){
  ifstream voc;
  voc.open("vocabulary");
  string term, entry;
  getline(voc,entry); //lê cada entrada do vocabulário, com nome e id do termo
  int id;
  bool found = voc.eof();
  while (!found && !voc.eof()){
    int pos=0;
    while (entry[pos] != ' ')  pos++;
    term = entry.substr(pos+1,entry.size()); //lê linearmente o vocabulário, até encontrar o termo ou
    id = stoi(entry.substr(0,pos));         // acabar o arquivo
    found=(termToFind.compare(term) == 0);
    getline(voc,entry);
  }
  voc.close();
  if (!found){
    return 0;
  }
  return id;
}

vector<int> queryProcessor::getDocs(string termToFind){
  int termId = getTermID(termToFind); //encontra o ID do termo
  if (termId == 0){
    vector<int> empty;
    return empty;
  }
  run r("invIndex");
  vector<int> ret = r.readAndDecode();
  while ((ret[0] != termId) && (!r.isEOF())){
    ret = r.readAndDecode(); //lê o índice até encontrar o termo em questão
  }
  r.close();
  vector<int> retorno;
  for (int i=0; i<ret.size(); i++){
    if (i%2 == 1) retorno.push_back(ret[i]); //lê todos os documentos da lista
  }
  if (ret[0] != termId){
    vector<int> empty;
    return empty;
  }
  return retorno;
}

string queryProcessor::findDoc(int docId){
  ifstream docs;
  docs.open("docs",ios::in);
  string term, entry;
  getline(docs,entry);
  int pos=0;
  while (entry[pos] != ' ') pos++;
  int id = stoi(entry.substr(0,pos));
  term = entry.substr(pos+1,entry.size());
  while ((id != docId) && (!docs.eof())){ //procura no arquivo de documentos, linearmente
    getline(docs,entry);
    pos=0;
    while (entry[pos] != ' ') pos++;
    id = stoi(entry.substr(0,pos)); //separa o nome da ID
    term = entry.substr(pos+1,entry.size());
  }
  if (docs.eof()){
    return "";
  }
  return term;
  docs.close();
}

vector<int> queryProcessor::mergeTerms(vector<int> term1, vector<int> term2){
  int pos1=0, pos2=0;
  vector<int> all;
  while ((pos1 < term1.size()) && (pos2 < term2.size())){ //mantém dois ponteiros,
    if (term1[pos1] < term2[pos2])  pos1++;               //que é uma boa solução para listas ordenadas.
    else if (term1[pos1] > term2[pos2])  pos2++;
    else{
      all.push_back(term1[pos1]);
      pos1++;
      pos2++;
    }
  }
  return all;
}

void queryProcessor::process(string consulta){
  parser pars;
  pars.normalizeText(consulta);
  vector<string> termos = pars.getTerms(consulta); //normaliza e separa os termos da consulta
  string prev = termos[0];
  vector<int> term1 = getDocs(prev);
  int sum=0;
  for (int i=0; i<term1.size(); i++){
    term1[i]+=sum;
    sum = term1[i];
  }
  for (int i=1; i < termos.size(); i++){
    vector<int> term2 = getDocs(termos[i]); //processa as listas dos termos, um por um,
    sum=0;                                  //fazendo a interseção à medida que busca.
    for (int k=0; k<term2.size(); k++){
      term2[k]+=sum;
      sum = term2[k];
    }
    term1 = mergeTerms(term1, term2);
  }

  if (term1.size() == 0){
    cout << "Termo nao encontrado!\n";
    return;
  }
  cout << "O termo se encontra nos seguintes documentos:\n";
  for (int i=0; i<term1.size(); i++){
      cout << findDoc(term1[i]) << "\n";
  }
  return;
}
