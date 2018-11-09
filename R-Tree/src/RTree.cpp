#include "RTree.h"

using namespace std;

RTree::RTree()
{
    //ctor
}

RTree::RTree(int ts)
{
    threshold=ts;
    root=nullptr;
}

void RTree::Insert(point E)
{
    if(!root)
    {
        root=new Node(E,E);
        return;
    }
    Node **L;
    Node* LL=nullptr;
    Node* rootsplit=nullptr;
    ChooseLeaf(L,E);
    if((*L)->data.size()<threshold)
    {
        (*L)->Install(E);
        //cout<<"Installed"<<endl;
    }
    else
    {
        //cout<<"Uh, oh, we need a leaf split"<<endl;
        LL=SplitLeaf(L,E);
    }
    //cout<<"not adjusted"<<endl;
    rootsplit=AdjustTree((*L),LL);
    /*cout<<"adjusted"<<endl;
    cout<<"Root: "<<endl;
    cout<<"cmin: "<<root->cmin.first<<","<<root->cmin.second<<endl;
    cout<<"cmax: "<<root->cmax.first<<","<<root->cmax.second<<endl;*/
    if(rootsplit)
    {
        //cout<<"root splited"<<endl;
        Node* p=new Node();
        p->children.push_back(rootsplit);
        p->children.push_back(root);
        p->cmin=make_pair(min(rootsplit->cmin.first,root->cmin.first),min(rootsplit->cmin.second,root->cmin.second));
        p->cmax=make_pair(max(rootsplit->cmax.first,root->cmax.first),max(rootsplit->cmax.second,root->cmax.second));
        root=p;

    }
}

void RTree::ChooseLeaf(Node**& N,point E)
{
    N=&root;
    double area1,area2,increaseArea,minarea;
    int chosenleaf;
    Node* aux;
    //cout<<"choosing";
    while((*N)->children.size()>0)
    {
        //cout<<".";
        for(int i=0;i<(*N)->children.size();i++)
        {
            aux=(*N)->children[i];
            area2=area1;
            area1=abs((min(aux->cmin.first,E.first)-(max(aux->cmax.first,E.first))))*abs((min(aux->cmin.second,E.second)-(max(aux->cmax.second,E.second))));
            increaseArea=area1-aux->area;
            //cout<<increaseArea<<endl;
            if(i==0)
            {
                minarea=increaseArea;
                chosenleaf=i;
            }
            else if(increaseArea<minarea)
            {
                minarea=increaseArea;
                chosenleaf=i;
            }
            else if(increaseArea=minarea)
            {
                if(area1<area2)
                {
                    chosenleaf=i;
                }
            }
        }
        //cout<<chosenleaf<<endl;
        N=&((*N)->children[chosenleaf]);
    }
    //cout<<endl;
    return;
}

void RTree::ChooseNode(Node**& N,Node* E)
{
    N=&root;
    double area1,area2,increaseArea,minarea;
    int chosenleaf;
    Node* aux;
    //cout<<"Choosing node";
    if((*N)->children.size()==0)
    {
        return;
    }
    while((*N)->children.size()>0)
    {
        //cout<<".";
        for(int i=0;i<(*N)->children.size();i++)
        {
            aux=(*N)->children[i];
            if(aux==E)
            {
                return;
            }
            if(E->cmin.first>=aux->cmin.first && E->cmin.second >= aux->cmin.second && E->cmax.first<=aux->cmax.first && E->cmax.second <= aux->cmax.second)
            {
                N=&((*N)->children[i]);
                break;
            }
            if(i==(*N)->children.size()-1)
            {
                N=&root;
                Node* q=root;
                exhNodeSearch(N,q,E);
                return;
            }
        }
    }
}

void RTree::exhNodeSearch(Node** &N, Node* q, Node* E)
{
    if(q->children.size()==0) {return;}
    for(int i=0;i<q->children.size();i++)
    {
        exhNodeSearch(N,q->children[i],E);
        if(q->children[i]==E)
        {
            N=&q;
            return;
        }
    }
}

