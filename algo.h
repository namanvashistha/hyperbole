#ifndef ALGO_H
#define ALGO_H
#include<iostream>
#include<list>
#include<string>
#include<sstream>
#include<map>
#include<stack>
#include"variable.h"
class algo{
    public:
        list<string> l;
        string errors="";
        variable *varib;
        algo();
        void display();
        void declare();
        void fetch();
        int precedence(string op);
        int applyOp(int a, int b, string op);
        int evaluate(string tokens);
        void expression();
        int condition();
        bool condition_evaluate(string);
        int string_to_int(string);
        string int_to_string(int);
        void replace_substring(string&,int,int,string);
        string trim(string);
        void show_errors();
};

algo::algo(){

}

void algo::display(){
    list<string>::iterator it=l.begin();
    for(it++;it!=l.end();it++){
        string dis_string=*it;
        if(varib->get_value(dis_string)!="\0"){
            cout<<varib->get_value(dis_string);
        }
        else if(dis_string[0]=='"' && dis_string[dis_string.length()-1]=='"'){
            dis_string=dis_string.substr(1,dis_string.size()-2);
            cout<<dis_string;
        }
        else if(dis_string=="nextln"){
            cout<<endl;
        }
        else{
            errors+="undefined variable;";
        }
    }
}

void algo::fetch(){
    list<string>::iterator it=l.begin();
    for(it++;it!=l.end();it++){
        string temp_cin;
        cin>>temp_cin;
        if(varib->get_value(*it)=="\0" ){
            errors+="undefined variable;";
        }
        varib->set_value(*it,temp_cin);
    }
}

void algo::declare(){
    list<string>::iterator it=l.begin();
    for(it++;it!=l.end();it++){
        string dec="declared";
        varib->set_value(*it,dec);
    }
}

int algo::precedence(string op){
    if(op == "||")
        return 1;
    if(op == "&&")
        return 2;
    if(op == "==" || op == "!=")
        return 3;
    if(op == "<=" || op == ">=" || op == ">" || op == "<")
        return 4;
    if(op == "+" || op == "-")
        return 5;
    if(op == "*" || op == "/")
        return 6;
    
    return 0;
}

int algo::applyOp(int a, int b, string op){
        if(op == "+") return a + b;
        else if(op == "-") return a - b;
        else if(op == "*") return a * b;
        else if(op == "/") return a / b;
        else if(op == "&&") return a && b;
        else if(op == "||") return a || b;
        else if(op == "==") return a == b;
        else if(op == "!=") return a != b;
        else if(op == ">=") return a >= b;
        else if(op == "<=") return a <= b;
        else if(op == ">") return a > b;
        else if(op == "<") return a < b;
}

void algo::expression(){
    string lhs="",rhs="";
    int flag=0;

    for(auto it=l.begin();it!=l.end();it++){
        string exp=*it;
        for(int i=0;i<exp.length();i++){
            if(exp[i]=='='){
                flag=1;
                continue;
            }
            if(flag==0)
                lhs+=exp[i];
            else
                rhs+=exp[i];
        }
    }
    if(varib->get_value(lhs)=="\0" ){
            errors+="undefined variable;";
        }
    if(rhs[0]=='"' && rhs[rhs.length()-1]=='"'){
        rhs=rhs.substr(1,rhs.size()-2);
    }
    else{
        rhs=int_to_string(evaluate(rhs));
    }
    varib->set_value(lhs,rhs);
}

int algo::condition(){
    string con="";
    list<string>::iterator it=l.begin();
    for(it++;it!=l.end();it++){
        string exp=*it;
        for(int i=0;i<exp.length();i++){
            if(exp[i]==' '){
                continue;
            }
            con+=exp[i];
        }
    }
    return evaluate(con);
}

int algo::evaluate(string tokens){
    string rhs="";
    for(int i=0;i<tokens.length();i++){
        rhs+=tokens[i];
        if(isalpha(tokens[i]) || isdigit(tokens[i]))
            if(isalpha(tokens[i+1]) || isdigit(tokens[i+1]))
                continue;
        if(i!=tokens.length()-1)
            rhs+=" ";
    }
    tokens=rhs;
    int i;
    stack <int> values;
    stack <string> ops;
    for(i = 0; i < tokens.length(); i++){
        if(tokens[i] == ' ')
            continue;
        else if(tokens[i] == '('){
            string temp="";
            temp=tokens[i];
            ops.push(temp);
        }
        else if(isdigit(tokens[i])){
            int val = 0;
            while(i < tokens.length() && isdigit(tokens[i])){
                val = (val*10) + (tokens[i]-'0');
                i++;
            }
            values.push(val);
        }
        else if(isalpha(tokens[i])){
            string var = "";
            while(i < tokens.length() && (isdigit(tokens[i]) || isalpha(tokens[i]) )) {
                var += tokens[i];
                i++;
            }
            if(varib->get_value(var)!="\0"){
                values.push(string_to_int(varib->get_value(var)));
            }
            else{
                errors+="undefined variable;";
            }
        }
        else if(tokens[i] == ')'){
            while(!ops.empty() && ops.top() != "(")
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                string op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        }
        else{
            string temp="";
            temp=tokens[i];
            if(tokens[i]=='=' || tokens[i]=='!' || tokens[i]=='<' || tokens[i]=='>' || tokens[i]=='&' || tokens[i]=='|'){
                if(tokens[i+1]==' '){
                    if(tokens[i+2]=='=' || tokens[i+2]=='&' || tokens[i+2]=='|'){
                        temp+=tokens[i+2];
                        i=i+2;
                    }
                }
            }

            while(!ops.empty() && precedence(ops.top()) >= precedence(temp)){
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                string op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.push(temp);
        }
    }
    while(!ops.empty()){
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        string op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }
    return values.top();
}

int algo::string_to_int(string str){
    int num;
    stringstream number(str);
	number>>num;
    return num;
}

string algo::int_to_string(int num){
    string str;
    stringstream ss;
    ss << num;
    str = ss.str();
    return str;
}

void algo::replace_substring(string &str,int first,int last,string sub_str){
    string temp="";
    for(int i=0;i<str.length();i++){
        if(i<first || i>last){
            temp+=str[i];
        }
        else{
            for(int j=0;j<sub_str.length();j++){
                temp+=sub_str[j];
            }
            i=last;
        }
    }
    str=temp;
}

void algo::show_errors(){
    cout<<"\n\n";
    for(int i=0;i<errors.length();i++){
        if(errors[i]==';'){
            cout<<endl;
            continue;
        }
        cout<<errors[i];
    }
}

#endif // ALGO_H