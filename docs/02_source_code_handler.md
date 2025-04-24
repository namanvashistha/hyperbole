# Chapter 2: The Source Code Handler - Your Code Librarian

In [Chapter 1: The IDE Shell - Your Welcome Desk](01_ide_shell.md), we saw how Hyperbole starts up and gives you options like opening a file (`O`), getting help (`H`), or exiting (`E`). When you choose `'O'`, the IDE Shell doesn't magically handle the file itself. Instead, it calls upon another specialist module: the **Source Code Handler**.

Think of the Source Code Handler as Hyperbole's dedicated librarian. If the IDE Shell is the front desk, the Source Code Handler is the person who goes into the stacks, finds the specific book (your code file), opens it up, and prepares it for reading by breaking it down neatly.

## What's the Point? Managing Your Blueprints

Why do we need a separate Source Code Handler? Imagine you want to build something complex, like a model airplane. You'd have blueprints (your source code) stored somewhere. You need a system to:

1.  See which blueprints you have (`.hyp` files).
2.  Pick one blueprint to work on.
3.  Open it up so you can see or edit it (maybe on a large table or using special software).
4.  Understand the instructions on the blueprint by breaking them down into steps and parts (parsing the code).
5.  Show the organized steps clearly (displaying the parsed code).

The Source Code Handler does all this for your Hyperbole code files.

**Use Case:** Let's follow what happens when you press 'O' in the IDE Shell. You want to work with a Hyperbole code file named `my_program.hyp`. The Source Code Handler will be responsible for finding this file (or helping you create it), letting you edit it, reading its contents, and preparing those contents for the next stage: running the code.

## Key Responsibilities: The Librarian's Tasks

The Source Code Handler, represented by the `source` class in `source.h`, has several key jobs:

1.  **Listing Files (`list_dir`):** Like a librarian showing you a catalog, it looks inside the `source/` folder and lists all the existing `.hyp` files.
2.  **Opening Editor (`open_editor`):** This is like the librarian handing you the book and maybe a magnifying glass or directing you to a quiet reading desk. It tries to open your chosen `.hyp` file using an external text editor (like Sublime Text or Notepad) so you can easily write or modify your code.
3.  **Reading & Parsing (`open_file`):** This is the core preparation step. The librarian carefully reads the book (your `.hyp` file) line by line, sentence by sentence (or word by word). It translates the raw text into a structured format that the rest of Hyperbole can easily understand. This format is a "list of lists of strings," which we'll call `lol`.
4.  **Displaying Parsed Code (`show_file`):** After preparing the structured version (`lol`), it can display this organized version back to you within the Hyperbole IDE's terminal window. This shows you how Hyperbole *understood* your code.
5.  **Creating New Files (`new_file`):** If the file you asked for doesn't exist, the librarian can create a new, empty book for you.

## Following the 'Open File' Path

Let's trace the steps when you choose 'O' in the IDE Shell:

1.  **List Available Files:** The IDE Shell first tells the Source Code Handler to show you the available files.

    ```c++
    // --- From ide.h (inside case 'O') ---
    source src; // Create a Source Code Handler object
    // ...
    src.list_dir(); // Ask it to list files in the 'source' directory
    ```

    The `list_dir` function looks inside the `source` folder:

    ```c++
    // --- Simplified from source.h ---
    bool source::list_dir(){
        cout<<"\nsource codes\n````````````\n";
        DIR *dir; // Represents the directory
        struct dirent *ent; // Represents an entry (file/folder) in the directory

        if ((dir = opendir ("source")) != NULL) { // Try to open the 'source' directory
            while ((ent = readdir (dir)) != NULL) { // Read each entry
                string f_name = ent->d_name; // Get the entry's name
                // Check if the filename ends with '.hyp' (simplified check)
                if(f_name.length() > 4 && f_name.substr(f_name.length() - 4) == ".hyp") {
                    cout << f_name << "\t"; // Print the filename
                }
            }
            closedir (dir); // Close the directory
        } else {
            cout << "Error opening source directory!" << endl;
        }
        cout<<"\n\nEnter a name ... to open OR ... create one" << endl;
        return true;
    }
    ```

    This shows you a list like `my_program.hyp other_code.hyp` and asks you to type a name.

2.  **Get Filename:** The IDE Shell waits for you to enter a filename. Let's say you type `my_program`.

    ```c++
    // --- From ide.h (inside case 'O') ---
    cin >> src.filename; // Read the name you typed into the handler's filename variable
                         // (It automatically adds ".hyp" later)
    ```

