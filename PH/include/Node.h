#ifndef NODE_H
#define NODE_H
#define BS 32

#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef vector<float> point;


class Node
{
    public:
        string data;
        string postfix;
        Node* Point_to;
        Node(string _bs,int dim);
        Node();
        vector<Node*> NodeVec;
        virtual ~Node();

    protected:

    private:
};

#endif // NODE_H
