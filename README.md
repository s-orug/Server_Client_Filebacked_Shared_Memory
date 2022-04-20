# README

## Objective

The objective of this project is have the server to load a text file into file-backed shared memory which allows the client to change any lower case characters to upper case.

## Included Files

The project includes the following files:

- makefile
- text-server.c
- text-client.c
- text-memory.c
- README.md

## Makefile

When `make` is executed in the terminal. The make file executes two commands.

`gcc -o text-client text-client.c`

`gcc -o text-server text-server.c`

When `make clean` is executed in the terminal. The make file removes any executable or intermediary build files created by `make`. It is done using the following command in bash.

`rm -rf text-client text-server *.o`

## Server

To run the server a command in the following format must be entered.

`./text-server`

The server will start and display that it started.

Everytime a client connects:

- The server will log the client's request, memory segment opening, filename, file closing, and memory closing.
- If the server is successfully able to open the file, the it will send a signal to the client to make changes.

## Client

A client can be started by entering a command in the following format.

`./text-client dat/dante.txt`

The client will start, parse text using four threads, and replace any lower-case characters with its upper-case version.

Everytime a client connects:

- The client will send a filename to the server.
- After the server is able to open the file and load the file into file-backed shared memory, the client starts the conversion process using threads.
- The client uses 4 threads.

## Header Files

For this project, `text-memory.h` is created primarily to contain any used libraries and globally defined values.