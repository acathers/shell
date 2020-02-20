#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* 
 * A function that executes a command using fork and execv
 */
void execute_plain(char *cmd, char *const argv[]) {
  int status;
  pid_t pid = fork();
  if(pid == 0){ // child
    execv(cmd,argv);
    exit(-127);
  } else if(pid > 0) {  // parent 
    waitpid(pid, &status, 0);
    if(WEXITSTATUS(status)){
    fprintf(stdout, "** Command successful **\n");
    } else {
    fprintf(stdout, "** Command failed **\n"); //only executes if execv returns(fails)
    }
  } else { // handle failed to fork
    fprintf(stdout, "** Command failed **\n"); //used same message for failed to fork since assignment didn't have any specifics
  }
}

/* 
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to a file
 */
void execute_output_to_file(char *cmd, char *const argv[], char *filename) {


  int status;
  pid_t pid = fork();
  if(pid == 0){ // child
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    close(fd);
    execv(cmd,argv);
    fprintf(stdout, "** Command failed **\n"); //only executes if execv returns(fails)
  } else if(pid > 0) {  // parent 
    waitpid(pid, &status, 0);
    if(WIFEXITED(status)){
    fprintf(stdout, "** Command successful **\n");
    } else {
      fprintf(stdout, "** Command failed **\n"); //only executes if execv returns(fails)
    }
  } else { // handle failed to fork
    fprintf(stdout, "** Command failed **\n"); //used same message for failed to fork since assignment didn't have any specifics
  }


}

/* 
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to another command
 */
void execute_output_to_other(char *cmd1, char *const argv1[], char *cmd2_with_argv2) {
  fprintf(stdout,"Error: %s() is not implemented!\n\n", __FUNCTION__);

  // WRITE CODE HERE

}


