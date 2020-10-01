// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "test.h"
#include "Experiments.h"
#include "main.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string dir4 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_11.txt";
const string file = "UK200_01.txt";

int main() {
	int op = 1;
	if (op == 0) {
		perm_rep alg;
		//alg.test_routePartition();
		//alg.test_opt2_route();
		//alg.test_neigh();
		//alg.test_shiftCustomer();
		//alg.test_localSearch_r();
		//alg.test_shakeRandom_r(4);
		//alg.test_VNS();		
		//alg.test_GRASP();
		//alg.test_SA();
		//alg.test_addStations();
		//alg.test_swapDepot();
		alg.test_changeDepot();
		//alg.test_permutationToSolution();
		//alg.test_permutationToSolutionAlt();
		//alg.test_splitRoute();
		//alg.test_new_evaluation();
	}
	if (op == 1) {
		//exp_sa(dir3);
		//exp_sa(dir4);
		//exp_model(dir3);
		//exp_vns(dir4);
		exp_vnsl(dir4);
		//exp_vns2(dir4); // correto
		//exp_vns3(dir4);
		//exp_vns3(dir4);
		//genBigMatrix(dir3);
		//exp_instances(dir3);
		//exp_brkga(dir3);
		//exp_sa(dir3);
		//exp_grasp(dir3);
		//exp_instances(dir3);
	}	
	else if (op == 2) {
		Solution init;
		perm_rep alg;
		alg.loadInstance(dir3, file, 3);
		alg.printInstance();

		try {			
			init = alg.testPermutation({}); // UK10_01.txt // optmal

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
		model.optmize(-1);
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
		Solution s = alg.sA(1000, 100, 100, 10, 600);
		s.debug(cout);
		cout << endl;
		return 0;

	}
	else if (op == 5) {
		perm_rep alg;
		alg.test_swapDepot();
	}
	else if (op == 6){
		perm_rep alg;
		alg.loadInstance(dir4, file, 3);
		alg.printInstance();
		vector<string> BSSs = { "Shipley",
								"Mexborough",
								"Wombwell",
								"Staveley",
								"Horbury",
								"Kearsley",
								"Shirebrook",
								"Stoke-on-Trent",
								"Liverpool",
								"Kimberley",
								"Knaresborough",
								"Litherland",
								"North_Ferriby",
								"Alsager",
								"Ashby_de_la_Zouch",
								"Cotgrave",
								"Denton",
								"Golborne",
								"Kirkham",
								"Northwich" };
		vector<string> depot = {"Royston"};
		Solution s = alg.VNSL(BSSs, 25, 600);
		s.debug(cout);
		cout << endl;
		return 0;
	}
	else if (op == 7) {
		//adapt_model(dir4);
		//adaptAll_n();
		adaptAll("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/");
		return 0;
	}

}
