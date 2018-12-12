#include "Node.h"

Node::Node(string _bs,int dim)
{
    data=_bs.substr(0,dim);
    //cout<<data<<endl;
    postfix=_bs.substr(dim,_bs.size()-dim);
    //cout<<postfix<<endl;
    Point_to=NULL;
    //ctor
}

Node::Node()
{
    data="";
    postfix="";
    Point_to=NULL;
}

Node::~Node()
{
    //dtor
}
