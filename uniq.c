#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];
char currline[512];
char nextline[512]; //! FIX: CANNOT HANDLE LONGER LINES

//-c flag --> prefix==1, otherwise prefix==0
//-i flag --> ignore==1, otherwise ignore==0
void uniq(int fd, int prefix, int ignore)
{
    int n, j;

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        int start = 0;
        while (start < n)
        {
            // searches for the first occurrence of '\n'
            char *ptr = strchr(buf + start, '\n');
            if (!ptr)
                ptr = buf + n - 1;
            if (ptr)
            {
                // copy into nextline
                int ind = 0;
                memset(nextline, '\0', sizeof(nextline));
                for (j = start; j <= ptr - buf; j++, ind++)
                {
                    nextline[ind] = buf[j];
                }
                // compare currline and nextline
                if (strcmp(nextline, currline) != 0)
                {
                    // if they are different, print currline to stout
                    printf(1, "%s", currline);
                    // nextline becomes currline
                    strcpy(currline, nextline);
                }
                start = ptr - buf + 1;
            }
        }
        // print the last line
        printf(1, "%s\n", currline);
    }

    if (n < 0)
    {
        printf(1, "uniq: read error\n");
        exit();
    }
}

// print duplicate lines, called when -d flag is up
// TODO
void duplicate(int fd, int ignore)
{
    int n, j;

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        int start = 0;
        while (start < n)
        {
            // searches for the first occurrence of '\n'
            char *ptr = strchr(buf + start, '\n');
            if (!ptr)
                ptr = buf + n - 1;
            if (ptr)
            {
                // copy into nextline
                int ind = 0;
                memset(nextline, '\0', sizeof(nextline));
                for (j = start; j <= ptr - buf; j++, ind++)
                {
                    nextline[ind] = buf[j];
                }
                // compare currline and nextline
                if (strcmp(nextline, currline) != 0)
                {
                    // if they are different, print currline to stout
                    printf(1, "%s", currline);
                    // nextline becomes currline
                    strcpy(currline, nextline);
                }
                start = ptr - buf + 1;
            }
        }
        // print the last line
        printf(1, "%s\n", currline);
    }

    if (n < 0)
    {
        printf(1, "uniq: read error\n");
        exit();
    }
}

int main(int argc, char *argv[])
{
    int fd, i;

    if (argc <= 1)
    {
        uniq(0, 0, 0);
        exit();
    }

    for (i = 1; i < argc; i++)
    {
        if ((fd = open(argv[i], 0)) < 0)
        {
            printf(1, "uniq: cannot open %s\n", argv[i]);
            exit();
        }
        uniq(fd, 0, 0);
        duplicate(fd, 0);
        close(fd);
    }

    exit();
}