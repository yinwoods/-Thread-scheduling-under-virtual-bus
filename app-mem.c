#include <stdio.h>
#include <string.h>

int main(void) {

    FILE *fp0 = NULL;
    char Buf[4096];

    strcpy(Buf, "Mem is char dev!");

    fp0 = fopen("/dev/memdev0", "r+");
    if(fp0 == NULL) {
        printf("Open Memdev0 Error!\n");
        return -1;
    }

    fwrite(Buf, sizeof(Buf), 1, fp0);

    fseek(fp0, 0, SEEK_SET);

    strcpy(Buf, "Buf is NULL!");
    printf("Buf: %s\n", Buf);

    fread(Buf, sizeof(Buf), 1, fp0);

    printf("Buf: %s\n", Buf);

    return 0;
}
