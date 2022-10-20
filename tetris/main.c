#include <termios.h>
#include <unistd.h>

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

struct termios orig_termios;

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
}

void Render()
{
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