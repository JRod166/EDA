#ifndef RTREE_H
#define RTREE_H

#include <Node.h>

using namespace std;

class RTree
{
    public:
        int threshold;
        Node* root;
        RTree();
        RTree(int ts);
        virtual ~RTree();
        Node* Search(Node* T, point S);
        void Insert(point E);
        void ChooseLeaf(Node** &p,point E);
        void ChooseNode(Node** &p,Node* E);
        void exhNodeSearch(Node** &p,Node* q, Node* E);
        Node* AdjustTree(Node* L, Node* LL);
        Node* SplitLeaf(Node** &p,point E);
        Node* SplitNode(Node** &p, Node* E);
        void PickSeeds(Node* L);
        void NextSeeds(Node* L);

    protected:

    private:
};

#endif // RTREE_H
