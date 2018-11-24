#include "RTree.h"

RTree::RTree()
{
    root=new Node;
    m=0;
}

RTree::RTree(unsigned int max_children)
{
    m=max_children;
    root=new Node;
}

bool RTree::overlap(point p, point gmin, point gmax)
{
    if(p.first>gmin.first && gmax.first>p.first && p.second>gmin.second && gmax.second>p.second)
        return 1;
    return 0;
}

bool RTree::findpointleaf (Node**& p,point d)
{
    if ((*p)->data!=0){
        if ((*(*p)->data)==d)
            return 1;
        return 0;
    }

    vector<Node**> possibleleafs;
    for (int unsigned i=0;i<(*p)->children.size();i++)
    {
        if(overlap(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax))
            possibleleafs.push_back(&((*p)->children[i]));
    }
    cout<<possibleleafs.size();
    for(int unsigned j=0;j<possibleleafs.size();j++)
    {
            p=possibleleafs[j];
            if(findpointleaf(p,d)){
                break;
            }
    }

}

void RTree::chooseleaf (point d, Node**& p, stack<Node**>& path)
{
    path.push(p);
    //cout<<"HIJOS: "<<(*p)->children.size()<<endl;
    if ((*p)->children.size()==0 || (*p)->children[0]->data!=0){
       // cout<<(*p)->data<<endl;
        return;
    }
   // cout<<"sale"<<endl;
    Node** F=&((*p)->children[0]);
    for (int unsigned i=1;i<(*p)->children.size();i++)
    {
        if (distance_rec_point(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax)<distance_rec_point(d,(*F)->cmin,(*F)->cmax))
            F=&((*p)->children[i]);
        else if  (distance_rec_point(d,(*p)->children[i]->cmin,(*p)->children[i]->cmax)==distance_rec_point(d,(*F)->cmin,(*F)->cmax))
        {
            if (area((*F)->cmin,(*F)->cmax)>area((*p)->children[i]->cmin,(*p)->children[i]->cmax))
                F=&((*p)->children[i]);
        }
    }
    p=F;
    chooseleaf(d,p,path);
  //  cout<<"DONE WELL CHoosE"<<endl;
}

void RTree::Insert(point d)
{
    stack<Node**> path;
    Node** p=&root;
    chooseleaf(d,p,path);
    (*p)->children.push_back(new Node (d));
    (*p)->children[(*p)->children.size()-1]->set_rec();
    if ((*p)->children.size()>m)
        {
            splitNode(p,path);
        }
    adjusttree(p,path);

   cout<<d.first<<","<<d.second<<" added."<<endl;
}

void RTree::adjusttree (Node**& p, stack<Node**>& path)
{
   // cout<<path.size()<<endl;
  // cout<<path.size()<<endl;
  (*path.top())->set_rec();
    if((*path.top())->children.size()>m){
        splitNode(path.top(),path);
    (*path.top())->set_rec();
    }
    if(p==&root)
        return;
    path.pop();
    p=path.top();
    adjusttree(p,path);
    //cout<<"DONE WELL AJUST"<<endl;
}

void RTree::splitNode (Node**& p, stack<Node**>& path)
{
    int i,j;
    Node *l,*r;
    vector <Node*> temp=(*p)->children;
    pickseeds(i,j,p);
    l=new Node;
    l->children.push_back((*p)->children[i]);
    l->set_rec();
    r=new Node;
    r->children.push_back((*p)->children[j]);
    r->set_rec();
    temp.erase(temp.begin()+j);
    temp.erase(temp.begin()+i);
    typecor d1,d2;
    for (int unsigned k=0;k<(*p)->children.size()-2;k++)
    {
        if (l->children.size()>(m/2))
        {
            r->children.insert(r->children.begin(),temp.begin(),temp.end());
            r->set_rec();
            break;
        }
        else if (r->children.size()>(m/2))
        {
            l->children.insert(l->children.begin(),temp.begin(),temp.end());
            l->set_rec();
            break;
        }
        picknext(i,p,temp,l,r,d1,d2);
        if(d1<d2)
        {
            l->children.push_back(temp[i]);
            l->set_rec();
        }
        else if(d2<d1)
        {
            r->children.push_back(temp[i]);
            r->set_rec();
        }
        else
        {
            if(area(l->cmin,l->cmax)>area(r->cmin,r->cmax))
            {
                r->children.push_back(temp[i]);
                r->set_rec();
            }
            else if (area(l->cmin,l->cmax)<area(r->cmin,r->cmax))
            {
                l->children.push_back(temp[i]);
                l->set_rec();
            }
            else
            {
                if(l->children.size()<r->children.size())
                {
                    l->children.push_back(temp[i]);
                    l->set_rec();
                }
                else
                {
                    r->children.push_back(temp[i]);
                    r->set_rec();
                }
            }

        }
        temp.erase(temp.begin()+i);
    }


    if(p==&root)
    {
        Node* aux=new Node;
        aux->children.push_back(l);
        aux->children.push_back(r);
        root=aux;
    }
    else
    {
        path.pop();
        for (int unsigned e=0;e<(*path.top())->children.size();e++){
            if((*path.top())->children[e]==*p)
            {
                (*path.top())->children.erase((*path.top())->children.begin()+e);
            }
        }
        //(*path.top())->children.pop_back();
        (*path.top())->children.push_back(l);
        (*path.top())->children.push_back(r);
        (*path.top())->set_rec();
        p=path.top();
        if((*path.top())->children.size()>m)
        {
            splitNode(p,path);
        }

    }
}

