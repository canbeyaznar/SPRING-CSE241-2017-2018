//Can Beyaznar 161044038

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class SparseMatrix
{
	public:
		SparseMatrix();
		SparseMatrix(const char * );
		SparseMatrix(const SparseMatrix& );
		~SparseMatrix();
		
		friend SparseMatrix operator +(const SparseMatrix&);
		friend SparseMatrix operator -(const SparseMatrix&);
		SparseMatrix& operator =(const SparseMatrix& );
		
		//transpose();
		
	friend ostream& operator<<(ostream& , const SparseMatrix& );
	vector<int> RowIndex;
	vector<vector<int> > Index;	
	vector<vector<double> > Data;
	private:
		
		
};

const SparseMatrix operator +(const SparseMatrix& , const SparseMatrix& );
const SparseMatrix operator -(const SparseMatrix& , const SparseMatrix& );
const SparseMatrix operator *(const SparseMatrix& , const SparseMatrix& );
