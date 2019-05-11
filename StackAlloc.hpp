#pragma once 
#include <iostream>
#include <memory>

//define StackAlloc struct
template <class T>
struct StackNode
{
    T _val;
    StackNode* _next;
};

template <class T, class Alloc = std::allocator<T> >
class StackAlloc
{
public:
    typedef StackNode<T> Node;
    typedef class Alloc::template rebind<Node>::other allocator;

    StackAlloc()
        :_head(nullptr)
    {}

    ~StackAlloc()
    {
        clear();
    }

    T top()
    {
        return _head->_val;
    }

    bool empty()
    {
        return _head == nullptr;
    }

    void push(const T val)
    {
        Node* cur = _allocator.allocate(1);
        //不调用_allcator.construct是因为不对创建的内存进行初始化
        //提高效率

        cur->_val = val;
        cur->_next = _head;

        _head = cur;
    }

    T& pop()
    {
        T& result = _head->_val;
        Node* cur = _head->_next;

        _allocator.deallocate(_head, 1);
        //这里不调用_allocator.destroy也是为了提高效率
        _head = cur;

        return result;
    }

    void clear()
    {
        Node* cur = _head;

        while (cur != nullptr)
        {
            Node* tmp = cur->_next;

            _allocator.deallocate(cur, 1);
            cur = tmp;
        std::cout << 'a' << std::endl;
        }

        _head = nullptr;
    }


private:
    Node* _head;
    allocator _allocator;

};
