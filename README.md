# Magic Language

##  Start Guide

Follow these simple steps to run your first magical script:

1. **Download the Zip:** Download the project's repository archive (`.zip`).
2. **Extract Content:** Extract all the contents of the ZIP folder into a directory of your choice.
3. **Write Your Code:** Open the included text file named **`magic_language.txt`** and write your custom magical commands.
4. **Launch the Interpreter:** Double-click the **`magic_language.exe`** executable file. The application will instantly read your text file and display the live output inside the terminal!

>  **Important Note:** The `magic_language.exe` executable and the `magic_language.txt` script file **must always reside in the exact same folder** to communicate properly.

---

##  General Syntax & Rules

*   **Comments:** Use the `^` symbol. Anything written after `^` on that line will be completely ignored by the parser.
    ```text
    summon mana = 100 ^ This is a comment and will not be executed
    ```
*   **String Concatenation:** Inside the output command, use the exclamation mark `!` to chain text strings and variable values together.
*   **Case Sensitivity:** All magical keyword commands must be written in **lowercase** (e.g., `summon`, `ward`, `prophecy`).

---

##  Spellbook Reference (Available Functions)

### 1. Memory & Variable Management
*   `summon [variable] = [value]` : Declares a variable or updates its value (accepts integers or other variables).
*   `summon [variable] = [val1] [operator] [val2]` : Performs basic arithmetic operations (`+`, `-`, `*`, `/`).
*   `summon [variable] = chaos [min] [max]` : Generates a secure random integer within the specified boundary limits.
*   `banish [variable]` : Erases a variable completely from memory.
*   `infuse [variable]` : Increments a variable's value by 1 (`++`).
*   `drain [variable]` : Decrements a variable's value by 1 (`--`).
*   `alchemy [var1] [var2]` : Swaps the stored values of two existing variables instantly.

### 2. Input / Output
*   `reveal "[text]" ! [variable]` : Prints strings and variable contents to the console screen.
*   `invoke [variable]` : Prompts the user for manual keyboard input and stores it into the target variable.

### 3. Grimmoires (Array/Vector Structures)
*   `grimmoire [array_name] [size]` : Reserves a continuous integer array of the given size.
*   `scribe [array_name] [index] [value]` : Writes a specific value or variable into an array index slot.
*   `extract [array_name] [index] [target_variable]` : Reads a value from an index slot and copies it into a standard variable.

### 4. Control Flow Blocks
Every conditional structure block must be explicitly closed using the **`end`** keyword.

*   `prophecy [val1] [logical_operator] [val2]` : Acts as a standard `if` block statement. Executes internal lines only if true.
*   `echo [number/variable]` : Acts as a standard `for` loop. Repeats internal operations for the specified iteration count.
*   `ritual [val1] [logical_operator] [val2]` : Acts as a standard `while` loop. Continuously loops as long as the condition evaluates to true.

#### Valid Logical Operators:
*   `bound` (`==`) | `unbound` (`!=`) | `weaker` (`<`) | `weakerAs` (`<=`) | `stronger` (`>`) | `strongerAs` (`>=`)

### 5. Error Handling Matrix (Fault Protection)
*   `ward` ... `curse` ... `end` : Acts as a standard `try-catch` block. If an operation causes a fatal exception (such as a division by zero or an out-of-bounds array position) inside the protected `ward` area, execution halts immediately and jumps straight into the emergency lines beneath the `curse` fallback section. If the code executes cleanly, the `curse` instructions are entirely ignored.

---

##  Full Script Example

Paste this blueprint directly into your `magic_language.txt` to run a comprehensive functional evaluation:

```text
reveal "=================================================="
reveal "         MAGIC LANGUAGE DIAGNOSTIC                "
reveal "=================================================="

summon health = 100
summon void = 0

reveal "Attempting to run safe initialization..."
summon health = health - 10
reveal "Current health status:" ! health

ward
    reveal "Attempting to channel volatile chaotic energy..."
    summon health = health / void ^ This division error will trigger the fallback!
curse
    reveal " CRITICAL: Magic shield broken! Activating containment ward..."
    summon health = 50
end

reveal "Final health status after containment:" ! health
reveal "=================================================="
```
