#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


/*
 * This function handles the execution of the compiled files. It executes the executable from the Output
 * folder and checks the following:
 *      - the exit code to be 0, meaning that the file executed without errors
 *      - the output of the program (symbolized through a printf statement) to be the same as the one in the
 *        check file of the current test
 * If the conditions mentioned above are checked then the test is noted as passed, if no some error message will be displayed.
 *
 * executableName - refers to the name of the executable file
 * absolutePathToExecutablesFolder - refers to the absolute path to the Output folder where the compiled file is located
 * fileName - name of the C file, without the extension
 * absolutePathToCFile - the absolute path to the C file
 */
void handle_c_file_execution(char *executableName, char *absolutePathToExecutablesFolder, char *fileName,
                             char *absolutePathToCFile) {

    printf("Program: %s.c\n", fileName);

    // We take the absolute path to the compiled file to execute it later
    char *absolutePathToTheCompiledFile = (char *) malloc(
            strlen(absolutePathToExecutablesFolder) + 1 + strlen(executableName) + 1);
    strcpy(absolutePathToTheCompiledFile, absolutePathToExecutablesFolder);
    strcat(absolutePathToTheCompiledFile, "/");
    strcat(absolutePathToTheCompiledFile, executableName);

    // We hold the name of the input's folder. This folder name should be specific and is described in the README.md file
    char *inputFilesFolder = (char *) malloc(strlen(fileName) + strlen("_inputfiles") + 1);
    strcpy(inputFilesFolder, fileName);
    strcat(inputFilesFolder, "_inputfiles");

    // We hold the absolute path for the above directory
    char *absolutePathToTheInputsFile = (char *) malloc(strlen(absolutePathToCFile) + 1 + strlen(inputFilesFolder) + 1);
    strcpy(absolutePathToTheInputsFile, absolutePathToCFile);
    strcat(absolutePathToTheInputsFile, "/");
    strcat(absolutePathToTheInputsFile, inputFilesFolder);


    char inputsForProgram[1000][101]; // -> represents a list of strings that later on will be sent as parameters to the current executing C program
    int k = 0; // -> number of tests

    // Here we take all the inputs from the directory and memorize them in the above created variable.
    DIR *inputDirectory = opendir(absolutePathToTheInputsFile);
    struct dirent *entry;
    if (inputDirectory != NULL) {
        while ((entry = readdir(inputDirectory)) != NULL) {
            if (entry->d_type == DT_REG && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
                char *absolutePathToInputFile = (char *) malloc(
                        strlen(absolutePathToTheInputsFile) + 1 + strlen(entry->d_name) + 1);
                strcpy(absolutePathToInputFile, absolutePathToTheInputsFile);
                strcat(absolutePathToInputFile, "/");
                strcat(absolutePathToInputFile, entry->d_name);
                FILE *inputFileReader;
                inputFileReader = fopen(absolutePathToInputFile, "r");
                fgets(inputsForProgram[k], 100, inputFileReader);
                k++;
            }
        }
        closedir(inputDirectory);
    } else {
        printf("The inputs directory does not exist or it is not created accordingly. See official docs for more info.");
    }

    // We hold the name of the check file's folder. This folder name should be specific and is described in the README.md file
    char *checkFilesFolder = (char *) malloc(strlen(fileName) + strlen("_checkfiles") + 1);
    strcpy(checkFilesFolder, fileName);
    strcat(checkFilesFolder, "_checkfiles");

    int numberOfTestPassed = 0;

    // Here, for each input
    for (int i = 0; i < k; i++) {

        // We create an executing command
        char *executingCommand = (char *) malloc(
                strlen(absolutePathToTheCompiledFile) + 1 + strlen(inputsForProgram[i]) + strlen(" > auxfile") + 1);
        strcpy(executingCommand, absolutePathToTheCompiledFile);
        strcat(executingCommand, " ");
        strcat(executingCommand, inputsForProgram[i]);
        strcat(executingCommand, " > auxfile"); // that redirects the printf mentioned above to an auxfile

        // We execute the command and remember the exit code
        int resultCode = system(executingCommand);

        // We take the output from the auxfile, then we delete the file
        FILE *programOutputFileReader;
        programOutputFileReader = fopen("auxfile", "r");
        char programOutput[100];
        fgets(programOutput, 99, programOutputFileReader);
        system("rm auxfile");

        // Then, for the specific test, we take the absolute path of the check file
        char *absolutePathToCheckFile = (char *) malloc(
                strlen(absolutePathToCFile) + 1 + strlen(inputFilesFolder) + 1 + strlen("checkfile1.txt") + 1);
        strcpy(absolutePathToCheckFile, absolutePathToCFile);
        strcat(absolutePathToCheckFile, "/");
        strcat(absolutePathToCheckFile, checkFilesFolder);
        strcat(absolutePathToCheckFile, "/checkfile");
        char fileNumber[50];
        sprintf(fileNumber, "%d", i + 1);
        strcat(absolutePathToCheckFile, fileNumber);
        strcat(absolutePathToCheckFile, ".txt");

        // Read the expected output from the check file for this specific test
        FILE *expectedOutputFileReader;
        expectedOutputFileReader = fopen(absolutePathToCheckFile, "r");
        char expectedOutput[100];
        fgets(expectedOutput, 99, expectedOutputFileReader);

        // We compare the results
        int ok = strcmp(expectedOutput, programOutput);

        // Then, we handle the output of our comparations accordingly
        if (resultCode == 256) { // exit code 1 * 256
            printf("Fail for test %d/%d.\n", i + 1, k);
            printf("The number of arguments from the input does not match with the program needs.\n");
        } else if (resultCode == 0 && ok == 0) {
            printf("Program executed with success for test %d/%d.\n", i + 1, k);
            numberOfTestPassed += 1;
        } else if (resultCode == 0) {
            printf("Program executed with success, but without the expected output for test %d/%d.\n", i + 1, k);
        } else {
            printf("Exit code %d Fail for test %d/%d.\n", resultCode, i + 1, k);

        }
    }
    printf("Tests passed %d/%d\n\n", numberOfTestPassed, k);
}

