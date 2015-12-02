
#include "tree.h"

#include <functional>
#include <iostream>
#include <stack>


void VisitNode(Node<int>::Ptr node)
{
    std::cout << node->Data() << std::endl;
}



void TraverseInOrder(Node<int>::Ptr node, std::function<void (Node<int>::Ptr node)> visit)
{
    std::stack<Node<int>::Ptr> parents;
    while (node != nullptr || !parents.empty())
    {
        if (node != nullptr) {
            parents.push(node);
            node = node->Left();
        }
        else {
            node = parents.top();
            parents.pop();
            visit(node);
            node = node->Right();
        }
    }
}



int main(int argc, char* argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    Node<int>::Ptr tree = Node<int>::Create(10);
    tree->Left() = Node<int>::Create(5);
    tree->Left()->Left() = Node<int>::Create(2);
    tree->Left()->Left()->Left() = Node<int>::Create(1);
    tree->Left()->Left()->Right() = Node<int>::Create(3);
    tree->Left()->Left()->Right()->Right() = Node<int>::Create(4);
    tree->Left()->Right() = Node<int>::Create(7);
    tree->Left()->Right()->Left() = Node<int>::Create(6);
    tree->Left()->Right()->Right() = Node<int>::Create(8);
    tree->Left()->Right()->Right()->Right() = Node<int>::Create(9);
    tree->Right() = Node<int>::Create(15);
    tree->Right()->Left() = Node<int>::Create(12);
    tree->Right()->Left()->Left() = Node<int>::Create(11);
    tree->Right()->Left()->Right() = Node<int>::Create(13);
    tree->Right()->Left()->Right()->Right() = Node<int>::Create(14);
    tree->Right()->Right() = Node<int>::Create(17);
    tree->Right()->Right()->Left() = Node<int>::Create(16);
    tree->Right()->Right()->Right() = Node<int>::Create(19);
    tree->Right()->Right()->Right()->Left() = Node<int>::Create(18);
    tree->Right()->Right()->Right()->Right() = Node<int>::Create(20);

    TraverseInOrder(tree, VisitNode);

    std::cout << "Program complete" << std::endl;

    exit(EXIT_SUCCESS);
}
