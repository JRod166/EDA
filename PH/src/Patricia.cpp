#include "Patricia.h"

Patricia::Patricia(int _dim)
{
    dim=_dim;
    size=0;
    //ctor
}

bool Patricia::insert(point A)
{
    HyperDate HD(dim,A);
    if(root.size()<=0)
    {
        cout<<"empty root"<<endl;
        root.push_back(new Node(HD.ProcessedBits,dim));
        return true;
    }
    Node ** P=NULL;
    bool flag=match_root(P,HD);
    if(!flag)
    {
        //cout<<"root node"<<endl;
        root.push_back(new Node(HD.ProcessedBits,dim));
        return true;
    }
    HD.ProcessedBits=HD.ProcessedBits.substr(dim,HD.ProcessedBits.size());
    int find_bif;
    find_bif=find_bifurcation(P,HD);
    if (find_bif==0)
    {
        //cout<<"nodo repetido"<<endl;
        return 0;
    }
    if (find_bif==1)
    {
        //cout<<"nodo pequeño"<<endl;
        int match_count=0;
        string aux1,aux2;
        //cout<<"size: "<<(*P)->postfix.size()<<"-"<<HD.ProcessedBits.size()<<endl;
        for(int i=0;i<(*P)->postfix.size()/dim;i++)
        {
            aux1=(*P)->postfix.substr(i*dim,dim);
            aux2=HD.subset(i);
            //cout<<aux1<<"->"<<aux2<<endl;
            if(aux1!=aux2)
            {
                break;
            }
            match_count+=dim;
        }
        aux1=(*P)->postfix.substr(match_count,(*P)->postfix.size()-match_count);
        aux2=HD.ProcessedBits.substr(match_count,HD.ProcessedBits.size()-match_count);
        //cout<<aux1<<"<-"<<endl<<"->"<<aux2<<endl;
        Node * AuxNode;
        (*P)->postfix=(*P)->postfix.substr(0,match_count);
        AuxNode=(*P)->Point_to;
        (*P)->Point_to=new Node();
        (*P)->Point_to->NodeVec.push_back(new Node(aux1,dim));
        (*P)->Point_to->NodeVec.push_back(new Node(aux2,dim));
        (*P)->Point_to->NodeVec[0]->Point_to=AuxNode;
        return 1;
    }
    if (find_bif==2)
    {
        //cout<<"nodo grande"<<endl;
        //cout<<HD.ProcessedBits<<endl;
        (*P)->NodeVec.push_back(new Node(HD.ProcessedBits,dim));
    }

}

bool Patricia::match_root(Node **& Ptr,HyperDate HD)
{
    //cout<<"matching"<<endl;
    for(int i=0;i<root.size();i++)
    {
        //cout<<root[i]->data<<endl;
        if(root[i]->data==HD.subset(0))
        {
            Ptr=&(root[i]);
            return true;
        }
    }
    return false;
}

int Patricia::find_bifurcation(Node**& Ptr, HyperDate &HD)
{
    int ret=1;
    if((*Ptr)->postfix==HD.ProcessedBits.substr(0,(*Ptr)->postfix.size()))
    {
        if((*Ptr)->postfix.size()==HD.ProcessedBits.size())
        {
            return 0;
        }
        HD.ProcessedBits=HD.ProcessedBits.substr((*Ptr)->postfix.size(),HD.ProcessedBits.size()-(*Ptr)->postfix.size());
        Ptr=&((*Ptr)->Point_to);
        ret=2;
        for(int i=0;i<=(*Ptr)->NodeVec.size();i++)
        {
            if(i==(*Ptr)->NodeVec.size())
            {
                return 2;
            }
            if((*Ptr)->NodeVec[i]->data==HD.ProcessedBits.substr(0,dim))
            {
                HD.ProcessedBits=HD.ProcessedBits.substr(dim,HD.ProcessedBits.size()-dim);
                Ptr=&((*Ptr)->NodeVec[i]);
                ret = 1;
            }
        }
        return find_bifurcation(Ptr,HD);
    }
    return ret;
}

