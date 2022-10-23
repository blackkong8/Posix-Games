#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct world
{
    int width, height;
    int size;
    char *buffer;
};

struct block
{
    char *shape;
};

typedef enum
{
    False,
    True
} bool;

typedef enum
{
    Disable,
    Enable
} state;

bool isGameRun;

char NullCode = ' ';

struct termios orig_termios;
struct world MainWorld;

void RawMode(state State)
{
    switch (State)
    {
    case Disable:
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        break;
    case Enable:
        // Init
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        // Set Flags
        raw.c_iflag &= ~(ICRNL | IXON | BRKINT | INPCK | ISTRIP);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_cc[VTIME] = 2;
        raw.c_cc[VMIN] = 0;
        // Set Raw Mode
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        break;
    }
}

void ClearScreen()
{
    printf("\x1b[H");
}

void ScrollScreen()
{
    printf("\x1b[2J");
}

void Init()
{
    isGameRun = True;

    MainWorld = (struct world){10, 10};

    MainWorld.size = MainWorld.width * MainWorld.height;
    MainWorld.buffer = (char *)malloc(sizeof(char) * MainWorld.size);
    memset(MainWorld.buffer, NullCode, MainWorld.size);

    memset(MainWorld.buffer, '@', MainWorld.width);

    RawMode(Enable);

    ScrollScreen();
    ClearScreen();
}

void HandleInput()
{
    char c = '\0';
    read(STDIN_FILENO, &c, 1);

    switch (c)
    {
    case 'q':
        isGameRun = False;
        break;
    default:
        break;
    }
}

void Update()
{
    char *SubBuff;
    SubBuff = (char *)malloc(sizeof(char) * MainWorld.size);
    memset(SubBuff, NullCode, MainWorld.size);
    for (int i = 0; i < MainWorld.size; i++)
    {
        if (i % MainWorld.width)
        {
            if (MainWorld.buffer[i] != NullCode &&
                (int)(i / MainWorld.height) < MainWorld.height &&
                MainWorld.buffer[i + MainWorld.width] == NullCode)
            {
                SubBuff[i + MainWorld.width] = MainWorld.buffer[i];
            }
        }
        else
        {
        }
    }
    free(MainWorld.buffer);
    MainWorld.buffer = SubBuff;
}

void Render()
{
    ClearScreen();
    for (int i = 0; i < MainWorld.size; i++)
    {
        if (i % MainWorld.width)
        {
            putchar(MainWorld.buffer[i]);
        }
        else
        {
            printf("\r\n");
        }
    }
}

void Exit()
{
    RawMode(Disable);
}

int main()
{
    Init();
    while (isGameRun)
    {
        HandleInput();
        Update();
        Render();
    }
    Exit();
    return 0;
}