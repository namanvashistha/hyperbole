#ifndef COMPILE_H
#define COMPILE_H
#include<iostream>
#include<list>
#include<string>
#include<map>
#include<stack>
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
        execute(list<list<string> > lol);
        void executing();
        void show_memory();
        bool show_file();
};

execute::execute(list<list<string> > lol){
    code=lol;
    alg.varib=&var;
}

void execute::executing(){
    stack<string> flow;
    int balance=0;
    list<list<string> >::iterator itr;
    for(itr=code.begin();itr != code.end();itr++){
        list<string>tl=*itr;
        list<string>::iterator it=tl.begin();
            advance(it,1);
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
                balance++;
                alg.condition();
            }
            else if(*it=="loop"){
                advance(it,-1);
                flow.push(*it);
                advance(it,1);
                balance++;
                if(alg.condition()==0) break;
            }
            else if(*it=="end"){
                int line=(alg.string_to_int(flow.top()))-2;
                itr=code.begin();
                advance(itr,line);
                //flow.pop();
                balance--;
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
    list<list<string>  >::iterator itr;
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