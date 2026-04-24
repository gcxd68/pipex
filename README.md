*This project has been created as part of the 42 curriculum by gdosch.*

# Pipex

## Description
Pipex is a project that explores in detail the UNIX mechanisms of process creation, pipeline redirection, and file descriptors handling. The main goal of this project is to simulate the behavior of the shell pipeline command: `< infile cmd1 | cmd2 > outfile`.

By implementing this project, we learn how to correctly use system calls like `fork()`, `pipe()`, `dup2()`, and `execve()`, while managing memory perfectly and preventing file descriptor leaks or zombie processes.

**Features:**
- **Mandatory**: Simulates `< file1 cmd1 | cmd2 > file2`.
- **Bonus**:
  - Handles multiple pipes sequentially: `< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2`.
  - Supports `<<` and `>>` behavior when the first parameter is `here_doc`.

## Instructions

### Compilation
A `Makefile` is provided to compile the project.
- Compile the mandatory executable:
  ```bash
  make
  ```
- Compile the bonus executable (which includes multiple pipes and `here_doc`):
  ```bash
  make bonus
  ```
- Clean object files: `make clean`
- Clean all generated files including the executable: `make fclean`
- Clean and recompile: `make re`

### Execution
**Mandatory Setup:**
```bash
./pipex <infile> "<cmd1>" "<cmd2>" <outfile>
```
*Example:* `./pipex infile "ls -l" "wc -l" outfile` 
*(behaves exactly like: `< infile ls -l | wc -l > outfile`)*

**Bonus - Multiple Pipes:**
```bash
./pipex <infile> "<cmd1>" "<cmd2>" "<cmd3>" ... "<cmdn>" <outfile>
```

**Bonus - Here Document:**
```bash
./pipex here_doc <LIMITER> "<cmd1>" "<cmd2>" <outfile>
```
*Example:* `./pipex here_doc END "grep a1" "wc -w" outfile` 
*(behaves exactly like: `grep a1 << END | wc -w >> outfile`)*

## Resources

### Documentation & Tutorials
- [Pipe manual page](https://linux.die.net/man/2/pipe)
- [Fork manual page](https://linux.die.net/man/2/fork)
- [Dup2 manual page](https://linux.die.net/man/2/dup2)
- [Waitpid manual page](https://linux.die.net/man/2/waitpid)
- [Execve manual page](https://linux.die.net/man/2/execve)
- [Unix Processes in C (CodeVault)](https://www.youtube.com/watch?v=cex9XrZCU14&list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY) - A highly recommended video series explaining UNIX multi-processing.

### AI Usage
- **Explanation and Research:** AI tools were used to query explanations on obscure edge cases found in the manual pages (for instance, the precise behavior of `dup2` when descriptors are the same, and pipeline race conditions).
- **Debugging & Profiling:** AI helped interpret complex `valgrind` reports relating to file descriptor leaks during the development of the multiple-pipes bonus.
- **Documentation:** An AI assistant was used to draft and format the Markdown structure of this README file based on the project instructions.
