#include <stdio.h>
#include <stdlib.h>

/*  Check a PIN.  Negative value indicates
    Already validated */
void check (int pin)
{
    if (pin < 0 || pin == 91234)
        printf ("PIN correct!\n");
    else
        printf ("Bad PIN\n");
}

int main (int argc, char *argv[])
{
    int pin = atoi (argv[1]);

    if (pin < 0)
        fprintf (stderr, "Positive numbers only!\n");
    else
        check (pin+90000);

    return 0;
}
