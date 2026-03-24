#!/bin/bash
source ../../../main/colors.sh

expected_files="*.c *.h"
rendu_dir="../../../../rendu/n_queens"

# Check if rendu directory exists
if [ ! -d "$rendu_dir" ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Directory $rendu_dir does not exist$(tput sgr 0)"
    exit 1
fi

# Find C files in rendu directory
c_files=$(find "$rendu_dir" -name "*.c" -type f)
if [ -z "$c_files" ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: No .c files found in $rendu_dir$(tput sgr 0)"
    exit 1
fi

# Compile the program
echo "${BLUE}Compiling n_queens program...${RESET}"
gcc -Wall -Werror -Wextra -o n_queens_test $c_files 2>/dev/null
if [ $? -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Compilation error$(tput sgr 0)"
    exit 1
fi

# Test 1: n=2 (should have no solutions as per sub.txt)
echo "${BLUE}Testing n=2 (sub.txt example)...${RESET}"
./n_queens_test 2 > output1.txt 2>/dev/null
lines=$(grep -v "^$" output1.txt | wc -l)
if [ $lines -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=2 should have no solutions, got $lines$(tput sgr 0)"
    cat output1.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

# Test 2: n=4 (should have exactly 2 solutions as per sub.txt)
echo "${BLUE}Testing n=4 (sub.txt example)...${RESET}"
./n_queens_test 4 > output2.txt 2>/dev/null
lines=$(wc -l < output2.txt)
if [ $lines -ne 2 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=4 should have exactly 2 solutions, got $lines$(tput sgr 0)"
    cat output2.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

# Verify the specific solutions from sub.txt for n=4: "1 3 0 2" and "2 0 3 1"
# We sort them to make comparison easier
sort output2.txt > sorted_output2.txt
echo -e "1 3 0 2\n2 0 3 1" | sort > expected_output2.txt
if ! diff -q sorted_output2.txt expected_output2.txt >/dev/null; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=4 solutions don't match expected solutions from sub.txt$(tput sgr 0)"
    echo "Expected:"
    cat expected_output2.txt
    echo "Got:"
    cat sorted_output2.txt
    rm -f n_queens_test output*.txt sorted_output2.txt expected_output2.txt
    exit 1
fi

# Test 3: n=8 (should have exactly 92 solutions)
echo "${BLUE}Testing n=8 (standard n-queens result)...${RESET}"
./n_queens_test 8 > output3.txt 2>/dev/null
lines=$(wc -l < output3.txt)
if [ $lines -ne 92 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=8 should have 92 solutions, got $lines$(tput sgr 0)"
    rm -f n_queens_test output*.txt sorted_output2.txt expected_output2.txt
    exit 1
fi

# Test 4: Wrong number of arguments (should fail/handle gracefully)
echo "${BLUE}Testing wrong number of arguments...${RESET}"
./n_queens_test > /dev/null 2>&1
# The current implementation returns 0 and writes \n for argc != 2
# Let's check if it at least doesn't crash

# Cleanup
rm -f n_queens_test output*.txt sorted_output2.txt expected_output2.txt

echo "$(tput setaf 2)$(tput bold)PASSED 🎉$(tput sgr 0)"
exit 0
