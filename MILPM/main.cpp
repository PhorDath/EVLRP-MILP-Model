// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "Experiments.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_01.txt";

int main() {

	//exp_model(dir3);
	//exp_sa(dir3);

	//return 0;
	int op = 1;
	if (op == 1) {
		perm_rep alg;
		alg.loadInstance(dir3, "UK200_01.txt", 3);
		//alg.printInstance();
		Solution init = alg.sA(1000, 100, 100, 50, 300);
		init.debug(cout);

		/*
		perm_rep alg;
		alg.loadInstance(dir3, "UK10_11.txt", 3);
		alg.printInstance();
		
		Solution init = alg.sA(1000, 100, 100, 50, 300);

		init.debug(cout);
		
		init.routes.at(0).at(5).key = 13;
		init.routes.at(0).at(5).n.key = 13;
		init.routes.at(0).at(5).n.type = "f_d";

		//Solution init = alg.testPermutation({ 6, 11, 5, 9, 8, 7, 10 });
		//Solution init = alg.testPermutation({ 7, 9, 5, 8, 11, 6, 10 });
		//Solution init = alg.testPermutation({ 7, 10, 8, 5, 6, 11, 9 });
		//Solution init = alg.testPermutation({ 6, 7, 11, 9, 5, 8, 10 });
		//init.debug(cout);

		//init.routes.at(0).at(5).key = 13;
		//init.routes.at(0).at(5).n.key = 13;
		//init.routes.at(0).at(5).n.type = "f_d";
		
		//return 0;

		Model model(dir3, "UK10_11.txt", 0, 3);
		model.setIS(init);
		model.optmize(0);
		Solution s1 = model.getSolution();
		
		s1.debug(cout);
		cout << endl;
		
		cout << "----------------------------------------------------\n";

		perm_rep a;
		a.loadInstance(dir3, "UK10_11.txt", 3);
		Solution s2 = a.procSol(s1);
		s2.debug(cout);
		cout << endl;
		*/

		return 0;
	}	
	if (op == 2) {
		perm_rep alg;
		alg.loadInstance(dir3, "UK10_11.txt", 3);
		//alg.printInstance();

		Solution s2 = alg.sA(1000, 100, 100, 50, 300);
		s2.debug(cout);
		//s2.saveXML("test.xml");

		//return 0;
	}

}
