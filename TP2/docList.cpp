#include "docList.hpp"

using namespace std;
//
documentList::documentList(){
    docNum = 1;
    domNum = 1;
    // vector<int> pageWeights;
    // vector<int> domainWeights;
    for (int i=0; i<300; i++){
        pageWeights.push_back(rand()%500);
        domainWeights.push_back(rand()%500);
    }
}

string documentList::getUrlDomain(string url){
    int posFim=0, nbarras=0;
    // cout << "url: " << url << "\n";// domaintoadd: " << domain <<"\n";
    // cout << "url[0]: " << url[posFim] <<"\n";
    while ((nbarras < 3) && (url[posFim] != '\0')){ //Vai até a posição da terceira barra, considerando que as duas primeiras estão em "http://"
    // while (nbarras < 3){
        // cout << "oi\n";
        if (url[posFim] == '/')    nbarras++;
        posFim++;
    }
    // int posBr = isBr(url);//Se for .br, a posição de referência passa a ser o .br
    // int i=posFim-1;
    // int j=posFim-1;
    // if (posBr > 0){
    //     i = posBr;
    //     j = posBr;
    // }
    // i--;
    // while ((url[i] != '.') && (i>0)) i--; //Anda-se para trás, até o próximo ponto '.'
    // if (i > url.length()) return ""; //Retorna URL vazia para evitar exceções de limites da string
    // string subUrl = url.substr(i,(j-i)); //Pega a próxima parte do domínio
    // bool isdmn = isDomain(subUrl);                //e checa se faz parte dos padrões de domínio
    // if (isdmn){
    //     i--;
    //     while ((url[i] != '.') && (url[i] != '/')) i--; //Vai até o próximo ponto ou barra
    // }
    // i++;
    // if (i > url.length()) return "";//Retorna URL vazia para evitar exceções de limites da string
    return url.substr(0,posFim); //Retorna o domínio
}

void documentList::addLength(int doc, int leng){
    // ofstream len;
    // len.open("docs/len", ios::out | ios::app);
    // len << doc << " " << leng << "\n";
    // len.close();
    map<int,int>::iterator it = lengths.find(doc);
    if (it != lengths.end()){
        pair<int,int> ins(doc,leng);
        lengths.insert(ins);
    }
    // unordered_map<int,int>::iterator it = lengths.find(doc);
    // pair<int,int> insere(doc,leng);
    // lengths.insert(insere);
    // if (it == lengths.end()){
    // }
}

void documentList::addUrl(string url){
    // if (url.size() == 0)    cout << "GRITO\n";

    string domain = getUrlDomain(url);
    int keyPg = hashFunc(url, pageWeights);
    int keyDom = hashFunc(domain, domainWeights);
    unordered_map<int,int>::iterator itDom = domains.find(keyDom);
    // cout << "exists: " << (itDom == domains.end()) << "\n";
    if (itDom == domains.end()){
        int num = domNum;
        domNum++;
        // cout << num;
        pair<int,int> adic(keyDom, num);
        domains.insert(adic);
        int fileNumber = num/1000;
        ofstream arq;
        stringstream ss;
        ss << "docs/domains/" << fileNumber;
        string fileName = ss.str();
        arq.open(fileName, ios::out | ios::app);
        arq << num << " " << domain << "\n";
        arq.close();
    }

    unordered_map<int,int>::iterator itPg = pages.find(keyPg);
    if (itPg == pages.end()){
        int num = docNum;
        docNum++;
        pair<int,int> adic(keyPg, num);
        pages.insert(adic);
        int fileNumber = num/1000;
        ofstream arq;
        stringstream ss;
        ss << "docs/urls/" << fileNumber;
        string fileName = ss.str();
        arq.open(fileName, ios::out | ios::app);
        arq << num << " " << url << "\n";
        arq.close();

        ofstream fai;
        itDom = domains.find(keyDom);
        string murilo = "docs/mapPgDom/";
        murilo.append(to_string(fileNumber));
    	fai.open(murilo, ios::out | ios::app);
    	fai << num << " " << itDom->second << "\n";
        fai.close();
    }

    // cout << "keydom: " << keyDom << "\n";


    // unordered_map<int,string>::iterator it = urls.find(doc);
    // if (it == urls.end()){
    //     pair<int,string> insere(doc,url);
    //     urls.insert(insere);
    // }
}

