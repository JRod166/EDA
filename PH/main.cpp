#include <iostream>
#include "Patricia.h"
#include <fstream>

using namespace std;

vector <vector <string> > characteristics;

void extractcharacteristics()
{
    string aux;
    fstream input;
    vector<string > vec_aux;
    input.open("YearPredictionMSD.txt",std::ifstream::in);
	if(!input.is_open())
        cout<<"no se pudo abrir archivo";
	char c;
	while (input.get(c))
    {
        if(c=='\n')
        {
            characteristics.push_back(vec_aux);
            vec_aux.clear();
            //cout<<cont<<endl;
        }
        else if(c!=',')
        {
            aux+=c;
        }
        else
        {
            vec_aux.push_back(aux);
            //cout<<aux<<endl;
            aux="";
        }
    }
}

void insertPoints(Patricia &PH)
{
    point A;
    for (int i=0; i<characteristics.size();i++)
    {
        for(int j=0;j<characteristics[i].size();j++)
        {
            A.push_back(stod(characteristics[i][j]));
        }
        //cout<<"inserting "<<i+1<<endl;
        PH.insert(A);
        //cout<<PH.root.size()<<endl;
        A.clear();
    }
    //characteristics.clear();
}



int main()
{
/*
    Patricia PH(2);
    point A;
    A.push_back(2.1);
    A.push_back(2.1);
    PH.insert(A);
    A[0]=-20.1;
    A[1]=-20.1;
    PH.insert(A);
    A[0]=-2.1;
    A[1]=-2.1;
    PH.insert(A);
    A[0]=-20.1;
    A[1]=-20.1;
    PH.insert(A);
    */
    extractcharacteristics();
    Patricia PH(characteristics[0].size());
    insertPoints(PH);
    point A;
    int search_idx=0;
    while(search_idx!=-1){
    cout<<"Inserte indice a buscar ( 0 - "<<characteristics.size()<<")"<<endl;
    cin>>search_idx;
    for (int i=0;i<characteristics[search_idx].size();i++)
    {
        //cout<<characteristics[5][i]<<endl;
        A.push_back(stod(characteristics[search_idx][i]));
    }
    //PH.root.size();
    PH.proximity(A);}
    return 0;
}
