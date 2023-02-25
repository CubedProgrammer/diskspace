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
    const char search[7] = "\n/dev/";
    int mpipes[2];
    pipe(mpipes);
    int mpid = fork();
    if(mpid == 0)
    {
        dup2(mpipes[1], STDOUT_FILENO);
        execl("/usr/bin/mount", "/usr/bin/mount", (char *)NULL);
        exit(1);
    }
    else
        close(mpipes[1]);
    int mounts = mpipes[0];
    size_t msize = read(mounts, mdat, sizeof(mdat));
    close(mounts);
    mdat[msize] = '\0';
    char *devstr = strstr(mdat, search);
    char *mpstr, *spaceptr;
    struct statvfs disk;
    struct statvfs *pdisk = &disk;
    unsigned long total, remaining, used;
    unsigned eu = geteuid();
    cpcdt_date date = cpcdt_make_date(sec_since_epoch());
    cpcdt_readable_date(cbuf, date);
    FILE *file = fopen("diskspace.txt", "a");
    fputs(cbuf, file);
    fputc('\n', file);
    while(devstr != NULL)
    {
        mpstr = strchr(devstr, ' ');
        *mpstr = '\0';
        mpstr += 4;
        spaceptr = strchr(mpstr, ' ');
        *spaceptr = '\0';
        statvfs(mpstr, pdisk);
        total = disk.f_frsize * disk.f_blocks;
        if(eu == 0)
            remaining = disk.f_frsize * disk.f_bfree;
        else
            remaining = disk.f_frsize * disk.f_bavail;
        used = total - remaining;
        fprintf(file, "%s %s %lu %lu %lu\n", devstr + 1, mpstr, used, remaining, total);
        devstr = strstr(spaceptr + 1, search);
    }
    fclose(file);
    return 0;
}
