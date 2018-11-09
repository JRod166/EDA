#include "Node.h"

Node::Node()
{
    //ctor
}

Node::Node(point minim, point maxim)
{
    cmin=minim;
    cmax=maxim;
    area=abs(cmin.first-cmax.first)*abs(cmin.second-cmax.second);
    data.push_back(minim);
}

void Node::Install(point E)
{
    cmin=make_pair(min(cmin.first,E.first),min(cmin.second,E.second));
    cmax=make_pair(max(cmax.first,E.first),max(cmax.second,E.second));
    area=abs(cmin.first-cmax.first)*abs(cmin.second-cmax.second);
    data.push_back(E);
}

Node::~Node()
{
    //dtor
}
