#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 1024

void tokenize_command(char *command, char **args) {
  char *token;
  int i = 0;

  token = strtok(command, " ");
  while (token != NULL) {
    args[i++] = token;
    token = strtok(NULL, " ");
  }
  args[i] = NULL;
}

int execute_command(char **args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    // Child process

    // Check if the command is "count"
    if (strcmp(args[0], "count") == 0) {
      // Execute the count command
      int count = 0;
      FILE *fp = fopen(args[1], "r");
      if (fp != NULL) {
        char c;
        while ((c = fgetc(fp)) != EOF) {
          count++;
        }
        fclose(fp);
      } else {
        printf("Error opening file\n");
        exit(1);
      }

      // Display the count
      printf("%d\n", count);
    } else {
      // Execute the command using the system() function
      system(args[0]);
    }

    exit(0);
  } else {
    // Parent process
    waitpid(pid, &status, 0);
    return status;
  }
}

int main() {
  char command[MAX_ARGS];
  char *args[MAX_ARGS];
  int status;

  while (1) {
    printf("myshell$ ");
    fgets(command, MAX_ARGS, stdin);
    // Remove the trailing newline character
    command[strlen(command) - 1] = '\0';

    // Tokenize the command line
    tokenize_command(command, args);

    // Execute the command
    status = execute_command(args);
    if (status != 0) {
      if (strcmp(args[0], "count") == 0) {
        printf("Error executing count command\n");
      } else {
        printf("Command not found\n");
      }
    }
  }

  return 0;
}

