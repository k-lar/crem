#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    const char platform[] = "windows";

#elif defined(__linux__) /* any linux distribution */
    const char platform[] = "linux";

#else
    const char platform[] = "other";

#endif

/* Boolean type definition because of portability */
typedef enum {
    false = ( 1 == 0 ),
    true = ( ! false )
} bool;

/* Function prototypes: */
char* concatString(char* str1, char* str2, char* str3);
int fileExists(char* fileName);
void version(void);
void help(void);
char* getFileName(int give_dir);
int isDir(const char* fileName);
void createSource(void);
void removeSource(void);
void addReminder(char* reminder);
int* bubbleSort(int* array, int array_length);
bool hasOnlyChars(char* str, char* allowedChars);
void removeReminder(char* target);
void showReminders(void);

char* concatString(char* str1, char* str2, char* str3) {
    if ((str2 = malloc(strlen(str1)+strlen(str3)+1)) != NULL){
        str2[0] = '\0';   /* ensures the memory is an empty string */
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

void version(void) {
    char ver[] = "0.1.0";
    printf("%s\n", ver);
}

void help(void) {
    printf("For this program to work as intended, add this line to the bottom of your .bashrc file:\n");
    printf("  crem --show\n");
    printf("Usage:\n");
    printf("  crem                    Creates $HOME/.config/creminders file\n");
    printf("  crem -a                 Add an entry inside the reminders file\n");
    printf("  crem -r                 Remove an entry [or multiple entries, seperated with \",\"]\n");
    printf("  crem -R                 Remove $HOME/.config/creminders file\n");
    printf("  crem --show             Prints your reminders to the terminal\n");
    printf("  crem --version          Prints what version of crem you have installed\n");
}

char* getFileName(int give_dir) {
    char* crempath_dir = (char* )malloc(50);
    char* crempath = (char* )malloc(50);
    char* path = (char* )malloc(50);
    char* crem_extension = (char* )malloc(16);
    if (strcmp(platform, "linux") == 0) {
        path = getenv("HOME");
        crem_extension = "/.config/crem/";
        crempath_dir = concatString(path, crempath_dir, crem_extension);
        crempath = concatString(crempath_dir, crempath, "creminders");

        if (give_dir == 1) {
            return crempath_dir;
        } else if (give_dir == 0) {
            return crempath;
        }

    } else if (strcmp(platform, "windows") == 0) {
        path = getenv("APPDATA");;
        crem_extension = "/crem/";
        crempath_dir = concatString(path, crempath_dir, crem_extension);
        crempath = concatString(crempath_dir, crempath, "creminders");

        if (give_dir == 1) {
            return crempath_dir;
        } else if (give_dir == 0) {
            return crempath;
        }

    } else {
        printf("Program was not built for this system.\n");
        exit(1);
    }
    return NULL;
}

int isDir(const char* fileName) {
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

void createSource(void) {
    char* name = getFileName(0);
    char* creminders_choice = (char* )malloc(16);
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
                printf("fopen failed\n");
                exit(1);
            } else {
                fclose(path);
            }
        }
    } else {
        FILE* path = fopen(name, "w");

        if (path == NULL) {
            printf("fopen failed\n");
            exit(1);
        } else {
            fclose(path);
        }
    }
    free(creminders_choice);
}

void removeSource(void) {
    char* creminders = getFileName(0);
    if (fileExists(creminders)) {
        if (remove(creminders) == 0) {
            printf("File deleted successfully.\n");
        } else {
            printf("Error deleting file.\n");
        }
    }
}

void addReminder(char* reminder) {
    char* creminders = getFileName(0);
    if (fileExists(creminders)) {
        FILE *file = fopen(creminders, "a");
        if (file == NULL) {
            createSource();
        }

        fprintf(file, "%s\n", reminder);
        fclose(file);

    } else {
        createSource();
        addReminder(reminder);
    }
}

int* bubbleSort(int* array, int array_length) {
    int i, j, temp;
    for (i = 0 ; i < ( array_length - 1 ); i++){
        for (j= 0 ; j < array_length - i - 1; j++){
            if(array[j] < array[j+1]){
                temp=array[j];
                array[j]   = array[j+1];
                array[j+1] = temp;
            }
        }
    }
    return array;
}

