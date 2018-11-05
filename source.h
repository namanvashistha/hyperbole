#ifndef SOURCE_H
#define SOURCE_H
#include<iostream>
#include<fstream>
#include<list>
#include<string>
using namespace std;

class source{
    public:
        list<list<string>> lol;
        string filename;
        bool new_file();
        bool open_file();
        bool show_file();
        bool close_file();

};
bool source::new_file(){
    ofstream file;
    file.open("source/"+filename+".txt");
    return true;
}

bool source::open_file(){
    fstream file;
    file.open("source/"+filename+".txt");
    if(file.fail()) return false;
    string s="";
    while(file){
        string p;
        getline(file,p);
        s+=p;
    }
    list<string> l;
    string word="";
    for(unsigned int i=0;i< s.length();i++){
        if(s[i]!=' ' && s[i]!=';'){
            word+=s[i];
        }
        else{
            if(word!=""){
                l.push_back(word);
            }
            word="";
        }
        if(s[i]==';'){
            lol.push_back(l);
            l.erase(l.begin(),l.end());
        }
    }
    return true;
}

bool source::show_file(){
    list<list<string>>::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        cout<<"\t";
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
        }
        cout<<";"<<endl;
    }
}
bool source::close_file(){
    lol.erase(lol.begin(),lol.end());
}
#endif // SOURCE_H
