#ifndef COMPILE_H
#define COMPILE_H
#include<iostream>
#include<list>
#include<string>
#include<map>
#include"variable.h"
#include"keyword.h"
#include"algo.h"

class compile
{
    public:
        list<list<string>> code;
        variable var;
        keyword key;
        algo alg;
        compile(list<list<string>> lol);
        void executing();
        void show_memory();
        bool show_file();
};

compile::compile(list<list<string>> lol){
    code=lol;
    alg.varib=&var;
}

void compile::executing(){
    list<list<string>>::iterator itr;
    for (itr=code.begin(); itr != code.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it=tl.begin();
            alg.l=tl;
            if(*it=="display"){
                alg.display();
            }
            else if(*it=="fetch"){
                alg.fetch();
            }
            else if(*it=="declare"){
                alg.declare();
            }
            else{
                alg.expression();
            }
    }
    alg.show_errors();
}

void compile::show_memory(){
    map<string,string>::iterator it;
    cout<<"\n\nMemory:\n";
    for(it=var.variables.begin();it!=var.variables.end();it++){
        cout<<"\n"<<it->first<<"="<<it->second<<"\n";
    }
}

bool compile::show_file(){
    list<list<string>>::iterator itr;
    for (itr=code.begin(); itr != code.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        cout<<"\t";
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
        }
        cout<<";"<<endl;
    }
}

#endif // COMPILE_H
