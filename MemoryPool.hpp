#pragma once
#include <climits>
#include <cstddef>
#include <cstdint>
#include <memory.h>
#include <iostream>

template <class T, size_t BlockSize = 4096>
class MemoryPool
{
public:
    //Member Types  与标准库里的allocator保持一致
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef const T*        const_pointer;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

    template <typename U> 
    struct rebind
    {
        typedef MemoryPool<U> other;
    };

    inline MemoryPool() noexcept 
    {
        MemoryBlock* m = new MemoryBlock;
        m->_head = nullptr;
        m->_next = nullptr;
        _memory_block = m;

        _free_node = nullptr;
    }

    inline virtual ~MemoryPool() noexcept
    {
        if (_memory_block->_head != nullptr)
        {
            MemoryBlock* next = _memory_block->_head->_next;
            operator delete(reinterpret_cast<void*>(_memory_block->_head));

            _memory_block->_head = next;
        }
    }

    void* CreateBlock()
    {
        char* newBlock = reinterpret_cast<char*>(operator new(BlockSize));
        //让内存块连起来
        if (_memory_block->_head == nullptr)
        {
            _memory_block->_head = reinterpret_cast<MemoryBlock*>(newBlock);
            _memory_block->_next = nullptr;
        }
        else
        {
            reinterpret_cast<MemoryBlock*>(newBlock)->_next = _memory_block->_head;
            _memory_block->_head = reinterpret_cast<MemoryBlock*>(newBlock);
        }

        //让空闲结点指向可用内存块
        _free_node = reinterpret_cast<FreeNode*>(newBlock + sizeof(MemoryBlock));
        char* cur = reinterpret_cast<char*>(_free_node);
        FreeNode* prev = _free_node;

        //让剩下的可用空间全部作为FreeNode结点并连起来
        for (int i = 2; BlockSize - sizeof(MemoryBlock) - (sizeof(FreeNode) * i) > sizeof(FreeNode); i++)
        {
            cur = cur + sizeof(FreeNode);
            prev->_next = reinterpret_cast<FreeNode*>(cur);
            prev = reinterpret_cast<FreeNode*>(cur);
        }
    }

    T* allocate(size_type n = 1)
    {
        if (_free_node == nullptr) //如果没有空闲结点了，就在申请一个内存块
        {
            CreateBlock();
        }
        T* res = reinterpret_cast<T*>(_free_node);
        _free_node = _free_node->_next;
        
        return res;
    }

    void deallocate(T* p, size_type n = 1)
    {
        if (p != nullptr)
        {
            reinterpret_cast<FreeNode*>(p)->_next = _free_node;
            _free_node = reinterpret_cast<FreeNode*>(p);
        }
    }


private:
    struct FreeNode
    {
        T _data;
        FreeNode* _next;
    };

    struct MemoryBlock
    {
        MemoryBlock* _head;
        MemoryBlock* _next;
    };

    typedef struct FreeNode FreeNode;
    typedef struct MemoryBlock MemoryBlock;

    FreeNode *_free_node;
    MemoryBlock *_memory_block;
};
