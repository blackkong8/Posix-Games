# Posix에서 동작하는 Snake 게임
## includes
* <termios.h>
* <stdlib.h>
* <unistd.h>
* <string.h>
* <stdio.h>
## 동작
키보드 [ w a s d ]를 이용하여 조작할수 있습니다.
## World
기본 크기는 10x10 입니다.

[main.c 190번째 줄](https://github.com/blackkong8/Posix-Games/blob/b584dd9b1fdae8f7952e64d7507d8cd597cd61e0/snake/main.c#L190)을 통해 수정할수 있습니다.
```
World = (struct world){10, 10, 10*10};
```
구조는 ```{가로, 세로, 가로 * 세로}```와 같습니다.
## 인게임 스크린샷
![screenshot](https://github.com/blackkong8/Posix-Games/blob/master/snake/screenshot.png?raw=true)