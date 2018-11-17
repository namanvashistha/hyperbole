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
        int precedence(char op);
        int applyOp(int a, int b, char op);
        int evaluate(string tokens);
        void expression();
        bool condition();
        bool condition_evaluate(string);
        int string_to_int(string);
        string int_to_string(int);
        void replace_substring(string&,int,int,string);
        string trim(string);
        void show_errors();
};

algo::algo(){
    //cout<<evaluate("100 * ( 2 + 12 ) / 14");
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

int algo::precedence(char op){
    if(op == '+'||op == '-')
    return 1;
    if(op == '*'||op == '/')
    return 2;
    return 0;
}

int algo::applyOp(int a, int b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
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

bool algo::condition(){
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
    string lhs="",rhs="";
    int flag=0;
    for(int i=0;i<con.length();i++){
        lhs+=con[i];
        if(con[i]=='&' && con[i+1] =='&'){
            flag=1;
            i++;
            continue;
        }
        else if(con[i]=='|' && con[i+1] =='|'){
            lhs="";
        }
    }
    
    cout<<condition_evaluate(con)<<endl;
    replace_substring(con,0,0,"_");
}

bool algo::condition_evaluate(string tokens){
    int flag=100;
    string lhs="",rhs="";
    for(int i=0;i<tokens.length();i++){
        if(tokens[i]=='=' && tokens[i+1]=='='){
            flag=1;
            i++;
            continue;
        }
        else if(tokens[i]=='!' && tokens[i+1]=='='){
            flag=2;
            i++;
            continue;
        }
        if(tokens[i]=='='){
            flag=3;
            continue;
        }
        else if(tokens[i]=='<' && tokens[i+1]=='='){
            flag=4;
            i++;
            continue;
        }
        else if(tokens[i]=='<'){
            flag=5;
            continue;
        }
        else if(tokens[i]=='>' && tokens[i+1]=='='){
            flag=6;
            i++;
            continue;
        }
        else if(tokens[i]=='>'){
            flag=7;
            continue;
        }
        if(flag==100)
            lhs+=tokens[i];
        else
            rhs+=tokens[i];
    }
    bool res=false;
    if(flag==1){
        if(evaluate(lhs)==evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    else if(flag==2){
        if(evaluate(lhs)!=evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    else if(flag==3){
        if(varib->get_value(lhs)=="\0" ){
            errors+="undefined variable;";
        }
        else{
            res=true;
        }
        if(rhs[0]=='"' && rhs[rhs.length()-1]=='"'){
            rhs=rhs.substr(1,rhs.size()-2);
        }
        else{
            rhs=int_to_string(evaluate(rhs));
        }
        varib->set_value(lhs,rhs);

    }
    else if(flag==4){
        if(evaluate(lhs)<=evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    else if(flag==5){
        if(evaluate(lhs)<evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    else if(flag==6){
        if(evaluate(lhs)>=evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    else if(flag==7){
        if(evaluate(lhs)>evaluate(rhs)){
            res=true;
        }
        else{
            res=false;
        }
    }
    return res;
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
    stack <char> ops;
    for(i = 0; i < tokens.length(); i++){
        if(tokens[i] == ' ')
            continue;
        else if(tokens[i] == '('){
            ops.push(tokens[i]);
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
            while(!ops.empty() && ops.top() != '(')
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        }
        else{
            while(!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])){
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.push(tokens[i]);
        }
    }
    while(!ops.empty()){
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        char op = ops.top();
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
