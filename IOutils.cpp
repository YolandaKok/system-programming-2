#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ./mirror_client -n 1 -c ./common -i ./1_input -m ./1_mirror -b 100 -l log_file1

/* Read the arguments */
int readArgs(int argc, char* argv[], int& id, char*& common_dir, char*& input_dir,
             char*& mirror_dir, int& buffer_size, char*& log_file) {
    int i;
    for ( i = 1; i < argc; i+=2 ) {
        if(!strcmp(argv[i], "-n")) {
            id = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-c")) {
            common_dir = (char*)malloc(strlen(argv[i+1]) + 1);
            strcpy(common_dir, argv[i+1]);
        }
        else if(!strcmp(argv[i], "-i")) {
            input_dir = (char*)malloc(strlen(argv[i+1]) + 1);
            strcpy(input_dir, argv[i+1]);
        }
        else if(!strcmp(argv[i], "-b")) {
            buffer_size = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-m")) {
            mirror_dir = (char*)malloc(strlen(argv[i+1]) + 1);
            strcpy(mirror_dir, argv[i+1]);
        }
        else if(!strcmp(argv[i], "-l")) {
            log_file = (char*)malloc(strlen(argv[i+1]) + 1);
            strcpy(log_file, argv[i+1]);
        }
    }
}

void writeLogFile(char *log_file, char *filename, int bytes, int mode, int id) {
    /* Open the log file and create */
    /*int fd = open(log_file, O_WRONLY | O_APPEND | O_CREAT, 0660);
    char buffer[100];
    sprintf(buffer, "Receive %s %d\n", filename, bytes);
    write(fd, buffer, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    close(fd);*/
    FILE *fp = fopen(log_file, "a");
    if(mode == 1) {
        /* Write for the id */
        fprintf(fp, "%s %d\n", "Client", id);
    }
    else if(mode == 2) {
        /* Write for bytes received */
        fprintf(fp, "%s %s %d\n", "Received", filename, bytes);
    }
    else if(mode == 3) {
        /* Write for bytes sent */
        fprintf(fp, "%s %s %d\n", "Sent", filename, bytes);
    }
    fclose(fp);
}