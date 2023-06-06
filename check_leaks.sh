!/bin/bash

valgrind --leak-check=full --suppressions=./readline.supp ./minishell
