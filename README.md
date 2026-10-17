# flashcard cli

A CLI tool for creating and reviewing flashcards!  
  
  
  
## Features
-Ways to create/delete sets and add/remove cards in the CLI  
-A command to review flashcards one by one with the option of shuffling or not shuffling them  
-A write mode where you're given a term/definition from a card in your set to write the definition/term that fits with it  
-An option to view all your sets/all the cards in a set  


## What I plan to add/change
-Support for Unicode characters  
-A way to easily write letters with accents  
-Much better UI  
-Breaking up the horrendous TerminalManager.cpp mega file into multiple smaller files   
-Instant reaction to where you don't have to click enter in cases where you're only typing one char into the CLI at once  
-A Spaced Repetition Algorithm (like SM-2 or a simple digital version of the Leitner system)  
-A better system for storing info on a set  


#### if you want to contribute
just write whatever you want into the code (whether it's something I plan to change or not), create a pull request, and then I'll review it and probably add it!

## How do you compile it?
First either click on "code" then "download ZIP" in GitHub, OR open a terminal, run "cd path/to/your/folder", then run "git clone [URL to this repo]" (the second option is assuming you have git)  
  
From there open a terminal, run "cd path/to/the/folder/you/downloaded/to", then run "g++ -g *.cpp -o flashcards" to compile it all.
