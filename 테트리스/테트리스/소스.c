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

int rotate_block();      //��� ȸ��
char is_crash(int d, int s);           //������ �� �浹����
char is_FullLine();     //���ٿϼ��Ǻ�
int block_down(int r, int d, int s);        //��ϳ�����
int input();       //������Է�
char print();
int create_block();     //�������� ��ϻ���
char print_area();      //ȭ�����
void area_init(int r, int d, int s);//ȭ�� �ʱ�ȭ
void area_change();     //���� ���̸� ���Ÿ� ����ιٲٱ�
void r_area_change();      //�߸��� ���̸� ���縦 �������� ���ŷ� �ٲٱ�
int block_side(int r, int d, int s);    //��� �������̵�
int is_crash2(int d, int s);    //������ �̵��Ҷ� �浹����
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
            if (j == 0 || j == AREA_COL + 1)      //�翷 �� �� 9�� ����
            {
                area[i][j] = 9, now_area[i][j] = 9;
            }
            else if (i == AREA_ROW)     //�� �� 1�μ��� -> �浹�������� ��ϸ��߱�
            {
                area[i][j] = 1, now_area[i][j] = 1;
            }
            else        //������ 0
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
    int r = (rand() % 7);   //�������� 7�� ��� �� �ϳ� ����
    block_change(r);
    return block_down(r, 0, 0);     //��ϳ�����
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
    
    if (is_crash(d, s)==2)    //�翷���� ��Ҵ��� Ȯ��
    {
        r_area_change();    //������ ���������� ����
        print_area();
        if (s > 0)      //�����ʺ��� ������
        {
            return s - 1;   //���� s������ ����
        }
        else    //���ʺ��� ������
        {
            return s + 1;   //���� s������ ����
        }
    }

    else if (is_crash(d, s) == 1)
    {
        print_area();
        printf("dd\n");
        r_area_change();
        return 0;
    }

    area_change();  //�ƴϸ� ���Ÿ� ����� �ٲ��ֱ�
    print_area();   //���
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

    print_area();   //���
    time_t start_time = time(NULL);
    while (1)
    {
        int ans = input();      //������Է¹ޱ�
        if (ans==1||ans==-1)
        {
            area_change();
            area_init(r, d, s);     //�ʱ�ȭ
            s = block_side(r, d, ans + s);  //s�� ���Ϲޱ�
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
        if (now_time - start_time >= 1)     //1�ʵ���
        {
            break;
        }
    }
    if (is_crash(d, s) == 1)   //�ؿ� ������ִ���Ȯ��
    {
        return create_block();  //�������
    }
    //�ùٸ����̸�
    area_change();      //���Ÿ� ����� ����
    area_init(r, d, s);   //�ʱ�ȭ
    print_area();
    return block_down(r, d + 1, s);     //d+1�� ��ĭ�� ������
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
    {        //Ű���� �Է� Ȯ�� (true / false)
        char c = _getch();      // ����Ű �Է½� 224 00�� ������ �Ǳ⿡ �տ� �ִ� �� 224�� ����
        if (c == -32)
        {
            c = _getch();  // ���� �Է°��� �Ǻ��Ͽ� �����¿� ���
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