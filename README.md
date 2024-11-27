# Minishell_42



 `➜ minishell$ cat <`
zsh: segmentation fault (core dumped)  ./minishell


on a regle le pb pour sleep 100 et pour es ^C tout court. on affiche bien exit 130. 
 tout va bien.
 Par contre il faut gerer les control C au sein du heredoc. `yet to fix`

 ➜ minishell$ ls `fixed`
libft  Makefile  minishell  minishell_tester  obj  README.md  src
 ➜ minishell$ ^C
 ➜ minishell$ echo $?
0

c1b2c1% ./minishell `fixed`
 ➜ minishell$ ^C
 ➜ minishell$ echo $?

 // pathfinder signals executer heredoc minishell
