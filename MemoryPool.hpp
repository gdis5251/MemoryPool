#pragma once
#include <climits>
#include <cstddef>
#include <cstdint>

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
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap;           

    template <typename U> 
    struct rebind
    {
        typedef MemoryPool<U> other;
    };

    //Member function
    MemoryPool()
    {
        _currentBlock = nullptr;
        _currentSlot = nullptr;
        _lastSlot = nullptr;
        _freeSlots = nullptr;
    }

    ~MemoryPool()
    {
        _slot_pointer cur = _currentBlock;
        while (cur != nullptr)
        {
            _slot_pointer next = cur->next;
            operator delete(reinterpret_cast<void*>(cur));
            cur = next;
        }
    }

    pointer allocate(size_t n = 1, const_pointer hint = 0)
    {
        if (_freeSlots != nullptr)
        {
            pointer result = reinterpret_cast<pointer>(_freeSlots);
            _freeSlots = _freeSlots->next;
            return result;
        }
        else
        {
            if (_currentSlot >= _lastSlot)
                allocateBlock();
            return reinterpret_cast<pointer>(_currentSlot++);
        }
    }

    void deallocate(pointer p, size_type n = 1)
    {
        if (p != nullptr)
        {
            reinterpret_cast<_slot_pointer>(p)->next = _freeSlots;
            _freeSlots = reinterpret_cast<_slot_pointer>(p);
        }
    }


private:
    union _Slot
    {
        value_type element;
        _Slot* next;
    };

    typedef char* _data_pointer;
    typedef _Slot _slot_type;
    typedef _Slot* _slot_pointer;

    _slot_pointer _currentBlock;
    _slot_pointer _currentSlot;
    _slot_pointer _lastSlot;
    _slot_pointer _freeSlots;

    void allocateBlock()
    {
        //先开好一片区域
        _data_pointer newBlock = reinterpret_cast<_data_pointer>(operator new(BlockSize));
        
        //然后把这个新开的空间跟旧空间连在一起
        reinterpret_cast<_slot_pointer>(newBlock)->next = _currentBlock;
        _currentBlock = reinterpret_cast<_slot_pointer>(newBlock);

        //让内存对齐
        _data_pointer body = newBlock + sizeof(_slot_pointer);
        size_type bodyPadding = padPointer(body, alignof(_slot_type));
        _currentSlot = reinterpret_cast<_slot_pointer>(body + bodyPadding);
        _lastSlot = reinterpret_cast<_slot_pointer>
            (newBlock + BlockSize - sizeof(_slot_type) + 1);
    }

    size_type padPointer(_data_pointer p, size_type align) const noexcept
    {
        uintptr_t result = reinterpret_cast<uintptr_t>(p);
        return ((align - result) % align);
                            
    }

    static_assert(BlockSize >= 2 * sizeof(_slot_type), "BlockSize too small.");
};
