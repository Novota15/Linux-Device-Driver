#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>

// code for testing simple_char_driver

#define DEVICE "/dev/simple_char_driver"

int file = 0;
int ppos = 0;

int write_device()
{
        int write_length = 0;
        ssize_t ret;
        char *data = (char *)malloc(1024 * sizeof(char));

        printf("enter data to write into device\n");
        scanf(" %[^\n]", data); //takes input until new line
        write_length = strlen(data);
        ret = write(file, data, write_length, &ppos);
        if (ret == -1)
                printf("writing failed\n");
        else
                printf("writing success\n");
        free(data);
        return 0;
}

int read_device()
{
        int read_length = 0;
        ssize_t ret;
        char *data = (char *)malloc(1024 * sizeof(char));

        printf("enter the length of the buffer to read\n");
        scanf("%d", &read_length);
        memset(data, 0, sizeof(data));
        data[0] = '\0';
        ret = read(file, data, read_length, &ppos);
        printf("DEVICE_READ : %s\n", data);
        if (ret == -1)
                printf("reading failed\n");
        else
                printf("reading success\n");
        free(data);
        return 0;
}

int lseek_device()
{
        int lseek_offset = 0;
        int seek_value = 0;
        
        printf("enter the seek offset\n");
        scanf("%d", &lseek_offset);
        printf("0 for SEEK_SET, 1 for SEEK_CUR and 2 for SEEK_END\n");
        scanf("%d", &seek_value);
        printf("seek value = %d\n", seek_value);

        switch (seek_value) {
        case 0: lseek(file, lseek_offset, 0);
                return 0;
                break;
        case 1: lseek(file, lseek_offset, 1);
                return 0;
                break;
        case 2: lseek(file, lseek_offset, 2);
                return 0;
                break;
        default :
                printf("unknown option selected\n");
                break;
        }
        return 0;
}

int main()
{
        // O_RDWR option stands for read, write permissions.
        char option;
        if (access(DEVICE, F_OK) == -1) {
                printf("module %s not loaded\n", DEVICE);
                return 0;
        } else
                printf("module %s loaded, will be used\n", DEVICE);
        
        file = open(DEVICE, O_RDWR);

        while (1) {
                printf("please enter:\n\
                    w to write\n\
                    r to read\n\
                    s to seek\n\
                    e to exit\n");
                scanf("%c", &option);
                switch (option) {
                        case 'w' : printf("write option selected\n");
                                
                                write_device();
                                // close(file); /*closing the device*/
                                while (getchar() != '\n');
                                break;
                        case 'r' : printf("read option selected\n");
                                
                                read_device();
                               //  close(file); /*closing the device*/
                                while (getchar() != '\n');
                                break;
                        case 's' : printf("lseek  option selected\n");
                                
                                lseek_device();
                                // close(file); /*closing the device*/
                                while (getchar() != '\n');
                                break;
                        case 'e' : printf("exiting\n");
                                close(file); /*closing the device*/
                                return 0;
                        default : printf("unknown option selected\n");
                                break;
                }
        }
        return 0;
}