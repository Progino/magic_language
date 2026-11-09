# Magic Language

##  Start Guide
To run tour script you have to download the zip and extract it( in the same folder).
Then you write yuor magical code in the txt file, and then SAVE IT.
After that you can open the exe file that will run your code.

##  General Syntax & Rules
For comment you have to write '^' and then your comment.
Inside the output command, use the exclamation mark '!' to chain text strings and variable values together.
Every command must be written in lowercase.

## Functions

### 1. Memory & Variable Management
'summon variable = value' declares or update a variaable, you can also make basics operstions 'summon variable = value1 operator value2.
You can also generate a random number between two values with chaos 'summon variable = chaos min max'.
'banish variable' erases a variable.
'infuse/drain value' add or subtracts 1 to the value.
'alchemy variable1 variable2' swaps two variables

### 2. Input / Output
'reveal "text" ! variable ! "text"' prints strings and variables.
'invoke variable' ask the user to insert the value of a variable.

### 3. Grimmoires (Array/Vector Structures)
'grimmoire vectorName size' declare a vector.
'scribe vectorName position value' write a value in a specific position.
'extract vectorName position variable' read the value in that position and saves it in the variable.

### 4. Control Flow Blocks
All condition blocks must have at the end the keyword 'end'
Logical operators: bound(==), unbound(!=), weaker(<), weakerAs(<=), stronger(>), strongerAs(>=).
'prophecy value1 logicalOperator value2' check for example if value1 is bigger than value 2.
'echo value1 logicalOperator value' is a for loop that repeats as many times it is value1.
'ritual value1 logicalOperator value2' is a while loop that repeats until the logical operation is false.

### 5. Error Handling Matrix (Fault Protection)
before an operation that can fail you can write 'ward' then if it fails it goes to the line where you wrote 'curse'.
there it execute the code until the keyword 'end'.
If the operation inside ward doesn't fail it skips the curse block.

##  Full Script Example

Paste this code into 'magic_language.txt'.

```text
summon health = 100
summon void = 0

reveal "Attempting to run safe initialization..."
summon health = health - 10
reveal "Current health status:" ! health

ward
    reveal "Attempting to divide by zero"
    summon health = health / void ^ This division error will trigger the fallback!
curse
    reveal " CRITICAL ERROR"
    summon health = 50
end

reveal "Final health:" ! health

```
code that shows a echo(for) loop:

```text
summon energy = 5
reveal "start energy: " ! energy

reveal "starting the loop"
echo 5
    reveal "energy remained: " ! energy
    drain energy
end
reveal "loop finished"
```




