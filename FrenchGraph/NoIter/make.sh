#!/bin/bash
echo "Compiling project:"
g++ main.cpp -O0 -ggdb -Wall -Wextra -Werror -o solve.out -std=gnu++0x
g++ checker.cpp -O0 -ggdb -Wall -Wextra -Werror -o check.out -std=gnu++0x
echo "Processing ten tests from /test/:"
for ((id = 1; id <= 10; id++))
  do 	
  	echo "/test/Input0${id}.txt testing result:"
 	./solve.out "./test/Input0${id}.txt" "./test/Output0${id}.txt" 
  	./check.out "./test/Output0${id}.txt" "./test/Answer0${id}.txt"
  done
exit 0
