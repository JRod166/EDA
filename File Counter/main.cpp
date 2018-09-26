#include <iostream>
#include <windows.h>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctype.h>

using namespace std;
vector<string> stop_words;
vector <string> files;
long long sw_counter=0;

void print_directory()
{
    DIR * directorio;
      struct dirent * elemento;
      string elem;
      string dir="DataSet";   // Directorio a ser explorado
      if (directorio = opendir(dir.c_str()))
      {
       while (elemento = readdir(directorio))
       {
        elem = elemento->d_name;
        if (elem != "." && elem != "..")
        {
            files.push_back(elem);
        }
       }
      }
      closedir(directorio);
}
bool is_not_stopword(string word)
{
    vector<string>::iterator SWit;
    SWit= find(stop_words.begin(),stop_words.end(),word);
    if (distance(stop_words.begin(),SWit)==stop_words.size()){return true;}
    //cout<<"Stopword: "<<word<<endl;
    sw_counter++;
    return false;
}

int main()
{
    vector<string> word_list;
    vector<string>::iterator it;
    vector<vector<int> > word_ocurrency;
    ifstream ifs;
    print_directory();
    word_ocurrency.resize(files.size());
    char c;
    long long word_counter=0;
    string word_auxiliar;
    ifs.open("StopWords.txt",std::ifstream::in);
    while(ifs.get(c))
    {
        if(isalpha(c)||c=='\''|| c== '’' || c == '‘'){
            word_auxiliar+=tolower(c);
        }
        else{
            stop_words.push_back(word_auxiliar);
            word_auxiliar="";
        }
    }
    ifs.close();
    for (int i=0;i<files.size();i++){
        ifs.open("DataSet/"+files[i], std::ifstream::in);
        cout<<files[i]<<endl;
        word_ocurrency[i].resize(word_list.size());
        while(ifs.get(c)){
            if (isalpha(c)||c=='\''|| c== '’' || c == '‘'){
                word_auxiliar+=tolower(c);
            }
            else{
                if(word_auxiliar!="" && is_not_stopword(word_auxiliar)){
                    word_counter++;
                    //cout<<word_auxiliar<<", ";
                    it=find(word_list.begin(),word_list.end(),word_auxiliar);
                    if (distance(word_list.begin(),it)==word_list.size()){
                        word_list.push_back(word_auxiliar);
                        word_ocurrency[i].push_back(1);
                    }
                    else{
                        word_ocurrency[i][distance(word_list.begin(),it)]++;
                    }
                }
                word_auxiliar="";
            }
        }
        ifs.close();
    }
    ofstream ofs;
    ofs.open("Results.csv",ios::trunc);
    ofs<<"Words"<<";";
    for (int k=0;k<word_list.size()-1;k++)
    {
        ofs<<word_list[k]<<";";
    }
    ofs<<word_list[word_list.size()-1]<<endl;
    for (int j=0; j<files.size();j++)
    {
        ofs<<endl<<files[j]<<";";
        for (int i=0;i<word_list.size();i++)
        //cout<<word_ocurrency[j]<<"  "<<word_list[j]<<endl;
        if (i>=word_ocurrency[j].size()) ofs<<"0"<<";";
        else ofs<<word_ocurrency[j][i]<<";";
    }
    ofs.close();
    cout<<"Total: "<<word_counter+sw_counter<<endl;
    cout<<"Not StopWords: "<<word_counter<<endl;
    cout<<"StopWords: "<<sw_counter<<endl;
    return 0;
}
