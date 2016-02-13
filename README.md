<h1>RSHELL</hi>
===============

<h2>Introduction</h2>
-------------------
Rshell is an interactive program that mimics the functionality of a 
bash shell with linux commands. Some commands include ls, mkdir, echo, clear, rmdir
rm, chmod and running bash scripts.

<h3>Functionality</h3>
--------------------

This program takes input from the user in the main function. 
We also used the getlogin() and gethostname() functions to 
extract the user information for display. From there,
we pass the input into a string and we then check for the "#" symbol, which
represents a comment in this program. Anything after a comment is disregarded.
After checking for a comment, we create a new string comprising of everything that
came before the comment, and we then pass this into our parse function.

Our parse function first finds the length of the command that we are 
executing. 

<h4>Bugs</h4>
---------------
It might have bugs but we coded this code so we dont know... If it works, then
there are no bugs to report.
If it doesn't work, then...for a good time call (951) 313 1245
our code is beautiful and flawless just like my body --Jeet




