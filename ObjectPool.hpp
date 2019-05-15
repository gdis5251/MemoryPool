#pragma once
#include <iostream>

template <class T>
class ObjectPool
{
private:
    //管理内存块
    struct Block
    {
        //按字节数去管理，比较好控制
        //内存块管理
        char* _start = nullptr;
        
        size_t _byte_size = 0; //表示块有多少个字节
        size_t _byte_left = 0; //表示块剩余多少个字节
        Block* _next = nullptr; //链接下一个块


        Block(size_t byte_size)
        {
            _start = (char*)malloc(byte_size);
            _byte_size = byte_size;
            _byte_left = byte_size;
            _next = nullptr;
        }

        ~Block()
        {
            free(_start);
            _byte_left = 0;
            _byte_size = 0;
            _next = nullptr;
        }
    };

public:
    //init_num 表示第一次申请多少个对象
    ObjectPool(size_t init_num = 32)
    {
        //申请出来第一个块
        _head = _tail = new Block(init_num * _obj_size);
    }

    ~ObjectPool()
    {
        Destory();
    }

    T*& ObjNext(T* memory)
    {
        return *(T**)memory;
    }

    //只提供每次调用提供一个对象
    T* New()
    {
        T* obj = nullptr;
        //如果空闲链表有结点，优先在空闲链表里面取内存
        if (_free_list != nullptr)
        {
            obj = _free_list;
            _free_list = ObjNext(_free_list);
        }
        else//表示空闲链表没有结点，这个时候要在Block内存块里切内存
        {
            //先检查Block里面还还没有内存
            //如果当前的Block已经没有空间可以分配
            if (_tail->_byte_left == 0)
            {
                Block* newBlock = new Block(_tail->_byte_size * 2);
                _tail->_next = newBlock;
                _tail = newBlock;
            }

            //走到这里表示一定有内存
            obj = (T*)_tail->_start + (_tail->_byte_size - _tail->_byte_left);
            //将剩余的字节数更新
            _tail->_byte_left -= _obj_size;

        }
        new(obj) T();
        return obj;
    }

    void Delete(T* memory)
    {
        memory->~T();
        if (_free_list == nullptr)
        {
            _free_list = memory;

            //(*(T**)memory) = nullptr;
            ObjNext(memory) = nullptr;
        }
        else
        {
            //(*(T**)memory) = _free_list;
            ObjNext(memory) = _free_list;
            _free_list = memory;
        }
    }

    void Destory()
    {
        while (_head != nullptr)
        {
            delete _head;
            _head = _head->_next;
        }

        _head = nullptr;
        _tail = nullptr;
    }

private:
    static size_t ObjSize()
    {
        return sizeof(T) > sizeof(T*) ? sizeof(T) : sizeof(T*);
    }

private:
    //空闲链表管理指针
    T* _free_list = nullptr;

    //块管理
    Block* _head = nullptr;
    Block* _tail = nullptr;

    //对象大小，因为可能对象的字节数小于指针
    static size_t _obj_size;
};

template <class T>
size_t ObjectPool<T>::_obj_size = ObjectPool<T>::ObjSize();
