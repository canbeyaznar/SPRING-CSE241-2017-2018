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
		SparseVector(string FileName);
		SparseVector(const SparseVector &copyConst);
		~SparseVector();
		
		friend SparseVector operator +(const SparseVector&);
		friend SparseVector operator -(const SparseVector&);
		SparseVector& operator =(const SparseVector& vector); 
		
		friend ostream& operator<<(ostream& os, const SparseVector& vector);
		friend double  dot(const SparseVector& leftVector, const SparseVector& rightVector);
		
		vector<int> Index;
		vector<double> Data;
	private:
		string NameofFile;
		
		
};
class SparseMatrix
{
	public:
		SparseMatrix();
		SparseMatrix(string FileName);
		SparseMatrix(const SparseMatrix &copyConst);
		~SparseMatrix();
		
		friend SparseMatrix operator +(const SparseMatrix&);
		friend SparseMatrix operator -(const SparseMatrix&);
		SparseMatrix& operator =(const SparseMatrix& Matrix);
		
		//transpose();
		
	friend ostream& operator<<(ostream& os, const SparseMatrix& Matrix);
	vector<int> RowIndex;
	vector<vector<int> > Index;	
	vector<vector<double> > Data;
	private:
		string NameofFile;
		
};

const SparseMatrix operator +(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix);
const SparseMatrix operator -(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix);
const SparseMatrix operator *(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix);

const SparseVector operator +(const SparseVector& leftVector, const SparseVector& rightVector);
const SparseVector operator -(const SparseVector& leftVector, const SparseVector& rightVector);

int check_errors(ifstream* f) {
	int stop = 0;
	if (f->eof()) {
	stop = 0;
	}
	if (f->fail()) {
	stop = 1;
	}
	if (f->bad()) {
	stop = 1;
	}
	return stop;
}
int main()
{	
	ofstream outfile; 
	outfile.open("output.txt", ios::out | ios::trunc );
	//Creating a SparseVector from file 
	SparseVector a1("input3.txt"); 
	outfile<<"a1"<<endl<<a1<<endl;
	//Binary operations and assignment 
	a1 = a1 + a1; 
	outfile<<"a1"<<endl<<a1<<endl;
	//Creating SparseMatrix from file 
	SparseMatrix m1("input.txt"); SparseMatrix m2("input2.txt");
	outfile<<"m2"<<endl<<m2<<endl;
	
	//Transpose 
	//outfile<<m2.transpose()<<endl;
	//Dot product 
	
	outfile<<m1*m1<<endl;
	return 0;
	
	
}

