#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
   cout<<"Mi programa acaba aqui"<<endl<<endl;
   system("pause");
   system("cls");
   int DATO=10;
   int VECES=10;
   int a=0;
   for(int i=0;i<8;i++)
      cout<<endl;
  
   cout<<"           ******  *    *   *     *   ***   *   ***   *     *    *** "<<endl;
   cout<<"           *       *    *   **    *  *   *  *  *   *  **    *   *   *"<<endl;
   cout<<"           *       *    *   * *   *  *      *  *   *  * *   *   *   *"<<endl;
   cout<<"           ***     *    *   *  *  *  *      *  *   *  *  *  *   *****"<<endl;
   cout<<"           *       *    *   *   * *  *      *  *   *  *   * *   *   *"<<endl;
   cout<<"           *       *    *   *    **  *   *  *  *   *  *    **   *   *"<<endl;
   cout<<"           *        ****    *     *   ***   *   ***   *     *   *   *"<<endl;
  
  
   for(int i=0;i<9;i++)
      cout<<endl;
     
   while(a<=VECES)
   {
      system("color a1");
      Sleep(DATO);
      system("color b2");
      Sleep(DATO);
      system("color c3");
      Sleep(DATO);
      system("color d4");
      Sleep(DATO);
      system("color e5");
      Sleep(DATO);
      system("color f6");
      Sleep(DATO);
      system("color 07");
      Sleep(DATO);
     
      a++;
     
   }


}

