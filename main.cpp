#include<iostream>
#include<list>
#include<string>
#include"source.h"
#include"variable.h"
using namespace std;
int main(){
    cout<<"1.New 2.Open"<<endl;
    list<list<string>> lol;
    map<string,string> variables;
    source src;
    variable var;
    string key="hahahaha",value="hohohoho";
    var.set_value(variables,key,value);
    cout<<var.get_value(variables,"hahahaha")<<endl;
    string filename="code.txt";
    src.open(filename,lol);
    src.show(lol);


}
