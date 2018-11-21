#ifndef SOURCE_H
#define SOURCE_H
#include<iostream>
#include<dirent.h>
#include<fstream>
#include<list>
#include<string>
using namespace std;

class source{
    public:
        list<list<string>> lol;
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
    file.open("source/"+filename+".txt");
    return true;
}

bool source::list_dir(){
   DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("source")) != NULL) {
        int nxt_lin=0;
        while ((ent = readdir (dir)) != NULL) {
            string f_name=ent->d_name;
            if(f_name[f_name.size()-1]=='t'){
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
    string topicName="source/"+filename+".txt";
    topicName = "subl \"" + topicName + "\"";
    system(topicName.c_str());
}

bool source::open_file(){

    fstream file;
    file.open("source/"+filename+".txt");
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
        }
    }
    return true;
}

bool source::show_file(){
    list<list<string>>::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        cout<<"\t";
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
        }
        cout<<";"<<endl;
    }
}

bool source::write_file(){
    ofstream file;
    file.open("source/"+filename+".txt");
    list<list<string>>::iterator itr;
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
