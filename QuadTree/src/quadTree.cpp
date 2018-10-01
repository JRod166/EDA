#include "quadTree.h"
#include <iostream>

using namespace std;

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

void quadTree::qtrangefind(point A, node**& p)
{
    p=&root;
    int ind=-1;
    typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    dis/=2;
    while ((*p)->children[0])
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

double quadTree::distancia(point A, point B)
{
    double res;
    res=pow((A.first-B.first),2)+pow((A.second-B.second),2);
    res=sqrt(res);
    return res;
}

void quadTree::addpoint(point A)
{
    node** p;
    qtrangefind(A,p);
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
        qtrangefind((*zone)->data[i],p);
        (*p)->data.push_back((*zone)->data[i]);
    }
    (*zone)->data.resize(0);

}

vector<node*> quadTree::findcircleareas(point A, int radio)
{
    node **p;
    vector<node*> zones;
    p=&root;
    int ind=-1;
    typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    dis/=2;
    while ((*p)->children[0])
    {
        for(int i=0;i<4;i++)
        {
            point B=make_pair((*p)->cmax.first,(*p)->cmin.second);
            point C=make_pair((*p)->cmin.first,(*p)->cmax.second);
            if(distancia(A,(*p)->cmin)<=radio || distancia(A,(*p)->cmax)<=radio || distancia(A,B)<=radio || distancia(A,C)<=radio   )
            {
                zones.push_back((*p)->children[i]);
            }
        }
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
    zones.push_back((*p));
    return zones;
}

vector <point> quadTree::findcircle(point A,int radio)
{
    vector< node*> zones;
    vector <point> res;
    node  **p;
    zones=findcircleareas(A,radio);
    cout<<zones.size()<<endl;
    for(int i=0;i<zones.size();i++)
    {
        for(int j=0;j<zones[i]->data.size();j++)
        {
            if(distancia(A,zones[i]->data[j])<=radio)
            {
                res.push_back(zones[i]->data[j]);
            }
        }
    }
    return res;

}


quadTree::~quadTree()
{
    //dtor
}
