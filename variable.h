#ifndef VARIABLE_H
#define VARIABLE_H
#include<string>
#include<map>
#include<iostream>
using namespace std;

class variable{
    public:
        map<string,string> variables;
        string errors="";
        bool set_value(string &key,string &value);
        string get_value(string key);
        void show_errors();
};

bool variable::set_value(string &key,string &value){
    int flag=0;
    for(int i=0;i<key.length();i++){
        if(!(isalpha(key[i]) || isdigit(key[i]) || key[i]=='_')){
            flag=1;
            break;
        }
    }
    if((!isalpha(key[0])) || flag==1){
        errors+=key+" is a  illegal name of variable;";
    }
    map<string,string>::iterator it;
    int p=0;
    for(it=variables.begin();it!=variables.end();it++){
        if(it->first==key){
            p=1;
            break;
        }
    }
    if(p==1){
        variables[key]=value;
    }
    else
        variables.insert({key,value});
}

string variable::get_value(string key){
    map<string,string>::iterator it;
    int p=0;
    for(it=variables.begin();it!=variables.end();it++){
        if(it->first==key){
            p=1;
            break;
        }
    }
    if(p==1){
        if(variables.at(key)!="\0")
            return variables.at(key);
        else
            return "\0";
    }
    else
        return "\0";
}

void variable::show_errors(){
    cout<<"\n\n";
    for(int i=0;i<errors.length();i++){
        if(errors[i]==';'){
            cout<<endl;
            continue;
        }
        cout<<errors[i];
    }
}

#endif // VARIABLE_H
