#!/bin/bash
source colors.sh

rank=$1
level=$2

# Save base directory (where script was launched from)
base_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Centralized temp file to track subject
subject_file="/tmp/.current_subject_${rank}_${level}"

# Define subject pool using case statement instead of associative array
get_subjects() {
    case "$level" in
        level1)
            echo "broken_gnl filter"
            ;;
        level2)
            echo "n_queens permutations powerset rip tsp"
            ;;
        *)
            echo ""
            ;;
    esac
}

pick_new_subject() {
    subjects_list=$(get_subjects)
    IFS=' ' read -r -a qsub <<< "$subjects_list"
    count=${#qsub[@]}
    random_index=$(( RANDOM % count ))
    chosen="${qsub[$random_index]}"
    echo "$chosen" > "$subject_file"
}

prepare_subject() {
    mkdir -p "$base_dir/../../rendu/$chosen"
    
    # Create appropriate files based on subject requirements
    case $chosen in
        "broken_gnl")
            [ ! -f "$base_dir/../../rendu/$chosen/get_next_line.c" ] && \
                cp "$base_dir/../$rank/$level/broken_gnl/broken_gnl.c" "$base_dir/../../rendu/$chosen/get_next_line.c"
            [ ! -f "$base_dir/../../rendu/$chosen/get_next_line.h" ] && \
                cp "$base_dir/../$rank/$level/broken_gnl/get_next_line.h" "$base_dir/../../rendu/$chosen/get_next_line.h"
            ;;
        "filter")
            touch "$base_dir/../../rendu/$chosen/filter.c"
            ;;
        *)
            # For other subjects, create generic .c file
            touch "$base_dir/../../rendu/$chosen/$chosen.c"
            ;;
    esac

    cd "$base_dir/../$rank/$level/$chosen" || {
        echo -e "${RED}Subject folder not found.${RESET}"
        exit 1
    }

    clear
    echo -e "${CYAN}${BOLD}Your subject: $chosen${RESET}"
    echo "=================================================="
    cat sub.txt
    echo
    echo -e "=================================================="
    echo -e "${YELLOW}Type 'test' to test your code, 'solution' to see the answer, 'next' to get a new question, or 'exit' to quit.${RESET}"
}

# Initial subject selection
if [ -f "$subject_file" ]; then
    chosen=$(cat "$subject_file")
    echo -e "${BLUE}🔁 Resuming with previously chosen subject: $chosen${RESET}"
else
    pick_new_subject
    chosen=$(cat "$subject_file")
    echo -e "${GREEN}🎯 New subject chosen: $chosen${RESET}"
fi

prepare_subject

# Interactive loop
while true; do
    echo -e "${MAGENTA}${BOLD}Enter command: ${RESET}"
    read command

    case $command in
        test)
            if [ -f "tester.sh" ]; then
                echo -e "${BLUE}Running tester (Max 10s)...${RESET}"
                bash tester.sh &
                test_pid=$!
                counter=0
                while [ $counter -lt 10 ] && kill -0 $test_pid 2>/dev/null; do
                    sleep 1
                    ((counter++))
                done

                if kill -0 $test_pid 2>/dev/null; then
                    echo -e "${RED}${BOLD}TIMEOUT!${RESET} Your code might have an infinite loop."
                    kill -9 $test_pid 2>/dev/null
                    # Kill child processes (the compiled program)
                    pkill -P $test_pid 2>/dev/null
                else
                    echo -e "${CYAN}Test completed.${RESET}"
                fi
            else
                echo -e "${YELLOW}No tester available for this subject.${RESET}"
            fi
            ;;
        solution)
            echo -e "${GREEN}--- Correct Solution for $chosen ---${RESET}"
            case $chosen in
                "broken_gnl")
                    cat "$base_dir/../$rank/$level/$chosen/get_next_line.c"
                    ;;
                "filter")
                    cat "$base_dir/../$rank/$level/$chosen/filter.c"
                    ;;
                *)
                    echo -e "${YELLOW}No specific solution file found for $chosen.${RESET}"
                    ;;
            esac
            echo -e "${GREEN}-------------------------------------${RESET}"
            ;;
        next)
            pick_new_subject
            chosen=$(cat "$subject_file")
            echo -e "${GREEN}🎯 New subject chosen: $chosen${RESET}"
            prepare_subject
            ;;
        exit)
            echo -e "${RED}Exiting exam mode...${RESET}"
            rm -f "$subject_file"
            cd "$base_dir"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown command. Use 'test', 'next', or 'exit'.${RESET}"
            ;;
    esac
done