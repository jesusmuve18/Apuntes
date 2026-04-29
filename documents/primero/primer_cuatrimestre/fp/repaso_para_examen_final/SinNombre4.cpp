#include <iostream>
using namespace std;

int main() 
{
    int int_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    //array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    cout << "vector size: " << (sizeof int_array )/(sizeof (int))<< endl;
    //cout << "array size: " << size(arr) << endl;

    return 0;
}
