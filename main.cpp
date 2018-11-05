#include<iostream>
#include<list>
#include<string>
#include"source.h"
#include"variable.h"
using namespace std;
int main(){
    BEGIN: system("clear");
    cout<<"Choose an option\n\t\t\t1.New 2.Open"<<endl;
    cout<<"Enter the name of file"<<endl;

    source src;
    variable var;
    char flow='r';
    string filename;
    cin>>filename;
    system("clear");
    cout<<filename<<endl;
    while(flow=='r' || flow=='R'){
        if(!src.open_file(filename)){
            src.new_file(filename);
        }
        src.show_file();
        cout<<"Press 'R' to Refresh and 'Q' to close this file "<<endl;
        cin>>flow;
        if(flow=='q' || flow=='Q') goto BEGIN;

    }
}
