#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/statvfs.h>
#include<unistd.h>
#include<cpcdt_date_struct.h>
int main(int argl, char *argv[])
{
    char cbuf[100];
    char mdat[20000];
    const char search[6] = "/dev/";
    int mounts = open("/proc/self/mounts", O_RDONLY);
    size_t msize = read(mounts, mdat, sizeof(mdat));
    close(mounts);
    mdat[msize] = '\0';
    char *devstr = strstr(mdat, search);
    char *mpstr, *spaceptr;
    struct statvfs disk;
    struct statvfs *pdisk = &disk;
    unsigned long total, remaining, used;
    cpcdt_date date = cpcdt_make_date(sec_since_epoch());
    cpcdt_readable_date(cbuf, date);
    FILE *file = fopen("diskspace.txt", "a");
    fputs(cbuf, file);
    fputc('\n', file);
    while(devstr != NULL)
    {
        mpstr = strchr(devstr, ' ');
        *mpstr++ = '\0';
        spaceptr = strchr(mpstr, ' ');
        *spaceptr = '\0';
        statvfs(mpstr, pdisk);
        total = disk.f_frsize * disk.f_blocks;
        remaining = disk.f_frsize * disk.f_bfree;
        used = total - remaining;
        fprintf(file, "%s %s %lu %lu %lu\n", devstr, mpstr, used, remaining, total);
        devstr = strstr(spaceptr + 1, search);
    }
    fclose(file);
    return 0;
}
