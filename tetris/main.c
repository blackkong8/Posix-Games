
typedef enum
{
    False,
    True
} bool;

bool isGameRun;

void Init()
{
    isGameRun = True;
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