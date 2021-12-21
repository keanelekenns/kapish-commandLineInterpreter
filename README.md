#### Keanelek Enns
#### V00875807
#### UVIC CSC 360 Assignment 1
#### Due Jan. 31, 2019


I received some inspiration from the tutorial posted at https://brennan.io/2015/01/16/write-a-shell-in-c/.

I also conversed with a few peers, namely Nat Comeau, Louie Kedziora, and Dana Wiltsie. No code was shared between us.

To run this code, one must call make, then ./kapish (it is also possible to call make kapish.o and run ./kapish.o but the user must change its permissions).

All commands must be the very first argument given (just as with any other typical shell); this includes ctrl+d which behaves like exit.
If a user attempts to exit with ctrl+d after typing arguments, it is not likely to work (so far during testing it has
required pressing ctrl+d twice). UPDATE: I was told by TA that we don't need to handle the case where users type things before pressing ctrl+d.

Note that for the !commandprefix functionality, adding arguments after the first !command arg will not modify the previous commands
(I believe this is what the specification meant when it said "kapish need not support editing of the previously issued command"). Also,
simply typing "!" and enter executes the last command.

When killing a child process using ctrl+c, you must be in the kapish terminal (with the cursor blinking waiting for input).

NOTE: My makefile was not working when trying to use multiple c files and write it in the format given (separate build types for kapish.o and kapish)
I spent hours trying to figure it out and could not make anything work.
So I have included all of the contents of my node and doublylinkedlist files
in my kapish.c file. I hope that this will not affect any style points. I
made it clear where the codes start and end.

Running valgrind on kapish resulted in no possible memory leaks.

Finally: I do not know how many bonus marks history is worth, but seeing as it was the most difficult part of the assignment,
it would make sense to make it worth at least all the other built-in commands, but maybe I am the only one that thinks that. 

Thank you to the markers for taking time to review and test the code!!