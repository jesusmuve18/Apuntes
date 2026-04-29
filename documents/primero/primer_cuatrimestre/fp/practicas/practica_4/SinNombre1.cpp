
#include<iostream>
using namespace std;
int main(){
    cout << "Hola" << endl;
    for(int x=0; x<10; x++){
        printf("%c[1A \rProgreso: %d\n", 033, x);
        #ifdef _WIN32
        #include <Windows.h>
            Sleep(250);
        #else
            sleep(1);
        #endif
    }
    return 0;
}
