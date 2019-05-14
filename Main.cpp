#include "MemoryPool.hpp"
#include <ctime>

int main()
{
    //MemoryPool<int> a;
    //int *p = a.New();
    //*p = 1;
    //a.Delete(p);
    
    int start = clock();
    MemoryPool<int> a;
    for (int i = 0; i < 1000; i++)
    {
        int *p = a.New();
        *p = i;
    }
    int end = clock();
    std::cout << "use time : " << (end - start) / CLOCKS_PER_SEC << std::endl;


    return 0;
}

