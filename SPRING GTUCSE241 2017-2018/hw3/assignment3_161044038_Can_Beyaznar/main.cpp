#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "SparseMatrix.h"
#include "SparseVector.h"

using namespace std;

int main()
{
	ofstream outfile;
	outfile.open("output.txt", ios::out | ios::trunc );
	
	SparseMatrix a1("input.txt");
	SparseMatrix a2("input2.txt");
	
	SparseVector a3("input3.txt");
	SparseVector a4("input4.txt");
	
	cout<<a1<<endl;
	cout<<a2<<endl;
	
	cout<<a1*a2<<endl;
	
	a1=a1*a2;
	
	//a1=(-a1)+(-a2);
	cout<<a1<<endl;
return 0;
}
