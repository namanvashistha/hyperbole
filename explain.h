#ifndef EXPLAIN_H
#define EXPLAIN_H
#include<list>
#include<string>
#include<vector>
#include"ui.h"
using namespace std;

// "Explain this program" — annotates the code with plain-English comments, so
// every example reads like well-commented source. Pure heuristics over the
// parsed tokens; no execution. (hyp_colorize / ui::vis_width come from headers
// included before this one.)

static string expl_trim(const string &s){
    size_t a = s.find_first_not_of(" \t");
    if(a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}

static string expl_collapse(const string &s){   // squeeze runs of spaces
    string o; bool sp = false;
    for(char c : s){
        if(c == ' '){ if(!sp && !o.empty()) o += ' '; sp = true; }
        else { o += c; sp = false; }
    }
    return expl_trim(o);
}

static void expl_replace(string &s, const string &from, const string &to){
    size_t p = 0;
    while((p = s.find(from, p)) != string::npos){ s.replace(p, from.size(), to); p += to.size(); }
}

// Render an expression/condition readably: relations as math symbols, the rest
// spaced out. e.g.  "(n%i==0)" -> "n mod i = 0",  "i<=n" -> "i ≤ n".
static string expl_humanize(string s){
    s = expl_trim(s);
    if(s.size() >= 2 && s.front() == '(' && s.back() == ')')   // strip one outer pair
        s = expl_trim(s.substr(1, s.size() - 2));
    expl_replace(s, "==", " = ");
    expl_replace(s, "!=", " ≠ ");
    expl_replace(s, "<=", " ≤ ");
    expl_replace(s, ">=", " ≥ ");
    expl_replace(s, "&&", " and ");
    expl_replace(s, "||", " or ");
    expl_replace(s, "%",  " mod ");
    expl_replace(s, "<",  " < ");
    expl_replace(s, ">",  " > ");
    expl_replace(s, "+",  " + ");
    expl_replace(s, "-",  " - ");
    expl_replace(s, "*",  " * ");
    expl_replace(s, "/",  " / ");
    return expl_collapse(s);
}

static string expl_list(const vector<string> &xs){   // "a, b and c"
    string o;
    for(size_t i = 0; i < xs.size(); i++){
        if(i == 0)                  o = xs[i];
        else if(i + 1 == xs.size()) o += " and " + xs[i];
        else                        o += ", " + xs[i];
    }
    return o;
}

static string expl_display(const vector<string> &args){
    vector<string> parts; bool nl = false;
    for(const string &a : args){
        if(a == "nextln") nl = true;
        else              parts.push_back(a);   // quoted literal or variable name
    }
    if(parts.empty()) return nl ? "prints a blank line" : "prints nothing";
    string body;
    for(size_t i = 0; i < parts.size(); i++){ if(i) body += ", "; body += parts[i]; }
    return "prints " + body + (nl ? ", then a new line" : "");
}

static string expl_assign(const string &full){
    size_t eq = full.find('=');
    if(eq == string::npos) return "evaluates " + expl_humanize(full);
    string lhs = expl_trim(full.substr(0, eq));
    string rhs = expl_trim(full.substr(eq + 1));
    string nl = lhs, nr = rhs;
    expl_replace(nl, " ", ""); expl_replace(nr, " ", "");
    if(nr == nl + "+1") return "increases " + lhs + " by 1";
    if(nr == nl + "-1") return "decreases " + lhs + " by 1";
    return "sets " + lhs + " to " + expl_humanize(rhs);
}

// One English comment for a statement (covers structure & block closers too, so
// the whole program ends up fully commented).
static string expl_comment(const string &kw, const vector<string> &rest,
                           const string &args, const string &full){
    if(kw == "BEGIN")     return "the program starts";
    if(kw == "END")       return "the program ends";
    if(kw == "declare")   return "declares " + expl_list(rest);
    if(kw == "display")   return expl_display(rest);
    if(kw == "fetch")     return "reads " + string(rest.size() > 1 ? "values" : "a value")
                                  + " into " + expl_list(rest) + " from the user";
    if(kw == "loop")      return "repeat while " + expl_humanize(args);
    if(kw == "loopit")    return "go back and repeat the loop";
    if(kw == "check")     return "if " + expl_humanize(args) + " …";
    if(kw == "checkit")   return "end of the check";
    if(kw == "otherwise") return "… otherwise, do this instead";
    if(kw == "leaveit")   return "end of the otherwise block";
    return expl_assign(full);
}

// UTF-8-aware truncation to `maxcp` codepoints (so cutting a comment never
// splits a multibyte char like ≤).
static string expl_truncate(const string &s, int maxcp){
    if(maxcp < 1) return "";
    int cp = 0; size_t i = 0;
    while(i < s.size() && cp < maxcp){
        unsigned char c = s[i];
        int len = (c < 0x80) ? 1 : ((c >> 5) == 0x6) ? 2 : ((c >> 4) == 0xE) ? 3 : 4;
        i += len; cp++;
    }
    return i >= s.size() ? s : s.substr(0, i) + "…";
}

// The annotated view: code on the left, a dim "# explanation" comment beside
// each line (or stacked underneath when the terminal is too narrow).
inline void explain_program(const list<list<string> > &lol, const string &filename){
    system("clear");
    int rows, cols; ui::term_size(rows, cols);
    ui::heading("✦ " + filename + ".hyp — explained", ui::MAGENTA);
    cout << "\n  " << ui::DIM << "your code, annotated line by line" << ui::RESET << "\n\n";

    struct Row { string gutter, code, comment; };
    vector<Row> out;
    int depth = 0, maxw = 0;

    for(const list<string> &tl : lol){
        vector<string> toks(tl.begin(), tl.end());
        if(toks.empty()) continue;
        string lineNo = toks[0];
        string kw = (toks.size() >= 2) ? toks[1] : "";
        vector<string> rest(toks.begin() + (toks.size() >= 2 ? 2 : 1), toks.end());
        string args, full = kw;
        for(size_t i = 0; i < rest.size(); i++){ args += (i ? " " : "") + rest[i]; full += " " + rest[i]; }

        bool opener = (kw == "loop" || kw == "check" || kw == "otherwise");
        bool closer = (kw == "loopit" || kw == "checkit" || kw == "leaveit");
        if(closer && depth > 0) depth--;

        string code = string(depth * 2, ' ');
        for(size_t i = 1; i < toks.size(); i++){
            code += hyp_colorize(toks[i]);
            if(i + 1 < toks.size()) code += " ";
        }
        code += ui::GREY + string(";") + ui::RESET;

        string ln = lineNo; while(ln.size() < 2) ln = " " + ln;
        Row r;
        r.gutter  = string("  ") + ui::GREY + ln + ui::RESET + "  ";   // visible width 6
        r.code    = code;
        r.comment = expl_comment(kw, rest, args, full);
        out.push_back(r);

        int w = ui::vis_width(code);
        if(w > maxw) maxw = w;
        if(opener) depth++;
    }

    const int gutterW = 6;
    int commentCol = maxw + 3;                         // where the "#" column sits
    int budget = cols - gutterW - commentCol - 2;      // room left for comment text
    bool inlineOk = budget >= 14;                      // else stack comments below
    const string CMT = ui::fg(106, 153, 85);           // classic comment green

    for(const Row &r : out){
        if(inlineOk){
            cout << r.gutter << r.code;
            if(!r.comment.empty()){
                int pad = commentCol - ui::vis_width(r.code);
                if(pad < 1) pad = 1;
                string cm = (ui::vis_width(r.comment) > budget) ? expl_truncate(r.comment, budget - 1) : r.comment;
                cout << string(pad, ' ') << CMT << "# " << cm << ui::RESET;
            }
            cout << "\n";
        } else {
            cout << r.gutter << r.code << "\n";
            if(!r.comment.empty()){
                int avail = cols - gutterW - 4;
                string cm = (ui::vis_width(r.comment) > avail) ? expl_truncate(r.comment, avail - 1) : r.comment;
                cout << string(gutterW, ' ') << CMT << "# " << cm << ui::RESET << "\n";
            }
        }
    }

    cout << "\n";
    ui::rule(49);
    cout << "  " << ui::key_pill("any key", 148,163,184) << ui::DIM << " back to your file" << ui::RESET << "\n";
    ui::pause();
}

#endif // EXPLAIN_H
