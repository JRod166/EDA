#ifndef PATRICIA_H
#define PATRICIA_H
#include "Node.h"
#include "HyperDate.h"
#include <cmath>


//#include <unordered_set> //key, hash, equal_to, allocator

class Patricia
{
    public:
        //unordered_set<Node,NodeHash,NodeCompare> root; //vector de nodos, usa hash para el rapido aceso
        vector <Node*> root;
        int size;
        int dim;
        Patricia(int _dim);
        bool insert(point _Pt);
        bool match_root(Node **& Ptr,HyperDate HD);
        int find_bifurcation(Node **& Ptr,HyperDate & HD);
        void proximity(point _Pt);
        int Binary2Hex(string bin);
        void getData(HyperDate HD,Node* _Pt,string root_val,int &cont);
        bool find_point(Node**& Ptr,HyperDate _HD, vector <Node*> & _Path );
        void ToFloat(string Date);
        float GetFloat32(string Bin);
        virtual ~Patricia();

    protected:

    private:
};

#endif // PATRICIA_H