ostream& operator<<(ostream& os, const SparseMatrix& Matrix)
{
	for(int i=0; i<Matrix.RowIndex.size(); i++)
	{
		os<<Matrix.RowIndex[i]<<" ";
		for(int j=0; j<Matrix.Index[i].size(); j++)
		{
			os<<Matrix.Index[i][j]<<":"<<Matrix.Data[i][j]<<" ";
		}
		os<<endl;
	}
	
	return os;
}
SparseMatrix& SparseMatrix :: operator =(const SparseMatrix& Matrix)
{
	int i,j;
	int RowSize=this->RowIndex.size();
	int RowSize2=Matrix.RowIndex.size();
	int control=0, controlRow=0, controlData=0;
	int totalIndexLen=0;
	int totalIndexLen_Matrix=0;
	if(this==&Matrix)
	{
		return *this;
	}
	
	else //control negative of same Matrix, for example a1=-a1; 
	{
		if(RowSize==RowSize2)
		{
			for(i=0; i<RowSize; i++)
			{
				if(this->RowIndex[i]==Matrix.RowIndex[i])
				{
					controlRow++;
				}
				if(this->Index[i].size()==Matrix.Index[i].size())
				{
					totalIndexLen=totalIndexLen+this->Index[i].size();
					totalIndexLen_Matrix=totalIndexLen_Matrix+Matrix.Index[i].size();
					for(j=0; j<this->Index[i].size(); j++)
					{
						if(this->Data[i][j]==Matrix.Data[i][j]*(-1))
						{
							controlData++;
						}			
					}
				}
			}
		}
		if(controlData==totalIndexLen && controlRow==RowSize) 
		{
			this->RowIndex=Matrix.RowIndex;
			for(i=0; i<Matrix.RowIndex.size(); i++)
			{
				this->Index[i]=Matrix.Index[i];
				this->Data[i]=Matrix.Data[i];
			}
			return *this;
		}
		else if((controlRow==RowSize && totalIndexLen_Matrix==totalIndexLen))
		{
			for(i=0; i<Matrix.RowIndex.size(); i++)
			{
				vector<int> copyIndex;
				vector<double> copyData;
				this->RowIndex=Matrix.RowIndex;
				for(j=0; j<Matrix.Index[i].size(); j++)
				{
					copyIndex.push_back(Matrix.Index[i][j]);
					copyData.push_back(Matrix.Data[i][j]);
					
				}
				this->Index[i]=copyIndex;
				this->Data[i]=copyData;
			}
			return *this;
		}
		
	}
	
	
	//If these two matrixes aren't same erase Matrix's Indexes...
	
	for(i=0; i<=this->RowIndex.size(); i++)
	{
		this->Index[i].clear();
		this->Data[i].clear();
	}	
	
	this->RowIndex.clear();
	this->RowIndex=Matrix.RowIndex;
	for(i=0; i<Matrix.RowIndex.size(); i++)
	{
		vector<int> copyIndex;
		vector<double> copyData;
		for(j=0; j<Matrix.Index[i].size(); j++)
		{
			copyIndex.push_back(Matrix.Index[i][j]);
			copyData.push_back(Matrix.Data[i][j]);
		}
		this->Index.insert(this->Index.begin()+i,copyIndex);
		this->Data.insert(this->Data.begin()+i,copyData);
	}
	
	return *this;
}
const SparseMatrix operator +(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix)
{
	int i,j,control,control2;
	SparseMatrix Result;
	int len1;
	int len2;
	int leftRow=0,rightRow=0;
	
	control=1;
	control2=1;

	while(control2!=0)
	{
		i=0;
		j=0;
		control=1;
		vector<int> copyIndex;
		vector<double> copyData;
		
		if(leftRow>=leftMatrix.RowIndex.size())
		{
			int doOnce=1; // it will copy last datas' of left matrix 
			while(rightRow<rightMatrix.RowIndex.size())// And gets all the datas to the end of the right matrix...
			{
				
				vector<int> copyIndex2;
				vector<double> copyData2;
				Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
				while(j<rightMatrix.Index[rightRow].size())
				{
					if(doOnce==1)
					{
						int x=0;
						while(x<copyIndex.size())
						{
							copyIndex2.push_back(copyIndex[x]);
							copyData2.push_back(copyData[x]);
						}	
						doOnce=0;
					}
					copyIndex2.push_back(rightMatrix.Index[rightRow][j]);
					copyData2.push_back(rightMatrix.Data[rightRow][j]);
					j++;
				}
				Result.Index.push_back(copyIndex2);
				Result.Data.push_back(copyData2);
				j=0;
				rightRow++;
			}
			return Result;
		}
		else if(rightRow>=rightMatrix.RowIndex.size())
		{
			int doOnce=1;	// it will copy last datas' of right matrix 
			while(leftRow<leftMatrix.RowIndex.size()) // And gets all the datas to the end of the left matrix...
			{
				vector<int> copyIndex2;
				vector<double> copyData2;
				Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
				while(i<leftMatrix.Index[leftRow].size())
				{
					if(doOnce==1)
					{
						int x=0;
						while(x<copyIndex.size())
						{
							copyIndex2.push_back(copyIndex[x]);
							copyData2.push_back(copyData[x]);
						}	
						doOnce=0;
					}
					copyIndex2.push_back(leftMatrix.Index[leftRow][i]);
					copyData2.push_back(leftMatrix.Data[leftRow][i]);
					i++;
				}
				Result.Index.push_back(copyIndex2);
				Result.Data.push_back(copyData2);
				i=0;
				leftRow++;
			}
			return Result;
		}
		else
		{
			if(leftMatrix.RowIndex[leftRow]==rightMatrix.RowIndex[rightRow]) 
			{
				while(control!=0)
				{
					if(i>=leftMatrix.Index[leftRow].size()) //If rows are the same but left matrix's row's datas ends copy take all datas of right matrix
					{
						while(j<rightMatrix.Index[rightRow].size())
						{
							copyIndex.push_back(rightMatrix.Index[rightRow][j]);
							copyData.push_back(rightMatrix.Data[rightRow][j]);
							j++;
						}
						Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
						Result.Index.push_back(copyIndex);
						Result.Data.push_back(copyData);
						rightRow++;
						leftRow++;
						control=0; // if row ends break the loop
					}
					else if(j>=rightMatrix.Index[rightRow].size()) //If rows are the same but right matrix's row's datas ends copy take all datas of left matrix
					{
						while(i<leftMatrix.Index[leftRow].size())
						{
							copyIndex.push_back(leftMatrix.Index[leftRow][i]);
							copyData.push_back(leftMatrix.Data[leftRow][i]);
							i++;
						}
						Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
						Result.Index.push_back(copyIndex);
						Result.Data.push_back(copyData);
						
						leftRow++;
						rightRow++;
						control=0; // if row ends break the loop
					}
					else
					{
						if(leftMatrix.Index[leftRow][i]==rightMatrix.Index[rightRow][j]) //changes location of left and right matrix
						{
							copyIndex.push_back(leftMatrix.Index[leftRow][i]);
							copyData.push_back(leftMatrix.Data[leftRow][i]+rightMatrix.Data[rightRow][j]);
							i++;
							j++;
						}
						else if(leftMatrix.Index[leftRow][i]<rightMatrix.Index[rightRow][j]) //changes location of left matrix's index
						{
							copyIndex.push_back(leftMatrix.Index[leftRow][i]);
							copyData.push_back(leftMatrix.Data[leftRow][i]);
							i++;
						}
						else if(leftMatrix.Index[leftRow][i]>rightMatrix.Index[rightRow][j]) //changes location of right matrix's index
						{
							copyIndex.push_back(rightMatrix.Index[rightRow][j]);
							copyData.push_back(rightMatrix.Data[rightRow][j]);
							j++;
						}
						
					}
				}
				
			}
			else if(leftMatrix.RowIndex[leftRow]<rightMatrix.RowIndex[rightRow]) //changes location of left matrix's row
			{
				Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
				Result.Index.push_back(leftMatrix.Index[leftRow]);
				Result.Data.push_back(leftMatrix.Data[leftRow]);
				leftRow++;
			}
			else if(leftMatrix.RowIndex[leftRow]>rightMatrix.RowIndex[rightRow])	//changes location of right matrix's row
			{
				Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
				Result.Index.push_back(rightMatrix.Index[rightRow]);
				Result.Data.push_back(rightMatrix.Data[rightRow]);
				rightRow++;
			}
		}
	}
	return Result;	
}

