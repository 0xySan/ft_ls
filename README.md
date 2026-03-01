# ft_ls

ft_ls is a 42 Project where you recreate the famous ls command you mostly use of linux and mac.

## Features

We recreate it in the mandatory part only with the following flags :
- `-a` : Displays all files ( so basically displays all files that start with a '.' ),
- `-d` : List the directories themselves, not their contents.
- `-f` : Do not sort and add all ( so basically `-aU` ), also disables `--color`.
- `-g` : Does just like `-l` but without listing the owner
- `-G` : In a long listing format, does not display group names ( so basically like `-g`, but for groups, here you still need to add `-l` to see it's use. )
- `-h` : print sizes in human readable format
- `--si` : likewise of `-h`, but uses powers of 1000 not 1024
- `-l` : Display info in following format `permission links owner group date_of_modification name_of_file`.
- `-R` : Make it open folder to find files recursively,
- `-r` : Reverse the order of all files,
- `-s` : print size of each file in blocks
- `-t` : Sort files by the time they were lastly modified by ns,
- `-U` : do not sort; list entries in directory order, but can be nullified with another `-t` after it.
- `--width=COLS` : set output width to COLS. 0 mean no limits.
- `--help` : display an help message just like this.
- `--color` : enable colorized output, so like folders and executables, etc.. will have colors to be distinguable.

Even thought this subject can be done without doing the norminette I followed most of it to make the challenge more difficult. Except for bonuses as they can be pretty hard.

This project is a W.I.P.
