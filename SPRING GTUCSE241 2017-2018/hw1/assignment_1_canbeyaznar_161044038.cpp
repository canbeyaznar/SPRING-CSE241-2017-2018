#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#define MAXLEN 100

using namespace std;
void resetStr(char str[])
{
	int i=0;
	while(str[i]!='\0')
	{
		str[i]='\0';
		i++;
	}
}
void deleteWhiteSpaces(char str[])
{
	int i=0, j=0;
    while (str[i])
    {
        if (str[i] != ' ')
           str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
}
int find_NumOfNames(char str[])
{
	int i=0, result=0;
	while(str[i]!='\0')
	{
		if(str[i]=='*')
		{
			result++;
		}
		i++;
	}
	return result;
}

int isOnlyNum(char str[])
{
	int i=0,control=0;
	while(str[i]!='\0')
	{
		if(isdigit(str[i]) || str[i]=='.')
			control++;
		i++;
	}
	if(i==control)
		return 1;
	else
		return 0;
}
double startTest(char str[])
{
	int numOfParts=0,location,i,control;
	char str2[MAXLEN],temp[MAXLEN];
	char Names[MAXLEN];
	int Num,Len,Mark;
	double result=0;
	cout<<"What is "<<str<<"?:"<<endl;
	
	cin.getline(str2,100);
	deleteWhiteSpaces(str2);
	control=isOnlyNum(str2);
	Len=find_NumOfNames(str2);
	if(control==1)
	{
		sscanf(str2,"%lf",&result);
		return result;
	}
	else
	{
		i=0;
		Mark=0;
		while(Mark<Len)
		{
			Num=0;
			location=0;
			while(str2[i]!='*')
			{
				temp[location]=str2[i];
				location++;
				i++;
			}
			i++;
			sscanf(temp,"%d",&Num);
			resetStr(temp);
			location=0;
			while(str2[i]!='+' && str2[i]!='\0')
			{
				temp[location]=str2[i];
				i++;
				location++;
			}
			i++;
			strcpy(Names,temp);
			resetStr(temp);
			result=result+startTest(Names)*(double)Num;
			Mark++;
		}
		return result;
	}
	
	
}
int main()
{
	char str[MAXLEN];
	double result;
	cout<<"Define the object: ";
	cin.getline(str,MAXLEN);
	deleteWhiteSpaces(str);
	result=startTest(str);
	cout<<endl<<"Total cost of "<<str<<" is "<<result<<endl;
	
}
