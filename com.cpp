#include <iostream>

#include <string>
#include <list>
using namespace std;

int main()
{
    list<string> l;
   list<list<string>> lol;
   string word="";
    string s="first line ; second line ; third line ;";
    for(unsigned int i=0;i< s.length()  ;i++){
        if(s[i]==';'){
            lol.push_back(l);
            l.erase(l.begin(),l.end());
            i++;
            }
        if(s[i]!=' '){
            word+=s[i];
        }
        else{
            l.push_back(word);
            word="";
        }
    }
    list<list<string>>::iterator itr;
    for (itr=lol.begin(); itr != lol.end(); itr++){
        list<string>tl=*itr;
        list<string>::iterator it;
        for(it=tl.begin();it!=tl.end();it++){
            if(*it=="display"){
                cout<<"yes";
                next(it,1);
                cout<<*it<<endl;
            }
        }
    }


   return 0;
}
