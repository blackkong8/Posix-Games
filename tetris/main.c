
typedef enum
{
    False,
    True
} bool;

bool isGameRun;



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

    ScrollScreen();
    ClearScreen();
}

void HandleInput()
{
}

void Update()
{
}

void Render()
{
}

void Exit()
{
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