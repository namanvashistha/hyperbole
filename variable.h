#ifndef VARIABLE_H
#define VARIABLE_H
#include<string>
#include<map>
#include<iostream>
using namespace std;

class variable{
    public:
        map<string,string> variables;
        bool set_value(string &key,string &value);
        string get_value(string key);
};

bool variable::set_value(string &key,string &value){
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


#endif // VARIABLE_H
