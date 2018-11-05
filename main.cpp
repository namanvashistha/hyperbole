#include<iostream>
#include<list>
#include<string>
#include"source.h"
#include"variable.h"
using namespace std;
int main(){
    BEGIN: system("clear");
    cout<<"Enter the name of file"<<endl;

    source src;
    variable var;
    char flow='r';
    cin>>src.filename;
    while(1){
        if(!src.open_file()){
            src.new_file();
        }
        system("clear");
        cout<<src.filename<<"\n\n";
        cout<<"please go to source/"<<src.filename<<".txt and write the code in hyperbole\n\n\n";
        for(int i=0;i<50;i++) cout<<"`"; cout<<endl;
        src.show_file();
        cout<<endl;
        for(int i=0;i<50;i++) cout<<"`";
        cout<<"\n\nPress 'R' to Refresh,'C' to Compile and 'Q' to close this file ";
        cin>>flow;
        if(flow=='q' || flow=='Q'){
            goto BEGIN;
        }
        src.close_file();
    }
}
