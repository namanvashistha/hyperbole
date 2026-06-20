#ifndef UI_H
#define UI_H
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<functional>
#include<termios.h>
#include<unistd.h>
#include<sys/ioctl.h>
using namespace std;

// Tiny presentation helpers (colors + boxes) shared across the UI.
// Display-only: no program logic lives here.
namespace ui{
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string DIM      = "\033[2m";
    const string RED      = "\033[31m";
    const string GREEN    = "\033[32m";
    const string YELLOW   = "\033[33m";
    const string BLUE     = "\033[34m";
    const string MAGENTA  = "\033[35m";
    const string CYAN     = "\033[36m";
    const string BCYAN    = "\033[96m";
    const string GREY      = "\033[90m";

    // Pad a plain-ASCII string to `width` columns, centered.
    inline string center(const string &s, int width){
        int len = (int)s.size();
        if(len >= width) return s;
        int total = width - len;
        int left  = total/2;
        int right = total - left;
        return string(left,' ') + s + string(right,' ');
    }

    // A horizontal rule made of `═`, optionally colored.
    inline void rule(int width, const string &color = GREY){
        cout << color << "  ";
        for(int i=0;i<width;i++) cout << "─";
        cout << RESET << "\n";
    }

    // A centered box of plain-text lines, drawn with rounded borders.
    inline void box(const vector<string> &lines, int width, const string &color = BCYAN){
        cout << color << BOLD;
        cout << "  ╭"; for(int i=0;i<width;i++) cout << "─"; cout << "╮\n";
        for(const string &ln : lines)
            cout << "  │" << center(ln, width) << "│\n";
        cout << "  ╰"; for(int i=0;i<width;i++) cout << "─"; cout << "╯\n";
        cout << RESET;
    }

    // A section heading: bold colored title + a matching rule.
    inline void heading(const string &title, const string &color = BCYAN, int width = 49){
        cout << "\n" << color << BOLD << "  " << title << RESET << "\n";
        rule(width, GREY);
    }

