#ifndef IDE_H
#define IDE_H
#include<iostream>
#include<list>
#include<string>
#include<stdlib.h>
#include"source.h"
#include"help.h"
#include"execute.h"
using namespace std;
int main(){

    BEGIN: system("clear");
    cout << "\033[1;31m";
 	printf("\t  *************************************************\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                               *\n");
	printf("\t  *        -----------------------------          *\n");
	printf("\t  *            WELCOME TO HYPERBOLE               *\n");
	printf("\t  *        -----------------------------          *\n");
	printf("\t  *                                               *\n");
	printf("\t  *    HYPERBOLE is a compiler build using C++    *\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                               *\n");
	printf("\t  *                                     hyperbole *\n");
	printf("\t  *                                               *\n");
	printf("\t  *************************************************\n\n\n");
    cout<<"      Enter 'O' to open a file , 'H' to get help or 'E' to exit ";
    cout<<"\033[0m";
    char ch;
    cin>>ch;
    string flow="start";
    help hlp;
    source src;
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
                cout<<"\n\nEnter a keyword from the above list to get help OR 'Q' to exit help: ";
                cin>>flow;
                if(flow=="q" || flow=="Q"){
                    goto BEGIN;
                }
            }
        break;
        case 'o':
        case 'O':system("clear");
            src.list_dir();
            if(flow=="start")
                cin>>src.filename;
            src.open_editor();
            FILE:system("clear");
            while(1){
                if(!src.open_file()){
                    src.new_file();
                }
                system("clear");
                src.show_file();
                flow=cin.get();
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
#endif // IDE_H