#include<iostream>
#include<list>
#include<string>
#include"source.h"
#include"variable.h"
#include"help.h"
using namespace std;
int main(){
    BEGIN: system("clear");
    cout<<"Enter 'O' to open a file , 'H' to get help and 'E' to exit ";
    char ch;
    cin>>ch;
    string flow="start";
    help hlp;
    source src;
    variable var;
    switch(ch){
        case 'h':
        case 'H':
            while(1){
                system("clear");
                cout<<"HELP\n\n";
                hlp.show();
                if(flow!="start"){
                    cout<<"\n"<<flow<<": "<<hlp.show(flow)<<endl;
                }
                cout<<"\n\nEnter a keyword from above list to get help and 'Q' to exit help: ";
                cin>>flow;
                if(flow=="q" || flow=="Q"){
                    goto BEGIN;
                }
            }
        break;
        case 'o':
        case 'O':
            cout<<"Enter the name of file"<<endl;
            cin>>src.filename;
            FILE:
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
                cout<<"\n\nPress 'R' to Refresh,'C' to Compile n Run and 'Q' to close this file "<<endl;
                cin>>flow;
                if(flow=="q" || flow=="Q"){
                    goto BEGIN;
                }
                if(flow=="c" || flow=="C"){
                    goto COMPILE;
                }
                src.close_file();
            }
        break;
            COMPILE:













            cout<<"\n\nPress any key to continue. "<<endl;
            cin>>ch;
            src.close_file();
            goto FILE;
        break;

    }
}