int documentList::getDocId(string name){
    int keyPg = hashFunc(name, pageWeights);
    unordered_map<int,int>::iterator it = pages.find(keyPg);
    if (it == pages.end())    return -1;
    return it->second;
}

void documentList::addEdge(string docUrl, vector<string> edges, vector<int> edNums){
    // arqDom.open("docs/pagerankDom", ios::out | ios::app);
    string domain = getUrlDomain(docUrl);
    int keyPg = hashFunc(docUrl, pageWeights);
    int keyDom = hashFunc(domain, domainWeights);
    unordered_map<int,int>::iterator it = pages.find(keyPg);
    int pgNum = it->second;
    unordered_map<int,int>::iterator itDom = domains.find(keyDom);
    int domnNum = itDom->second;
    int fileNumberPg = pgNum/1000;
    int fileNumberDom = pgNum/1000;

    unordered_set<int> edgesPg;
    unordered_set<string> edgesDoms;
    for (int i=0; i<edNums.size(); i++){
        if (edgesPg.find(edNums[i]) == edgesPg.end()){
            edgesPg.insert(edNums[i]);
        }
        string dd = getUrlDomain(edges[i]);
        if (edgesDoms.find(dd) == edgesDoms.end()){
            edgesDoms.insert(dd);
        }
    }
    // pair<int,int> fPg(pgNum, edgesPg.size());
    // fowardPages.insert(fPg);
    unordered_map<int,unordered_set<int> >::iterator itFow = fowPageRank.find(domnNum);
    if (itFow == fowPageRank.end()){
        // if (domnNum == 4142)    cout << "aqui!!\n\n\n";
        unordered_set<int> auxxx;
        for (unordered_set<string>::iterator marcia = edgesDoms.begin(); marcia != edgesDoms.end(); marcia++){
            int marciaKey = hashFunc(*marcia,domainWeights);
            unordered_map<int,int>::iterator gerson = domains.find(marciaKey);
            int auxMarcia = gerson->second;
            if (auxxx.find(auxMarcia) == auxxx.end()){
                auxxx.insert(auxMarcia);
            }
        }
        pair<int,unordered_set<int> > fDom(domnNum, auxxx);
        fowPageRank.insert(fDom);
    }
    else{
        for (unordered_set<string>::iterator marcia = edgesDoms.begin(); marcia != edgesDoms.end(); marcia++){
            int marciaKey = hashFunc(*marcia,domainWeights);
            unordered_map<int,int>::iterator gerson = domains.find(marciaKey);
            int auxMarcia = gerson->second;
            if (itFow->second.find(auxMarcia) == itFow->second.end()){
                itFow->second.insert(auxMarcia);
            }
        }
        // itFow->second += edgesDoms.size();
    }

    for (unordered_set<string>::iterator itEdge = edgesDoms.begin(); itEdge != edgesDoms.end(); ++itEdge){
        // string edgeDomain = getUrlDomain(edges[i]);
        int edgeDKey = hashFunc(*itEdge,domainWeights);
        it = domains.find(edgeDKey);
        // int edgeDomain = *it
        unordered_map<int, unordered_set<int> >::iterator itDom = domainPageRank.find(it->second);
        if (itDom == domainPageRank.end()){
            unordered_set<int> usAux;
            pair<int,unordered_set<int> > aux(it->second,usAux);
            // cout << "adding domain number: " << domnNum << "\n";
            domainPageRank.insert(aux);
        }
        itDom = domainPageRank.find(it->second);
        unordered_set<int>::iterator findDom = itDom->second.find(domnNum);
        if (findDom == itDom->second.end()){
            itDom->second.insert(domnNum);
        }
    }

    // for (int i=0; i<edges.size(); i++){
    //     // int edgePKey = hashFunc(edges[i],pageWeights);
    //     // unordered_map<int,int>::iterator it = pages.find(edgePKey);
    //     // // int edgeDomain = *it
    //     // unordered_map<int, unordered_set<int> >::iterator itPage = pagePageRank.find(it->second);
    //     // if (itPage == pagePageRank.end()){
    //     //     unordered_set<int> usAux;
    //     //     pair<int,unordered_set<int> > aux(it->second,usAux);
    //     //     // cout << "adding domain number: " << domnNum << "\n";
    //     //     pagePageRank.insert(aux);
    //     // }
    //     // itPage = pagePageRank.find(it->second);
    //     // unordered_set<int>::iterator findPage = itPage->second.find(pgNum);
    //     // if (findPage == itPage->second.end()){
    //     //     itPage->second.insert(pgNum);
    //     // }
    //
    //     string edgeDomain = getUrlDomain(edges[i]);
    //     int edgeDKey = hashFunc(edgeDomain,domainWeights);
    //     it = domains.find(edgeDKey);
    //     // int edgeDomain = *it
    //     unordered_map<int, unordered_set<int> >::iterator itDom = domainPageRank.find(it->second);
    //     if (itDom == domainPageRank.end()){
    //         unordered_set<int> usAux;
    //         pair<int,unordered_set<int> > aux(it->second,usAux);
    //         // cout << "adding domain number: " << domnNum << "\n";
    //         domainPageRank.insert(aux);
    //     }
    //     itDom = domainPageRank.find(it->second);
    //     unordered_set<int>::iterator findDom = itDom->second.find(domnNum);
    //     if (findDom == itDom->second.end()){
    //         itDom->second.insert(domnNum);
    //     }
    // }

    // unordered_map<int, unordered_set<int> >::iterator itPR = domainPageRank.find(domnNum);
    // if (itPR == domainPageRank.end()){
    //     unordered_set<int> usAux;
    //     pair<int,unordered_set<int> > aux(domnNum,usAux);
    //     // cout << "adding domain number: " << domnNum << "\n";
    //     domainPageRank.insert(aux);
    // }
    //
    // // cout << "pgnum: " << pgNum << "\n";
    // unordered_map<int, unordered_set<int> >::iterator itPRPG = pagePageRank.find(pgNum);
    // if (itPRPG == pagePageRank.end()){
    //     unordered_set<int> usAux;
    //     pair<int,unordered_set<int> > aux(pgNum,usAux);
    //     // cout << "adding domain number: " << domnNum << "\n";
    //     pagePageRank.insert(aux);
    // }
    // cout << "aehoo2 searching for domain: " << domnNum << "\n";
    // itPR = domainPageRank.find(domnNum);
    // itPRPG = pagePageRank.find(pgNum);
    // cout << "isPresent: " << (itPR == domainPageRank.end()) << " domain: " << domain <<    "\n";
    // vector<int> domNums;
    // for (int i=0; i<edges.size(); i++){
    //     // if (edges[i].compare("http://kiind.nl") == 0) cout << docUrl << "\n";// << edges[i] << "\n";
    //     string domEdg = getUrlDomain(edges[i]);
    //     int keyDomEdg = hashFunc(domEdg, domainWeights);
    //     int keyPageEdg = hashFunc(edges[i], pageWeights);
    //     unordered_map<int,int>::iterator itPgEdg = pages.find(keyPageEdg);
    //     int pgNumEdg = itPgEdg->second;
    //     // cout << "size: " << itPRPG->second.size() << "pgnumedg: " << pgNumEdg << " aehoo1\n";
    //     unordered_set<int>::iterator rubens = itPRPG->second.find(pgNumEdg);
    //     // cout << "morreu =(\n";
    //     if (rubens == itPRPG->second.end()){
    //         itPRPG->second.insert(pgNumEdg);
    //     }
    //     // cout << "dominio: " << domEdg << "size: " << domEdg.size() << "\n";
    //     unordered_map<int,int>::iterator itDomEdg = domains.find(keyDomEdg);
    //     int domnNumEdg = itDomEdg->second;
    //     domNums.push_back(domnNumEdg);
    //     unordered_set<int>::iterator itEdg = itPR->second.find(domnNumEdg);
    //     if (itEdg == itPR->second.end()){
    //         itPR->second.insert(domnNumEdg);
    //     }
    // }
    // cout << "pgrankdom size: " << domainPageRank.size() << "\n";
    // cout << "pgrankpg size: " << pagePageRank.size() << "\n";
    // cout << "aehoo3\n";
    // stringstream ss;
    // ss << "docs/PageRankPg/" << fileNumberPg;
    // string fileName = ss.str();
    // ofstream arq;//, arqDom;
    // arq.open(fileName, ios::out | ios::app);
    // arq << pgNum;
    // for (vector<int>::iterator itNum=edNums.begin(); itNum!=edNums.end();    ++itNum){
    //     arq << " " << *itNum;
    // }
    // arq << "\n";
    // arq.close();
    //
    // stringstream ss1;
    // ss1 << "docs/PageRankDom/" << fileNumberDom;
    // fileName = ss1.str();
    // ofstream arq1;//, arqDom;
    // arq1.open(fileName, ios::out | ios::app);
    // arq1 << domnNum;
    // for (vector<int>::iterator itNum=domNums.begin(); itNum!=domNums.end();    ++itNum){
    //     arq1 << " " << *itNum;
    // }
    // arq1 << "\n";
    // arq1.close();
    // unordered_map<int,vector<string>>::iterator it = pageRankGraph.find(doc);
    // if (it == pageRankGraph.end()){
    //     vector<string> edges;
    //     edges.push_back(edge);
    //     pair<int,vector<string> > ins(doc,edges);
    //     pageRankGraph.insert(ins);
    // }
    // else{
    //     it->second.push_back(edge);
    // }
}

