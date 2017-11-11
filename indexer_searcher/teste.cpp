#include<bits/stdc++.h>
#include <iostream>
using namespace std;

char replace (int a){
	if (a > -97 && a < -91)	return 'a';
	else if (a > -89 && a < -85)	return 'e';
	else if (a > -84 && a < -80)	return 'i';
	else if (a > -79 && a < -73)	return 'o';
	else if (a > -72 && a < -67)	return 'u';
	else return '{';
}

int main(){
	char a1;
	ifstream file;
	file.open("teste");
	file.get(a1);
	if ((int) a1 == -61){
		file.get(a1);
		cout << (int) a1 << "\n";
	}
	file.close();
	//string asd;
	//cin >> asd;
	char c;
	while ()
	cout << asd.size() << "\n";
	for (int i=0; i<asd.size(); i++){
		if ((int) asd[i] == -61){
			// cout << i << "\n";
			//cout << i << " " <<(int) asd[i+1] << " " << replace((int) asd[i+1]) << "\n";
			asd[i+1] = replace((int) asd[i+1]);
			asd.erase(asd.begin()+i);
		}
		//cout << (int) asd[i] << "\n";
	}
	cout << asd.size() << "\n";
	cout << asd << "\n";
	//while (!cin.eof()){
	//	int a = a1;
	//	if (a == -61){
	//		cin >> a1;
	//		cout << (char) abs((int) a1) << "\n";
	//	}
	//	else 		cout << "no aew =(\n";
	//	//cout << a << "\n";
	//	cin >> a1;
	//}
	return 0;
}
