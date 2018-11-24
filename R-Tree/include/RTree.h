#ifndef RTREE_H
#define RTREE_H
#include "Node.h"
#include <stack>


class RTree
{
    public:
        unsigned int m;
        RTree();
        RTree(unsigned int max_children);
        void Insert (point d);
        typecor euclidian(point A, point B);
        typecor distance_rec_point(point d,point cmin, point cmax);
        void newLimits(point Amin,point Amax,point Bmin, point Bmax, point& Cmin, point& Cmax);
        void adjusttree (Node**& p,stack<Node**>& path);
        void splitNode (Node**& p,stack<Node**>& path);
        void pickseeds (int& ii, int& jj, Node** p);
        bool findpointleaf (Node**& p,point d);
        bool overlap(point p, point gmin, point gmax);
        void picknext (int& ii, Node** p,vector<Node*>&temp,Node* l, Node* r,typecor& d1f,typecor& d2f);
        typecor area (point cmin, point cmax);
        Node* root;
        void circle(Node* p,vector<point>* points,point A, int radio);
        void chooseleaf (point d, Node**& p,stack<Node**>& path);
        virtual ~RTree();

    protected:

    private:
};

#endif // RTREE_H
