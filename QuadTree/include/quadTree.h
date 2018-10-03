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
        void qtrangefind (point A, node**& p);
        void addpoint (point A);
        void split(node**& zone);
        void incircle(node* p,vector<point>* points,point A, int radio);
        double distancia(point A,point B);
        virtual ~quadTree();

    protected:

    private:
};

#endif // QUADTREE_H
