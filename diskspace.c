#include<stdio.h>
#include<windows.h>
#include<cpcdt_date_struct.h>
#pragma comment(lib, "cpcdt")
int main(int argl, char *argv[])
{
    char cbuf[100];
    cpcdt_date date = cpcdt_make_date(sec_since_epoch());
    cpcdt_readable_date(cbuf, date);
    FILE *file = fopen("diskspace.txt", "a");
    fputs(cbuf, file);
    fputc('\n', file);
    GetLogicalDriveStringsA(sizeof(cbuf), cbuf);
    DWORD bytes, sectors;
    DWORD f, t;
    long long unsigned clsize;
    long long unsigned total;
    long long unsigned remaining;
    long long unsigned used;
    for(char *it = cbuf; *it != '\0';)
    {
        GetDiskFreeSpaceA(it, &sectors, &bytes, &f, &t);
        clsize = (long long unsigned)bytes * sectors;
        total = t * clsize;
        remaining = f * clsize;
        used = total - remaining;
        fprintf(file, "%s %I64u %I64u %I64u\n", it, used, remaining, total);
        for(; *it != '\0'; ++it);
        ++it;
    }
    fclose(file);
    return 0;
}