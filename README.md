# TABOO

Taboo is a game intended for at least 3 players.
One player is given a random word and the objective is to describe the word to other players without using a given set of words.
The first player to guess the word wins.

This repository is a network implementation of this game.
The application consists of two executable files - client and server.
Server is responsible for creating game rooms and accept connection requests from players.

# Compilation

To setup the cmake environment use:
```
cmake -S . -B build -DCMAKE_AUTOMOC=1
```

If you intent to use this repository for development, consider adding `-DCMAKE_EXPORT_COMPILE_COMMANDS=1` to the command above.
This will generate compile commands for `C/C++` lsp.

To build the project use:
```
(cd build && make)
```

# Running

After building, there are two binaries: `client` and `server` in `build/` directory.
They can be executed to setup the game.
