#!/bin/bash

valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp ./minishell


#bash ./check_leaks.sh