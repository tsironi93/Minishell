# Minishell

A minimal UNIX shell implementation in C that mimics bash functionality. This project implements core shell features including command execution, piping, redirections, environment variables, and built-in commands.

## Overview

Minishell is a teaching project that recreates essential shell mechanics:
- **Lexical analysis** (tokenization) and parsing of shell input
- **Command execution** with pipe support (`|`)
- **Input/Output redirections** (`<`, `>`, `>>`, `<<`)
- **Environment variable expansion** (`$VAR`)
- **Built-in commands** (echo, cd, pwd, export, unset, env, exit)
- **Signal handling** (SIGINT, SIGTSTP, SIGCHLD)
- **Garbage collection** wrapper for memory management
- **Readline integration** for interactive prompt with history

## Project Structure

```
Minishell/
├── minishell.h          # main header with structs and function prototypes
├── src/                 # source code
│   ├── main.c           # entry point, REPL loop
│   ├── init.c           # initialization of data structures
│   ├── lexer.c          # tokenization of input
│   ├── lexer_helpers.c  # quote/expansion helper functions
│   ├── expansion.c      # variable expansion ($VAR)
│   ├── expansion_utils.c
│   ├── classify.c       # token type classification
│   ├── syntax_errors.c  # syntax validation
│   ├── execution.c      # command execution logic
│   ├── execution_utils.c
│   ├── pipe.c           # pipeline (|) handling
│   ├── redirections.c   # I/O redirection (<, >, >>)
│   ├── heredoc.c        # here-document (<<) handling
│   ├── signals.c        # signal handlers
│   ├── echo.c           # echo builtin
│   ├── cd.c             # cd builtin
│   ├── pwd.c            # pwd builtin
│   ├── env.c            # env builtin
│   ├── export.c         # export builtin
│   ├── unset.c          # unset builtin
│   ├── exit.c           # exit builtin
│   ├── garbage_colector.c  # memory management / GC
│   ├── gc_wrappers.c    # malloc/strdup wrappers
│   ├── basic_linked.c   # environment linked list
│   ├── token_linked.c   # token linked list
│   ├── utils.c          # misc utilities
│   └── error_exit.c     # error handling
├── libft/               # custom C library (must be built)
└── Makefile             # build rules

```

## Key Data Structures

### `t_token` — Tokenized input
```c
typedef struct s_tocken
{
    t_token_type	type;      // COMMAND, ARGS, PIPE, REDIRECT_OUT, etc.
    char			*value;    // token string value
    struct s_tocken	*next;
    struct s_tocken	*previous;
}	t_token;
```

### `t_data` — Global execution context
```c
typedef struct s_data
{
    t_token		*tokens;          // linked list of tokens
    char		**buildins;       // builtin command names
    t_gc		gc;               // garbage collector
    char		*input;           // raw input
    char		**env_full;       // environment as 2D array
    char		**env_cmd_paths;  // PATH directories
    t_env		*env;             // environment as linked list
    int			exit_code;        // last exit status
    t_heredoc	*heredoc;         // heredoc delimiters/fds
}	t_data;
```

### `t_gc` — Garbage Collector
```c
typedef struct s_gc
{
    t_gcobj	*objects;       // tracked allocations
    void	**roots[100];   // root pointers for mark-and-sweep
    int		root_count;
}	t_gc;
```

## Requirements

- **Linux** (tested on Debian/Ubuntu)
- **GCC** or Clang
- **GNU Readline** library (`libreadline-dev` on Ubuntu/Debian)
- **Make**
- **Custom libft** (provided in `libft/` folder)

### Install dependencies (Debian/Ubuntu):
```bash
sudo apt-get install -y build-essential libreadline-dev
```

## Build

Build minishell:
```bash
make
```

## Usage

Run the interactive shell:
```bash
./minishell
```

You'll see a prompt (by default `minishell$`). Type commands as in bash:

### Basic Commands
```bash
minishell$ echo hello world
hello world

minishell$ pwd
/home/user/Minishell

minishell$ ls -la
# ... file listing ...

minishell$ whoami
user
```

### Pipes
```bash
minishell$ echo "apple banana cherry" | grep banana
banana
```

