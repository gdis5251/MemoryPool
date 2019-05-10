#include <iostream>
#include <cassert>  //assert()
#include <ctime>  //clock()
#include <vector> //vector<T>
#include <list>  //list<T>
#include <stack> //stack<T>
#include "StackAlloc.hpp"
#include "MemoryPool.hpp"

const int elems = 1000000;
const int reps = 100;

int main(void)
{
    clock_t start;

    ////Test list
    //std::list<int, std::allocator<int> > l;
    //start = clock();
    //for (int i = 0; i < reps; i++)
    //{
    //    assert(l.empty());

    //    for (int j = 0; j < elems; j++)
    //    {
    //        l.push_back(j);
    //    }

    //    for (int j = 0; j < elems; j++)
    //    {
    //        l.pop_back();
    //    }
    //}
    //std::cout << "Defualt allocator time is : ";
    //std::cout << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

    //Test StackAlloc
    StackAlloc<int, std::allocator<int> > sa;
    start = clock();
    for (int i = 0; i < reps; i++)
    {
        assert(sa.empty());

        for (int j = 0; j < elems; j++)
        {
            sa.push(j);
        }

        for (int j = 0; j < elems; j++)
        {
            sa.pop();
        }
    }
    std::cout << "Use Default allocator time is : ";
    std::cout << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

    //Test StackAlloc MemoryPool
    StackAlloc<int, MemoryPool<int> > sm;
    start = clock();
    for (int i = 0; i < reps; i++)
    {
        assert(sm.empty());

        for (int j = 0; j < elems; j++)
        {
            sm.push(j);
        }

        for (int j = 0; j < elems; j++)
        {
            sm.pop();
        }
    }
    std::cout << "Use MemoryPool time is : ";
    std::cout << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
    return 0;
}
