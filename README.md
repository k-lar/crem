# crem (c reminders)

A continuation of my `*rem` series of programs.  

This is my first real program written in C, and my goal was at first to just get it working,  
as I thought that was challenging enough, but as time went on I kept upping the portability  
aspect until I reached the ANSI/C89/C90 standard. I tried to make it as portable as possible  
but do keep in mind, this is my first C program, there are bound to be horrendous bugs  
sprinkled in. In all of my testing at the end, I haven't been able to crash it. I'm sure that  
someone more experienced can tear all of my code apart and explain all the undefined behavior  
that I haven't been able to catch.

## Features

- Adding / removing entries

## Installation

**To install from source:**
```console
# Clone git repo and go into directory
git clone https://gitlab.com/k_lar/crem; cd crem

# Install
sudo make install

# Uninstall
sudo make uninstall
```

## Usage

```console
# Adding entries:
crem -a "Super important note"

# Removing entries:
# This one removes entry [1]
crem -r 1

# This removes entries [2], [7] and [3]
crem -r 2,7,3

# Display entries:
crem --show

# You can chain commands like so:
crem -r 3,6 -rn -a "Meeting at 12:00" --show

# For additional commands
crem -h
```

Example:

```console
user@pc:~$ crem --show
################ Reminders ###############
[1] - Check weird noise in PC
[2] - Visit John sometime next week
[3] - Cool editor: helix-editor.com
[4] - Read "Crafting Interpreters"
[5] - Rewrite vim config in lua
[6] - Release crem 1.0
##########################################
```

Notes / reminders are stored in a separate file in `~/.config/crem/creminders` on Linux and Unix
systems and on Windows they should be in `C:/crem/creminders`

## Dependencies

No dependencies. It should be C89 compliant, and work everywhere

## Feedback

Any and all feedback is appreciated. If you find any problems, please open an issue.


