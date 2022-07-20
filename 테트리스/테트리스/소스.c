#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define AREA_ROW 20 
#define AREA_COL 10
#define BLOCK_ROW 4
#define BLOCK_COL 4

#define CENTER 4

#define LEFT 75
#define RIGHT 77


int area[AREA_ROW + 1][AREA_COL + 6];
int now_area[AREA_ROW + 1][AREA_COL + 6];

int block[7][BLOCK_ROW][BLOCK_COL] = {
    {
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
    } ,
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    }
};

int now_block[BLOCK_ROW][BLOCK_COL];
int fast_block[BLOCK_ROW][BLOCK_COL];

int rotate_block();      //블록 회전
char is_crash(int d, int s);           //내려갈 때 충돌판정
char is_FullLine();     //한줄완성판별
int block_down(int r, int d, int s);        //블록내리기
int input();       //사용자입력
char print();
int create_block();     //랜덤으로 블록생성
char print_area();      //화면출력
void area_init(int r, int d, int s);//화면 초기화
void area_change();     //옳은 값이면 과거를 현재로바꾸기
void r_area_change();      //잘못된 값이면 현재를 옳은값인 과거로 바꾸기
int block_side(int r, int d, int s);    //블록 옆으로이동
int is_crash2(int d, int s);    //옆으로 이동할때 충돌판정
int block_change();
int block_change2();
int r_block_change();


void area_init(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {

            if (area[i][j + 4 + s] == now_block[i - d][j])
            {
                area[i][j + 4 + s] = 0;
            }
        }
    }
}
char print_area()
{
    //system("cls");
    printf("\n\n\n\n");
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            printf("%d", now_area[i][j]);
        }
        printf("\n");
    }
}

char create_area()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            if (j == 0 || j == AREA_COL + 1)      //양옆 맨 끝 9로 설정
            {
                area[i][j] = 9, now_area[i][j] = 9;
            }
            else if (i == AREA_ROW)     //맨 밑 1로설정 -> 충돌판정으로 블록멈추기
            {
                area[i][j] = 1, now_area[i][j] = 1;
            }
            else        //나머지 0
            {
                area[i][j] = 0, now_area[i][j] = 0;
            }
        }
    }


    print_area();
}
int create_block()
{
    r_area_change();
    int r = (rand() % 7);   //랜덤으로 7개 블록 중 하나 결정
    block_change(r);
    return block_down(r, 0, 0);     //블록내리기
}

int block_side(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
        area[i][j + 4 + s] += now_block[i - d][j];
        }
    }
    
    if (is_crash(d, s)==2)    //양옆끝에 닿았는지 확인
    {
        r_area_change();    //닿으면 이전값으로 변경
        print_area();
        if (s > 0)      //오른쪽벽에 닿으면
        {
            return s - 1;   //이전 s값으로 변경
        }
        else    //왼쪽벽에 닿으면
        {
            return s + 1;   //이전 s값으로 변경
        }
    }

    else if (is_crash(d, s) == 1)
    {
        print_area();
        printf("dd\n");
        r_area_change();
        return 0;
    }

    area_change();  //아니면 과거를 현재로 바꿔주기
    print_area();   //출력
    return s;
}



int block_down(int r, int d, int s)
{
    printf("%d\n", r);
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[i][j + 4 + s] += now_block[i - d][j];
        }
    }

    print_area();   //출력
    time_t start_time = time(NULL);
    while (1)
    {
        int ans = input();      //사용자입력받기
        if (ans==1||ans==-1)
        {
            area_change();
            area_init(r, d, s);     //초기화
            s = block_side(r, d, ans + s);  //s값 리턴받기
        }
        else if (ans == 2)
        {
            area_change();
            area_init(r, d, s);
            block_change2();
            rotate_block();
            for (int i = d; i < BLOCK_ROW + d; i++)
            {
                for (int j = 0; j < BLOCK_COL; j++)
                {
                    area[i][j + 4 + s] += now_block[i - d][j];
                }
            }
            if (is_crash(d,s) == 2)
            {
                r_area_change();
                r_block_change();
            }
        }
        time_t now_time = time(NULL);
        if (now_time - start_time >= 1)     //1초동안
        {
            break;
        }
    }
    if (is_crash(d, s) == 1)   //밑에 블록이있는지확인
    {
        return create_block();  //다음블록
    }
    //올바른값이면
    area_change();      //과거를 현재로 변경
    area_init(r, d, s);   //초기화
    print_area();
    return block_down(r, d + 1, s);     //d+1로 한칸씩 내리기
}




char is_crash(int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            if (area[i][j + 4 + s] == 2)
            {
                return 1;
            }
            else if (area[i][j + 4 + s] == 10)
            {
                return 2;
            }
        }
    }
    return 0;
}



void area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            now_area[i][j] = area[i][j];
        }
    }
}

void r_area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            area[i][j] = now_area[i][j];
        }
    }
}

int input()
{
    if (_kbhit())
    {        //키보드 입력 확인 (true / false)
        char c = _getch();      // 방향키 입력시 224 00이 들어오게 되기에 앞에 있는 값 224를 없앰
        if (c == -32)
        {
            c = _getch();  // 새로 입력값을 판별하여 상하좌우 출력
            switch (c)
            {
            case LEFT:
                return -1;
            case RIGHT:
                return 1;
            case 72:
                return 2;
            }
        }
    }
    return 0;
}

int block_change(int r)
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = 0;
            now_block[i][j] = block[r][i][j];
        }
    }
}

int rotate_block()
{
    int tmp[BLOCK_ROW][BLOCK_COL];
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            tmp[i][j] = now_block[i][j];
        }
    }
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j]=tmp[BLOCK_COL-1-j][i];
        }
    }
}

int block_change2()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            fast_block[i][j]= now_block[i][j];
        }
    }
}
int r_block_change()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = fast_block[i][j];
        }
    }
}


int main()
{
    srand(time(NULL));
    /*for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            printf("%c", block7[i][j]);
        }
        printf("\n");
    }*/
    create_area();
    create_block();
}