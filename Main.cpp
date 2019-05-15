#include "ObjectPool.hpp"
#include <ctime>

int main()
{
    char arr[1000000];

    int start = clock();
    ObjectPool<char> a;
    for (int i = 0; i < 1000000; i++)
    {
        char *p = a.New();
        *p = i;
        arr[i] = *p;
    }
    int end = clock();
    std::cout << "use time : " << (end - start) << std::endl;

    char arr1[1000000];
    start = clock();
    for (int i = 0; i < 1000000; i++)
    {
        char *p = new char(i);
        arr1[i] = *p;
    }
    end = clock();
    std::cout << "use time : " << (end - start) << std::endl;

    return 0;
}