/*
 * This function receives the name of the file to compile and the path to the folder where the file is located at. It creates
 * the output folder and compiles the code using gcc, the file resulted being generated in the folder created earlier.
 * If a compile error occurs, then the error will be written to the errors.txt file in the created folder above.
 *
 * pathOfFile - refers to the path of C file we are going to handle
 * pathOfFileFolder - refers to the path of the folder where the C file is locating
 */
void handle_c_file_compilation(char *nameOfFile, char *pathOfFilesFolder) {

    // We change the directory for compiling the c file
    char *changeToDirectory = (char *) malloc(strlen("cd ") + strlen(pathOfFilesFolder) + 1);
    strcpy(changeToDirectory, "cd ");
    strcat(changeToDirectory, pathOfFilesFolder);

    // Generate the file name without the C extension to use in the name of the output file
    char *fileNameWithoutExt = (char *) malloc(strlen(nameOfFile));
    strncpy(fileNameWithoutExt, nameOfFile, strlen(nameOfFile) - 2);

    // We verify if the Output folder exists, and if not we create it
    char *makeOutputDir = (char *) malloc(strlen("mkdir Output  && "));
    char *absolutePathToOutputFolder = (char *) malloc(strlen(pathOfFilesFolder));
    strcpy(absolutePathToOutputFolder, pathOfFilesFolder);
    strcat(absolutePathToOutputFolder, "/Output");
    DIR *outputDirectory = opendir(absolutePathToOutputFolder);
    if (outputDirectory == NULL) {
        strcpy(makeOutputDir, "mkdir Output  && ");
    } else {
        strcpy(makeOutputDir, "");
    }

    // We create the gcc command to use it for compiling the program
    char *createOutputCommand = (char *) malloc(strlen("gcc -o ./Output/output_") + strlen(fileNameWithoutExt) + 1);
    strcpy(createOutputCommand, "gcc -o ./Output/output_");
    strcat(createOutputCommand, fileNameWithoutExt);
    strcat(createOutputCommand, " ");

    // We add to the previous command the name of the file and error handling part
    char *commandToCompile = (char *) malloc(
            strlen(createOutputCommand) + strlen(nameOfFile) + strlen(" 2> ./Output/compiling_errors.txt") + 1);
    strcpy(commandToCompile, createOutputCommand);
    strcat(commandToCompile, nameOfFile);
    strcat (commandToCompile, " 2> ./Output/compiling_errors.txt");

    // We create the combined command
    char *combinedCommands = (char *) malloc(
            strlen(changeToDirectory) + strlen(" && ") + strlen(makeOutputDir) +
            strlen(commandToCompile) + 1);
    strcpy(combinedCommands, changeToDirectory);
    strcat(combinedCommands, " && ");
    strcat(combinedCommands, makeOutputDir);
    strcat(combinedCommands, commandToCompile);

    // We execute the above created command
    system(combinedCommands);

    // If any error is occurring we add it to the errors.txt file, so we verify the file to se if something was added to it
    // and if so, we notify the user that a compiling problem appeared in a specific folder.
    FILE *fileReader;
    char *absolutePathToOutputFolderCopy = (char *) malloc(
            strlen(absolutePathToOutputFolder) + strlen("/compiling_errors.txt") + 1);
    strcpy(absolutePathToOutputFolderCopy, absolutePathToOutputFolder);
    strcat(absolutePathToOutputFolderCopy, "/compiling_errors.txt");
    fileReader = fopen(absolutePathToOutputFolderCopy, "r");
    char error[100];
    fgets(error, 100, fileReader);
    if (strcmp(error, "") != 0) {
        printf("Compilation error on file %s. Check the Output folder for errors.txt .", nameOfFile);
    } else {
        char *aux = (char *) malloc(strlen("output_") + strlen(fileNameWithoutExt) + 1);
        strcpy(aux, "output_");
        strcat(aux, fileNameWithoutExt);
        handle_c_file_execution(aux, absolutePathToOutputFolder, fileNameWithoutExt, pathOfFilesFolder);
    }
}

