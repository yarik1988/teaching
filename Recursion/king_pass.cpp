#include <iostream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;
struct info
{
    int val;
    int prev_x;
    int prev_y;
};

info board[8][8];

bool check(int x,int y)
{
    return (x>=0&&x<8&&y>=0&&y<8);
}


void print_board()
{
    for (int i=0;i<8;i++)
        {
                for (int j=0;j<8;j++)
                cout<<setw(4)<<board[i][j].val<<" ";
                cout<<endl;
        }

}

void print_board_prev_x()
{
        cout<<endl;
    for (int i=0;i<8;i++)
        {
                for (int j=0;j<8;j++)
                cout<<setw(4)<<board[i][j].prev_x<<" ";
                cout<<endl;
        }

}

void print_board_prev_y()
{
        cout<<endl;
    for (int i=0;i<8;i++)
        {
                for (int j=0;j<8;j++)
                cout<<setw(4)<<board[i][j].prev_y<<" ";
                cout<<endl;
        }

}

void shortest_path(int px, int py, int val)
{
if (!check(px,py)) return;
board[px][py].val=val;
for (int x=-1;x<=1;x++)
 for (int y=-1;y<=1;y++)
{
      if (x==0&&y==0) continue;
      if (check(px+x,py+y)&&(board[px+x][py+y].val>val+1||board[px+x][py+y].val==0)&&board[px+x][py+y].val!=-1)
        {
            board[px+x][py+y].prev_x=px;
            board[px+x][py+y].prev_y=py;
            shortest_path(px+x,py+y,val+1);
        }
}

}

int main()
{
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++)
        {
                       if (rand()%3==0) board[i][j].val=-1;
                            else board[i][j].val=0;
                       board[i][j].prev_x=0;
                       board[i][j].prev_y=0;
        }
    board[7][7].val=0;
    print_board();
    cout<<endl;

    shortest_path(0,0,1);
    print_board();


    print_board_prev_x();
    print_board_prev_y();

    if (board[7][7].val==0) cout<<"Impossible";
        else
    {
        int cur_x=7;
        int cur_y=7;
        while (cur_x!=0||cur_y!=0)
        {
            cout<<cur_x<<" "<<cur_y<<endl;
            info tmp=board[cur_x][cur_y];
            cur_x=tmp.prev_x;
            cur_y=tmp.prev_y;
        }
    }


    return 0;
}
