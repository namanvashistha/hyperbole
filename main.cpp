#include<iostream>
#include<list>
#include<string>
#include<stdlib.h>
#include"source.h"
#include"help.h"
#include"execute.h"
using namespace std;
int main(){
     system("color 0A");
    BEGIN: system("clear");

    cout<<"Enter 'O' to open a file , 'H' to get help or 'E' to exit ";

    char ch;
    cin>>ch;
    string flow="start";
    help hlp;
    source src;
    switch(ch){
        case 'h':
        case 'H':
            system("color 1F");
            while(1){
                system("clear");
                cout<<"HELP\n\n";
                hlp.show();
                if(flow!="start"){
                    cout<<"\n"<<flow<<": "<<hlp.show(flow)<<endl;
                }
                cout<<"\n\nEnter a keyword from above list to get help OR 'Q' to exit help: ";
                cin>>flow;
                if(flow=="q" || flow=="Q"){
                    goto BEGIN;
                }
            }
        break;
        case 'o':
        case 'O':
            system("color 2F");
            cout<<"\nsource codes\n````````````\n";
            src.list_dir();
            cout<<"\n\nEnter a name from the above list to open OR Enter a new name to create one"<<endl;
            if(flow=="start")
                cin>>src.filename;
            FILE:
            src.open_editor();
            while(1){
                if(!src.open_file()){
                    src.new_file();
                }
                system("clear");
                cout<<src.filename<<"\n\n";
                cout<<"please go to source/"<<src.filename<<".hyp and write the code in hyperbole\n\n\n";
                for(int i=0;i<50;i++) cout<<"`"; cout<<endl;
                src.show_file();
                cout<<endl;
                for(int i=0;i<50;i++) cout<<"`";
                cout<<"\n\nPress 'R' to Refresh,'C' to Compile n Run OR 'Q' to close this file "<<endl;
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
            COMPILE: system("clear");
            cout<<"Terminal\n````````\n";
            execute exe(src.lol);
            exe.executing();
            exe.show_memory();

            cout<<"\n\nPress Enter to continue... ";
            cin.get();cin.get();
            src.close_file();
            goto FILE;
        break;
    }
}
