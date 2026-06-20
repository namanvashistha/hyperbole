# hyperbole

**Hyperbole** is a tiny IDE, compiler, and interpreter for its own toy language
(`.hyp` files), written in C++. It runs entirely in the terminal: open or create
a source file, edit it in your `$EDITOR`, see how Hyperbole parses it, and run it
— all from one menu-driven shell.

![hyperbole](https://i.imgur.com/Ct2yYvh.png)
![hyperbole](https://i.imgur.com/MiIQlXN.png)
![hyperbole](https://i.imgur.com/iAi9Mni.png)
![hyperbole](https://i.imgur.com/FOhvYTK.png)
![hyperbole](https://i.imgur.com/hDSPoCL.png)
![hyperbole](https://i.imgur.com/A6irx0I.png)

## Quick start

You need a C++ compiler (`g++` or `clang++`). From the project root:

```bash
g++ hyperbole.cpp -o hyperbole   # build
./hyperbole                      # run
```

Or run it in the browser via Docker (serves the CLI over a web terminal on
port 8080):

```bash
docker compose up --build
```

## Using the IDE

The welcome screen offers three choices:

| Key | Action                          |
|-----|---------------------------------|
| `O` | open or create a `.hyp` file    |
| `H` | help — browse language keywords |
| `E` | exit                            |

### Opening files

After pressing `O` you get an interactive list of the programs in `source/`.
Navigate it with the keyboard:

| Key            | Action                          |
|----------------|---------------------------------|
| `↑` / `↓` (or `k` / `j`) | move the selection     |
| `Enter`        | open the highlighted file       |
| `+ new file…`  | last entry — create a new file  |
| `Q` / `Esc`    | go back to the main menu        |

The selected file opens in your `$EDITOR` (falling back to `vim`/`vi`/`nano`,
then GUI editors). When you save and quit the editor, Hyperbole shows how it
parsed your code, where:

| Key | Action            |
|-----|-------------------|
| `R` | refresh from disk |
| `C` | compile & run     |
| `Q` | close the file    |

## The `.hyp` language

A program lives between `BEGIN:` and `END;`. Statements end with `;`. Core
keywords:

| Keyword   | Meaning                                            |
|-----------|----------------------------------------------------|
| `declare` | declare one or more variables                      |
| `display` | print a variable or string literal                 |
| `fetch`   | read input from the keyboard into a variable       |
| `nextln`  | move the cursor to the next line                    |
| `loop … loopit` | repeat a block while a condition holds       |
| `check … checkit` / `otherwise … leaveit` | conditional branches   |

Example (`source/first_n.hyp`) — print `1..n`:

```
BEGIN:
	declare i,n;
	i=1;
	display "Enter a number ";
	fetch n;
	loop i<=n:
		display i,nextln;
		i=i+1;
	loopit;
END;
```

More samples live in `source/` (`check_prime.hyp`, `pattern1.hyp`,
`pattern2.hyp`, `sum_first_n.hyp`).

## Architecture

The IDE is a set of header-only modules wired together by `hyperbole.cpp` →
`ide.h`:

| File         | Role                                                     |
|--------------|----------------------------------------------------------|
| `ide.h`      | the menu shell / main loop                                |
| `source.h`   | file picker, editor launch, parsing into tokens, display |
| `help.h`     | loads `keyword.txt` and shows keyword help               |
| `execute.h`  | interprets the parsed program                            |
| `algo.h`     | language logic & string/number helpers                   |
| `variable.h` | the variable (memory) store                              |
| `ui.h`       | terminal presentation helpers (colors, boxes, rules)     |

A deeper walkthrough is in [`docs/`](docs/):

1. [IDE Shell](docs/01_ide_shell.md)
2. [Source Code Handler](docs/02_source_code_handler.md)
3. [Help System](docs/03_help_system.md)
4. [Execution Engine](docs/04_execution_engine.md)
5. [Language Logic Unit (ALGO)](docs/05_language_logic_unit__algo_.md)
6. [Variable Store](docs/06_variable_store.md)

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for our code of conduct and the
process for submitting pull requests.

## Authors

- [@namanvashistha](https://github.com/namanvashistha) — **Naman Vashistha**, maintainer and developer

## License

Licensed under the MIT License — see [LICENSE](LICENSE) for details.
