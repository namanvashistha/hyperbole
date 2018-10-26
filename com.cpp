#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

int main(){
    fstream file;
    string filename="code.txt";
    file.open("code.txt");
    string s="";
    while(file){

        string p;
        getline(file,p);
        s+=p;
    }
    list<string> l;
    list<list<string>> lol;
    string word="";
    for(unsigned int i=0;i< s.length()  ;i++){
        if(s[i]!=' ' && s[i]!=';' ){
            word+=s[i];
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
            i++;
        }
    }

    list<list<string>>::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        for(it=tl.begin();it!=tl.end();it++){
            cout<<*it<<" ";
        }
        cout<<endl;
    }

    return 0;
}