3.  **Open External Editor:** The IDE Shell then asks the Source Code Handler to try and open this file in an external editor.

    ```c++
    // --- From ide.h (inside case 'O') ---
    src.open_editor(); // Ask the handler to launch an editor
    ```

    The `open_editor` function tries common editors:

    ```c++
    // --- Simplified from source.h ---
    bool source::open_editor(){
        string command1 = "subl \"source/" + filename + ".hyp\""; // Command for Sublime Text
        // Try running Sublime Text, if it fails (returns non-zero)...
        if(system(command1.c_str()) != 0) {
            string command2 = "notepad \"source/" + filename + ".hyp\""; // Command for Notepad
            system(command2.c_str()); // Try running Notepad
        }
        return true;
    }
    ```

    This attempts to launch Sublime Text, and if that doesn't work (maybe it's not installed), it tries Notepad. This lets you edit `source/my_program.hyp` in a familiar environment.

4.  **Read and Parse the File:** *After* you've potentially edited and saved the file in the external editor, the IDE Shell enters a loop where it reads, shows, and potentially compiles the code. The first step inside this loop is to read and parse the file content.

    ```c++
    // --- From ide.h (inside the FILE loop within case 'O') ---
    if(!src.open_file()){ // Try to read and parse the file
        src.new_file();   // If it failed (e.g., didn't exist), create it
    }
    ```

    The `open_file` function does the heavy lifting of reading `source/my_program.hyp` and turning it into the `lol` structure.

    ```c++
    // --- Simplified logic of source::open_file() ---
    // 1. Open the file "source/filename.hyp". If it fails, return false.
    // 2. Read the entire file content into one big string 's'.
    // 3. Prepare an empty outer list: list<list<string>> lol;
    // 4. Prepare an empty inner list 'l' for the current line. Add line number "1".
    // 5. Loop through the big string 's' character by character:
    //    a. If it's part of a word, add it to a temporary 'word' string.
    //    b. Handle quoted strings ("...") carefully.
    //    c. If it's a space, comma, or end-of-line (';' or ':') and 'word' is not empty:
    //          Add 'word' to the inner list 'l'. Reset 'word'.
    //    d. If it's an end-of-line (';' or ':'):
    //          Add the completed inner list 'l' to the outer list 'lol'.
    //          Clear the inner list 'l'.
    //          Increment line number. Add new line number (as string) to 'l'.
    // 6. After the loop, if 'l' has content, add it to 'lol'.
    // 7. Return true (success).
    ```

    **Example:** If `my_program.hyp` contains:
    `VAR X: 10; PRINT X;`

    The resulting `lol` structure would look something like this (visualized):
    ```
    [
        ["1", "VAR", "X", "10"],  // Line 1: ["LineNo", "Word1", "Word2", "Word3"]
        ["2", "PRINT", "X"]       // Line 2: ["LineNo", "Word1", "Word2"]
    ]
    ```
    This structured `lol` is stored within the `src` object.

5.  **Show Parsed Code:** The IDE Shell then asks the Source Code Handler to display this structured code.

    ```c++
    // --- From ide.h (inside the FILE loop) ---
    system("clear"); // Clear screen first
    src.show_file();  // Ask the handler to display the parsed code
    ```

    The `show_file` function iterates through the `lol` structure:

    ```c++
    // --- Simplified from source.h ---
    bool source::show_file(){
        cout << filename << ".hyp\n````````````\n"; // Show filename header
        // Tell user where the actual file is
        cout << "Code as understood by Hyperbole (from source/" << filename << ".hyp):\n";

        // Iterate through the outer list (each element is a line list)
        for (list<list<string> >::iterator itr = lol.begin(); itr != lol.end(); itr++){
            list<string> current_line = *itr; // Get the inner list for the current line
            list<string>::iterator it = current_line.begin(); // Iterator for words in the line

            cout << *it << "\t"; // Print line number (first element) and a tab
            it++; // Move past the line number

            // Iterate through the rest of the words in the inner list
            for (; it != current_line.end(); it++){
                cout << *it << " "; // Print each word followed by a space
            }
            cout << ";" << endl; // Add a semicolon and newline for display
        }
        cout << "\nOptions: 'R'efresh, 'C'ompile, 'Q'uit file view" << endl;
        return true;
    }
    ```
    This prints the structured code neatly in the terminal, showing the line numbers and the separated words (tokens) that Hyperbole recognized.

## Under the Hood: Parsing into `lol`

The most crucial part is `open_file` where the raw text becomes the structured `lol`. Let's visualize the flow when `open_file` is called:

