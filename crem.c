#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PLATFORM "windows"

#elif defined(__linux__) // any linux distribution
    #define PLATFORM "linux"

#else
    #define PLATFORM "other"

#endif

char fileName[] = "/home/klar/.config/crem/creminders";

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

/* void createSource() { */
/*     struct stat info; */

/*     if (stat(getFileName(0), &info) != 0) */
/*         printf("cannot access %s\n", pathname); */
/*     else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows */
/*         printf("%s is a directory\n", pathname); */
/*     else */
/*         printf("%s is no directory\n", pathname); */

/* } */

char* getFileName(int give_dir) {
    if (PLATFORM == "linux") {
        char* path = getenv("HOME");
        char* crem_extension = "/.config/crem/";
        char* crempath_dir;
        crempath_dir = concatString(path, crempath_dir, crem_extension);
        /* if((crempath = malloc(strlen(path)+strlen(crem_extension)+1)) != NULL){ */
        /*     crempath[0] = '\0';   // ensures the memory is an empty string */
        /*     strcat(crempath,path); */
        /*     strcat(crempath,crem_extension); */
        /* } else { */
        /*     printf("malloc failed!\n"); */
        /*     exit(1); */
        /* } */

        /* printf(crempath); */
        char* crempath;
        crempath = concatString(crempath_dir, crempath, "creminders");
        if (give_dir == 1) {
            return crempath_dir;
        } else if (give_dir == 0) {
            return crempath;
        }

    } else if (PLATFORM == "windows") {
        char* path = getenv("APPDATA");;
        char* crem_extension = "/crem/";
        char* crempath_dir;
        crempath_dir = concatString(path, crempath_dir, crem_extension);

        char* crempath;
        crempath = concatString(crempath_dir, crempath, "creminders");
        return crempath;

    } else if (PLATFORM == "other") {
        printf("Program was not built for this system.");
        exit(1);
    }
}

void showReminders() {
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // note that fgets don't strip the terminating \n, checking its
        // presence would allow to handle lines longer that sizeof(line)
        if (strcmp(line, "\n")) {
            printf("%s", line);
        }
    }
    fclose(file);
}

void version() {
    char ver[] = "0.0.1";
    printf("%s\n", ver);
}

void help() {
    char help_msg[] =
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
        help();
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

        } else {
            printf("Unknown argument passed: %s\n", argv[arg_num]);
            arg_num++;
        }
    }

    printf(getFileName(1));
    return 0;
}
