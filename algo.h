#ifndef ALGO_H
#define ALGO_H
#include<iostream>
#include<list>
#include<string>
#include<map>
#include"variable.h"
class algo{
    public:
        list<string> l;
        variable *varib;
        void display();
        void fetch();
};


void algo::display(){
    list<string>::iterator it=l.begin();
    it=next(it,1);
    if(varib->get_value(*it)!="\0"){
        cout<<varib->get_value(*it);
    } else {
        cout<<*it;
    }
}

void algo::fetch(){
    string temp_cin;
    cin>>temp_cin;
    list<string>::iterator it=l.begin();
    it=next(it,1);
    varib->set_value(*it,temp_cin);
}


#endif // ALGO_H
