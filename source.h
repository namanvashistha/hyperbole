#ifndef SOURCE_H
#define SOURCE_H
#include<iostream>
#include<cstdlib>
#include<dirent.h>
#include<fstream>
#include<list>
#include<string>
#include<vector>
#include<algorithm>
#include<cctype>
#include<termios.h>
#include<unistd.h>
#include"algo.h"
#include"ui.h"
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
        bool pick_file();
        bool open_editor();

};

// Syntax-highlight helpers (defined lower down) — forward-declared so the file
// picker, which appears above them, can render a live source preview.
static bool   is_hyp_keyword(const string &w);
static string hyp_highlight_line(const string &line);
bool source::new_file(){
    ofstream file;
    file.open("source/"+filename+".hyp");
    // Seed brand-new files with a tiny working program so first-time users
    // see real .hyp syntax (and can Compile & Run it immediately) instead of
    // staring at an empty buffer in vim.
    file << "BEGIN:\n"
         << "\tdeclare name;\n"
         << "\tdisplay \"Hello! What is your name? \";\n"
         << "\tfetch name;\n"
         << "\tdisplay \"Welcome, \" name nextln;\n"
         << "END;\n";
    return true;
}

bool source::list_dir(){
    ui::heading("source codes", ui::MAGENTA);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("source")) != NULL) {
        int nxt_lin=0;
        while ((ent = readdir (dir)) != NULL) {
            string f_name=ent->d_name;
            if(f_name[f_name.size()-1]=='p'){
                f_name= f_name.substr(0, f_name.size()-4);
                if(nxt_lin%5==0) cout<<"\n  ";
                cout<<ui::CYAN<<ui::BOLD<<f_name<<".hyp"<<ui::RESET<<"\t";
                nxt_lin++;
            }
        }
        closedir (dir);
    } else {
         perror ("");
    }
    cout<<"\n\n";
    ui::rule(49);
    cout<<ui::DIM<<"  type a name to open it, or a new name to create one  > "<<ui::RESET;
    return true;
}

// Interactive file picker: arrow-key (or j/k) navigation over the .hyp files
// in source/, plus a "new file" entry at the bottom. Sets `filename` on Enter.
// Returns false if the user backs out with Q/Esc. Works in the ttyd web
// terminal because we drive the TTY directly with termios raw mode.
bool source::pick_file(){
    vector<string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("source")) != NULL){
        while ((ent = readdir(dir)) != NULL){
            string f_name = ent->d_name;
            if(f_name.size() > 4 && f_name.substr(f_name.size()-4) == ".hyp")
                files.push_back(f_name.substr(0, f_name.size()-4));
        }
        closedir(dir);
    }
    sort(files.begin(), files.end());

    int total = (int)files.size() + 1; // last entry = "new file"
    int sel = 0;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);   // raw-ish: read keys as they arrive, no echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    tcflush(STDIN_FILENO, TCIFLUSH);    // drop the stray '\n' left by the menu's cin>>ch,
                                        // otherwise it reads as Enter and auto-opens file #1

    bool chose = false;
    while(true){
        system("clear");
        ui::heading("source codes", ui::MAGENTA);
        cout << "\n";
        for(int i=0;i<(int)files.size();i++){
            if(i==sel) cout << "  " << ui::GREEN << ui::BOLD << "❯ " << files[i] << ".hyp" << ui::RESET << "\n";
            else       cout << "    " << ui::CYAN << files[i] << ".hyp" << ui::RESET << "\n";
        }
        if(sel==(int)files.size())
            cout << "  " << ui::GREEN << ui::BOLD << "❯ + new file…" << ui::RESET << "\n";
        else
            cout << "    " << ui::YELLOW << "+ new file…" << ui::RESET << "\n";
        cout << "\n";
        ui::rule(49);

        // Detail panel: a live, syntax-highlighted preview of the highlighted
        // file (fixed height so the footer stays put), mirroring the help screen.
        const int PREVIEW = 10;
        if(sel < (int)files.size()){
            cout << ui::DIM << "  source/" << files[sel] << ".hyp" << ui::RESET << "\n";
            vector<string> plines;
            ifstream pf("source/" + files[sel] + ".hyp");
            string pl;
            while(getline(pf, pl)) plines.push_back(pl);
            for(int r=0;r<PREVIEW;r++){
                if(r >= (int)plines.size()) { cout << "\n"; continue; }
                if(r==PREVIEW-1 && (int)plines.size() > PREVIEW)
                    cout << ui::DIM << "  ⋮ " << (plines.size()-(PREVIEW-1)) << " more line(s)" << ui::RESET << "\n";
                else
                    cout << "  " << hyp_highlight_line(plines[r]) << "\n";
            }
        } else {
            cout << ui::DIM << "  start a brand-new .hyp program" << ui::RESET << "\n";
            cout << "\n  you'll name it, then it opens in your editor\n";
            cout << "  pre-seeded with a tiny working example.\n";
            for(int r=4;r<PREVIEW+1;r++) cout << "\n";
        }
        ui::rule(49);
        cout << "  " << ui::DIM << "↑/↓ move     " << ui::RESET
             << ui::key_pill("Enter", 52,211,153) << ui::DIM << " open     " << ui::RESET
             << ui::key_pill("Q", 148,163,184)    << ui::DIM << " back"      << ui::RESET << "\n";

        int c = getchar();
        if(c == '\033'){                 // escape sequence, e.g. arrow keys: ESC [ A/B
            if(getchar() == '['){
                int k = getchar();
                if(k=='A') sel = (sel - 1 + total) % total;
                else if(k=='B') sel = (sel + 1) % total;
            } else {                     // lone Esc = back out
                break;
            }
        } else if(c=='\n' || c=='\r'){
            chose = true;
            break;
        } else if(c=='k'){ sel = (sel - 1 + total) % total; }
          else if(c=='j'){ sel = (sel + 1) % total; }
          else if(c=='q' || c=='Q'){ break; }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore cooked mode
    if(!chose) return false;

    if(sel == (int)files.size()){
        system("clear");
        ui::heading("new file", ui::MAGENTA);
        cout << ui::DIM << "  enter a name (without .hyp)  > " << ui::RESET;
        cin >> filename;
        cin.ignore(); // drop the trailing newline so later prompts don't skip
        new_file();   // seed it with a starter example before the editor opens
    } else {
        filename = files[sel];
    }
    return true;
}

