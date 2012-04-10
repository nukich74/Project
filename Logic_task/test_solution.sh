#!/bin/bash
echo "Enter amount of functions to generate"
read param1
echo "Compiling program..."
g++ main4.cpp -O0 -ggdb -Wall -Wextra -Werror -o solve.out -std=gnu++0x
echo "Compiling generator..."
g++ gen.cpp -O0 -ggdb -Wall -Wextra -Werror -o gen.out -std=gnu++0x
echo "Compiling checker"
g++ check.cpp -O0 -ggdb -Wall -Wextra -Werror -o check.out -std=gnu++0x
echo "Generating tests..."
./gen.out input.txt $param1
echo "Current test:"
cat input.txt
echo "Result:"
./solve.out input.txt output.txt
cat output.txt
echo "Checking..."
./check.out input.txt output.txt
exit 0
