// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "test.h"
#include "Experiments.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_11.txt";
const string file = "UK10_20.txt";

int main() {
	int op = 1;
	if (op == 0) {
		perm_rep alg;
		//alg.test_routePartition();
		//alg.test_opt2_route();
		//alg.test_neigh();
		//alg.test_shiftCustomer();
		//alg.test_localSearch_r();
		//alg.test_shakeRandom_r(3);
		//alg.test_VNS();		
		alg.test_GRASP();
		//alg.test_addStations();
		//alg.test_permutationToSolution();
	}
	if (op == 1) {
		exp_sa(dir3);
		//exp_model(dir3);
		//exp_brkga(dir3);

	}	
	else if (op == 2) {
		Solution init;
		perm_rep alg;
		alg.loadInstance(dir3, file, 3);
		alg.printInstance();

		try {
			
			init = alg.testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal
			//init = alg.testPermutation({ 6, 11, 5, 9, 8, 7, 10 }); // UK10_11.txt // optmal
			//init = alg.testPermutation({ 7, 10, 8, 11, 9, 5, 6 }); // UK10_11.txt
			//init = alg.testPermutation({ 10, 7, 8, 5, 11, 9, 6 }); // UK10_11.txt			
			//init = alg.testPermutation({ 6, 11, 5, 9, 7, 10, 8 }); // UK10_11.txt
			//init = alg.testPermutation({ 7, 9, 5, 8, 11, 6, 10 }); // UK10_11.txt
			//init = alg.testPermutation({ 7, 10, 8, 5, 6, 11, 9 }); // UK10_11.txt
			//init = alg.testPermutation({ 6, 7, 11, 9, 5, 8, 10 }); // UK10_11.txt
			//init = alg.testPermutation({ 77, 169, 108, 149, 66, 116, 140, 176, 102, 165, 126, 90, 206, 86, 65, 84, 139, 103, 89, 150, 81, 127, 63, 162, 109, 106, 144, 78, 57, 159, 101, 105, 138, 198, 136, 123, 68, 155, 132, 96, 141, 171, 154, 195, 82, 203, 192, 59, 180, 207, 164, 181, 166, 129, 184, 178, 205, 173, 131, 110, 148, 115, 146, 191, 61, 107, 122, 157, 152, 170, 156, 188, 151, 95, 201, 168, 145, 83, 119, 130, 135, 204, 174, 74, 91, 208, 185, 99, 177, 163, 69, 88, 93, 124, 182, 172, 113, 70, 98, 104, 161, 94, 175, 73, 193, 79, 62, 71, 183, 128, 158, 186, 75, 187, 112, 64, 153, 80, 60, 194, 202, 125, 118, 114, 97, 196, 92, 85, 100, 143, 67, 111, 160, 117, 58, 189, 137, 190, 179, 87, 76, 147, 199, 134, 142, 121, 200, 197, 167, 133, 120, 72 }); // UK200_01.txt
			//init = alg.testPermutation({ 72, 61, 179, 73, 113, 152, 100, 105, 155, 144, 188, 156, 142, 184, 204, 200, 59, 197, 127, 137, 90, 64, 62, 68, 171, 121, 162, 126, 154, 83, 98, 82, 193, 174, 109, 159, 132, 114, 69, 106, 140, 81, 163, 74, 117, 92, 108, 77, 80, 129, 148, 196, 143, 96, 67, 177, 165, 122, 118, 110, 190, 101, 125, 128, 86, 138, 135, 123, 79, 136, 146, 191, 192, 160, 153, 158, 173, 112, 102, 116, 199, 91, 168, 185, 166, 89, 198, 161, 93, 175, 172, 169, 88, 167, 60, 84, 208, 195, 176, 149, 76, 94, 151, 201, 95, 70, 124, 133, 145, 203, 97, 157, 58, 164, 202, 107, 207, 85, 139, 183, 205, 181, 78, 147, 180, 87, 57, 111, 178, 189, 119, 194, 66, 130, 170, 131, 71, 134, 120, 65, 104, 141, 103, 75, 186, 150, 115, 99, 63, 182, 206, 187 }); // UK200_11.txt
			//init = alg.testPermutation({ 15, 6, 8, 10, 7, 12, 16, 9, 11, 13, 14 }); // UK15_11.txt
			//init = alg.testPermutation({ 13, 9, 14, 11, 16, 12, 15, 6, 8, 10, 7 }); // UK15_11.txt
			//init = alg.testPermutation({ 15, 6, 8, 10, 7, 12, 9, 13, 14, 11, 16 }); // UK15_11.txt
			//init = alg.testPermutation({ 113, 152, 72, 144, 105, 155, 200, 156, 204, 138, 100, 184, 142, 188, 179, 61, 59, 197, 127, 137, 90, 64, 69, 106, 140, 81, 163, 74, 83, 109, 159, 132, 114, 126, 154, 117, 92, 108, 77, 80, 129, 62, 68, 171, 121, 162, 151, 94, 180, 201, 174, 193, 82, 98, 96, 67, 177, 165, 116, 199, 91, 168, 122, 118, 110, 190, 101, 125, 173, 112, 102, 79, 123, 135, 160, 153, 158, 89, 166, 185, 192, 136, 146, 191, 161, 93, 175, 172, 169, 88, 167, 198, 73, 124, 107, 202, 133, 203, 145, 58, 164, 76, 157, 149, 176, 195, 208, 84, 60, 97, 205, 181, 78, 147, 87, 57, 86, 128, 207, 85, 139, 183, 95, 70, 71, 131, 189, 75, 103, 186, 141, 134, 104, 65, 120, 143, 148, 196, 170, 130, 66, 194, 119, 178, 111, 150, 99, 115, 63, 206, 182, 187 }); // UK200_11.txt


			//init.debug(cout);

			// UK10_11.txt
			/*
			init.routes.at(0).at(5).key = 13;
			init.routes.at(0).at(5).n.key = 13;
			init.routes.at(0).at(5).n.type = "f_d";

			// UK15_11.txt

			init.routes.at(0).at(4).key = 20;
			init.routes.at(0).at(4).n.key = 20;
			init.routes.at(0).at(4).n.type = "f_d";
			*/

			//
			init.debug(cout);
		}
		catch (PermutationInf e) {
			cout << e.what() << endl;
		}

		return 0;


	}
	else if (op == 3) {
		Model model(dir3, file, 0, 3);
		model.printInst();
		//model.setIS(init);
		model.optmize(0);
		Solution s1 = model.getSolution();
		s1.saveXML("test.xml");
		s1.debug(cout);
		cout << endl;
		return 0;

	}
	else if (op == 4) {
		perm_rep alg;
		alg.loadInstance(dir3, file, 3);
		alg.printInstance();
		Solution s = alg.sA(1000, 100, 100, 50, 300);

		s.debug(cout);
		cout << endl;
		return 0;

	}
	else if (op == 5) {
		adaptAll();
		return 0;
	}

}