const SparseMatrix operator -(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix)
{
	int i,j,control,control2;
	SparseMatrix Result;
	int len1;
	int len2;
	int leftRow=0,rightRow=0;
	
	control=1;
	control2=1;

	while(control2!=0)
	{
		i=0;
		j=0;
		control=1;
		vector<int> copyIndex;
		vector<double> copyData;
		
		if(leftRow>=leftMatrix.RowIndex.size())
		{
			int doOnce=1;
			while(rightRow<rightMatrix.RowIndex.size())
			{
				
				vector<int> copyIndex2;
				vector<double> copyData2;
				Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
				while(j<rightMatrix.Index[rightRow].size()) //sorun burada
				{
					if(doOnce==1)
					{
						int x=0;
						while(x<copyIndex.size())
						{
							copyIndex2.push_back(copyIndex[x]);
							copyData2.push_back(copyData[x]);
						}	
						doOnce=0;
					}
					copyIndex2.push_back(rightMatrix.Index[rightRow][j]);
					copyData2.push_back(rightMatrix.Data[rightRow][j]*(-1));
					j++;
				}
				Result.Index.push_back(copyIndex2);
				Result.Data.push_back(copyData2);
				j=0;
				rightRow++;
			}
			return Result;
		}
		else if(rightRow>=rightMatrix.RowIndex.size())
		{
			int doOnce=1;
			while(leftRow<leftMatrix.RowIndex.size())
			{
				vector<int> copyIndex2;
				vector<double> copyData2;
				Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
				while(i<leftMatrix.Index[leftRow].size())
				{
					if(doOnce==1)
					{
						int x=0;
						while(x<copyIndex.size())
						{
							copyIndex2.push_back(copyIndex[x]);
							copyData2.push_back(copyData[x]);
						}	
						doOnce=0;
					}
					copyIndex2.push_back(leftMatrix.Index[leftRow][i]);
					copyData2.push_back(leftMatrix.Data[leftRow][i]);
					i++;
				}
				Result.Index.push_back(copyIndex2);
				Result.Data.push_back(copyData2);
				//Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
				i=0;
				leftRow++;
			}
			return Result;
		}
		else
		{
			if(leftMatrix.RowIndex[leftRow]==rightMatrix.RowIndex[rightRow])
			{
				while(control!=0)
				{
					if(i>=leftMatrix.Index[leftRow].size())
					{
						while(j<rightMatrix.Index[rightRow].size())
						{
							copyIndex.push_back(rightMatrix.Index[rightRow][j]);
							copyData.push_back(rightMatrix.Data[rightRow][j]*(-1));
							j++;
						}
						Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
						Result.Index.push_back(copyIndex);
						Result.Data.push_back(copyData);
						rightRow++;
						leftRow++;
						control=0;
					}
					else if(j>=rightMatrix.Index[rightRow].size())
					{
						while(i<leftMatrix.Index[leftRow].size())
						{
							copyIndex.push_back(leftMatrix.Index[leftRow][i]);
							copyData.push_back(leftMatrix.Data[leftRow][i]);
							i++;
						}
						Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
						Result.Index.push_back(copyIndex);
						Result.Data.push_back(copyData);
						
						leftRow++;
						rightRow++;
						control=0;
					}
					else
					{
						if(leftMatrix.Index[leftRow][i]==rightMatrix.Index[rightRow][j])
						{
							if(leftMatrix.Data[leftRow][i]+rightMatrix.Data[rightRow][j]*(-1) !=0)
							{
								copyIndex.push_back(leftMatrix.Index[leftRow][i]);
								copyData.push_back(leftMatrix.Data[leftRow][i]+rightMatrix.Data[rightRow][j]*(-1));
							}
							i++;
							j++;
						}
						else if(leftMatrix.Index[leftRow][i]<rightMatrix.Index[rightRow][j])
						{
							copyIndex.push_back(leftMatrix.Index[leftRow][i]);
							copyData.push_back(leftMatrix.Data[leftRow][i]);
							i++;
						}
						else if(leftMatrix.Index[leftRow][i]>rightMatrix.Index[rightRow][j])
						{
							copyIndex.push_back(rightMatrix.Index[rightRow][j]);
							copyData.push_back(rightMatrix.Data[rightRow][j]*(-1));
							j++;
						}
						
					}
				}
				
			}
			else if(leftMatrix.RowIndex[leftRow]<rightMatrix.RowIndex[rightRow])
			{
				Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
				Result.Index.push_back(leftMatrix.Index[leftRow]);
				Result.Data.push_back(leftMatrix.Data[leftRow]);
				leftRow++;
			}
			else if(leftMatrix.RowIndex[leftRow]>rightMatrix.RowIndex[rightRow])
			{
				Result.RowIndex.push_back(rightMatrix.RowIndex[rightRow]);
				Result.Index.push_back(rightMatrix.Index[rightRow]);
				Result.Data.push_back(rightMatrix.Data[rightRow]);
				rightRow++;
			}
		}
	}
	return Result;	
}

