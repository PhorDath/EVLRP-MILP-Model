#pragma once

#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <chrono>
#include "Model_setAdapt.h"
#include "Model_PMedians.h"
#include "Model.h"
#include "perm_rep.h"
#include "routes_rep.h"
#include "test.h"
#include "lrp_opt.h"

using namespace std;

const int MAX = 10000;

string getDate();

void exp_model(string dir);
void exp_sa(string dir1);
void exp_vns(string dir1);
void exp_vnsl(string dir1);
void exp_vns2(string dir1);
void exp_vns3(string dir1);
void exp_grasp(string dir1);
void exp_instances(string dir1);
void exp_lowerbound(string dir, string file);
void genBigMatrix(string dir1);
void adapt_model(string dir);
void test1();

vector<Solution> exp_vns_(string dir1, string output);
vector<Solution> exp_vnsl_(string dir1, string output, vector<string> BSSs, vector<string> DPTs);
pair<float, float> exp(string dir, string region, int pct);
void call_exp(string dir, string region, int pct);
void exp_opt(string dir, int op);
///

void exp1(string dir1, string dir2);
void exp2(string dir1, string dir2);
void exp3(string dir1, string dir2, int type);
void exp4(string dir1, string dir2, int type);
void exp_greed(string dir1, string dir2, int type);
void exp_SA(string dir1, string dir2, int type);
int menuModel();
string menuInstance(string curDir);

float totalCost(vector<Solution> sols, string dir, bool write);
float totalCost_Desarmotized(vector<Solution> sols, string dir, bool write);

#endif EXPERIMENTS_H