/*
 * This function receives an absolute path to a directory and searches for C files inside.
 *
 * absolutePathToTheDirectory - refers to the absolute path to the directory we want to search in
 */
void get_c_files_from_directory(char *absolutePathToTheDirectory) {

    DIR *dir;
    struct dirent *entry;

    // We open the director received as parameter and if it's exists we check every file to be regular and to be ending
    // in .c extension
    // If these two conditions are checked, we go to the compiling step.
    dir = opendir(absolutePathToTheDirectory);
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && strcmp(entry->d_name + strlen(entry->d_name) - 2, ".c") == 0) {
                handle_c_file_compilation(entry->d_name, absolutePathToTheDirectory);
            }
        }
        closedir(dir);
    }
}

/*
 * This function gets the absolute path for each directory, calling the get_c_files_from_directory to handle
 * the finding c files part.
 *
 * folderStructure - refers to the entire folder structure,
 *                   but also contains the entire absolute path to the folder structure
 *                   because I did not know how to get the absolute path for a folder
 *                   without specifying the path to the executing place
 * firstFolderInTheStructure - refers to the actual first folder in the folder structure,
 *                             from where the search for C files will begin
 */
void get_path_for_each_directory(char *folderStructure, char *firstFolderInTheStructure) {

    // We get the actual folder structure from the absolute path
    char *actualFolderStructure = strstr(folderStructure, firstFolderInTheStructure);

    // We take the part of the path that does not belong to the actual folder structure from where we want to test the
    // C files.
    char *absolutePathBeforeTheFolderStructure = (char *) malloc(strlen(folderStructure) + 1);
    absolutePathBeforeTheFolderStructure[0] = '\0';
    char *folderStructureCopy = (char *) malloc((strlen(folderStructure) + 1));
    strcpy(folderStructureCopy, folderStructure);
    char *p = strtok(folderStructureCopy, "/");
    while (p) {
        if (strcmp(p, firstFolderInTheStructure) == 0) {
            break;
        } else {
            strcat(absolutePathBeforeTheFolderStructure, "/");
            strcat(absolutePathBeforeTheFolderStructure, p);
            p = strtok(NULL, "/");
        }
    }

    // For every folder in the actual folder structure we search separately C files, by adding the folders to the base
    // path from the root. Ex: path/dir1 -> path/dir1/dir2 etc.
    char *actualFolderStructureCopy = (char *) malloc(strlen(actualFolderStructure) + 1);
    strcpy(actualFolderStructureCopy, actualFolderStructure);
    char *q;
    q = strtok(actualFolderStructureCopy, "/");
    while (q) {
        strcat(absolutePathBeforeTheFolderStructure, "/");
        strcat(absolutePathBeforeTheFolderStructure, q);
        get_c_files_from_directory(absolutePathBeforeTheFolderStructure);
        q = strtok(NULL, "/");
    }
}

int main() {

    get_path_for_each_directory(
            "/Users/poe/Desktop/proiect_so/director1",
            "director1"
    );
}