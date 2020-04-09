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
	int op = 0;
	if (op == 0) {
		//exp_sa(dir3);
		exp_model(dir3);
	}
	if (op == 1) {

		perm_rep alg;
		alg.loadInstance(dir3, "UK10_11.txt", 3);
		alg.printInstance();		
		Solution init = alg.sA(1000, 100, 100, 50, 300);
		
		/*
		*/
		try {
			//Solution init = alg.testPermutation({ 7, 10, 8, 11, 9, 5, 6 }); // UK10_11.txt
			Solution init = alg.testPermutation({ 10, 7, 8, 5, 11, 9, 6 }); // UK10_11.txt
			//Solution init = alg.testPermutation({ 6, 11, 5, 9, 8, 7, 10 }); // UK10_11.txt
			//Solution init = alg.testPermutation({ 7, 9, 5, 8, 11, 6, 10 }); // UK10_11.txt
			//Solution init = alg.testPermutation({ 7, 10, 8, 5, 6, 11, 9 }); // UK10_11.txt
			//Solution init = alg.testPermutation({ 6, 7, 11, 9, 5, 8, 10 }); // UK10_11.txt
			//Solution init = alg.testPermutation({ 77, 169, 108, 149, 66, 116, 140, 176, 102, 165, 126, 90, 206, 86, 65, 84, 139, 103, 89, 150, 81, 127, 63, 162, 109, 106, 144, 78, 57, 159, 101, 105, 138, 198, 136, 123, 68, 155, 132, 96, 141, 171, 154, 195, 82, 203, 192, 59, 180, 207, 164, 181, 166, 129, 184, 178, 205, 173, 131, 110, 148, 115, 146, 191, 61, 107, 122, 157, 152, 170, 156, 188, 151, 95, 201, 168, 145, 83, 119, 130, 135, 204, 174, 74, 91, 208, 185, 99, 177, 163, 69, 88, 93, 124, 182, 172, 113, 70, 98, 104, 161, 94, 175, 73, 193, 79, 62, 71, 183, 128, 158, 186, 75, 187, 112, 64, 153, 80, 60, 194, 202, 125, 118, 114, 97, 196, 92, 85, 100, 143, 67, 111, 160, 117, 58, 189, 137, 190, 179, 87, 76, 147, 199, 134, 142, 121, 200, 197, 167, 133, 120, 72 }); // UK200_01.txt
			init.debug(cout);
		}
		catch (PermutationInf e) {
			cout << e.what() << endl;
		}
		
		
		return 0;
		//init.routes.at(0).at(5).key = 13;
		//init.routes.at(0).at(5).n.key = 13;
		//init.routes.at(0).at(5).n.type = "f_d";

		//return 0;

		Model model(dir3, "UK10_11.txt", 0, 3);
		//model.setIS(init);
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