Node* RTree::AdjustTree(Node* L, Node* LL)
{
    //cout<<"Adjusting tree"<<endl;
    Node *N =L;
    Node *NN=LL;
    Node** P;
    Node* roots=nullptr;
    Node* aux,*aux2;
    while(N!=root){
    ChooseNode(P,N);
    //cout<<L->cmin.first<<","<<L->cmin.second<<endl;
    //cout<<L->cmax.first<<","<<L->cmax.second<<endl;
    //cout<<"chosen node"<<endl;
    if(NN)
    {
        //cout<<"splitted node"<<endl;
        //cout<<(*P)->children.size()<<endl;
        if((*P)->children.size()>=threshold)
        {
            //cout<<"Uh, Oh, we need an upper split"<<endl;
            LL=SplitNode(P,NN);
        }
        else
        {
            //cout<<"push splitted"<<endl;
            (*P)->children.push_back(NN);
            LL=nullptr;
        }
    }
    //cout<<"time to adjust"<<endl;
    for(int i =0;i<(*P)->children.size();i++)
    {
        //if((*P)==root){cout<<"raiz"<<endl;}
        aux=(*P)->children[i];
        //cout<<(*P)->cmin.first<<","<<(*P)->cmin.second<<"->"<<aux->cmin.first<<","<<aux->cmin.second<<endl;
        //cout<<(*P)->cmax.first<<","<<(*P)->cmax.second<<"->"<<aux->cmax.first<<","<<aux->cmax.second<<endl;
        (*P)->cmin=make_pair((min((*P)->cmin.first,aux->cmin.first)),(min((*P)->cmin.second,aux->cmin.second)));
        (*P)->cmax=make_pair((max((*P)->cmax.first,aux->cmax.first)),(max((*P)->cmax.second,aux->cmax.second)));
    }
    N=(*P);
    //ChooseNode(P,L);
    }
    return LL;
}

Node* RTree::SplitNode(Node** &L,Node* E)
{
    ///quadratic cost algorithm
    Node* LL=new Node();
    double area=0,area2,aux;
    int group1,group2;
    vector <Node*> data;
    data=(*L)->children;
    data.push_back(E);
    (*L)->children.clear();
    ///PickSeeds
    //cout<<"picking seeds"<<endl;
    for(int i=0;i<data.size();i++)
    {
        for(int j=0;j<data.size();j++)
        {
            aux=abs(min(data[i]->cmin.first,data[j]->cmin.first)-max(data[i]->cmax.first,data[j]->cmax.first))*abs(min(data[i]->cmin.second,data[j]->cmin.second)-max(data[i]->cmax.second,data[j]->cmax.second));
            if(aux>=area)
            {
                area=aux;
                group1=i;
                group2=j;
            }
        }
    }
    (*L)->children.push_back(data[group1]);
    (*L)->cmin=data[group1]->cmin;
    (*L)->cmax=data[group1]->cmax;
    LL->children.push_back(data[group2]);
    LL->cmin=data[group2]->cmin;
    LL->cmax=data[group2]->cmax;
    data.erase(data.begin()+max(group1,group2));
    data.erase(data.begin()+min(group1,group2));
    ///picknexts
    while(data.size()>0)
    {
        aux=0;
        for(int i=0;i<data.size();i++)
        {
            area2=abs(min((*L)->cmin.first,data[i]->cmin.first)-max((*L)->cmax.first,data[i]->cmax.first))*abs(min((*L)->cmin.second,data[i]->cmin.second)-max((*L)->cmax.second,data[i]->cmax.second));
            area=abs(min(LL->cmin.first,data[i]->cmin.first)-max(LL->cmax.first,data[i]->cmax.first))*abs(min(LL->cmin.second,data[i]->cmin.second)-max(LL->cmax.second,data[i]->cmax.second));
            if(abs(area2-area)>=0)
            {
                if(area2<area)
                {
                    group1=1;
                }
                else
                {
                    group1=2;
                }
                group2=i;
            }
        }
        if(group1==1)
        {
            (*L)->children.push_back(data[group2]);
            (*L)->cmin=make_pair(min((*L)->cmin.first,data[group2]->cmin.first),min((*L)->cmin.second,data[group2]->cmin.second));
            (*L)->cmax=make_pair(max((*L)->cmax.first,data[group2]->cmax.first),max((*L)->cmax.second,data[group2]->cmax.second));
            data.erase(data.begin()+group2);
        }
        else
        {
            LL->children.push_back(data[group2]);
            LL->cmin=make_pair(min(LL->cmin.first,data[group2]->cmin.first),min(LL->cmin.second,data[group2]->cmin.second));
            LL->cmax=make_pair(max(LL->cmax.first,data[group2]->cmax.first),max(LL->cmax.second,data[group2]->cmax.second));
            data.erase(data.begin()+group2);
        }
    }
    return LL;
}