bool hasOnlyChars(char* str, char* allowedChars) {
    int strLen = strlen(str);
    int charsLen = strlen(allowedChars);
    int i, j;
    bool charExists;

    for (i = 0; i < strLen; i++) {
        charExists = false;

        /* Check if character exists in allowedChars */
        for (j = 0; j < charsLen; j++) {
            if (str[i] == allowedChars[j]) {
                charExists = true;
                break;
            }
        }

        /* If character doesn't exist, return false */
        if (!charExists) {
            return false;
        }
    }

    /* If all characters exist, return true */
    return true;
}

void removeReminder(char* target) {
    char* creminders = getFileName(0);
    char buffer[1024];
    char* token;
    const int entry_max = 25;
    int* entries_arr = (int* )malloc(sizeof(int) * entry_max);
    int arr_count = 0;
    int entry_num;
    char* number_chars = "0123456789";
    char* num_array_chars = "0123456789,";
    char* tmp_filename = (char* )malloc(50);
    memset(entries_arr, 0, sizeof(int)*entry_max);

    if (hasOnlyChars(target, number_chars) == true) {
        int entry = atoi(target);
        entries_arr[0] = entry;

    } else if (hasOnlyChars(target, num_array_chars) == true) {
        char* entries_str = target;

        token = strtok(entries_str, ",");
        /* loop through the string to extract all other tokens */
        while (token != NULL) {
            entries_arr[arr_count] = atoi(token);
            arr_count++;
            if (arr_count > entry_max) {
                printf("Reached entry removal max!\n");
                printf("Entered too many targets, expected max is %d.\n", arr_count, entry_max);
                printf("Aborting removal.\n");
                free(tmp_filename);
                free(entries_arr);
                return;
            }
            token = strtok(NULL, ",");
        }

    } else {
        printf("Bad removal parameter: %s\n", target);
        free(tmp_filename);
        free(entries_arr);
        return;
    }

    tmp_filename = concatString(getFileName(1), tmp_filename, "tmp");
    bubbleSort(entries_arr, entry_max);

    for (entry_num = 0; entry_num < entry_max; entry_num++) {
        if (entries_arr[entry_num] == 0) {
            continue;
        }

        if (fileExists(creminders)) {
            FILE* file1 = fopen(creminders, "r");
            FILE* file2 = fopen(tmp_filename, "w");
            int curr_line;

            if (file1 == NULL) {
                printf("Error: file does not exist\n");
                free(tmp_filename);
                free(entries_arr);
                return;
            }

            curr_line = 1;
            while (fgets(buffer, sizeof(buffer), file1)) {
                  if (curr_line != entries_arr[entry_num]) {
                     fputs(buffer, file2);
                  }
                  curr_line++;
            }

            fclose(file1);
            fclose(file2);

            remove(creminders);
            rename(tmp_filename, creminders);
        }
    }
    free(tmp_filename);
    free(entries_arr);
}

void showReminders(void) {
    FILE* file = fopen(getFileName(0), "r"); /* should check the result */
    int line_num = 1;
    char line[512];

    if (file != NULL) {
        printf("########## Creminders ##########\n");
        while (fgets(line, sizeof(line), file)) {
            /* note that fgets don't strip the terminating \n, checking its */
            /* presence would allow to handle lines longer that sizeof(line) */
            if (strcmp(line, "\n")) {
                printf("[%d] - %s", line_num, line);
                line_num++;
            }
        }
        printf("################################\n\n");
        fclose(file);
    } else {
        printf("creminders file not found!\n");
    }
}

int main(int argc, char* argv[]) {
    int arg_num;
    if (argc == 1) {
	if (fileExists(getFileName(0)) == 0) {
	    createSource();
        if (fileExists(getFileName(0))) {
            printf("Success!\n");
            printf("For help using this program, use the -h flag.\n");
        }
	} else {
            help();
	}
        return 0;
    }

    for (arg_num = 1; arg_num < argc;) {
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
            if (argv[arg_num+1] != NULL) {
                addReminder(argv[arg_num+1]);
            }
            arg_num = arg_num + 2;

        } else if (strcmp(argv[arg_num], "-r") == 0 || strcmp(argv[arg_num], "--remove") == 0) {
            if (argv[arg_num+1] != NULL) {
                removeReminder(argv[arg_num+1]);
            }
            arg_num = arg_num + 2;

        } else {
            printf("Unknown argument passed: %s\n", argv[arg_num]);
            arg_num++;
        }
    }

    return 0;
}
