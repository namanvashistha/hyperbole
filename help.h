#ifndef HELP_H
#define HELP_H
#include<string>
#include<map>
#include<iostream>

class help
{
    public:
        map<string,string> keywords;
        help();
        void show();
        string show(string key);
};

help::help(){
    fstream file;
    file.open("keyword.txt");
    if(file.fail()){
        cout<<"cannot read keywords configuration file";
    }
    string s="";
    while(file){
        string p;
        getline(file,p);
        s+=p;
    }
    string k="",v="";
    int flag=0;
    for(unsigned int i=0;i< s.length();i++){
        if(flag==0){
            if(s[i]==':'){
                flag=1;
                continue;
            }
            k+=s[i];
        }
        else{
            if(s[i]==';'){
                keywords.insert({k,v});
                k="";v="";
                flag=0;
                continue;
            }
            v+=s[i];
        }
    }
}

void help::show(){
    map<string,string>::iterator it;
    for(it=keywords.begin();it!=keywords.end();it++){
        cout<<"- "<<it->first<<"\n";
    }
}

string help::show(string key){
    map<string,string>::iterator it;
    int p=0;
    for(it=keywords.begin();it!=keywords.end();it++){
        if(it->first==key){
            p=1;
            break;
        }
    }
    if(p==1){
        if(keywords.at(key)!="\0")
            return keywords.at(key);
        else
            return "doesn't exist";
    }
    else
        return "doesn't exist";
}

#endif // HELP_H
