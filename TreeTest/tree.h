#pragma once

#include <memory>


template<typename T>
class Node
{
public:
    using Ptr = std::shared_ptr<Node>;

public:
    // life-cycle
    Node(T data)
        : _data(std::make_shared<T>(data))
    {
    }

    Node(T data, Ptr left, Ptr right)
        : Node(data)
        , _left(left)
        , _right(right)
    {
    }


public:
    // properties
    T Data() { return *_data; }
    void Data(T data) { _data = std::make_shared<T>(data); }
    void Data(std::shared_ptr<T> data) { _data = data; }

    Ptr& Left() { return _left; }
    Ptr& Right() { return _right; }

public:
    static Ptr Create(T data) { return std::make_shared<Node<T>>(data); }
    static Ptr Create(std::shared_ptr<T> data) { return std::make_shared<Node<T>>(data); }

private:
    std::shared_ptr<T> _data;
    Ptr _left;
    Ptr _right;
};

