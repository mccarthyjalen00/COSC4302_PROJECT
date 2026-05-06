/*
Class: COSC_4302_FALL_2023
Group Members: Victoria Kitchen and Jalen McCarthy
Requirements: In part 1, fork() is used to create a child process and execute the command. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define	MAX_LINE_LEN 	80
#define MAX_ARGS 		64
#define MAX_ARG_LEN 	16
#define MAX_PATHS 		64
#define MAX_PATH_LEN 	96
#define CHAR_MAX 		1024
#define WHITESPACE	    " "
#define TRUE			1

extern int errno;

//Print command prompt to screen
void printPrompt(){

    char promptString[] = "bash";
    printf("\033[0;31m");
    printf("%s:$ ", promptString);
    printf("\033[0m");
}

//Read user input from standard input
void readCommand(char *command[]){
    fgets(command, MAX_LINE_LEN, stdin);
}

//Processing user input in command string
void parseCommand(char command[], char * args[]) {

    size_t length = strlen(command);
    if (command[length - 1] == '\n') {
        command[length - 1] = '\0';
    }
    
    //Tokenize the command and place the parameters in args
    char *token;
    token = strtok(command, " ");
    int i = 0;

    while(token != NULL){
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    //Place NULL in last element of args to show end of argument list
    args[i] = NULL;

    //Check user command for "exit", 
    //if "exit" exists, the program will exit
    if(strcmp(command, "exit") == 0)
        exit(0);
}

int parsePath(char **dirs) {

    //Declare pointers for storing PATH environment variable
    //and its copy
	char *pathEnvVar;
	char *thePath;
    
    //Create a delimiter for PATH directories ":"
	const char delimiter[2] = ":";

    int i;

    //Loop through and set each element to NULL
	while(*dirs++) {
		*dirs = NULL;
	}
    
   //Obtain value of PATH environment variable
   //Allocate memory for 'thePath' and copy that value of PATH environment variable
   pathEnvVar = (char *) getenv ("PATH");
   thePath = (char *) malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);

   //Place the different paths (directories) in dirs
   char *token = (char *) malloc(strlen(pathEnvVar));

  //tokenize the path with the dilimiter :
   token = strtok(thePath, delimiter);

   //Place each directory in dirs
   while( token != NULL ) {
	  token = strtok(NULL, delimiter);
	  *dirs = token;
	  dirs++;
   }
    //Successful parsing if we reach this
   return 1;
}

char *lookupPath(char **argv, char **dir, char *filepath) {

	int i;
	char *result;
	char pName[MAX_PATH_LEN];
	int pathSize = sizeof(*dir)/sizeof(char);

	strcpy(filepath, "");
    
    //Check to see if file name is already an absolute path name
	if(*argv[0] == '/') {
		return argv[0];
	}

   //Look in PATH directories
   //access() will see if the file is in a dir and then return the filepath
	for (i = 1; i < pathSize; i++) {
		dir++;
		strcat(filepath, *dir);
		strcat(filepath, "/");
		strcat(filepath, argv[0]);


		if (access(filepath, F_OK) == 0) {
			return filepath;
		}

        //Reset filepath for next iteration
		memset(filepath, 0, CHAR_MAX);
		char filepath[CHAR_MAX] = "";

	}
    //Return NULL if executable not found
	return NULL;
}

//Driver Code
int main(){

    char **dirs = (char **) malloc(sizeof(char)*MAX_PATHS);
    char commandLine[CHAR_MAX];
    char *args[MAX_ARGS];

    //Read the PATH variable for the environment, then builds and array, dirs[] of the directories in PATH
    parsePath(dirs);

    while (TRUE) {
           //Prints Bash, get user input, and then parse the commmand
            printPrompt();
            readCommand(&commandLine);
            parseCommand(commandLine, args);

            //Create Child process
            pid_t childProcessID;
            int pid = fork();
            int stat;

            if(pid == 0){
                char *filepath = (char *) malloc(sizeof(char) * CHAR_MAX);
                //Validate the path of the command and see if the command is there
                lookupPath(args, dirs, filepath);
                
                //Execute the command in the child process 
                //Or print an error if execution did not work
                if(execv(filepath, args) < 0){
                        printf("Exiting with error %d\n", errno);
                        exit(2);
                }
            //Wait for child process to end then print exit id
            }
            else{
                    childProcessID = wait(&stat);
                    if(WIFEXITED(stat)){
                        printf("Child %d, exited with %d\n", childProcessID, WEXITSTATUS(stat));
                    }
            }
    }
    //End of function
    return 0;
}