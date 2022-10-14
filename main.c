#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct world
{
    int width, height, size;
    char *buffer;
};

struct vector2
{
    int x, y;
};

typedef enum
{
    Disable,
    Enable
} state;

typedef enum
{
    False,
    True
} bool;

struct termios orig_termios;

struct world World;
struct vector2 Player;
struct vector2 Force;
struct vector2 Apple;

bool isGameRun;

void RawMode(state State)
{
    switch (State)
    {
    case Disable:
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        break;

    case Enable:
        tcgetattr(STDIN_FILENO, &orig_termios);

        struct termios raw = orig_termios;
        raw.c_iflag &= ~(
            ICRNL |                 // Fix Ctrl-M
            IXON |                  // Disable Ctrl-S, Ctrl-Q
            BRKINT | INPCK | ISTRIP // ETC...
        );
        raw.c_oflag &= ~(
            OPOST // Turn off all Output Processing
        );
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(
            ECHO |   // ECHO off
            ICANON | // Reading Input byte-by-byte
            IEXTEN | // Disable Ctrl-V
            ISIG     // Disable Ctrl-C, Ctrl-Z
        );
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 2;

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        break;

    default:
        break;
    }
}

int main()
{
    int x, y;

    isGameRun = True;

    int score = 0;

    World = (struct world){10, 10, 10*10};
    World.buffer = (char *)malloc(sizeof(char) * World.size);
    memset(World.buffer, 0, World.size);

    Player = (struct vector2){1, 0};
    Apple = (struct vector2){rand() % World.height + 1, rand() % World.width};

    World.buffer[Apple.y * World.width + Apple.x] = -2;

    printf("\x1b[2J");

    RawMode(Enable);

    while (isGameRun)
    {
        printf("\x1b[H");
        // memset(World.buffer, 0, World.size);

        char c = '\0';
        read(STDIN_FILENO, &c, 1);

        switch (c)
        {
        case 'a':
            Force.x = -1;
            Force.y = 0;
            break;

        case 'd':
            Force.x = 1;
            Force.y = 0;
            break;

        case 'w':
            Force.y = -1;
            Force.x = 0;
            break;

        case 's':
            Force.y = 1;
            Force.x = 0;
            break;

        case 'q':
            isGameRun = False;
            break;

        default:
            break;
        }

        x = Player.x;
        y = Player.y;

        Player.x += Force.x;
        Player.y += Force.y;

        if (!(1 <= Player.x && Player.x < World.width))
        {
            Player.x = x;
        }

        if (!(0 <= Player.y && Player.y < World.height))
        {
            Player.y = y;
        }

        if (World.buffer[Player.y * World.width + Player.x] <= 0)
        {
            if (World.buffer[Player.y * World.width + Player.x] == -2)
            {
                score += 1;
                Apple = (struct vector2){rand() % World.height + 1, rand() % World.width};
                World.buffer[Apple.y * World.width + Apple.x] = -2;
            }
            World.buffer[Player.y * World.width + Player.x] = -1;
        }
        else
        {
            isGameRun = False;
            break;
        }

        for (int i = 0; i < World.size; i++)
        {
            if (i % World.width)
            {
                switch (World.buffer[i])
                {
                case -2:
                    putchar('@');
                    break;
                case -1:
                    putchar('%');
                    if (Force.x != 0 || Force.y != 0)
                        World.buffer[i] = score;
                    break;
                case 0:
                    putchar(' ');
                    break;
                default:
                    putchar('*');
                    World.buffer[i]--;
                    break;
                }
            }
            else
                printf("\r\n");
        }
        printf("\r\nPlayer\tx: %03d y: %03d", Player.x, Player.y);
        printf("\r\nApple\tx: %03d y: %03d", Apple.x, Apple.y);
        printf("\r\nScore\t%d", score);
    }

    printf("\x1b[H");

    RawMode(Disable);

    printf("Score:\t%d\n", score);

    return 0;
}