SparseMatrix operator +(const SparseMatrix& Matrix) // it will just copy the matrix
{
	SparseMatrix Result;
	
	for(int i=0; i<Matrix.RowIndex.size(); i++)
	{
		vector<int> copyIndex;
		vector<double> copyData;
		Result.RowIndex.push_back(Matrix.RowIndex[i]);
		for(int j=0; j<Matrix.Index[i].size(); j++)
		{
			copyIndex.push_back(Matrix.Index[i][j]);
			copyData.push_back(Matrix.Data[i][j]);
		}
		Result.Index.push_back(copyIndex);
		Result.Data.push_back(copyData);
	}
	return Result;
}

SparseMatrix operator -(const SparseMatrix& Matrix)
{
	SparseMatrix Result;
	
	for(int i=0; i<Matrix.RowIndex.size(); i++)
	{
		vector<int> copyIndex;
		vector<double> copyData;
		Result.RowIndex.push_back(Matrix.RowIndex[i]);
		for(int j=0; j<Matrix.Index[i].size(); j++)
		{
			copyIndex.push_back(Matrix.Index[i][j]);
			copyData.push_back(Matrix.Data[i][j]*(-1));
		}
		Result.Index.push_back(copyIndex);
		Result.Data.push_back(copyData);
	}
	return Result;
}

