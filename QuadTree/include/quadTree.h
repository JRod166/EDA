#ifndef QUADTREE_H
#define QUADTREE_H

#include "node.h"


class quadTree
{
    public:
        int ejex, ejey;
        unsigned int treeshold;
        node* root;
        quadTree();
        quadTree(int ts, int length_ejex, int length_ejey);
        void qtfind (point A, node**& p);
        void addpoint (point A);
        void split(node**& zone);
        virtual ~quadTree();

    protected:

    private:
};

#endif // QUADTREE_H
