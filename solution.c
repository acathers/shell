#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char **parse_command(char *input);

/*
 * A function that executes a command using fork and execv
 */
void execute_plain(char *cmd, char *const argv[]) {
  int status;
  pid_t pid = fork();
  if (pid == 0) {  // child
    execv(cmd, argv);
    fprintf(stdout, "\t** Command failed **\n");
  } else if (pid > 0) {  // parent
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      fprintf(stdout, "** Command successful **\n");
    }
  } else {
    fprintf(stdout, "** Command failed **\n");
  }
}

/*
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to a file
 */

void execute_output_to_file(char *cmd, char *const argv[], char *filename) {
  int status;
  pid_t pid = fork();
  if (pid == 0) {  // child
    int original_stdout = dup(STDOUT_FILENO);
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execv(cmd, argv);
    dup2(original_stdout, STDOUT_FILENO);
    fprintf(stdout, "\t** Command failed **\n");
  } else if (pid > 0) {  // parent
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      fprintf(stdout, "** Command successful **\n");
    }
  } else {
    fprintf(stdout, "** Command failed **\n");
  }
}

/*
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to another command
 */
void execute_output_to_other(char *cmd1, char *const argv1[],
                             char *cmd2_with_argv2) {
  int fds[2];
  int status, status2;
  pipe(fds);

  pid_t pid1 = fork();
  if (!pid1) {  
    close(fds[1]);
    close(0);
    dup(fds[0]);

    char **command = parse_command(cmd2_with_argv2);

    execv(command[0], command);
    perror("execv");
  }

  pid_t pid2 = fork();
  if (!pid2) {  
    close(fds[0]);
    close(1);
    dup(fds[1]);
    execv(cmd1, argv1);
    perror("execv");
  }

  waitpid(pid2, &status, 0);
  close(fds[1]);
  waitpid(pid1, &status2, 0);
      if (WIFEXITED(status) && WIFEXITED(status2)) {
      fprintf(stdout, "** Command successful **\n");
    }else {
      fprintf(stdout, "** Command failed **\n");
    }
}

char **parse_command(char *input) {
  char **command = malloc(8 * sizeof(char *));
  char *separator = " ";
  char *parsed;
  int index = 0;

  parsed = strtok(input, separator);
  while (parsed != NULL) {
    command[index] = parsed;
    index++;

    parsed = strtok(NULL, separator);
  }

  command[index] = NULL;
  return command;
}