const SparseMatrix operator *(const SparseMatrix& leftMatrix, const SparseMatrix& rightMatrix)
{
	SparseMatrix Result;
	
	int i,j;
	int control=1, control2=1;
	
	int leftRowLen=leftMatrix.RowIndex.size();
	int rightRowLen=rightMatrix.RowIndex.size();
	
	int leftIndexLen, rightIndexLen;
	
	int leftRow=0;
	int rightRow=0;
	while(control!=0)
	{
		control2=1;
		i=0; 
		j=0;
		vector<int> copyIndex;
		vector<double> copyData;
		
		if(leftRow>=leftRowLen || rightRow>=rightRowLen)
			return Result;
		
		if(leftMatrix.RowIndex[leftRow]==rightMatrix.RowIndex[rightRow])
		{
			while(control2!=0)
			{
				leftIndexLen=leftMatrix.Index[leftRow].size();
				rightIndexLen=rightMatrix.Index[rightRow].size();
				
				if(i>=leftIndexLen || j>=rightIndexLen)
				{
					control2=0;
				}
				else
				{
					if(leftMatrix.Index[leftRow][i]==rightMatrix.Index[rightRow][j])
					{
						copyIndex.push_back(leftMatrix.Index[leftRow][i]);
						copyData.push_back(leftMatrix.Data[leftRow][i]*rightMatrix.Data[rightRow][j]);
						i++;
						j++;
					}
					else if(leftMatrix.Index[leftRow][i]<=rightMatrix.Index[rightRow][j])
					{
						i++;
					}
					else if(leftMatrix.Index[leftRow][i]>=rightMatrix.Index[rightRow][j])
					{
						j++;
					}
				}
			}
			
			Result.RowIndex.push_back(leftMatrix.RowIndex[leftRow]);
			Result.Index.push_back(copyIndex);
			Result.Data.push_back(copyData);
			leftRow++;
			rightRow++;
		}
		else if(leftMatrix.RowIndex[leftRow]<rightMatrix.RowIndex[rightRow])
		{
			leftRow++;
		}
		else if(leftMatrix.RowIndex[leftRow]>rightMatrix.RowIndex[rightRow])
		{
			rightRow++;
		}
	}
	return Result;
}

/*
SparseMatrix::transpose()
{
	SparseMatrix Result;
	
	SparseMatrix Result;
	
	int i,j,k;
	
	vector<vector<int> > tempIndex;	
	vector<vector<double> > tempData;
	vector<int> tempRow;
	
	vector<vector<int> > tempIndex2;	
	vector<vector<double> > tempData2;
	
	int indexLocation,rowLocation;
	int temp=0;
	
	int control;
	for(i=0; i<this->RowIndex.size(); i++)
	{
		vector<int> copyIndex;
		vector<double> copyData;
		indexLocation=this->Index[i][0];
		
		rowLocation=this->RowIndex[i];
		for(k=temp; k<rowLocation; k++)
		{
			
		}
		for(k=0; k<indexLocation; k++)
		{
			copyIndex.push_back(k);
			copyData.push_back(0);
		}
		copyIndex.push_back(this->Index[i][0]);
		copyData.push_back(this->Data[i][0]);
		for(j=1; j<this->Index[i].size(); j++)
		{
			temp=k+1;
			indexLocation=this->Index[i][j];
			for(k=temp; k<indexLocation; k++)
			{
				copyIndex.push_back(this->Index[i][j]-indexLocation+k);
				copyData.push_back(0);
			}
			copyIndex.push_back(k);
			copyData.push_back(this->Data[i][j]);
			
		}
		tempIndex.push_back(copyIndex);
		tempData.push_back(copyData);
	}
	
	control=0;
	rowLocation=0;*/
	/*for(i=0; i<this->RowIndex.size(); i++)
	{
		for(i=0; i<this->Index[i].size();)
	}
	

	return 0;
	
}*/

