// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
//#include "Experiments.h"

using namespace std;

const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";

int main() {
	cout << "1 - Models \n";
	cout << "2 - algorithms \n";
	int op = 2;
	//cin >> op;
	if (op == 1) {
		//expModels();
	}
	else if (op == 2) {
		Model model(dir3, "UK10_01.txt", 0, 3);

		/*
		perm_rep alg;
		alg.loadInstance(dir3, "UK10_02.txt", 3);
		alg.printInstance();
		alg.test4();
		*/
		
		//exp_SA();
	}



	return 0;
}