    // Current terminal size (rows, cols). Falls back to 24x80 when it can't be
    // queried (e.g. piped output) so layout code always has sane numbers.
    inline void term_size(int &rows, int &cols){
        struct winsize ws;
        if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0 && ws.ws_col > 0){
            rows = ws.ws_row; cols = ws.ws_col;
        } else {
            rows = 24; cols = 80;
        }
    }

    // ---- truecolor (24-bit) helpers, for the entry screen ----
    // Most modern terminals and the ttyd web terminal support 24-bit color.
    inline string fg(int r, int g, int b){
        return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
    }
    inline int lerp(int a, int b, double t){ return (int)(a + (b - a) * t + 0.5); }

    // Print block-art rows (cells are '#' = filled, ' ' = empty) as solid
    // blocks, sweeping a left→right color gradient across the whole wordmark.
    inline void gradient_art(const vector<string> &rows, int indent = 4,
                             int r1 = 34,  int g1 = 211, int b1 = 238,    // cyan
                             int r2 = 232, int g2 = 121, int b2 = 249){   // fuchsia
        size_t w = 0;
        for(const string &r : rows) w = max(w, r.size());
        for(const string &row : rows){
            cout << string(indent, ' ');
            for(size_t i = 0; i < row.size(); i++){
                if(row[i] == ' '){ cout << ' '; continue; }
                double t = (w > 1) ? (double)i / (double)(w - 1) : 0.0;
                cout << fg(lerp(r1,r2,t), lerp(g1,g2,t), lerp(b1,b2,t)) << BOLD << "█";
            }
            cout << RESET << "\n";
        }
    }

    // ---- uniform single-key input ----
    // Arrow keys are returned as these sentinels (above the ASCII range).
    enum { KEY_UP = 256, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESC = 27 };

    // Read ONE keypress without requiring Enter (raw mode for the duration of
    // the read, then the terminal is restored). Arrow keys come back as the
    // KEY_* sentinels above. Falls back to a normal getchar() when stdin is
    // not a TTY (e.g. piped input in tests). Use this everywhere so navigation
    // feels identical on every screen.
    inline int read_key(){
        struct termios oldt, newt;
        if(!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &oldt) != 0)
            return getchar();
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN]  = 1;   // block until exactly one byte is available
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int c = getchar();
        if(c == 27){                       // ESC — could be an arrow sequence
            int c1 = getchar();
            if(c1 == '['){
                switch(getchar()){
                    case 'A': c = KEY_UP;    break;
                    case 'B': c = KEY_DOWN;  break;
                    case 'C': c = KEY_RIGHT; break;
                    case 'D': c = KEY_LEFT;  break;
                    default:  c = KEY_ESC;
                }
            } else {
                c = KEY_ESC;
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }

    // Discard any input already sitting in the buffer (e.g. the trailing newline
    // left by the program's `fetch`/`cin >>`, or fast typeahead). Without this a
    // following pause()/read_key() would instantly swallow that stale byte and
    // return without actually waiting for the user.
    inline void drain_input(){
        if(!isatty(STDIN_FILENO)) return;
        struct termios oldt, newt;
        if(tcgetattr(STDIN_FILENO, &oldt) != 0) return;
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN]  = 0;   // non-blocking: read returns immediately when empty
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        while(getchar() != EOF) { /* consume everything pending */ }
        clearerr(stdin);        // clear the EOF flag so later reads work normally
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    // Wait for any single keypress (no Enter needed). Drains stale input first so
    // it always blocks for a fresh keystroke.
    inline void pause(){ drain_input(); read_key(); }

    // Stream a string out character-by-character for a "live terminal" feel,
    // instead of the whole result appearing at once. Skips the delay when stdout
    // isn't a TTY (piped output / tests) so nothing hangs or slows there.
    inline void type_out(const string &s, int ms = 7){
        if(ms <= 0 || !isatty(STDOUT_FILENO)){ cout << s; return; }
        for(char c : s){
            cout << c << flush;
            usleep(ms * 1000);
        }
    }

    // Reusable arrow-key selection loop — the one navigation model used on every
    // screen (home menu, file picker, help, viewer actions). `draw(sel)` renders
    // the whole screen for the current highlighted index `sel`. Returns the chosen
    // index on Enter, or -1 if the user backs out with Q/Esc/←. Up/Down or k/j move.
    inline int select_menu(int count, const function<void(int)> &draw){
        if(count <= 0) return -1;
        int sel = 0;
        while(true){
            draw(sel);
            int k = read_key();
            if(k == KEY_UP   || k == 'k') sel = (sel - 1 + count) % count;
            else if(k == KEY_DOWN || k == 'j') sel = (sel + 1) % count;
            else if(k == '\n' || k == '\r')    return sel;
            else if(k == 'q'  || k == 'Q' || k == KEY_ESC || k == KEY_LEFT) return -1;
        }
    }

    // A horizontal rule that fades along the same cyan→fuchsia gradient.
    inline void gradient_rule(int width, int indent = 2){
        cout << string(indent, ' ');
        for(int i = 0; i < width; i++){
            double t = (width > 1) ? (double)i / (double)(width - 1) : 0.0;
            cout << fg(lerp(34,232,t), lerp(211,121,t), lerp(238,249,t)) << "─";
        }
        cout << RESET << "\n";
    }

    // Visible width of a string: skips ANSI escape sequences and counts UTF-8
    // codepoints (so colored / accented / box-drawn text measures correctly).
    inline int vis_width(const string &s){
        int w = 0; size_t i = 0, n = s.size();
        while(i < n){
            if(s[i] == '\033'){                 // skip CSI … final byte
                while(i < n && s[i] != 'm' && s[i] != 'J' && s[i] != 'H') i++;
                if(i < n) i++;
                continue;
            }
            if(((unsigned char)s[i] & 0xC0) != 0x80) w++;  // count non-continuation bytes
            i++;
        }
        return w;
    }

    // A little "app window" card for a code sample: rounded border, a title bar
    // with three traffic-light dots + a filename, then the already-highlighted
    // lines. `width` is the inner column count.
    inline void code_panel(const string &filename, const vector<string> &lines,
                           int width, int indent = 4){
        const string ind(indent, ' ');
        const string b = fg(88, 92, 110);   // soft border
        auto hr = [&](const char *l, const char *r){
            cout << ind << b << l;
            for(int i=0;i<width;i++) cout << "─";
            cout << r << RESET << "\n";
        };
        auto row = [&](const string &content){
            int pad = width - vis_width(content);
            cout << ind << b << "│" << RESET << content
                 << string(pad > 0 ? pad : 0, ' ') << b << "│" << RESET << "\n";
        };
        hr("╭", "╮");
        row(string("  ") + fg(255,95,86) + "●" + RESET + " " + fg(255,189,46) + "●" + RESET
            + " " + fg(39,201,63) + "●" + RESET + "   " + DIM + filename + RESET);
        hr("├", "┤");
        for(const string &ln : lines) row(string("  ") + ln);
        hr("╰", "╯");
    }

    // A keycap "pill": label on a colored block with dark text — reads as a button.
    inline string key_pill(const string &label, int r, int g, int b){
        return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m"
             + fg(18,18,26) + BOLD + " " + label + " " + RESET;
    }
}
#endif // UI_H
