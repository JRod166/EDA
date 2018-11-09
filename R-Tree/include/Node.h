#ifndef NODE_H
#define NODE_H

#include <utility>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

typedef double typecor;
typedef pair<double,double> point;

class Node
{
    public:
        point cmin;
        point cmax;
        double area;
        vector<Node*> children;
        vector <point> data;
        Node();
        virtual ~Node();
        Node (point cmin, point cmax);
        void Install(point E);

    protected:

    private:
};

#endif // NODE_H