### Redirections
```bash
# Output redirect
minishell$ echo "hello" > output.txt

# Append
minishell$ echo "world" >> output.txt

# Input redirect
minishell$ cat < output.txt
hello
world

# Here-document
minishell$ cat << EOF
> line 1
> line 2
> EOF
line 1
line 2
```

### Environment Variables
```bash
minishell$ export MY_VAR=hello
minishell$ echo $MY_VAR
hello

minishell$ MY_VAR=world ./some_command
# MY_VAR set to "world" only for this command
```

### Built-in Commands

#### `echo [args...]`
Print arguments. Supports `-n` flag (no newline).
```bash
minishell$ echo -n "hello"
hello
```

#### `cd [path]`
Change directory. Supports `.`, `..`, `~`, and absolute/relative paths.
```bash
minishell$ cd /tmp
minishell$ pwd
/tmp
```

#### `pwd`
Print working directory.
```bash
minishell$ pwd
/home/user/Minishell
```

#### `env`
Display environment variables.
```bash
minishell$ env | head
PATH=/usr/local/bin:/usr/bin:...
HOME=/home/user
...
```

#### `export VAR=value`
Set an environment variable.
```bash
minishell$ export USER_NAME=alice
minishell$ echo $USER_NAME
alice
```

#### `unset VAR`
Remove an environment variable.
```bash
minishell$ unset USER_NAME
minishell$ echo $USER_NAME
# (empty)
```

#### `exit [code]`
Exit the shell with optional exit code.
```bash
minishell$ exit 42
# shell exits with code 42
```

## Signal Handling

- **SIGINT** (Ctrl+C) — terminates current foreground command, returns to prompt
- **SIGTSTP** (Ctrl+Z) — pauses current command (if supported)
- **SIGCHLD** — handles child process termination

## Lexer & Parsing Pipeline

1. **Lexer** (`lexer.c`) — tokenizes raw input into a linked list of tokens
2. **Quote/Variable Expansion** (`expansion.c`) — expands `$VAR` and processes quotes
3. **Token Classification** (`classify.c`) — marks each token (COMMAND, ARGS, PIPE, REDIRECT_OUT, etc.)
4. **Syntax Validation** (`syntax_errors.c`) — checks for pipe/redirect syntax errors
5. **Execution** (`execution.c`) — parses and executes the token stream

## Execution Flow

1. User enters command at prompt
2. Lexer tokenizes input
3. Variable expansion and quote processing
4. Token classification and merge
5. For each command:
   - Check for redirections/here-docs
   - If builtin → execute in-process
   - If external → search PATH, fork, and exec
6. Handle pipes by creating child processes connected via pipes
7. Wait for child processes and update exit code

## Example: Complex Command

```bash
minishell$ cat file1.txt | grep "pattern" > output.txt 2>&1
```

**Execution:**
1. Lexer tokenizes: `cat`, `file1.txt`, `|`, `grep`, `"pattern"`, `>`, `output.txt`, `2>&1`
2. Parser detects 1 pipe → creates 2 processes
3. Process 1: `cat file1.txt` → stdout to pipe
4. Process 2: `grep "pattern"` → stdin from pipe, stdout to `output.txt`
5. Main shell waits for both to finish

## Garbage Collection

The GC wrapper tracks all allocations via `gc_malloc()`, `gc_strdup()`, etc. On exit or error, `gc_destroy()` frees all tracked memory automatically. This prevents memory leaks in the shell loop.

```c
char *str = gc_strdup(&data->gc, "hello");  // tracked
// ... use str ...
gc_destroy(&data->gc);  // frees all at once
```

## Testing

Run the shell and try:
```bash
./minishell
minishell$ echo $HOME
/home/user
minishell$ ls -la | grep minishell
minishell$ export TEST=value && echo $TEST
value
minishell$ exit
```

## Notes & Future Enhancements

- Processes run sequentially; background jobs (`&`) not yet implemented
- Job control (fg/bg) not implemented
- History persists only during session (readline library handles this)

## Author

Built as a school project (42 Heilbronn). Collaborator(s) Chiara Kappe.
