#ifndef IDE_H
#define IDE_H
#include<iostream>
#include<list>
#include<string>
#include<limits>
#include<stdlib.h>
#include"source.h"
#include"help.h"
#include"execute.h"
#include"explain.h"
#include"ui.h"
using namespace std;

// Build one syntax-highlighted line of the home-screen code sample. `pad` extra
// spaces give body-of-loop indentation; `term` is the trailing punctuation
// (";" for statements, ":" for block openers).
static string home_code(int pad, const vector<string> &toks, const string &term){
    string s(pad, ' ');
    for(size_t i=0;i<toks.size();i++){
        s += hyp_colorize(toks[i]);
        if(i+1<toks.size()) s += " ";
    }
    s += ui::GREY + term + ui::RESET;
    return s;
}

int main(){

    BEGIN: ;
    help hlp;     // loads keyword.txt — reused by the H screen and the stats line
    source src;

    // ---- home screen: fixed choices, so it's a simple keypress (O / H), no
    // arrow navigation. Rendered once; the footer lists the keys. ----
    system("clear");
    // Fit the welcome screen to the terminal: drop the heaviest sections first
    // (code sample, then "good for", then the big wordmark) as height shrinks, so
    // the menu is always visible without scrolling.
    int rows, cols; ui::term_size(rows, cols);
    bool art     = rows >= 16 && cols >= 68;   // big block wordmark
    bool goodfor = rows >= 24;                  // the use-case bullets
    bool sample  = rows >= 34 && cols >= 70;    // the code-sample card
    int ww = min(73, cols);                                  // centering width
    int rw = (cols - 6 < 67) ? max(10, cols - 6) : 67;       // rule width

    if(art){
        cout << "\n\n";
        ui::gradient_art({
            "#   #  #   #  ####   #####  ####   ####    ###   #      #####",
            "#   #   # #   #   #  #      #   #  #   #  #   #  #      #",
            "#####    #    ####   ####   ####   ####   #   #  #      ####",
            "#   #    #    #      #      #  #   #   #  #   #  #      #",
            "#   #    #    #      #####  #   #  ####    ###   #####  #####"
        }, 6);
        cout << "\n";
    } else {
        cout << "\n  " << ui::BCYAN << ui::BOLD << ui::center("H Y P E R B O L E", ww)
             << ui::RESET << "\n\n";
    }
    cout << ui::GREY << ui::center("a tiny interpreted language · an IDE in your terminal", ww)
         << ui::RESET << "\n";
    cout << ui::DIM  << ui::center("written in C++  ·  v1.0", ww) << ui::RESET << "\n\n";
    ui::gradient_rule(rw, 4);

    if(sample){
        // ---- a taste of the language, shown in an "app window" card ----
        cout << "\n    " << ui::BCYAN << ui::BOLD << "a taste of .hyp" << ui::RESET
             << ui::DIM << "   — read input, loop, print a result" << ui::RESET << "\n\n";
        ui::code_panel("example.hyp", {
            home_code(0, {"declare","i,","n,","sum"},            ";"),
            home_code(0, {"fetch","n"},                          ";"),
            home_code(0, {"loop","(i","<=","n)"},                ":"),
            home_code(4, {"sum","=","sum","+","i"},              ";"),
            home_code(4, {"i","=","i","+","1"},                  ";"),
            home_code(0, {"loopit"},                             ";"),
            home_code(0, {"display","\"sum = \"","sum","nextln"}, ";"),
        }, 63, 4);
    }

    if(goodfor){
        // ---- why you'd reach for it ----
        cout << "\n    " << ui::BCYAN << ui::BOLD << "good for" << ui::RESET << "\n";
        cout << "      " << ui::fg(120,200,255) << "▸" << ui::RESET << "  learning loops, variables & conditions hands-on\n";
        cout << "      " << ui::fg(160,170,255) << "▸" << ui::RESET << "  seeing how source is " << ui::BOLD << "tokenized" << ui::RESET << ", then run\n";
        cout << "      " << ui::fg(210,150,255) << "▸" << ui::RESET << "  a tiny, hackable interpreter you can read end to end\n";
    }

    cout << "\n";
    ui::gradient_rule(rw, 4);

    cout << "\n    " << ui::DIM << hlp.keywords.size() << " keywords  ·  "
         << count_hyp_files() << " example programs  ·  MIT licensed" << ui::RESET << "\n\n";
    cout << "    " << ui::key_pill("O", 52,211,153) << ui::DIM << "  open or create a file      " << ui::RESET
         <<           ui::key_pill("H", 250,204,21) << ui::DIM << "  browse help"                  << ui::RESET << "\n";

    int ch = ui::read_key();   // single keypress — fixed choices, no arrows
    switch(ch){
        case 'h':
        case 'H':
            hlp.browse();
            goto BEGIN;
        break;
        case 'o':
        case 'O':system("clear");
            if(!src.pick_file()){
                goto BEGIN;
            }
            FILE: ;
            while(1){
                src.close_file();                       // start clean, then (re)parse
                if(!src.open_file()){ src.new_file(); src.open_file(); }
                system("clear");
                src.show_file();                        // fixed actions → letter keys (see footer)
                int k = ui::read_key();
                if(k=='e' || k=='E'){ src.open_editor(); continue; } // edit, then reparse & redraw
                if(k=='c' || k=='C'){ goto COMPILE; }                // compile & run
                if(k=='x' || k=='X'){ explain_program(src.lol, src.filename); continue; } // plain-English walkthrough
                if(k=='?'){ hlp.browse(); continue; } // open help, keep your place
                if(k=='q' || k=='Q'){ goto BEGIN; }                  // close
                // R or any other key just refreshes (loop re-parses)
            }
        break;
            COMPILE: system("clear");
            ui::heading("▶ run  ·  " + src.filename + ".hyp", ui::GREEN);
            cout << "\n  " << ui::DIM << "output" << ui::RESET << "\n";
            ui::rule(49);
            cout << "\n";
            {
                execute exe(src.lol);
                exe.executing();    // streams output; fetch reads input live
                exe.show_memory();  // prints the "Memory" section
            }
            cout << "\n";
            ui::gradient_rule(67, 2);
            cout << "  " << ui::key_pill("R", 52,211,153)  << ui::DIM << " run again    " << ui::RESET
                 <<        ui::key_pill("Q", 148,163,184)  << ui::DIM << " back to file"  << ui::RESET << "\n";
            {
                ui::drain_input();             // ignore any leftover input from the program's fetch
                int k = ui::read_key();
                if(k=='r' || k=='R') goto COMPILE;   // re-run with a fresh interpreter
            }
            src.close_file();
            goto FILE;
        break;
    }
    system("clear");
    cout << "\n";
    ui::box({ "", "thanks for using HYPERBOLE", "" }, 49, ui::BCYAN);
    cout << "\n";
}
#endif // IDE_H