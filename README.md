# Simple Shell
The **Simple Shell** project is a C implementation of a basic UNIX command line interpreter.
It was developed as part of the Holberton School curriculum to understand how shells

## Features
-Displays a **prompt** in interactive mode.
-Reads and parses command lines.
-Executes external programs using `fork` and `execve`.
-Searches for executables in directories defined by the `PATH` environment variable.
-Handles errors when a command is not found (returns exit status 127).
-Implements the following built-in commands:
- 'exit' -> exits the shell with the status of the last executed command.
- 'env' -> prints the current environment variables.

## Usage
### Interactive mode
```bash
./hsh