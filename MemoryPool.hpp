#pragma once
#include <iostream>

template <class T>
class MemoryPool
{
private:
    struct Block
    {
        char* _block;
        Block* _next;

        explicit Block(size_t init_num = 32)
            :_next(nullptr)
        {
            _block = (char*)malloc(sizeof(Obj) * init_num);
        }

        ~Block()
        {
            free(_block);
            _block = nullptr;
        }
    };

public:
    MemoryPool(size_t init_num = 32)
    {
        _current_mem = (char*)new Block(init_num);
        _head = _tail = (Block*)_current_mem;
    }

    ~MemoryPool()
    {
        Destory();
        std::cout << "~MemoryPool" << std::endl;
    }

    T* New()
    {
        if (_free_list != nullptr)
        {
            T* ret = (T*)_free_list;
            _free_list = _free_list->_next;
            _obj_left--;
            return ret;
        }
        else if(_current_mem != nullptr && _obj_left > 0)
        {
            T* ret = (T*)_current_mem;
            _current_mem += sizeof(Obj);
            _obj_left--;
            return ret;
        }

        _obj_num *= 2;
        _obj_left = _obj_num - 1;
        _current_mem = (char*)new Block(_obj_num);

        if (_head == nullptr)
        {
            _head = (Block*)_current_mem;
            _tail = (Block*)_current_mem;
        }
        else
        {
            _tail->_next = (Block*)_current_mem;
            _tail = (Block*)_current_mem;
        }

        T* ret = (T*)_current_mem;
        _current_mem += sizeof(Obj);
        return ret;
    }

    void Delete(T* memory)
    {
        reinterpret_cast<Obj*>(memory)->_next = _free_list;
        _free_list = reinterpret_cast<Obj*>(memory);
    }

private:
    void Destory()
    {
        while (_head != _tail->_next)
        {
            delete (char*)_head;
            _head = _head->_next;
        }

        _head = nullptr;
        _tail = nullptr;
    }

private:
    union Obj
    {
        T _obj;
        Obj* _next;
    };

    size_t _obj_num = 32;
    int _obj_left = 32;
    Obj* _free_list = nullptr;
    Block* _head = nullptr;
    Block* _tail = nullptr;
    char* _current_mem = nullptr;
};

