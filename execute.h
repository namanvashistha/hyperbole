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

class execute{
    public:
        list<list<string> > code;
        variable var;
        keyword key;
        algo alg;
        stack<string> flow;
        int check_balance;
        int loop_balance;
        execute(list<list<string> > lol);
        void executing();
        void show_memory();
        bool show_file();
};

execute::execute(list<list<string> > lol){
    code=lol;
    alg.varib=&var;
    loop_balance=0;
    check_balance=0;
}

void execute::executing(){
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
                check_balance++;
                if(!alg.condition()){
                    int temp=check_balance;
                    while(check_balance!=(temp-1)){
                        advance(itr,1);
                        list<string>tl=*itr;
                        auto it=tl.begin();
                        advance(it,1);
                        if(*it=="check"){
                            check_balance++;
                        }
                        else if(*it=="checkit"){
                            check_balance--;
                        }
                    }
                }
            }
            else if(*it=="checkit"){
                check_balance--;
            }
            else if(*it=="otherwise"){
                check_balance--;
                if(!alg.condition()){
                    int temp=check_balance;
                    while(check_balance!=(temp-1)){
                        advance(itr,1);
                        list<string>tl=*itr;
                        auto it=tl.begin();
                        advance(it,1);
                        if(*it=="check" || *it=="otherwise"){
                            check_balance++;
                        }
                        else if(*it=="checkit" || *it=="leaveit"){
                            check_balance--;
                        }
                    }
                }
            }
            else if(*it=="loop"){
                loop_balance++;
                if(alg.condition()){
                    advance(it,-1);
                    if(flow.empty()){
                        flow.push(*it);
                    }
                    else{
                        if(*it!=flow.top()){
                            flow.push(*it);
                        }
                    }   
                    advance(it,1);
                } 
                else{
                    int temp=loop_balance;
                    while(loop_balance!=(temp-1)){
                        advance(itr,1);
                        list<string>tl=*itr;
                        auto it=tl.begin();
                        advance(it,1);
                        if(*it=="loop"){
                            loop_balance++;
                        }
                        else if(*it=="loopit"){
                            loop_balance--;
                        }
                    }
                    if(!flow.empty()){
                        flow.pop();
                    }
                }
            }
            else if(*it=="loopit"){
                int line=(alg.string_to_int(flow.top()))-2;
                itr=code.begin();
                advance(itr,line);
                loop_balance--;
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