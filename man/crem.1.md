---
title: crem
section: 1
header: User Manual
footer: crem 0.1.0
date: April 05, 2023
---

# NAME

crem - C reminders

# SYNOPSIS

**crem** [*OPTION*]...

# DESCRIPTION

crem is a basic notes/reminders management program that is primarily meant to be
run at the end of your .bashrc file.  

# OPTIONS

**-h, `--`help**
: display help message

**-a**
: add an entry inside the reminders file

**-r**
: remove an entry inside the reminders file. If there are multiple entries with the same number,
it will remove the first instance.

**-R**
: remove the reminders file (by default: $HOME/.config/crem/creminders)

**`--`show**
: prints all reminders on screen

**`--`version**
: prints installed crem version.

# EXAMPLES

`crem -a "Go to the store!"`
: will add en entry with the content "Go to the store!"

`crem -r 3`
: will remove the entry with the number 3

`crem -r 1,2,3`
: will remove entries 1,2,3

`crem --show`
: will print the contents of $HOME/.config/crem/creminders to the terminal

`crem -a "Sell TV" -r 2 --show`
: will add an entry, delete the entry [2], renumber everything and print it to the terminal

Printed reminders will be in this format:  
```
################ Reminders ###############
[1] - This is an example
[2] - Another example
[3] - And another!
##########################################
```

# FILES

$HOME/.config/crem/creminders
: This file is used to store all the user's reminder entries.

# AUTHOR

Written by `K_Lar`.

# REPORTING BUGS

Report any bugs you might find here: <https://gitlab.com/k_lar/crem/-/issues>