SparseMatrix::SparseMatrix()
{
	
}
SparseMatrix::SparseMatrix(string FileName)
{
	ifstream cinfromfile;
	cinfromfile.open(FileName.c_str());
	string NameofFile=FileName;
	int row, index;
	double data;
	char c;
	string line;
	if(cinfromfile.is_open())
	{
		while(!check_errors(&cinfromfile))
		{
			vector<int> copyIndex;
			vector<double> copyData;
			cinfromfile>>row;
			getline(cinfromfile, line);
			std::stringstream ss(line);
			do
			{
				ss>>index>>c>>data;
				if(data!=0)
				{
					copyIndex.push_back(index);
					copyData.push_back(data);
				}
			}while(ss);
			copyIndex.erase(copyIndex.begin()+(copyIndex.size()-1));
			copyData.erase(copyData.begin()+(copyData.size()-1));
			if(copyData[0]!=0)
				RowIndex.push_back(row);
			Index.push_back(copyIndex);
			Data.push_back(copyData);
		}
		RowIndex.erase(RowIndex.begin()+(RowIndex.size()-1));
		Index.erase(Index.begin()+(Index.size()-1));
		Data.erase(Data.begin()+(Data.size()-1));
	}
	
	cinfromfile.close();
	
	
}
SparseMatrix::SparseMatrix(const SparseMatrix &copyConst)
{
	for(int i=0; i<copyConst.RowIndex.size(); i++)
	{
		vector<int> copyIndex;
		vector<double> copyData;
		RowIndex.push_back(copyConst.RowIndex[i]);	
		for(int j=0; j<copyConst.Data[i].size(); j++)
		{
			copyIndex.push_back(copyConst.Index[i][j]);
			copyData.push_back(copyConst.Data[i][j]);
		}
		Index.push_back(copyIndex);
		Data.push_back(copyData);
	}
	
}

SparseMatrix::~SparseMatrix()
{
	
}
const  SparseVector operator +(const SparseVector& leftVector, const SparseVector& rightVector)
{
	int i,j,control=0;
	SparseVector Result;
	int len1=leftVector.Data.size();
	int len2=rightVector.Data.size();
	int maxLen;
	
	if(len1>len2)
		maxLen=len1;
	else
		maxLen=len2;
	
	j=0;
	i=0;
	control=1;
	while(control!=0)
	{
		if(i>=leftVector.Data.size())
		{
			while(j<maxLen)
			{
				Result.Index.push_back(rightVector.Index[j]);
				Result.Data.push_back(rightVector.Data[j]);
				j++;
			}
			return Result;
		}
		else if(j>=rightVector.Data.size())
		{
			while(i<maxLen)
			{
				Result.Index.push_back(leftVector.Index[i]);
				Result.Data.push_back(leftVector.Data[i]);
				i++;
			}
			return Result;
		}
		else
		{
			if(leftVector.Index[i]==rightVector.Index[j])
			{
				Result.Index.push_back(leftVector.Index[i]);
				Result.Data.push_back(leftVector.Data[i]+rightVector.Data[j]);
				j++;
				i++;
			}
			else if(leftVector.Index[i]<rightVector.Index[j])
			{
				Result.Index.push_back(leftVector.Index[i]);
				Result.Data.push_back(leftVector.Data[i]);
				i++;
			}
			
			else if(leftVector.Index[i]>rightVector.Index[j])
			{
				Result.Index.push_back(rightVector.Index[j]);
				Result.Data.push_back(rightVector.Data[j]);
				j++;
			}
		}
				
	}
	
	return Result;
}

