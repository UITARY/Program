#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void A()
{
    printf("  /\\ \n");
    printf(" /--\\\n");
    printf("/    \\\n");
}

void B()
{
    printf("|°°°°)\n");
    printf("|---(\n");
    printf("|____)\n");
}

void C()
{
    printf(" .~°~.\n");
    printf("/\n");
    printf("\\    . \n");
    printf(" `°°°\n");
}

void D()
{
    printf("|°°°°\\\n");
    printf("|    |\n");
    printf("|____/ \n");
}

void E()
{
    printf("|°°°°°\n");
    printf("|----   \n");
    printf("|_____\n");
}

void F()
{
    printf("|°°°°°\n");
    printf("|----   \n");
    printf("|\n");
}

void G()
{
    printf(" .~°~.\n");
    printf("/\n");
    printf("\\ °°°•\n");
    printf(" `°°°\n");
}

void H()
{
    printf("|    |\n");
    printf("|----|  \n");
    printf("|    |\n");
}

void I()
{
    printf("°°T°°°\n");
    printf("  | \n");
    printf("__|___\n");
}

void J()
{
    printf("°°°T°°\n");
    printf("   | \n");
    printf("\\__/\n");
}

void K()
{
    printf("|   /\n");
    printf("|-~(\n");
    printf("|   \\\n");
}

void L()
{
    printf("|\n");
    printf("| \n");
    printf("|_____\n");
}

void M()
{
    printf("|\\  /| \n");
    printf("| \\/ |   \n");
    printf("|    |\n");
}

void N()
{
    printf("|\\   | \n");
    printf("| \\  |   \n");
    printf("|  \\ |\n");
}

void O()
{
    printf(" .~°~.\n");
    printf("/     \\\n");
    printf("\\     /\n");
    printf(" `°~°`\n");
}

void P()
{
    printf("|°°°°\\\n");
    printf("|____/\n");
    printf("|\n");
}

void Q()
{
    printf(" .~°~.\n");
    printf("/     \\\n");
    printf("\\   \\ /\n");
    printf(" `°~°`\n");
}

void R()
{
    printf("|°°°°\\\n");
    printf("|____/\n");
    printf("|    \\  \n");
}

void S()
{
    printf(" .~°~.\n");
    printf("(____\n");
    printf(".    )\n");
    printf(" `°~°\n");
}

void T()
{
    printf("°°T°°°\n");
    printf("  | \n");
    printf("  |\n");
}

void U()
{
    printf("|    |\n");
    printf("|    |\n");
    printf("\\____/ \n");
}

void V()
{
    printf("\\    / \n");
    printf(" \\  / \n");
    printf("  \\/   \n");
}

void W()
{
    printf("|  |  |  \n");
    printf("|  |  |\n");
    printf("\\_/ \\_/  \n");
}

void X()
{
    printf("\\   / \n");
    printf(" \\ / \n");
    printf(" / \\ \n");
    printf("/   \\\n");
}

void Y()
{
    printf("\\   / \n");
    printf(" \\ / \n");
    printf("  |\n");
    printf("  |\n");
}

void Z()
{
    printf("°°°°/\n");
    printf("   /\n");
    printf("  /\n");
    printf(" /___ \n");
}

int main()
{
    char word[100];

    while (1)
    {
        system("clear");
        printf("Enter a word/phrase: ");
        scanf("%99s", word);

        for (int i = 0; i < strlen(word); i++)
        {
            char ch = toupper(word[i]);

            switch (ch)
            {
            case 'A':
                A();
                break;
            case 'B':
                B();
                break;
            case 'C':
                C();
                break;
            case 'D':
                D();
                break;
            case 'E':
                E();
                break;
            case 'F':
                F();
                break;
            case 'G':
                G();
                break;
            case 'H':
                H();
                break;
            case 'I':
                I();
                break;
            case 'J':
                J();
                break;
            case 'K':
                K();
                break;
            case 'L':
                L();
                break;
            case 'M':
                M();
                break;
            case 'N':
                N();
                break;
            case 'O':
                O();
                break;
            case 'P':
                P();
                break;
            case 'Q':
                Q();
                break;
            case 'R':
                R();
                break;
            case 'S':
                S();
                break;
            case 'T':
                T();
                break;
            case 'U':
                U();
                break;
            case 'V':
                V();
                break;
            case 'W':
                W();
                break;
            case 'X':
                X();
                break;
            case 'Y':
                Y();
                break;
            case 'Z':
                Z();
                break;
            default:
                printf("");
            }

            printf("\n");
        }
        sleep(strlen(word));
    }

    return 0;
}
