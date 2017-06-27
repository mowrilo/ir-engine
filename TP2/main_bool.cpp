#include <iostream>
#include <string>
#include "queryProcessor.hpp"

using namespace std;

int main(){
    queryProcessor qp;
    cout << "\n\nDigite sua consulta, ou \":quit\" para sair: ";
    string consulta;
    // bool coss, atx, prk;
    getline(cin,consulta); //lê a consulta
    // cin >> coss >> atx >> prk;
    while (consulta.compare(":quit") != 0){
        qp.process(consulta); //processa a consulta
        cout << "\n\nDigite sua consulta, ou \":quit\" para sair.: ";
        getline(cin,consulta);
    }
    return 0;
}
