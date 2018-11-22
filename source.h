#ifndef SOURCE_H
#define SOURCE_H
#include<iostream>
#include<dirent.h>
#include<fstream>
#include<list>
#include<string>
#include"algo.h"
using namespace std;

class source{
    public:
        list<list<string> > lol;
        string filename;
        bool new_file();
        bool open_file();
        bool show_file();
        bool write_file();
        bool close_file();
        bool list_dir();
        bool open_editor();

};
bool source::new_file(){
    ofstream file;
    file.open("source/"+filename+".hyp");
    return true;
}

bool source::list_dir(){
   DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("source")) != NULL) {
        int nxt_lin=0;
        while ((ent = readdir (dir)) != NULL) {
            string f_name=ent->d_name;
            if(f_name[f_name.size()-1]=='p'){
                f_name= f_name.substr(0, f_name.size()-4);
                if(nxt_lin%5==0) cout<<endl;
                cout<<f_name<<"\t";
                nxt_lin++;
            }
        }
        closedir (dir);
    } else {
         perror ("");
    }
}

bool source::open_editor(){
    string topicName="source/"+filename+".hyp";
    topicName = "subl \"" + topicName + "\"";
    if(system(topicName.c_str())){
        string topicName="source/"+filename+".hyp";
        topicName = "notepad \"" + topicName + "\"";
        system(topicName.c_str());
    }
}

bool source::open_file(){

    fstream file;
    file.open("source/"+filename+".hyp");
    if(file.fail()) return false;
    string s="";
    while(file){
        string p;
        getline(file,p);
        s+=p;
    }
    list<string> l;
    string word="";
    int str_flg=0;
    int lineNo=2;
    algo al;
    l.push_back(al.int_to_string(1));
    for(unsigned int i=0;i< s.length();i++){
        if(s[i]!=' ' && s[i]!=';' && str_flg==0){
            word+=s[i];
            if(s[i]=='"') str_flg=1;
        }
        else if(str_flg==1){
            word+=s[i];
            if(s[i]=='"') str_flg=0;
            continue;
        }
        else{
            if(word!=""){
                l.push_back(word);
            }
            word="";
        }
        if(s[i]==';'){
            lol.push_back(l);
            l.erase(l.begin(),l.end());
            l.push_back(al.int_to_string(lineNo));
            lineNo++;
        }
    }
    return true;
}

bool source::show_file(){
    list<list<string> >::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        int tab=0;
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
            if(tab==0) cout<<"\t";;
            tab++;
        }
        cout<<";"<<endl;
    }
}

bool source::write_file(){
    ofstream file;
    file.open("source/"+filename+".hyp");
    list<list<string> >::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        cout<<"\t";
        for(it=tl.begin();it!=tl.end();it++){
            file<<*it<<" ";
        }
        file<<";"<<endl;
    }
}

bool source::close_file(){
    lol.erase(lol.begin(),lol.end());
}
#endif // SOURCE_H
