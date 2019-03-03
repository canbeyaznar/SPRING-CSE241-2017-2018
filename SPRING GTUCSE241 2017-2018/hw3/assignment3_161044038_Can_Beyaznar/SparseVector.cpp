//Can Beyaznar 161044038

#include "SparseVector.h"


bool check_errors(ifstream *f);

SparseVector operator +(const SparseVector& leftVector, const SparseVector& rightVector)
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
		if(i>=leftVector.Data.size()) //if left vector's datas ends copy take all datas of right vector
		{
			while(j<maxLen)
			{
				Result.Index.push_back(rightVector.Index[j]);
				Result.Data.push_back(rightVector.Data[j]);
				j++;
			}
			return Result;
		}
		else if(j>=rightVector.Data.size()) //if right vector's datas ends copy take all datas of left vector
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
			else if(leftVector.Index[i]<rightVector.Index[j]) // if indexes are not same, add and change location of the small one
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

SparseVector operator -(const SparseVector& leftVector, const SparseVector& rightVector)
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
SparseVector::SparseVector(const char *pFilename)
{
	int index;
	char c;
	double data;
	int i,j,k;
	i=0;
	ifstream cinfromfile;
	cinfromfile.open(pFilename);
	
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

