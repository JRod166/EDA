#include "HyperDate.h"


HyperDate::HyperDate(int _dim, point A)
{
    //ctor
    dim=_dim;
    ProcessedBits=processPoint(A);
}

string HyperDate::processPoint(point A)
{
    vector <string> ConvertedBits;
    string conv="";
    for(int i=0;i<dim;i++)
    {
        //cout<<A[i]<<" -> ";
        ConvertedBits.push_back(GetBinary32(A[i]));
        //cout<<ConvertedBits[i]<<endl;
    }
    for(int j=0;j<BS;j++)
    {
        for(int k=0;k<dim;k++)
        {
           conv+=ConvertedBits[k][j];
        }
    }
    //cout<<conv<<endl;
    return conv;
}

string HyperDate::GetBinary32(float value)
{
    union
    {
         float input;   // assumes sizeof(float) == sizeof(int)
         int   output;
    }    data;

    data.input = value;

    bitset<sizeof(float) * CHAR_BIT>   bits(data.output);

    string mystring = bits.to_string<char,
                                          char_traits<char>,
                                          allocator<char> >();

    return mystring;
}

string HyperDate::subset(int pos)
{
    string aux="";
    char sum;
    aux=ProcessedBits.substr(pos*dim,dim);
    return aux;
}

HyperDate::~HyperDate()
{
    //dtor
}
