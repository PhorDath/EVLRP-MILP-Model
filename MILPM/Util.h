#pragma once
#include <string>
#include <Python.h>

using namespace std;

class Util
{
public:
	Util();
	static int drawGraph(string dir, string fileName, string func, string arg);
	~Util();
};

