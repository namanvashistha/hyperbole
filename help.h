#ifndef HELP_H
#define HELP_H
#include<string>
#include<map>
#include<vector>
#include<utility>
#include<iostream>
#include<fstream>
#include<termios.h>
#include<unistd.h>
#include"ui.h"

class help
{
    public:
        map<string,string> keywords;
        help();
        void show();
        string show(string key);
        void browse();
};

help::help(){
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

void help::show(){
    map<string,string>::iterator it;
    int col=0;
    for(it=keywords.begin();it!=keywords.end();it++){
        cout<<"    "<<ui::CYAN<<ui::BOLD<<it->first<<ui::RESET;
        if(++col%4==0) cout<<"\n"; else cout<<"\t";
    }
    if(col%4!=0) cout<<"\n";
}

string help::show(string key){
    map<string,string>::iterator it;
    int p=0;
    for(it=keywords.begin();it!=keywords.end();it++){
        if(it->first==key){
            p=1;
            break;
        }
    }
    if(p==1){
        if(keywords.at(key)!="\0")
            return keywords.at(key);
        else
            return "doesn't exist";
    }
    else
        return "doesn't exist";
}

// Which category a keyword belongs to (used to group the list and badge the
// detail card).
static string help_category(const string &k){
    if(k=="BEGIN" || k=="END")                          return "structure";
    if(k=="display" || k=="nextln")                     return "output";
    if(k=="fetch")                                      return "input";
    if(k=="declare")                                    return "variables";
    return "control flow";   // loop/loopit/check/checkit/otherwise/leaveit
}
// The accent color for a category (r,g,b out-params).
static void help_cat_color(const string &c, int &r, int &g, int &b){
    if(c=="overview"){ r=34; g=211; b=238; }          // cyan
    else if(c=="structure"){ r=192; g=132; b=252; }   // violet
    else if(c=="output"){ r=52; g=211; b=153; }       // green
    else if(c=="input"){ r=56; g=189; b=248; }        // sky
    else if(c=="variables"){ r=96; g=165; b=250; }    // blue
    else if(c=="errors"){ r=248; g=113; b=113; }      // red
    else { r=250; g=204; b=21; }                       // control flow — amber
}
// Rich, teachable reference entry for one keyword (or overview / error topic).
struct HelpInfo {
    string syntax;            // the general form, e.g.  display <item> … ;
    string blurb;             // plain-English explanation
    vector<string> example;   // a few runnable .hyp lines
    string output;            // what the example prints ("" if nothing)
    string seealso;           // related keywords
    string fix = "";          // suggested fix (used by error topics)
};

static HelpInfo help_info(const string &k){
    if(k=="getting started") return {
        "BEGIN:   …statements…   END;",
        "A Hyperbole program runs top to bottom between BEGIN: and END;. Every statement ends with ; — declare variables first, build values with expressions like sum = sum + i, print with display, and branch or repeat with check and loop.",
        {"BEGIN:", "  declare n, sum;", "  sum = 0;", "  fetch n;", "  loop (n > 0):", "    sum = sum + n;", "    n = n - 1;", "  loopit;", "  display \"total = \" sum nextln;", "END;"},
        "total = 6   (for n = 3)", "declare, display, loop" };
    if(k=="undefined variable") return {
        "undefined variable",
        "You read or printed a variable that was never given a value. Declare it (or fetch / assign into it) before you use it.",
        {"display total nextln;"}, "undefined variable", "declare, fetch",
        "add  declare total;  and set it, e.g.  total = 0;" };
    if(k=="illegal name") return {
        "<name> is a illegal name of variable",
        "Variable names must start with a letter and contain only letters, digits, or underscores. Names that start with a digit or contain symbols are rejected.",
        {"declare 2x;"}, "2x is a illegal name of variable", "declare",
        "rename it to start with a letter, e.g.  x2" };
    if(k=="BEGIN") return {
        "BEGIN:",
        "Opens the program. Every Hyperbole program starts with BEGIN: and runs top to bottom from here.",
        {"BEGIN:", "  display \"hi\" nextln;", "END;"}, "hi", "END" };
    if(k=="END") return {
        "END;",
        "Closes the program. Everything between BEGIN: and END; is your code.",
        {"BEGIN:", "  display \"bye\" nextln;", "END;"}, "bye", "BEGIN" };
    if(k=="declare") return {
        "declare <name>, <name>, … ;",
        "Creates one or more variables so you can store values in them. Declare a name before you use it.",
        {"declare i, n, sum;"}, "", "fetch, display" };
    if(k=="display") return {
        "display <item> <item> … ;",
        "Prints to the screen. Use \"quotes\" for literal text, a variable name for its value, and nextln for a new line.",
        {"declare s;", "s = 42;", "display \"score is \" s nextln;"}, "score is 42", "nextln, fetch" };
    if(k=="fetch") return {
        "fetch <name> ;",
        "Pauses and reads a value the user types, then stores it in the variable.",
        {"declare n;", "display \"enter n: \";", "fetch n;"}, "(waits for you to type)", "declare, display" };
    if(k=="nextln") return {
        "display … nextln ;",
        "Moves the output to the next line. Used inside a display statement.",
        {"display \"one\" nextln;", "display \"two\" nextln;"}, "one\ntwo", "display" };
    if(k=="loop") return {
        "loop ( <condition> ):  …  loopit ;",
        "Repeats the block as long as the condition is true. Always close it with loopit.",
        {"i = 1;", "loop (i <= 3):", "  display i nextln;", "  i = i + 1;", "loopit;"}, "1\n2\n3", "loopit, check" };
    if(k=="loopit") return {
        "loop ( … ):  …  loopit ;",
        "Marks the end of a loop block. Execution jumps back to the matching loop while its condition holds.",
        {"loop (i <= 3):", "  i = i + 1;", "loopit;"}, "", "loop" };
    if(k=="check") return {
        "check ( <condition> ):  …  checkit ;",
        "Runs the block only when the condition is true. Close it with checkit. Add an otherwise: block for the false case.",
        {"check (n == 0):", "  display \"zero\" nextln;", "checkit;"}, "zero   (when n is 0)", "checkit, otherwise" };
    if(k=="checkit") return {
        "check ( … ):  …  checkit ;",
        "Marks the end of a check block.",
        {"check (n == 0):", "  display \"zero\" nextln;", "checkit;"}, "", "check, otherwise" };
    if(k=="otherwise") return {
        "checkit; otherwise:  …  leaveit ;",
        "The block that runs when the matching check was false. Place it right after checkit and close it with leaveit.",
        {"check (n == 0):", "  display \"zero\" nextln;", "checkit;", "otherwise:", "  display \"not zero\" nextln;", "leaveit;"},
        "not zero   (when n is not 0)", "check, leaveit" };
    if(k=="leaveit") return {
        "otherwise:  …  leaveit ;",
        "Marks the end of an otherwise block.",
        {"otherwise:", "  display \"nope\" nextln;", "leaveit;"}, "", "otherwise, check" };
    return { k + ";", "", {k + ";"}, "", "" };
}

// Interactive language reference: keywords grouped by category with colored
// category badges, and a detail card that shows the description plus a
// syntax-highlighted snippet in the same "app window" used on the home screen.
// Arrow-key (or j/k) navigation, raw-mode TTY so it works in the ttyd web term.
void help::browse(){
    if(keywords.empty()) return;

    // Build the list in category order so it reads like a reference card.
    // A "getting started" overview leads, then keywords by category, then the
    // common error messages — all rendered with the same detail card.
    static const char *ORDER[] = {"structure","output","input","variables","control flow"};
    vector<pair<string,string> > items;   // (label, _)
    vector<string> itemCat;
    items.push_back({"getting started",""}); itemCat.push_back("overview");
    for(const char *cat : ORDER)
        for(map<string,string>::iterator it=keywords.begin(); it!=keywords.end(); ++it)
            if(help_category(it->first) == cat){ items.push_back(*it); itemCat.push_back(cat); }
    items.push_back({"undefined variable",""}); itemCat.push_back("errors");
    items.push_back({"illegal name",""});       itemCat.push_back("errors");

    int total = (int)items.size();
    int sel = 0;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Word-wrap helper: print `text` at the given width with a 2-space indent.
    auto wrap = [](const string &text, int width, const string &color){
        string line;
        size_t i = 0;
        while(i < text.size()){
            size_t j = text.find(' ', i);
            string word = (j==string::npos) ? text.substr(i) : text.substr(i, j-i);
            if(!line.empty() && (int)(line.size() + 1 + word.size()) > width){
                cout << "  " << color << line << ui::RESET << "\n";
                line = word;
            } else {
                line = line.empty() ? word : line + " " + word;
            }
            i = (j==string::npos) ? text.size() : j+1;
        }
        if(!line.empty()) cout << "  " << color << line << ui::RESET << "\n";
    };

    while(true){
        system("clear");
        ui::heading("HELP — language reference", ui::YELLOW);
        cout << "\n";

        // Compact category index — one row per category, fixed-width cells so the
        // highlight never shifts the layout as you move.
        string curCat = "";
        for(int i=0;i<total;i++){
            if(itemCat[i] != curCat){
                if(!curCat.empty()) cout << "\n";
                curCat = itemCat[i];
                int r,g,b; help_cat_color(curCat, r,g,b);
                int padn = (int)curCat.size() < 13 ? 13 - (int)curCat.size() : 1;
                cout << "  " << ui::fg(r,g,b) << ui::BOLD << curCat << string(padn,' ') << ui::RESET;
            }
            string cell = (i==sel) ? "[" + items[i].first + "]" : " " + items[i].first + " ";
            if(i==sel) cout << ui::GREEN << ui::BOLD << cell << ui::RESET << " ";
            else       cout << ui::DIM   << cell << ui::RESET << " ";
        }
        cout << "\n\n";
        ui::gradient_rule(67, 2);

        // ---- detail card for the highlighted keyword ----
        string kw = items[sel].first;
        HelpInfo info = help_info(kw);
        bool isErr = (itemCat[sel] == "errors");
        int r,g,b; help_cat_color(itemCat[sel], r,g,b);

        cout << "\n  " << ui::YELLOW << ui::BOLD << kw << ui::RESET
             << "   " << ui::key_pill(itemCat[sel], r,g,b) << "\n\n";
        wrap(info.blurb, 60, ui::RESET);

        cout << "\n  " << ui::DIM << (isErr ? "message" : "syntax") << ui::RESET << "\n";
        cout << "    " << (isErr ? ui::RED : ui::CYAN) << info.syntax << ui::RESET << "\n";

        cout << "\n  " << ui::DIM << (isErr ? "triggered by" : "example") << ui::RESET << "\n";
        // hyp_highlight_line comes from source.h (included before this header).
        vector<string> snip;
        for(const string &ln : info.example) snip.push_back(hyp_highlight_line(ln));
        string panelTitle = isErr ? "oops.hyp" : (kw=="getting started" ? "program.hyp" : kw + ".hyp");
        ui::code_panel(panelTitle, snip, 52, 2);

        if(!info.output.empty()){
            string olabel = isErr ? "▸ error " : "▸ prints";
            string ocol   = isErr ? ui::RED   : ui::GREEN;
            int lr = isErr ? 248 : 52, lg = isErr ? 113 : 211, lb = isErr ? 113 : 153;
            if(info.output.find('\n') == string::npos){
                cout << "  " << ui::fg(lr,lg,lb) << olabel << ui::RESET
                     << "   " << ocol << info.output << ui::RESET << "\n";
            } else {
                cout << "  " << ui::fg(lr,lg,lb) << olabel << ui::RESET << "\n";
                size_t p = 0;
                while(p < info.output.size()){
                    size_t nl = info.output.find('\n', p);
                    cout << "      " << ocol
                         << (nl==string::npos ? info.output.substr(p) : info.output.substr(p, nl-p))
                         << ui::RESET << "\n";
                    if(nl==string::npos) break;
                    p = nl + 1;
                }
            }
        }

        if(!info.fix.empty())
            cout << "  " << ui::fg(52,211,153) << "▸ fix   " << ui::RESET
                 << " " << ui::GREEN << info.fix << ui::RESET << "\n";

        if(!info.seealso.empty())
            cout << "\n  " << ui::DIM << "see also  " << ui::RESET
                 << ui::CYAN << info.seealso << ui::RESET << "\n";

        cout << "\n";
        ui::gradient_rule(67, 2);
        cout << "  " << ui::DIM << "↑/↓ move     " << ui::RESET
             << ui::key_pill("Q", 148,163,184) << ui::DIM << " back" << ui::RESET << "\n";

        int c = getchar();
        if(c == '\033'){                  // arrow keys: ESC [ A/B
            if(getchar() == '['){
                int k = getchar();
                if(k=='A') sel = (sel - 1 + total) % total;
                else if(k=='B') sel = (sel + 1) % total;
            } else break;                 // lone Esc = back out
        } else if(c=='k'){ sel = (sel - 1 + total) % total; }
          else if(c=='j'){ sel = (sel + 1) % total; }
          else if(c=='q' || c=='Q'){ break; }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

#endif // HELP_H
