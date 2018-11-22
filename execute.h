#ifndef COMPILE_H
#define COMPILE_H
#include<iostream>
#include<list>
#include<string>
#include<map>
#include"variable.h"
#include"keyword.h"
#include"algo.h"

class execute
{
    public:
        list<list<string> > code;
        variable var;
        keyword key;
        algo alg;
        execute(list<list<string>> lol);
        void executing();
        void show_memory();
        bool show_file();
};

execute::execute(list<list<string> > lol){
    code=lol;
    alg.varib=&var;
}

void execute::executing(){
    list<list<string> >::iterator itr;
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
            else if(*it=="check"){
                cout<<alg.condition();
            }
            else{
                alg.expression();
            }
    }
    alg.show_errors();
    var.show_errors();
}

void execute::show_memory(){
    map<string,string>::iterator it;
    cout<<"\nMemory:\n";
    for(it=var.variables.begin();it!=var.variables.end();it++){
        cout<<it->first<<" = "<<it->second<<"\n";
    }
}

bool execute::show_file(){
    list<list<string> >::iterator itr;
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
