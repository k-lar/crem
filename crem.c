#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PLATFORM "windows"

#elif defined(__linux__) /* any linux distribution */
    #define PLATFORM "linux"

#else
    #define PLATFORM "other"

#endif

char* concatString(char* str1, char* str2, char* str3) {
    if((str2 = malloc(strlen(str1)+strlen(str3)+1)) != NULL){
        str2[0] = '\0';   // ensures the memory is an empty string
        strcat(str2,str1);
        strcat(str2,str3);
    } else {
        printf("malloc failed!\n");
        exit(1);
    }
    return str2;
}

int fileExists(char* fileName) {
    struct stat buffer;
    return (stat(fileName, &buffer) == 0);
}

char* getFileName(int give_dir) {
    if (PLATFORM == "linux") {
        char* path = getenv("HOME");
        char* crem_extension = "/.config/crem/";
        char* crempath_dir = concatString(path, crempath_dir, crem_extension);
        char* crempath = concatString(crempath_dir, crempath, "creminders");

        if (give_dir == 1) {
            return crempath_dir;
        } else if (give_dir == 0) {
            return crempath;
        }

    } else if (PLATFORM == "windows") {
        char* path = getenv("APPDATA");;
        char* crem_extension = "/crem/";
        char* crempath_dir = concatString(path, crempath_dir, crem_extension);
        char* crempath = concatString(crempath_dir, crempath, "creminders");

        return crempath;

    } else if (PLATFORM == "other") {
        printf("Program was not built for this system.");
        exit(1);
    }
}

int isDir(const char* fileName) {
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

void createSource() {
    char* name = getFileName(0);
    char* creminders_choice;
    if (isDir(getFileName(1)) == false) {
    	mkdir(getFileName(1), 0700);
	    printf("Do you want to create creminders file? [Y/n]: ");
	    scanf("%s", creminders_choice);
        if (strcmp(creminders_choice, "n") == 0 || strcmp(creminders_choice, "N") == 0) {
            printf("Quitting.\n");
            exit(0);
        } else {
            FILE* path = fopen(name, "w");
            if (path == NULL) {
                printf("fopen failed, errno: %d\n", errno);
                exit(1);
            } else {
                fclose(path);
            }
        }
    } else {
	    FILE* path = fopen(name, "w");

        if (path == NULL) {
            printf("fopen failed, errno: %d\n", errno);
            exit(1);
        } else {
            fclose(path);
        }
    }
}

void removeSource() {
    char* creminders = getFileName(0);
    if (access(creminders, F_OK) != -1) {
        if (remove(creminders) == 0) {
            printf("File deleted successfully.\n");
        } else {
            printf("Error deleting file.\n");
        }
    }
    /* else { */
    /*     // file doesn't exist */
    /*     printf("File does not exist.\n"); */
    /* } */
}

int getFileLines() {
    char* creminders = getFileName(0);
    char buffer[1024];
    int line_count = 1;

    FILE* file = fopen(getFileName(0), "r"); /* should check the result */
    if (file != NULL) {
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            line_count++;
        }
        fclose(file);
    }
    return line_count;

}

void addReminder(char* reminder) {
    char* creminders = getFileName(0);
    if (access(creminders, F_OK) != -1) {
        FILE *file = fopen(creminders, "a");
        if (file == NULL) {
            createSource();
        }

        int line_num = getFileLines();

        fprintf(file, "[%d] - %s\n", line_num, reminder);
        fclose(file);
    }
    else {
        // file doesn't exist
        /* printf("File does not exist.\n"); */
        createSource();
        addReminder(reminder);
    }
}

void showReminders() {
    FILE* file = fopen(getFileName(0), "r"); /* should check the result */
    if (file != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            // note that fgets don't strip the terminating \n, checking its
            // presence would allow to handle lines longer that sizeof(line)
            if (strcmp(line, "\n")) {
                printf("%s", line);
            }
        }
        fclose(file);
    } else {
        printf("creminders file not found!\n");
    }
}

void version() {
    char ver[] = "0.0.1";
    printf("%s\n", ver);
}

void help() {
    const char help_msg[] =
    "For this program to work as intended, add this line to the bottom of your .bashrc file:\n"
    "  crem --show\n"
    "Or use the flag --add-to-sh to do it for you\n"
    "Usage:\n"
    "  crem                    Creates $HOME/.config/creminders file\n"
    "  crem -a                 Add an entry inside the reminders file\n"
    "  crem -r                 Remove an entry [or multiple entries, seperated with \",\"]\n"
    "  crem -R                 Remove $HOME/.config/creminders file\n"
    "  crem --show             Prints your reminders to the terminal\n"
    "  crem --add-to-sh        Adds reminder autodetection inside bash|zsh|fish\n"
    "  crem --version          Prints what version of crem you have installed\n";
    printf(help_msg);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
	if (fileExists(getFileName(0)) == 0) {
	    createSource();
	} else {
            help();
	}
        return 0;
    }

    for (int arg_num = 1; arg_num < argc;) {
        if (strcmp(argv[arg_num], "-v") == 0 || strcmp(argv[arg_num], "--version") == 0) {
            version();
            arg_num++;

        } else if (strcmp(argv[arg_num], "-h") == 0 || strcmp(argv[arg_num], "--help") == 0) {
            help();
            arg_num++;

        } else if (strcmp(argv[arg_num], "-s") == 0 || strcmp(argv[arg_num], "--show") == 0) {
            showReminders();
            arg_num++;

        } else if (strcmp(argv[arg_num], "-R") == 0 || strcmp(argv[arg_num], "--reset") == 0) {
            removeSource();
            arg_num++;

        } else if (strcmp(argv[arg_num], "-a") == 0 || strcmp(argv[arg_num], "--add") == 0) {
            addReminder(argv[arg_num+1]);
            arg_num = arg_num + 2;

        } else {
            printf("Unknown argument passed: %s\n", argv[arg_num]);
            arg_num++;
        }
    }

    return 0;
}