// int documentList::isBr(string url){
//     string br = ".br/"; //Checa se existe o padrão ".br" na URL
//     int find = url.find(br);
//     if (find == string::npos) find=-1;
//     return find;
// }

// float calcPPRValue(int npage, unordered_set<int> blinks){
//     float val = 1 - DFACT;
//     for (unordered_set<int>iterator it = blinks.begin(); it != blinks.end(); it++){
//         float pr
//     }
// }

void documentList::calcPagePR(){
    //Inicia...
    cout << "calculating page pagerank...\n";
    int prSize = pages.size();
    vector<float> values(prSize, 1);
    cout << "size of pages: " << prSize << "\n";
    // vector<float> prevValues(prSize, 1);
    int maxIt = 20;
    // float change;
    int iter = 0;
    while (iter < maxIt){
        iter++;
        float difference = 0;
        for (unordered_map<int, unordered_set<int> >::iterator it = pagePageRank.begin(); it != pagePageRank.end(); ++it){
            int npage = it->first;
            unordered_set<int> backlinks = it->second;
            // cout << "npage: " << npage << "\n";
            float prevPr = values[npage-1];
            float val = 1 - DFACT;
            for (unordered_set<int>::iterator itBL = backlinks.begin(); itBL != backlinks.end(); itBL++){
                // cout << "backlink: " << *itBL << "\n";
                float pr = values[*itBL - 1];
                unordered_map<int,int>::iterator findN = fowardPages.find(*itBL);
                float nFL = findN->second;
                // if (nFl == 0)
                val += DFACT*(pr/nFL);
            }
            values[npage-1] = val;//calcPPRValue(npage, backlinks);
            difference += abs(val - prevPr);
        }
        // for (int i=0; i<values.size())
        if (difference < 0.001)    break;
    }

    for (int i=0; i<values.size(); i++){
        string folderName = "docs/PageRankPg/values/";
        int nOfFile = (i+1)/1000;
        folderName.append(to_string(nOfFile));
        ofstream arq;
        arq.open(folderName, ios::out | ios::app);
        arq << (i+1) << " " << values[i] << "\n";
    }
    cout << "done!\n";
}

