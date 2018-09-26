#include "quadTree.h"

quadTree::quadTree()
{
    //ctor
}

quadTree::quadTree(int ts, int length_ejex, int length_ejey)
{
    treeshold=ts;
    ejex=length_ejex;
    ejey=length_ejey;
    root=new node(make_pair(ejex*-1,ejey*-1),make_pair(ejex,ejey));
}

void quadTree::qtfind(point A, node**& p)
{

    p=&root;
    int ind=-1;
    typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    dis/=2;
    while ((*p)->children[0]!=0)
    {
        dis=(*p)->cmax.first-(*p)->cmin.first;
        dis/=2;
        ind=-1;
        if (A.first>=(*p)->cmax.first-dis){
            ind+=2;
        }
        else
        {
            ind++;
        }
        if (A.second>=(*p)->cmax.second-dis)
        {
            ind+=2;
        }
        p=&((*p)->children[ind]);
    }
}

void quadTree::addpoint(point A)
{
    node** p;
    qtfind(A,p);
    (*p)->data.push_back(A);
    if((*p)->data.size()>treeshold)
    {
        split(p);
    }
}

void quadTree::split(node**& zone )
{
    typecor dis=(*zone)->cmax.first-(*zone)->cmin.first;
    dis/=2;
    int ind=0;
    typecor miniX,miniY;
    miniX=(*zone)->cmin.first;
    miniY=(*zone)->cmin.second;
    for(int i=0;i<2;i++)
    {
        for (int j=0;j<2;j++)
        {
            (*zone)->children[ind]=new node(make_pair(miniX+(dis*j),miniY+(dis*i)),make_pair(miniX+(dis+dis*j),miniY+(dis+dis*i)));
            ind++;
        }
    }
    node **p;
    for(int i=0;i<(*zone)->data.size();i++)
    {
        qtfind((*zone)->data[i],p);
        (*p)->data.push_back((*zone)->data[i]);
    }
    (*zone)->data.resize(0);

}

quadTree::~quadTree()
{
    //dtor
}