const  SparseVector operator -(const SparseVector& leftVector, const SparseVector& rightVector)
{
	int i,j,control=0;
	SparseVector Result;
	int len1=leftVector.Data.size();
	int len2=rightVector.Data.size();
	int maxLen;
	
	if(len1>len2)
		maxLen=len1;
	else
		maxLen=len2;
	
	j=0;
	i=0;
	control=1;
	while(control!=0)
	{
		if(i>=leftVector.Data.size())
		{
			while(j<maxLen)
			{
				Result.Index.push_back(rightVector.Index[j]);
				Result.Data.push_back(rightVector.Data[j]*(-1));
				j++;
			}
			return Result;
		}
		else if(j>=rightVector.Data.size())
		{
			while(i<maxLen)
			{
				Result.Index.push_back(leftVector.Index[i]);
				Result.Data.push_back(leftVector.Data[i]);
				i++;
			}
			return Result;
		}
		else
		{
			if(leftVector.Index[i]==rightVector.Index[j])
			{
				if(leftVector.Data[i]-rightVector.Data[i]!=0)
				{
					Result.Index.push_back(leftVector.Index[i]);
					Result.Data.push_back(leftVector.Data[i]-rightVector.Data[j]);
				}
				j++;
				i++;
			}
			else if(leftVector.Index[i]<rightVector.Index[j])
			{
				Result.Index.push_back(leftVector.Index[i]);
				Result.Data.push_back(leftVector.Data[i]);
				i++;
			}
			
			else if(leftVector.Index[i]>rightVector.Index[j])
			{
				Result.Index.push_back(rightVector.Index[j]);
				Result.Data.push_back(rightVector.Data[j]*(-1));
				j++;
			}
		}
				
	}
	return Result;
}
SparseVector operator -(const SparseVector& vector)
{
	SparseVector Result;
	
	for(int i=0; i<vector.Index.size(); i++)
	{
		if(vector.Data[i]!=0)
		{
			Result.Index.push_back(vector.Index[i]);
			Result.Data.push_back(vector.Data[i]*(-1));
		}
	}
	return Result;
}

SparseVector operator +(const SparseVector& vector)
{
	SparseVector Result;
	
	for(int i=0; i<vector.Index.size(); i++)
	{
		if(vector.Data[i]!=0)
		{
			Result.Index.push_back(vector.Index[i]);
			Result.Data.push_back(vector.Data[i]);
		}
	
	}
	return Result;
}

ostream& operator<<(ostream& os, const SparseVector& vector)
{
	for(int i=0; i<vector.Index.size(); i++)
		os<<vector.Index[i]<<":"<<vector.Data[i]<<" ";
	
	return os;
}


double dot(const SparseVector& leftVector, const SparseVector& rightVector)
{
	double Result=0.0;
	int i,j;
	int len1=leftVector.Index.size();
	int len2=rightVector.Index.size();
	

	
	i=0;
	j=0;
	while(true)
	{
		if(j>=len2 || i>=len1)
			return Result;
		
		if(leftVector.Index[i]==rightVector.Index[j])
		{
			Result=Result + (leftVector.Data[i]*rightVector.Data[j]);
			i++;
			j++;
		}
		else if(leftVector.Index[i]<=rightVector.Index[j])
		{
			i++;
		}
		else if(leftVector.Index[i]>=rightVector.Index[j])
		{
			j++;
		}	
	}
	return Result;
}

SparseVector& SparseVector :: operator =(const SparseVector& vector)
{
	if(this==&vector)
	{
		return *this;
	}
	this->Index=vector.Index;
	this->Data=vector.Data;
	return *this;
}


SparseVector::SparseVector()
{
	
}
SparseVector::SparseVector(const SparseVector &copyConst)
{
	for(int i=0; i<copyConst.Data.size(); i++)
	{
		Data.push_back(copyConst.Data[i]);
		Index.push_back(copyConst.Index[i]);
	}
}
SparseVector::SparseVector(string FileName)
{
	int index;
	char c;
	double data;
	int i,j,k;
	i=0;
	ifstream cinfromfile;
	cinfromfile.open(FileName.c_str());
	this->NameofFile=FileName;
	
	if(cinfromfile.is_open())
	{
		while(!check_errors(&cinfromfile))
		{
			cinfromfile>>index>>c>>data;
			Index.push_back(index);
			Data.push_back(data);
			i++;
		}
		Index.erase(Index.begin()+(Index.size()-1));
		Data.erase(Data.begin()+(Data.size()-1));
	}
	cinfromfile.close();
}

SparseVector::~SparseVector()
{
	
}