void documentList::calcDomPR(){
    //Inicia...
    cout << "calculating domain pagerank...\n";
    int domSize = domains.size();
    vector<double> values(domSize, 1);
    cout << "size of pages: " << domSize << "\n";
    // vector<double> prevValues(prSize, 1);
    int maxIt = 20;
    // double change;
    int iter = 0;
    while (iter < maxIt){
        iter++;
        double difference = 0;
        for (unordered_map<int, unordered_set<int> >::iterator it = domainPageRank.begin(); it != domainPageRank.end(); ++it){
            int npage = it->first;
            unordered_set<int> backlinks = it->second;
            // cout << "npage: " << npage << "\n";
            double prevPr = values[npage-1];
            double val = 1 - DFACT;
            for (unordered_set<int>::iterator itBL = backlinks.begin(); itBL != backlinks.end(); itBL++){
                // cout << "backlink: " << *itBL << "\n";
                double pr = values[*itBL - 1];
                // cout << "oi\n";
                unordered_map<int, unordered_set<int> >::iterator findN = fowPageRank.find(*itBL);
                // cout << "oi2\n";
                // cout << (findN == fowardDoms.end()) << "\n";
                double nFL = (double) findN->second.size();
                // cout << "oi3\n";
                double add = DFACT*(pr/nFL);

                if (isinf(add)){
                    add = 0;
                }
                //if (nFL > 0){
                val += add;
                //}
            }
            if (!isinf(val)){
                values[npage-1] = val;//calcPPRValue(npage, backlinks);
            }
            difference += abs(val - prevPr);
        }
        // for (int i=0; i<values.size())
        if (difference < 0.001)    break;
    }

    for (int i=0; i<values.size(); i++){
        string folderName = "docs/PageRankDom/values/";
        int nOfFile = (i+1)/1000;
        folderName.append(to_string(nOfFile));
        ofstream arq;
        arq.open(folderName, ios::out | ios::app);
        arq << (i+1) << " " << values[i] << "\n";
    }
    cout << "done!\n";
}

