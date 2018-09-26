#include "node.h"

node::node()
{
    //ctor
}

node::node(point cminim, point cmaxim)
{
    cmin=cminim;
    cmax=cmaxim;
    children.resize(4);
    for (int i=0; i<4;i++)
    {
        children[i]=nullptr;
    }
}

node::~node()
{
    //dtor
}
