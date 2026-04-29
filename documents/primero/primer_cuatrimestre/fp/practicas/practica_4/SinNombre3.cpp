#include<iostream>
using namespace std;

double cifras(int n)
{
	
	int a=0;
	
	while(n!=0)
	{
		n=n/10;
		a++;
	}
	return a;
}

int main()
{
	int n;
	cout<<"Numero: ";
	cin>>n;
	
	cout<<"Tiene "<<cifras(n)<<" cifras";
}


