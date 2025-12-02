#include<iostream>
#include<conio.h>
using namespace std;
int main()
{
   char a;
   int i=0, j=-1;
   cout<<"*";
   while(1)
   {
      a=_getch();
      cout<<endl;
      switch(a)
      {
         case('a'):
            i--;
            break;
         case('d'):
            i++;
            break;
         case('w'):
            j--;
            break;
         case('s'):
            j++;
            break;
      }
      system("cls");
      for(int t=0;t<=j;t++)
         cout<<"\n";
      for(int t=0;t<=i;t++)
         cout<<"  "; 
      cout<<"**** *    *  **   **   *   *"<<endl;
      for(int t=0;t<=i;t++)
         cout<<"  ";
      cout<<"*  * *    * *  * *  *  **  *"<<endl;
      for(int t=0;t<=i;t++)
         cout<<"  ";
      cout<<"**** *    *  **  *  *  * * *"<<endl;
      for(int t=0;t<=i;t++)
         cout<<"  ";
      cout<<"*  * *    * *  * *  *  *  **"<<endl;
      for(int t=0;t<=i;t++)
         cout<<"  ";
      cout<<"*  * ***  *  **   **   *   *"<<endl;
     
   }
}
