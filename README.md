<h1>RSHELL</hi>
===============

<h2>Introduction</h2>
-------------------
Rshell is an interactive program that mimics the functionality of a 
bash shell with linux commands. Some commands include ls, mkdir, echo, clear, rmdir
rm, chmod and running bash scripts.

<h2>Functionality</h2>
--------------------

This program takes input from the user in the main function. 
We also used the getlogin() and gethostname() functions to 
extract the user information for display. From there,
we pass the input into a string and we then check for the "#" symbol, which
represents a comment in this program. Anything after a comment is disregarded.
After checking for a comment, we create a new string comprising of everything that
came before the comment, and we then pass this into our parse function.

Our parse function works in the following manner:<\n>
    *removing Comments
    *checking for presence && or || 
    *implementing commands

<h2>Bugs</h2>
---------------
*Missing Functionality - && and ||
*Quotation marks are not supported by the echo command
*ls cannot access directory info on the first run on hammer-- only the first time though. the function worls properly whjen called the first time.
This behaviour is observed only on hammer. it works perfectly on cloud9, Mac OSX and Ubuntu.
*getlogin() doesn't work on cloud9. works on MACOSX hammer and Ubuntu.



