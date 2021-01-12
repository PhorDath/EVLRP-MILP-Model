// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "test.h"
#include "Experiments.h"
#include "Adaptation.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string dir4 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/25/";
const string dir_brelrp = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/";
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
		//alg.test_changeDepot();
		//alg.test_permutationToSolution();
		//alg.test_permutationToSolutionAlt();
		//alg.test_splitRoute();
		//alg.test_new_evaluation();
		//alg.test_bssReplacement_r();
		alg.test_unionRoute();
	}
	if (op == 1) {
		//string region = "alto_paranaiba";
	
		//exp(dir_brelrp, "alto_paranaiba", 20);
		//exp(dir_brelrp, "alto_paranaiba", 50);
		//exp(dir_brelrp, "alto_paranaiba", 80);

		//exp(dir_brelrp, "mata", 20);
		//exp(dir_brelrp, "mata", 50);
		//exp(dir_brelrp, "mata", 80);

		exp(dir_brelrp, "rio_doce", 20);
		//exp(dir_brelrp, "rio_doce", 50);
		//exp(dir_brelrp, "rio_doce", 80);

		//exp(dir_brelrp, "triangulo", 20);
		//exp(dir_brelrp, "triangulo", 50);
		//exp(dir_brelrp, "triangulo", 80);

		//exp(dir_brelrp, "jequitinhonha_mucuri", 20);
		//exp(dir_brelrp, "jequitinhonha_mucuri", 50);
		//exp(dir_brelrp, "jequitinhonha_mucuri", 80);

		//exp(dir_brelrp, "sul_de_minas", 20);
		//exp(dir_brelrp, "sul_de_minas", 50);
		//exp(dir_brelrp, "sul_de_minas", 80);

		//exp(dir_brelrp, "central", 20);
		//exp(dir_brelrp, "central", 50);
		//exp(dir_brelrp, "central", 80);

		/*
		string s = "jequitinhonha_mucuri/old";
		exp_vns2(dir_brelrp + s + "/20/");
		exp_vns2(dir_brelrp + s + "/50/");
		exp_vns2(dir_brelrp + s + "/80/");
		*/

		//exp_vns2(dir_brelrp + "alto_paranaiba/20/");
		//exp_vns2(dir_brelrp + "alto_paranaiba/50/");
		//exp_vns2(dir_brelrp + "alto_paranaiba/80/");

		//exp_vns2(dir_brelrp + "centro_oeste/20/");
		//exp_vns2(dir_brelrp + "centro_oeste/50/");
		//exp_vns2(dir_brelrp + "centro_oeste/80/");

		
		//exp_vns2(dir_brelrp + "rio_doce/20/");
		//exp_vns2(dir_brelrp + "rio_doce/50/");
		//exp_vns2(dir_brelrp + "rio_doce/80/");

		//exp_vns2(dir_brelrp + "sul_de_minas/20/");
		//exp_vns2(dir_brelrp + "sul_de_minas/50/");
		//exp_vns2(dir_brelrp + "sul_de_minas/80/");

		//exp_vns2(dir_brelrp + "triangulo/20/");
		//exp_vns2(dir_brelrp + "triangulo/50/");
		//exp_vns2(dir_brelrp + "triangulo/80/");

		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/20/");
		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/50/");
		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/80/");

		//exp_vns2(dir_brelrp + "mata/20/");
		//exp_vns2(dir_brelrp + "mata/50/");
		//exp_vns2(dir_brelrp + "mata/80/");

		//exp_vns2(dir_brelrp + "central/20/");
		//exp_vns2(dir_brelrp + "central/50/");
		//exp_vns2(dir_brelrp + "central/80/");

		//exp_vns2(dir_brelrp + "alto_paranaiba/80/");
		//exp_vns2(dir_brelrp + "centro_oeste/80/");
		//exp_vns2(dir_brelrp + "rio_doce/80/");
		//exp_vns2(dir_brelrp + "sul_de_minas/80/");
		//exp_vns2(dir_brelrp + "triangulo/80/");
		//exp_vns2(dir_brelrp + "mata/80/");
		//exp_vns2(dir_brelrp + "central/80/");

		//exp_vnsl(dir_brelrp + "alto_paranaiba/20/");
		//exp_vnsl(dir_brelrp + "alto_paranaiba/50/");
		//exp_vnsl(dir_brelrp + "alto_paranaiba/80/");

		//exp_vnsl(dir_brelrp + "centro_oeste/20/");
		//exp_vnsl(dir_brelrp + "centro_oeste/50/");
		//exp_vnsl(dir_brelrp + "centro_oeste/80/");

		//exp_vnsl(dir_brelrp + "rio_doce/20/");
		//exp_vnsl(dir_brelrp + "rio_doce/50/");
		//exp_vnsl(dir_brelrp + "rio_doce/80/");

		//exp_vnsl(dir_brelrp + "sul_de_minas/20/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/50/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/80/");

		//exp_vnsl(dir_brelrp + "triangulo/20/");
		//exp_vnsl(dir_brelrp + "triangulo/50/");
		//exp_vnsl(dir_brelrp + "triangulo/80/");

		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/20/");
		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/50/");
		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/80/");

		//exp_vnsl(dir_brelrp + "mata/20/");
		//exp_vnsl(dir_brelrp + "mata/50/");
		//exp_vnsl(dir_brelrp + "mata/80/");

		//exp_vnsl(dir_brelrp + "central/20/");
		//exp_vnsl(dir_brelrp + "central/50/");
		//exp_vnsl(dir_brelrp + "central/80/");

		//exp_vnsl(dir_brelrp + "alto_paranaiba/80/");
		//exp_vnsl(dir_brelrp + "centro_oeste/80/");
		//exp_vnsl(dir_brelrp + "rio_doce/80/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/80/");
		//exp_vnsl(dir_brelrp + "triangulo/80/");
		//exp_vnsl(dir_brelrp + "mata/80/");
		//exp_vnsl(dir_brelrp + "central/80/");

		//exp_sa(dir3);
		//exp_sa(dir4);
		//exp_model(dir3);
		//exp_model(dir_brelrp + "50/");
		//exp_vns(dir4);

		//exp_model(dir_brelrp + "25/");
		//for (int i = 0; i < 10; i++)
		//	exp_vns2(dir_brelrp + "central/50/"); // correto

		//for (int i = 0; i < 11; i++)
		//	exp_vnsl(dir_brelrp + "central/50/");
		//exp_lowerbound(dir_brelrp + "50/", "mg50_01.brelrp");
		//exp_lowerbound(dir3, "UK10_01.txt.ukelrp");


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
		Model model(dir_brelrp, "mg50_01.brelrp", 0, 5);
		model.printInst();
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
		Solution s = alg.sA(1000, 100, 100, 10, 600);
		s.debug(cout);
		cout << endl;
		return 0;

	}
	else if (op == 5) {
		perm_rep alg;
		alg.loadInstance(dir_brelrp + "50/", "mg50_01.brelrp", 5);
		alg.printInstance();
		alg.setOutputDir(dir_brelrp + "output/");
		Solution s = alg.VNS(25, 300);
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
		getAllData(dir3);
		//adapt_model(dir4);
		//adaptAll_n();
		//adaptAll("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/200/");
		return 0;
	}
	else if (op == 8) {
		perm_rep alg;
		alg.loadInstance(dir_brelrp, "mg_00.brelrp", 5);
		alg.printInstance();
		Solution s = alg.VNS(25, 300);
		s.debug(cout);

		//instance inst("D:/Victor/Pos-Graduacao/UFV/Research/Implementation/instances_generator/", "test.txt", 5);
		//inst.print(cout);
	}

}
