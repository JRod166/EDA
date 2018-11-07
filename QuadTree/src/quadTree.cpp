#include "quadTree.h"
#include <iostream>

using namespace std;

int cont=0;
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

void quadTree::incircle(node* p,vector<point>* points,point A, int radio)
{
        ///recorrido
    ///if nodo hoja in circulo add all puntos
    ///else
    ///for each punto in hoja
    /// if punto in circulo add punto

    for(int son2visit=0;son2visit<4;son2visit++)
    {
        if(p->children[son2visit])
        {
            if(A.first-radio <= p->cmax.first && A.first+radio >= p->cmin.first)
            {
                if(A.second-radio<= p->cmax.second && A.second+radio>= p->cmin.second)
                {
                    incircle(p->children[son2visit],(points),A,radio);
                }
            }
        }
        else
        {
            point B,C;
            B=make_pair(p->cmax.first,p->cmin.second);
            C=make_pair(p->cmin.first,p->cmax.second);
            if(distancia(A,p->cmax)<=radio && distancia(A,p->cmin)<=radio && distancia(A,B)<=radio && distancia(A,C)<=radio)
            {
                points->insert(points->end(),p->data.begin(),p->data.end());
            }
            else
            {
                for(int j=0;j<p->data.size();j++)
                {
                    if(distancia(p->data[j],A)<=radio)
                    {
                        points->push_back(p->data[j]);
                    }
                }
            }

            break;
        }
    }
}




quadTree::~quadTree()
{
    //dtor
}