// How many .hyp programs ship in source/ — used for the home-screen stats line.
static int count_hyp_files(){
    int n = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("source")) != NULL){
        while ((ent = readdir(dir)) != NULL){
            string f = ent->d_name;
            if(f.size() > 4 && f.substr(f.size()-4) == ".hyp") n++;
        }
        closedir(dir);
    }
    return n;
}

bool source::open_editor(){
    string path = "source/" + filename + ".hyp";
    // Launch a terminal editor inside the current TTY (works in the ttyd web
    // terminal too), so you get real editor keybindings for free. Honour the
    // user's $EDITOR first, then fall back through vim/vi/nano, and finally to
    // GUI editors for local desktop use.
    const char *env_ed = getenv("EDITOR");
    string editors[] = {
        env_ed ? string(env_ed) : "",
        "vim", "vi", "nano", "subl -w", "notepad"
    };
    for(const string &ed : editors){
        if(ed.empty()) continue;
        string cmd = ed + " \"" + path + "\"";
        if(system(cmd.c_str()) == 0) break;
    }
    return true;
}

bool source::open_file(){

    fstream file;
    file.open("source/"+filename+".hyp");
    if(file.fail()) return false;
    string s="";
    int cmt_flag=0;
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
        if(s[i]!=' ' && s[i]!=';' && s[i]!=',' && s[i]!=':' && str_flg==0){
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
        if(s[i]==';' || s[i]==':'){
            lol.push_back(l);
            l.erase(l.begin(),l.end());
            l.push_back(al.int_to_string(lineNo));
            while(!isalpha(s[i])){
                i++;
            }
            i--;
            lineNo++;
        }
    }
    return true;
}

// --- syntax highlighting for the parsed-code view ---
static bool is_hyp_keyword(const string &w){
    static const char *kws[] = {
        "BEGIN","END","declare","display","fetch","nextln",
        "loop","loopit","check","checkit","otherwise","leaveit"
    };
    for(const char *k : kws) if(w == k) return true;
    return false;
}

