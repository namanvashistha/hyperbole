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
        int string_to_int(string);
        string int_to_string(int);
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
    string final_rhs="";
    if(rhs[0]=='"' && rhs[rhs.length()-1]=='"'){
        final_rhs=rhs.substr(1,rhs.size()-2);
    }
    else{
        for(int i=0;i<rhs.length();i++){
            final_rhs+=rhs[i];
            if(isalpha(rhs[i]) || isdigit(rhs[i]))
                if(isalpha(rhs[i+1]) || isdigit(rhs[i+1]))
                    continue;
            if(i!=rhs.length()-1)
                final_rhs+=" ";
        }
        final_rhs=int_to_string(evaluate(final_rhs));
    }
    varib->set_value(lhs,final_rhs);
}

int algo::evaluate(string tokens){
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
