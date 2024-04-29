#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char expected[] = "password";
    char user[9];
    char pass[9];

    strcpy (user, argv[1]);
    strcpy (pass, argv[2]);

    if (!strcmp (pass, expected))
    {
        printf ("user %s logged in with password '%s'\n",
                user, pass);
    }

    return 0;
}

