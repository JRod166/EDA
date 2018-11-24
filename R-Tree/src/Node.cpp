#include "Node.h"


bool byfirst (point a, point b)
{
    return a.first < b.first;
}

bool bysecond (point a, point b)
{
    return a.second < b.second;
}

Node::Node()
{
    data=0;
}

Node::Node(point d)
{
    data=new point (d);
}

Node::Node(point mi, point ma)
{
    cmax= ma;
    cmin= mi;
    data=0;
}

void Node::update_rec(point mi, point ma)
{
    cmax= ma;
    cmin= mi;
}

void Node::set_rec ()
{

    if (data!=0)
    {
       // cout<<"miniiiiii"<<endl;
        cmin=make_pair(data->first-dis,data->second-dis);
        cmax=make_pair(data->first+dis,data->second+dis);
    }
    else
    {
       // cout<<"envolvente"<<endl;
        typecor xmin,ymin,xmax,ymax;
        vector <point> temp;

        for (int unsigned i=0;i<children.size();i++)
        {
            temp.push_back(children[i]->cmin);
            temp.push_back(children[i]->cmax);
        }

        sort(temp.begin(),temp.end(),byfirst);
        xmin=temp[0].first;
        xmax=temp[temp.size()-1].first;
        sort(temp.begin(),temp.end(),bysecond);
        ymin=temp[0].second;
        ymax=temp[temp.size()-1].second;
        cmin=make_pair(xmin-dis,ymin-dis);
        cmax=make_pair(xmax+dis,ymax+dis);
    }
}

Node::~Node()
{
    //dtor
}