// Highlight a whole raw source line (keeps original indentation/comments) for
// the file-picker preview. Walks the line: string literals green, keywords
// yellow, numbers cyan, operators magenta, everything else (incl. ; , :) plain.
static string hyp_highlight_line(const string &line){
    static const string ops = "+-*/%<>=!&|()";
    string out;
    size_t i = 0, n = line.size();
    while(i < n){
        char c = line[i];
        if(c == '"'){                                   // string literal
            size_t j = i + 1;
            while(j < n && line[j] != '"') j++;
            if(j < n) j++;                              // include closing quote
            out += ui::GREEN + line.substr(i, j-i) + ui::RESET;
            i = j;
        } else if(isalpha((unsigned char)c) || c == '_'){
            size_t j = i;
            while(j < n && (isalnum((unsigned char)line[j]) || line[j]=='_')) j++;
            string w = line.substr(i, j-i);
            out += is_hyp_keyword(w) ? (ui::YELLOW + ui::BOLD + w + ui::RESET) : w;
            i = j;
        } else if(isdigit((unsigned char)c)){
            size_t j = i;
            while(j < n && isdigit((unsigned char)line[j])) j++;
            out += ui::CYAN + line.substr(i, j-i) + ui::RESET;
            i = j;
        } else if(ops.find(c) != string::npos){
            out += ui::MAGENTA + string(1,c) + ui::RESET;
            i++;
        } else {
            out += c;                                   // whitespace, ; , : etc.
            i++;
        }
    }
    return out;
}

// Colorize a single parsed token: keywords pop yellow, string literals green,
// numbers cyan, operators magenta, everything else (variables) plain.
static string hyp_colorize(const string &tok){
    if(is_hyp_keyword(tok))
        return ui::YELLOW + ui::BOLD + tok + ui::RESET;
    if(!tok.empty() && tok[0] == '"')
        return ui::GREEN + tok + ui::RESET;
    static const string ops = "+-*/%<>=!&|()";
    string out;
    for(char c : tok){
        if(isdigit((unsigned char)c))
            out += ui::CYAN + string(1,c) + ui::RESET;
        else if(ops.find(c) != string::npos)
            out += ui::MAGENTA + string(1,c) + ui::RESET;
        else
            out += string(1,c);
    }
    return out;
}

bool source::show_file(){
    cout << "\n";
    // Build the highlighted code lines (with line numbers + re-derived block
    // indentation), then show them inside the same "app window" card as home.
    vector<string> lines;
    int depth = 0;
    for (list<list<string> >::iterator itr=lol.begin(); itr != lol.end(); ++itr){
        list<string> tl = *itr;
        if(tl.empty()) continue;
        list<string>::iterator it = tl.begin();
        string num = *it; ++it;                          // first token is the line number
        string kw  = (it != tl.end()) ? *it : "";        // first code token = statement kind
        bool opener = (kw=="BEGIN"||kw=="loop"||kw=="check"||kw=="otherwise");
        bool closer = (kw=="END"||kw=="loopit"||kw=="checkit"||kw=="leaveit");
        if(closer && depth>0) depth--;                   // dedent the closer itself

        string numcol = (num.size() < 2) ? string(2-num.size(),' ') + num : num;
        string s = ui::GREY + numcol + ui::RESET + "   " + string(depth*3, ' ');
        for(; it != tl.end(); ++it) s += hyp_colorize(*it) + " ";
        s += ui::GREY + ";" + ui::RESET;
        lines.push_back(s);

        if(opener) depth++;                              // indent everything after the opener
    }

    // Size the window to the longest line, clamped to the terminal width.
    int rows, cols; ui::term_size(rows, cols);
    int maxw = 20;
    for(const string &l : lines) maxw = max(maxw, ui::vis_width(l));
    int width = maxw + 2;
    if(width > cols - 8) width = cols - 8;

    ui::code_panel(filename + ".hyp", lines, width, 2);

    cout << "\n";
    cout << ui::DIM << "  keywords: declare display fetch nextln  ·  loop…loopit  ·  "
         << "check…checkit  otherwise…leaveit\n" << ui::RESET << "\n";
    cout<<"  "<<ui::key_pill("E", 96,165,250) <<ui::DIM<<" edit    "         <<ui::RESET
        <<     ui::key_pill("C", 52,211,153) <<ui::DIM<<" compile & run    " <<ui::RESET
        <<     ui::key_pill("X", 192,132,252)<<ui::DIM<<" explain    "       <<ui::RESET
        <<     ui::key_pill("?", 56,189,248) <<ui::DIM<<" help    "          <<ui::RESET
        <<     ui::key_pill("Q", 248,113,113)<<ui::DIM<<" close"             <<ui::RESET<<"\n";
    return true;
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
    return true;
}

bool source::close_file(){
    lol.erase(lol.begin(),lol.end());
    return true;
}
#endif // SOURCE_H
