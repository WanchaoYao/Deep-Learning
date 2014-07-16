#include <iostream> 
using namespace std;
                         
void test(char **ptr)
{
    cout << *ptr << endl;
    cout << *ptr[0] << endl;
    cout << **ptr << endl;
    cout << *(ptr+1)[0] << endl;
}
                                                                                      
int main()
{
    char *p[3] = {"abcc", "def", "ghi"};
    test(p);
                                                                                                         return 0;
}