void RTree::newLimits(point Amin,point Amax,point Bmin, point Bmax, point& Cmin, point& Cmax)
{
    Cmin=make_pair(min(Amin.first,Bmin.first),min(Amin.second,Bmin.second));
    Cmax=make_pair(max(Amax.first,Bmax.first),max(Amax.second,Bmax.second));
}

void RTree::picknext (int& ii, Node** p, vector<Node*>&temp,Node* l, Node* r,typecor& d1f,typecor& d2f)
{
    typecor difference=0;
    typecor d1,d2;
    ii=0;
    for (int unsigned k=0;k<temp.size();k++)
    {
        point cmin,cmax;

        newLimits(l->cmin,l->cmax,temp[k]->cmin,temp[k]->cmax,cmin,cmax);
        d1=area(cmin,cmax);
        d1=d1-area(l->cmin,l->cmax);

        newLimits(r->cmin,r->cmax,temp[k]->cmin,temp[k]->cmax,cmin,cmax);
        d2=area(cmin,cmax);
        d2=d2-area(r->cmin,r->cmax);
        if (fabs(d2-d1)>difference)
        {
            ii=k;
            d1f=d1;
            d2f=d2;
        }
    }

}

void RTree::pickseeds (int& ii, int& jj ,Node** p)
{
    typecor maxArea=0;
    typecor aux;
    point cmin,cmax;
    ii=0;
    jj=1;
    for(int unsigned i=0;i<(*p)->children.size();i++)
            for(int unsigned j=i+1; j<(*p)->children.size();j++ )
                {
                    newLimits((*p)->children[i]->cmin,(*p)->children[i]->cmax,(*p)->children[j]->cmin,(*p)->children[j]->cmax,cmin,cmax);
                    //cout<<cmin.first<<","<<cmin.second<<" "<<cmax.first<<","<<cmax.second<<endl;
                    aux=area(cmin,cmax)-area((*p)->children[i]->cmin,(*p)->children[i]->cmax)-area((*p)->children[j]->cmin,(*p)->children[j]->cmax);
                 ///   cout<<"aux: "<<aux<<endl;
                    if (aux>maxArea)
                    {
                        maxArea=aux;
                        ii=i;jj=j;
                    }
                }
}


typecor RTree :: euclidian(point A, point B)
{
    typecor x = pow(A.first-B.first,2);
    typecor y = pow(A.second-B.second,2);
    return sqrt(x+y);
}
typecor RTree :: area (point cmin, point cmax)
{
    return fabs((cmax.first-cmin.first)*(cmax.second-cmin.second));
}

typecor RTree :: distance_rec_point(point d,point cmin, point cmax)
{
    point center = make_pair(cmax.first-((cmax.first-cmin.first)/2),cmax.second-((cmax.second-cmin.second)/2));
    return euclidian(center,d);
}

void RTree::circle(Node* p,vector<point>* points,point A, int radio)
{
    if(p->children.size()!=0)
    {
        for(int son2visit=0;son2visit<p->children.size();son2visit++)
        {
            if(A.first-radio <= p->cmax.first && A.first+radio >= p->cmin.first)
            {
                if(A.second-radio<= p->cmax.second && A.second+radio>= p->cmin.second)
                {
                    circle(p->children[son2visit],(points),A,radio);
                }
            }
        }
    }
    else
    {
        point B=make_pair(p->data->first,p->data->second);
        if(euclidian(A,B)<=radio)
        {
            points->push_back(B);
        }
    }
}


RTree::~RTree()
{
    //dtor
}
