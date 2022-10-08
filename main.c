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
        raw.c_cc[VTIME] = 1;

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        break;

    default:
        break;
    }
}

int main()
{
    isGameRun = True;

    World.width = 10;
    World.height = 10;
    World.size = World.width * World.height;
    World.buffer = (char *)malloc(sizeof(char) * World.size);
    memset(World.buffer, ' ', World.size);

    Player = (struct vector2){1, 1};

    printf("\x1b[2J");

    RawMode(Enable);

    while (isGameRun)
    {
        printf("\x1b[H");
        memset(World.buffer, ' ', World.size);

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

        case '\0':
            Player.x += Force.x;
            Player.y += Force.y;

            Player.x = Player.x % World.width ? Player.x : 1;
            Player.y = Player.y % World.height ? Player.y : 1;
            break;

        case 'q':
            isGameRun = False;
            break;

        default:
            break;
        }

        World.buffer[Player.y * World.width + Player.x] = '@';

        for (int i = 0; i < World.size; i++)
        {
            if (i % World.width)
                putchar(World.buffer[i]);
            else
                printf("\r\n");
        }
    }

    RawMode(Disable);

    return 0;
}
