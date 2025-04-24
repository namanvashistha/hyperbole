# Tutorial: hyperbole

Hyperbole is a **simple programming environment** designed for its *own custom language* (`.hyp` files).
It acts like a basic **IDE (Integrated Development Environment)**, allowing users to *open* and *view* source code files, get *help* on language keywords, and **execute** the code using an *interpreter*.
The system includes components to handle **source code** (*parsing*), manage **variables** (*memory*), perform language **logic** (*operations*), and provide **help**.


**Source Repository:** [https://github.com/namanvashistha/hyperbole](https://github.com/namanvashistha/hyperbole)

```mermaid
flowchart TD
    A0["IDE Shell"]
    A1["Source Code Handler"]
    A2["Execution Engine"]
    A3["Language Logic Unit (ALGO)"]
    A4["Variable Store"]
    A5["Help System"]
    A0 -- "Manages source files" --> A1
    A0 -- "Displays help" --> A5
    A0 -- "Initiates execution" --> A2
    A1 -- "Uses utilities (string conv..." --> A3
    A2 -- "Delegates language tasks" --> A3
    A2 -- "Manages variable state (via..." --> A4
    A3 -- "Reads/Writes variables" --> A4
```

## Chapters

1. [IDE Shell](01_ide_shell.md)
2. [Source Code Handler](02_source_code_handler.md)
3. [Help System](03_help_system.md)
4. [Execution Engine](04_execution_engine.md)
5. [Language Logic Unit (ALGO)](05_language_logic_unit__algo_.md)
6. [Variable Store](06_variable_store.md)


---

