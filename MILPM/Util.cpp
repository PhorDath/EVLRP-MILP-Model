#include "Util.h"



Util::Util()
{
}

int Util::drawGraph(string dir, string fileName, string func, string arg)
{
	/*
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;



	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault((dir + fileName).c_str());
	// Error checking of pName left out 

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, func.c_str());
		// pFunc is a new reference 

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			
			pValue = PyUnicode_DecodeFSDefault(arg.c_str());
			if (!pValue) {
				Py_DECREF(pArgs);
				Py_DECREF(pModule);
				fprintf(stderr, "Cannot convert argument\n");
				return 1;
			}
			// pValue reference stolen here: 
			PyTuple_SetItem(pArgs, 0, pValue);
			
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", func);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", dir + fileName);
		return 1;
	}
	if (Py_FinalizeEx() < 0) {
		return 120;
	}
	*/
	return 0;
}


Util::~Util()
{
}
