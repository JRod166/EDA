#ifndef NODE_H
#define NODE_H
#include <utility>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
using namespace std;
typedef double typecor;
typedef pair<typecor,typecor> point;
#define dis 0.25

class Node
{
    public:
        Node();
        virtual ~Node();
        point cmin, cmax;
        vector <Node*> children;
        point* data;
        void set_rec ();
        Node (point minim, point maxim);
        Node (point d);

    protected:

    private:
};



#endif // Node_H