int documentList::hashFunc(string name, vector<int> weights){
    int n=0;
    int i=0;
    // cout << "weights size: " << weights.size() << "\n";
    for (char c: name){
        n+=c*weights[i];
        // cout << "oi daqui de dentro\n";
        i++;
    }
    return n;
}

// void documentList::addAnchor(string term, string doc){
//     unordered_map<string,vector<string> >::iterator it = anchorTerms.find(term);
//     if (it == anchorTerms.end()){
//         //map<string,int> anchors;
//         vector<string> vec;
//         vec.push_back(doc);
//         pair<string,vector<string> > par(term,vec);
//         anchorTerms.insert(par);
//         // pair<int,map<string,int> > ins(doc,anchors);
//         // pageRankGraph.insert(ins);
//     }
//     else{
//         it->second.push_back(doc);
//         // map<string,int>::iterator it2 = it->second.find(anchor);
//         // if (it2 == it->second.end()){
//         //     pair<string,int> bar(anchor,1);
//         //     it->second.insert(bar);
//         // }
//         // else{
//         //     it2.second++;
//         // }
//     }
// }

void documentList::writeDomPR(){
    ofstream arq;
    arq.open("docs/pagerankDom", ios::out | ios::app);
    for (unordered_map<int, unordered_set<int> >::iterator it = domainPageRank.begin(); it != domainPageRank.end(); it++){
        int domOrig = it->first;
        unordered_set<int> domDests = it->second;
        arq << domOrig;
        for (unordered_set<int>::iterator iit = domDests.begin(); iit != domDests.end(); iit++){
            arq << " " << *iit;
        }
        arq << "\n";
    }
}

// void documentList::writeAll(){
//     ofstream len,url,pr,at;
//     len.open("docs/lengths", ofstream::out | ofstream::app);
//     url.open("docs/urls", ofstream::out | ofstream::app);
//     pr.open("docs/pagerank", ofstream::out | ofstream::app);
//     at.open("docs/anchortext", ofstream::out | ofstream::app);
//
//     // len << "asd";
//     for (unordered_map<int,int>::iterator it = lengths.begin(); it != lengths.end(); it++){
//         len << it->first << " " << it->second << "\n";
//     }
//     //
//     for (unordered_map<int,string>::iterator it=urls.begin(); it!=urls.end(); it++){
//         url << it->first << " " << it->second << "\n";
//     }
//     //
//     for (unordered_map<int,vector<string> >::iterator it=pageRankGraph.begin(); it!=pageRankGraph.end(); it++){
//         pr << it->first;
//         vector<string> aux = it->second;
//         for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
//             pr << " " << *iit;
//         }
//         pr << "\n";
//     }
//
//     for (unordered_map<string,vector<string> >::iterator it=anchorTerms.begin(); it!=anchorTerms.end(); it++){
//         at << it->first;
//         vector<string> aux = it->second;
//         for (vector<string>::iterator iit=aux.begin(); iit != aux.end(); iit++){
//             at << " " << *iit;
//         }
//         at << "\n";
//     }
//
//     len.close();
//     url.close();
//     pr.close();
//     at.close();
// }
