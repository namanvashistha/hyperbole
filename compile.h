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
        compile(list<list<string>> lol);
        bool show_file();
};

compile::compile(list<list<string>> lol){
    code=lol;
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