bool Patricia::find_point(Node**& Ptr,HyperDate HD, vector <Node*>& Path)
{
    //cout<<(*Ptr)->postfix<<"<-"<<endl<<HD.ProcessedBits.substr(0,(*Ptr)->postfix.size())<<"<--"<<endl;
    if((*Ptr)->postfix==HD.ProcessedBits.substr(0,(*Ptr)->postfix.size()))
    {
        if((*Ptr)->postfix.size()==HD.ProcessedBits.size())
        {
            //cout<<"nice"<<endl;
            return 1;
        }
        HD.ProcessedBits=HD.ProcessedBits.substr((*Ptr)->postfix.size(),HD.ProcessedBits.size()-(*Ptr)->postfix.size());
        //cout<<HD.ProcessedBits<<endl;
        Ptr=&((*Ptr)->Point_to);
        //cout<<"size: ";
        //cout<<(*Ptr)->NodeVec.size()<<endl;
        Path.push_back(*Ptr);
        for(int i=0;i<=(*Ptr)->NodeVec.size();i++)
        {
            if(i==(*Ptr)->NodeVec.size())
            {
                return false;
            }

            if((*Ptr)->NodeVec[i]->data==HD.ProcessedBits.substr(0,dim))
            {
                //cout<<"matched"<<endl;
                HD.ProcessedBits=HD.ProcessedBits.substr(dim,HD.ProcessedBits.size()-dim);
                Ptr=&((*Ptr)->NodeVec[i]);
            }
        }
        return find_point(Ptr,HD,Path);
    }
    return 0;

}

void Patricia::proximity(point Pt)
{
    int cont=0;
    HyperDate HD(dim,Pt);
    vector<Node*> Path;
    Node**P;
    if (!match_root(P,HD))
    {
        return;
    }
    string root_val=HD.ProcessedBits.substr(0,dim);
    HD.ProcessedBits=HD.ProcessedBits.substr(dim,HD.ProcessedBits.size());
    if(!find_point(P,HD,Path))
    {
        return ;
    }
    if(Path.size()==0)
    {
        getData(HD,NULL,"",cont);
    }
    for(int i=Path.size()-1;i>=0;i--)
    {
        for(int j=0;j<Path[i]->NodeVec.size();j++)
        {
            getData(HD,Path[i]->NodeVec[j],root_val,cont);
        }
        break;
    }
    cout<<"# Proximity points "<<cont;
}

void Patricia::getData(HyperDate HD, Node* Pt, string root_val,int &cont)
{
    if(!Pt)
    {
        for(int i=0;i<root.size();i++)
        {
            if(!root[i]->Point_to)
            {
                ToFloat(root[i]->data+HD.ProcessedBits);
                cont++;
            }
        }
    }
    else
    {
        if(!Pt->Point_to)
        {
            ToFloat(root_val+HD.ProcessedBits.substr(0,HD.ProcessedBits.size()-Pt->postfix.size()-dim)+Pt->data+Pt->postfix);
            cont++;
        }
    }
}
int Patricia::Binary2Hex( string Binary )
{
    bitset<32> set(Binary);
    int hex = set.to_ulong();

    return hex;
}

float Patricia:: GetFloat32( string Binary )
{
    int HexNumber = Binary2Hex( Binary );

    bool negative  = !!(HexNumber & 0x80000000);
    int  exponent  =   (HexNumber & 0x7f800000) >> 23;
    int sign = negative ? -1 : 1;

    // Subtract 127 from the exponent
    exponent -= 127;

    // Convert the mantissa into decimal using the
    // last 23 bits
    int power = -1;
    float total = 0.0;
    for ( int i = 0; i < 23; i++ )
    {
        int c = Binary[ i + 9 ] - '0';
        total += (float) c * (float) pow( 2.0, power );
        power--;
    }
    total += 1.0;

    float value = sign * (float) pow( 2.0, exponent ) * total;

    return value;
}

void Patricia::ToFloat(string Dato)
{
    vector <string> data;
    float aux;
    data.resize(dim);
    for (int j=0;j<32;j++)
    {
        for(int i=0;i<dim;i++)
        {
            data[i]+=Dato[dim*j+i];
        }
    }
    for (int i=0;i<dim;i++)
    {
        aux=GetFloat32(data[i]);
        //cout<<aux<<", ";
    }
    //cout<<endl;
}

Patricia::~Patricia()
{
    //dtor
}
