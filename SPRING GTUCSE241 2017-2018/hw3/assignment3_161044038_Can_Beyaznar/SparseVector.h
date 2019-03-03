//Can Beyaznar 161044038

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class SparseVector
{
	public:
		SparseVector();
		SparseVector(const char *);
		SparseVector(const SparseVector& );
		~SparseVector();
		
		friend SparseVector operator +(const SparseVector&);
		friend SparseVector operator+(const SparseVector&, const SparseVector&);
		friend SparseVector operator-(const SparseVector&, const SparseVector&);
		friend SparseVector operator-(const SparseVector&);
		SparseVector& operator =(const SparseVector& ); 
		
		
		friend ostream& operator<<(ostream&, const SparseVector&);
			
		vector<int> Index;
		vector<double> Data;
	private:
		
		
		
};
double dot(const SparseVector&, const SparseVector&);
