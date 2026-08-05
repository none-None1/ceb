# Chicken/MiniChicken ELVM backend (CEB)

This project was planned by the author on 2024/5/10. The author started working on it on 2026/7/14 and finished on 2026/8/5.

## Features

* 24 bits
* Output always starts with a line feed (as it's hard to generate empty strings in Chicken).
* Input characters are given in the format of three octals (e.g.: `101102` for `AB`) as Chicken has no alphabet.
* Actually generates [MiniChicken](https://esolangs.org/wiki/MiniChicken) code. Run the `mc2c.py` file to translate the result to standard [Chicken](https://esolangs.org/wiki/Chicken) if you want.
* Tested with the [ChickenCS](https://github.com/none-None1/ChickenCS) interpreter.

## How to build?

First, make sure you have ELVM in `\~/elvm`. Then run `./gen.sh`, which combines all the C files and header files into one C file: `chn.c`. Finally, add it to the source code of `elc` in your ELVM and recompile it.

The author hopes to get this backend added into the list of ELVM backends.

It can run the FizzBuzz program in the [ELVM repository](https://github.com/shinh/elvm) without difficulty!

## How it works

Chicken is self-modifying, and data at the bottom of the stack is easier to access than data on the top of the stack. 

Therefore, we can use commands to store data of static length and the read data for dynamic memory.



|Data\*|stack\&input|registers\*\*, stdin/stdout, built-in constants|/|jump table|dynamic memory|
|-|-|-|-|-|-|
|Code|/|header (138 commands)|code|jump table initializer|/|

* \* Data is what that part of that stack will hold on runtime.
* \*\* Besides the 6 registers in ELVM, CEB adds I (for reading user input) and M (for dynamic memory). There are also 2 temporary registers T1 and T2.

