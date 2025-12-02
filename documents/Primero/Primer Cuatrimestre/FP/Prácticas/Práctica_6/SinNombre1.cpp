#include<iostream>
using namespace std;
int main()
{
	const int TERMINADOR=-1;
	int suma=0, n=0, i=1;
	
	do
	{
		suma=suma+n;
		cout<<"Numero "<<i<<": ";
		cin>>n;
		i++; //=i=i+1; i--  -> i=i-1
	} while(n!=TERMINADOR);
	
	cout<<"La suma es: "<<suma;
}
