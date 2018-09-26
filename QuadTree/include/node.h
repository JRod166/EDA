#ifndef NODE_H
#define NODE_H
#include <utility>
#include <vector>
#include <math.h>
using namespace std;
typedef double typecor;
typedef pair<typecor,typecor> point;

class node
{
    public:
        node();
        virtual ~node();
        point cmin, cmax;
        vector <node*> children;
        vector <point> data;
        node (point cminim, point cmaxim);

    protected:

    private:
};




#endif // NODE_H