Node* RTree::SplitLeaf(Node** &L,point E)
{
    ///quadratic cost algorithm
    Node* LL=new Node();
    double area=0,area2,aux;
    int group1,group2;
    vector <point> data;
    data=(*L)->data;
    data.push_back(E);
    (*L)->data.clear();
    //cout<<"Picking seeds"<<endl;
    ///PickSeeds
    for(int i=0;i<data.size();i++)
    {
        for(int j=0;j<data.size();j++)
        {
            aux=abs(data[i].first-data[j].first)*abs(data[i].second-data[j].second);
            if(aux>=area)
            {
                area=aux;
                group1=i;
                group2=j;
            }
        }
    }
    (*L)->data.push_back(data[group1]);
    (*L)->cmin=data[group1];
    (*L)->cmax=data[group1];
    LL->data.push_back(data[group2]);
    LL->cmax=data[group2];
    LL->cmin=data[group2];
    data.erase(data.begin()+max(group1,group2));
    data.erase(data.begin()+min(group1,group2));
    ///picknexts
    while(data.size()>0)
    {
        aux=0;
        for(int i=0;i<data.size();i++)
        {
            area2=abs(min((*L)->cmin.first,data[i].first)-max((*L)->cmax.first,data[i].first))*abs(min((*L)->cmin.second,data[i].second)-max((*L)->cmax.second,data[i].second));
            area=abs(min(LL->cmin.first,data[i].first)-max(LL->cmax.first,data[i].first))*abs(min(LL->cmin.second,data[i].second)-max(LL->cmax.second,data[i].second));
            if(abs(area2-area)>=0)
            {
                if(area2<area)
                {
                    group1=1;
                }
                else
                {
                    group1=2;
                }
                group2=i;
            }
        }
        if(group1==1)
        {
            (*L)->Install(data[group2]);
            data.erase(data.begin()+group2);
        }
        else
        {
            LL->Install(data[group2]);
            data.erase(data.begin()+group2);
        }
    }/*
    //cout<<"Group 1:"<<endl;
    for(int i=0;i<(*L)->data.size();i++)
    {
        //cout<<(*L)->data[i].first<<","<<(*L)->data[i].second<<endl;
    }
    //cout<<"cmin: "<<(*L)->cmin.first<<","<<(*L)->cmin.second<<endl;
    //cout<<"cmax: "<<(*L)->cmax.first<<","<<(*L)->cmax.second<<endl;
    //cout<<"Group 2:"<<endl;
    for(int i=0;i<LL->data.size();i++)
    {
        //cout<<LL->data[i].first<<","<<LL->data[i].second<<endl;
    }
    //cout<<"cmin: "<<LL->cmin.first<<","<<LL->cmin.second<<endl;
    //cout<<"cmax: "<<LL->cmax.first<<","<<LL->cmax.second<<endl;*/
    return LL;
}

RTree::~RTree()
{
    //dtor
}

