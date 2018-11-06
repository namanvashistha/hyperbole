#ifndef KEYWORD_H
#define KEYWORD_H
#include<string>
#include<map>
#include<iostream>

class keyword
{
    public:
        map<string,string> keywords;
        keyword();
};

keyword::keyword(){
    fstream file;
    file.open("keyword.txt");
    if(file.fail()){
        cout<<"cannot read keywords configuration file";
    }
    string s="";
    while(file){
        string p;
        getline(file,p);
        s+=p;
    }
    string k="",v="";
    int flag=0;
    for(unsigned int i=0;i< s.length();i++){
        if(flag==0){
            if(s[i]==':'){
                flag=1;
                continue;
            }
            k+=s[i];
        }
        else{
            if(s[i]==';'){
                keywords.insert({k,v});
                k="";v="";
                flag=0;
                continue;
            }
            v+=s[i];
        }
    }
}

#endif // KEYWORD_H