```mermaid
sequenceDiagram
    participant IDE as IDE Shell (ide.h)
    participant SH as Source Code Handler (source.h)
    participant FS as File System
    participant LOL as `lol` (List of Lists)

    IDE->>SH: Call open_file() for "my_program.hyp"
    SH->>FS: Open "source/my_program.hyp"
    FS-->>SH: Return file content ("VAR X: 10; PRINT X;")
    SH->>SH: Initialize empty LOL
    SH->>SH: Start parsing content character by character
    Note over SH: Sees 'V', 'A', 'R', adds to word="VAR"
    Note over SH: Sees ' ', adds word="VAR" to current line list ["1", "VAR"]
    Note over SH: Sees 'X', adds to word="X"
    Note over SH: Sees ':', adds word="X" to line list ["1", "VAR", "X"]
    Note over SH: Sees ' ', skips
    Note over SH: Sees '1', '0', adds to word="10"
    Note over SH: Sees ';', adds word="10" to line list ["1", "VAR", "X", "10"]
    SH->>LOL: Add completed line ["1", "VAR", "X", "10"] to LOL
    SH->>SH: Start new line list ["2"]
    Note over SH: Sees ' ', skips
    Note over SH: Sees 'P','R','I','N','T', adds to word="PRINT"
    Note over SH: Sees ' ', adds word="PRINT" to line list ["2", "PRINT"]
    Note over SH: Sees 'X', adds to word="X"
    Note over SH: Sees ';', adds word="X" to line list ["2", "PRINT", "X"]
    SH->>LOL: Add completed line ["2", "PRINT", "X"] to LOL
    SH-->>IDE: Return true (success)
```

The code achieves this using loops and string manipulation. It keeps track of the current `word` being built. When it hits a separator (space, comma, colon, semicolon), the completed `word` is added to the current line's list (`l`). When it hits a line terminator (semicolon or colon), the entire line list (`l`) is added to the main list-of-lists (`lol`), and a new line list is started with the incremented line number.

```c++
// --- Snippet from source::open_file() focusing on parsing ---
list<string> l;          // Inner list for the current line
string word = "";        // Temporary string to build words
int lineNo = 1;          // Current line number
algo al;                 // Helper object (from algo.h)
l.push_back(al.int_to_string(lineNo)); // Add line number "1" to first line

// Assume 's' contains the full file content "VAR X: 10; PRINT X;"
for (unsigned int i = 0; i < s.length(); i++) {
    // If it's not a separator and we are not inside a string literal...
    if (s[i] != ' ' && s[i] != ';' && s[i] != ',' && s[i] != ':') { // Simplified logic
        word += s[i]; // Append character to word
    } else {
        // It IS a separator. If we have a built word, add it to the line list.
        if (word != "") {
            l.push_back(word);
            word = ""; // Reset word
        }
    }

    // If it's a line ending character...
    if (s[i] == ';' || s[i] == ':') {
        lol.push_back(l); // Add the completed line list to the main list
        l.erase(l.begin(), l.end()); // Clear the line list for the next line
        lineNo++; // Increment line number
        l.push_back(al.int_to_string(lineNo)); // Add new line number to list
        // Skip potential extra spaces after ; or :
        while (i + 1 < s.length() && !isalnum(s[i+1])) {
             i++;
        }
    }
}
// Add the last line if file doesn't end with ; or : (simplified - actual code might need adjustment)
if (!l.empty() && l.size() > 1) { // Check if last line has more than just the line number
    lol.push_back(l);
}
```
This parsing logic turns the free-form text of your `.hyp` file into the very organized `lol` structure, ready for the next step. Notice the use of `algo al;` and `al.int_to_string(lineNo)`. This uses a helper function from the [Language Logic Unit (ALGO)](05_language_logic_unit__algo_.md) to convert the integer line number into a string before adding it to the list.

## Conclusion

The **Source Code Handler** acts as Hyperbole's file management expert. It finds your `.hyp` files, helps you open them in an external editor, reads their content, and most importantly, *parses* that content into a structured format (`lol` - a list of lists of strings) that represents your code line by line, word by word. It also displays this parsed structure back to you.

This `lol` structure is the crucial handover product. It's like the librarian giving you a neatly organized summary of the book's chapters and key sentences, rather than just the raw book itself. This makes it much easier for the next part of the Hyperbole factory, the [Execution Engine](04_execution_engine.md), to understand and run your code.

But before we get to running code, what if you need help understanding Hyperbole's commands or how the IDE works? The IDE Shell has another option for that: 'H' for Help. Let's see how that works next!

Onwards to [Chapter 3: The Help System](03_help_system.md)!

---

Generated by [AI Codebase Knowledge Builder](https://github.com/The-Pocket/Tutorial-Codebase-Knowledge)