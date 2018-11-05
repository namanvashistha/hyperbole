#ifndef SOURCE_H
#define SOURCE_H
#include<iostream>
#include<fstream>
#include<list>
#include<string>
using namespace std;

class source{
    public:
        bool open(string filename ,list<list<string>> &lol);
        bool show(list<list<string>> &lol);

};

bool source::open(string filename ,list<list<string>> &lol){
    fstream file;
    file.open("code.txt");
    string s="";
    while(file){
        string p;
        getline(file,p);
        s+=p;
    }
    list<string> l;
    string word="";
    for(unsigned int i=0;i< s.length();i++){
        if(s[i]!=' ' && s[i]!=';' ){
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

bool source::show(list<list<string>> &lol){
    list<list<string>>::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
        }
        cout<<";"<<endl;
    }
}

#endif // SOURCE_H
