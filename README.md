# Game of Life on C
Project was made for one of my study projects. It was necessary to make it from the minimum number of tools and libraries, so the code is pretty simple.

**Controls:**

***+ / -*** for speed control, ***Q*** to exit from the game.

The input data is a set of zeros and ones with a size of 25x80. You can submit data by pasting text in the terminal or by redirecting input.

      
  How to compile:
      
  > gcc game_of_life.c -lncurses -o game_of_life.o
      
  > ./game_of_life.o < test1.txt
      
  You may need to download ncurses packages.
