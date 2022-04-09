#include "WindowConsole.h"
#include<iostream>
#include<queue>
#include <cstdlib>
#include <ctime>
#include <string>
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
using namespace std;
struct position
{
    int x;
    int y;
};
struct pokemon
{
    char value = '+';
    position pos;
    position pre_pos;
    int turn = 0;
    bool status = 1;
};
void Push(queue <pokemon> q[144], int size, int i, int j, pokemon temp)
{
    if (temp.status == 0)
    {
        temp.pre_pos.x = i;
        temp.pre_pos.y = j;
        q[i * size + j].push(temp);
        //	cout << i << " and " << j << " pos " << temp.pos.x << " " << temp.pos.y << endl;
    }
}
void Build(pokemon** board, int size, queue <pokemon> q[144])
{

    for (int i = 0; i < size + 2; i++)
    {
        for (int j = 0; j < size + 2; j++)
        {
            //	cout << i << " " << j;
            if (i - 1 >= 0)
            {
                Push(q, size + 2, i, j, board[i - 1][j]);
                //		cout << "True1";
            }
            if (i + 1 <= size + 1)
            {
                Push(q, size + 2, i, j, board[i + 1][j]);
                //		cout << "True2";
            }
            if (j - 1 >= 0)
            {
                Push(q, size + 2, i, j, board[i][j - 1]);
                //		cout << "True3";
            }
            if (j + 1 <= size + 1)
            {
                Push(q, size + 2, i, j, board[i][j + 1]);
                //		cout << "True4";
            }
            //cout << endl;
        }
    }
}
int Turn(int x, int y, pokemon p)
{
    if (x == p.pos.x || y == p.pos.y) return 0;
    else return 1;
}
pokemon** GenerateBoard(int size)
{
    pokemon** board = new pokemon * [size + 2];
    for (int i = 0; i < size + 2; i++)
        board[i] = new pokemon[size + 2];
    //  Create the border
    for (int i = 0; i < size + 2; i++)
        for (int j = 0; j < size + 2; j++)
        {
            board[i][j].value = char(43);
            board[i][j].pos.x = i;
            board[i][j].pos.y = j;
            if (i == 0 || j == 0 || i == size + 1 || j == size + 1)
                board[i][j].status = 0;
        }

    srand((int)time(0));
    // Create board
    int count = 0;
    while (count != size * size)
    {
        int column, row;
        char a = char('A' + char(rand() % 26));
        for (int i = 0; i < 2; i++)
        {
            do
            {
                column = rand() % size + 1;
                row = rand() % size + 1;

            } while (int(board[column][row].value) != 43);
            board[column][row].value = a;
            count++;
        }
    }
    return board;
}
bool Matching(pokemon**& board, int size, int I_x, int I_y, int F_x, int F_y)
{
    if (board[I_x][I_y].status == 0 || board[F_x][F_y].status == 0) return 0;
    if (board[I_x][I_y].value != board[F_x][F_y].value || (I_x == F_x && I_y == F_y))
    {
        //	cout << a[I_x * n + I_y]<<"  "<< a[F_x * n + F_y]<<"False";
        return 0;
    }
    board[F_x][F_y].status = 0;

    queue<pokemon> q[144];
    Build(board, size, q);
    /*	for (int i = 0; i < (size + 2); i++)
        {
            for (int j = 0; j < (size + 2); j++)
            {
                cout << q[i * (size + 2) + j].size() << " ";
            }
            cout << endl;
        }
        cout << endl<<"Queue\n";
        for (int i = 0; i < (size + 2); i++)
        {
            for (int j = 0; j < (size + 2); j++)
            {
                int n= q[i * (size + 2) + j].size() ;
                printf("Queue [%d][%d]:\n", i, j);
                for (int k = 0; k < n; k++)
                {
                    cout << q[i * (size + 2) + j].front().pos.x << " " << q[i * (size + 2) + j].front().pos.y << endl;
                    q[i * (size + 2) + j].pop();
                }
            }
            cout << endl;
        }
        Build(board, size, q);*/
    int* dd = new int[(size + 2) * (size + 2)];
    for (int i = 0; i < (size + 2) * (size + 2); i++) dd[i] = 0;
    int check = 0;
    //cout << I_x<<" "<< I_y<<" "<< q[I_x * (size + 2) + I_y].size() << endl;
    while (!q[I_x * (size + 2) + I_y].empty())
    {
        pokemon temp = q[I_x * (size + 2) + I_y].front();
        if (!temp.status)
        {
            //	cout << temp.pos.x << " " << temp.pos.y << endl;
            if (temp.pos.x == F_x && temp.pos.y == F_y && temp.turn <= 2)
            {
                board[I_x][I_y].status = 0;
                return 1;
            }
            q[I_x * (size + 2) + I_y].pop();

            int u = temp.pos.x;
            int v = temp.pos.y;
            int pre_x = temp.pre_pos.x;
            int pre_y = temp.pre_pos.y;
            dd[u * (size + 2) + v] = 1;
            int N = q[u * (size + 2) + v].size();

            //		cout << N<<endl;
            for (int i = 0; i < N; i++)
            {
                pokemon p = q[u * (size + 2) + v].front();
                //	cout << u << " and " << v << " pos " << p.pos.x << " " << p.pos.y << endl;
                if (dd[p.pos.x * (size + 2) + p.pos.y] == 0)
                {

                    if (p.status == 0)
                    {
                        p.turn = temp.turn + Turn(pre_x, pre_y, p);
                        //	cout << pre_x << " " << pre_y << " " << u << " and " << v << " pos " << p.pos.x << " " << p.pos.y << " turn " << p.turn << endl;
                        if (p.turn <= 2)
                        {
                            /*if (p.pos.x == F_x && p.pos.y == F_y)
                            {
                                board[F_x][F_y].status = 0;
                                return 1;
                            }*/
                            q[I_x * (size + 2) + I_y].push(p);
                        }
                    }

                }
                q[u * (size + 2) + v].pop();
            }
        }
    }
    delete[] dd;
    board[F_x][F_y].status = 1;
    return 0;
}
void UpdateBoard(pokemon** board, int size, int count)
{
    const int COUNT = count;
    count = 0;
    position* Pos = new position[size * size];
    for (int i = 1; i < size + 1; i++)
    {
        for (int j = 1; j < size + 1; j++)
        {
            if (board[i][j].status != 0)
            {
                Pos[count] = board[i][j].pos;
                cout << Pos[count].x << " " << Pos[count].y << endl;
                count++;
            }
        }
    }
    srand((int)time(0));
    int n = rand() % 100;
    cout << n << " u and v\n";
    for (int i = 0; i < n; i++)
    {
        int u = rand() % COUNT;
        int v = u;
        do
        {
            v = rand() % COUNT;
        } while (v == u);
        cout << u << " " << v << endl;
        char temp = board[Pos[u].x][Pos[u].y].value;
        board[Pos[u].x][Pos[u].y].value = board[Pos[v].x][Pos[v].y].value;
        board[Pos[v].x][Pos[v].y].value = temp;
    }
}
bool CheckVecinity(pokemon** board, int size, pokemon p, pokemon I)
{
    int x = p.pos.x;
    int y = p.pos.y;
    int pre_x = p.pre_pos.x;
    int pre_y = p.pre_pos.y;
    //int pre_flag;
//	cout << p.value<<" "<<x<<" "<<y << endl;
//	cout << I.value<<endl;
    if (y - 1 >= 0)
    {
        //	cout << x << " " << y - 1 << " " << board[x][y - 1].value << " "
        //		<< Turn(x, y - 1, board[pre_x][pre_y]) << endl;
        if (board[x][y - 1].value == I.value && p.turn + Turn(x, y - 1, board[pre_x][pre_y]) <= 2
            && pre_x != x && pre_y != y - 1)
            return 1;
    }
    if (y + 1 <= size + 1)
    {
        {
            //	cout << x << " " << y + 1 << " " << board[x][y - 1].value << " "
            //		<< Turn(x, y + 1, board[pre_x][pre_y]) << endl;
            if (board[x][y + 1].value == I.value && p.turn + Turn(x, y + 1, board[pre_x][pre_y]) <= 2
                && pre_x != x && pre_y != y + 1)
                return 1;
        }
    }
    if (x - 1 >= 0)
    {
        {
            //	cout << x -1 << " " << y << " " << board[x - 1][y].value << " "
            //		<< Turn(x - 1, y , board[pre_x][pre_y]) << endl;
            if (board[x - 1][y].value == I.value && p.turn + Turn(x - 1, y, board[pre_x][pre_y]) <= 2
                && pre_x != x - 1 && pre_y != y)
                return 1;
        }
    }
    if (x + 1 <= size + 1)
    {
        {
            //	cout << x + 1 << " " << y << " " << board[x + 1][y].value << " "
            //		<< Turn(x + 1, y , board[pre_x][pre_y]) << endl;
            if (board[x + 1][y].value == I.value && p.turn + Turn(x + 1, y, board[pre_x][pre_y]) <= 2
                && pre_x != x + 1 && pre_y != y)
                return 1;
        }
    }
    return 0;
    /*if (!(pre_x == x - 1 && pre_y == y))
    {
        if ((board[x][y - 1].value == temp.value && p.turn + Turn(x, y - 1, board[pre_x][pre_y]) <= 2)
            || board[x][y + 1].value == temp.value && p.turn + Turn(x, y + 1, board[pre_x][pre_y]) <= 2
            || board[x + 1][y].value == temp.value && p.turn + Turn(x + 1, y, board[pre_x][pre_y]) <= 2)
            return 1;
    }
    if (!(pre_x == x + 1 && pre_y == y))
    {
        if ((board[x][y - 1].value == temp.value && p.turn + Turn(x, y - 1, board[pre_x][pre_y]) <= 2)
            || board[x][y + 1].value == temp.value && p.turn + Turn(x, y + 1, board[pre_x][pre_y]) <= 2
            || board[x - 1][y].value == temp.value && p.turn + Turn(x - 1, y, board[pre_x][pre_y]) <= 2)
            return 1;
    }
    if (!(pre_x == x && pre_y == y + 1))
    {
        if ((board[x - 1][y].value == temp.value && p.turn + Turn(x-1, y, board[pre_x][pre_y]) <= 2)
            || board[x + 1][y].value == temp.value && p.turn + Turn(x+1, y , board[pre_x][pre_y]) <= 2
            || board[x][y - 1].value == temp.value && p.turn + Turn(x, y-1, board[pre_x][pre_y]) <= 2)
            return 1;
    }
    if (pre_x == x && pre_y == y - 1)
    {
        if ((board[x - 1][y].value == temp.value && p.turn + Turn(x - 1, y, board[pre_x][pre_y]) <= 2)
            || board[x + 1][y].value == temp.value && p.turn + Turn(x + 1, y, board[pre_x][pre_y]) <= 2
            || board[x][y + 1].value == temp.value && p.turn + Turn(x, y + 1, board[pre_x][pre_y]) <= 2)
            return 1;
    }*/
}
bool Hint_pair(pokemon** board, int size, int I_x, int I_y)
{
    if (!board[I_x][I_y].status) return 0;
    queue<pokemon> q[144];
    Build(board, size, q);
    if (I_y - 1 >= 0)
    {
        //	cout << x << " " << y - 1 << " " << board[x][y - 1].value << " "
        //		<< Turn(x, y - 1, board[pre_x][pre_y]) << endl;
        if (board[I_x][I_y - 1].value == board[I_x][I_y].value)
        {
            cout << I_x << " " << I_y << " with " << I_x << " " << I_y - 1 << endl;
            return 1;
        }
    }
    if (I_y + 1 <= size + 1)
    {
        {
            //	cout << x << " " << y + 1 << " " << board[x][y - 1].value << " "
            //		<< Turn(x, y + 1, board[pre_x][pre_y]) << endl;
            if (board[I_x][I_y + 1].value == board[I_x][I_y].value)
            {
                cout << I_x << " " << I_y << " with " << I_x << " " << I_y + 1 << endl;
                return 1;
            }
        }
    }
    if (I_x - 1 >= 0)
    {
        {
            //	cout << x -1 << " " << y << " " << board[x - 1][y].value << " "
            //		<< Turn(x - 1, y , board[pre_x][pre_y]) << endl;
            if (board[I_x - 1][I_y].value == board[I_x][I_y].value)
            {
                cout << I_x << " " << I_y << " with " << I_x -1 << " " << I_y << endl;
                return 1;
            }
        }
    }
    if (I_x + 1 <= size + 1)
    {
        {
            //	cout << x + 1 << " " << y << " " << board[x + 1][y].value << " "
            //		<< Turn(x + 1, y , board[pre_x][pre_y]) << endl;
            if (board[I_x + 1][I_y].value == board[I_x][I_y].value)
            {
                cout << I_x << " " << I_y << " with " << I_x + 1 << " " << I_y << endl;
                return 1;
            }
        }
    }
    /*	for (int i = 0; i < (size + 2); i++)
        {
            for (int j = 0; j < (size + 2); j++)
            {
                cout << q[i * (size + 2) + j].size() << " ";
            }
            cout << endl;
        }
        cout << endl<<"Queue\n";
        for (int i = 0; i < (size + 2); i++)
        {
            for (int j = 0; j < (size + 2); j++)
            {
                int n= q[i * (size + 2) + j].size() ;
                printf("Queue [%d][%d]:\n", i, j);
                for (int k = 0; k < n; k++)
                {
                //	cout << q[i * (size + 2) + j].front().pos.x << " " << q[i * (size + 2) + j].front().pos.y << endl;
                    q[i * (size + 2) + j].pop();
                }
            }
            cout << endl;
        }
        Build(board, size, q);*/
    int* dd = new int[(size + 2) * (size + 2)];
    for (int i = 0; i < (size + 2) * (size + 2); i++) dd[i] = 0;
    int check = 0;
    //cout << I_x<<" "<< I_y<<" "<< q[I_x * (size + 2) + I_y].size() << endl;
//	cout << board[I_x][I_y].value << endl;;
    while (!q[I_x * (size + 2) + I_y].empty())
    {
        pokemon temp = q[I_x * (size + 2) + I_y].front();
        int u = temp.pos.x;
        int v = temp.pos.y;
        //	cout << temp.pos.x << " " << temp.pos.y << " " << temp.turn << endl;
        if (temp.value == board[I_x][I_y].value && temp.turn <= 2)
        {
            //	check= 1; cout <<"Hint"<< temp.pos.x << " " << temp.pos.y << " " << temp.turn << " "
            //		<<temp.value <<" "<< board[I_x][I_y].value << endl;
        }
        q[I_x * (size + 2) + I_y].pop();
        bool t = CheckVecinity(board, size, temp, board[I_x][I_y]);
        if (t)
        {
            //	cout << "CheckVec";
            return 1;
        }
        int pre_x = temp.pre_pos.x;
        int pre_y = temp.pre_pos.y;
        dd[u * (size + 2) + v] = 1;
        int N = q[u * (size + 2) + v].size();

        //		cout << N<<endl;
        for (int i = 0; i < N; i++)
        {
            pokemon p = q[u * (size + 2) + v].front();
            //	cout << u << " and " << v << " pos " << p.pos.x << " " << p.pos.y << endl;
            if (dd[p.pos.x * (size + 2) + p.pos.y] == 0)
            {

                if (temp.status == 0)
                {
                    p.turn = temp.turn + Turn(pre_x, pre_y, p);
                    //	cout << pre_x << " " << pre_y << " " << u << " and " << v << " pos " << p.pos.x << " " << p.pos.y << " turn " << p.turn << endl;
                    if (p.turn <= 2)
                    {
                        /*if (p.pos.x == F_x && p.pos.y == F_y)
                        {
                            board[F_x][F_y].status = 0;
                            return 1;
                        }*/
                        q[I_x * (size + 2) + I_y].push(p);
                    }
                }

            }
            q[u * (size + 2) + v].pop();
        }
    }
    delete[] dd;
    return 0;
}
bool Hint_board(pokemon** board, int size)
{
    for (int I = 1; I < size + 1; I++)
    {
        for (int J = 1; J < size + 1; J++)
        {
            if (board[I][J].status != 0)
            {
                for (int i = 1; i < size + 1; i++)
                {
                    for (int j = 1; j < size + 1; j++)
                    {
                        bool t = Matching(board, size, I, J, i, j);
                        if (t)
                        {
                            board[I][J].status = 1;
                            board[i][j].status = 1;
                            cout << I << " " << J << " with " << i << " " << j << endl;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
void printSquare(int j, int i, char value)
{
    GoTo(5 + j * 10, 1 + i * 5);
    cout << " ------- ";
    GoTo(5 + j * 10, 1 + i * 5 + 1);
    cout << "|       |";
    GoTo(5 + j * 10, 1 + i * 5 + 2);
    cout << "|   " << value << "   |";
    GoTo(5 + j * 10, 1 + i * 5 + 3);
    cout << "|       |";
    GoTo(5 + j * 10, 1 + i * 5 + 4);
    cout << " ------- ";
}
void printEmpty(int j, int i)
{
    GoTo(5 + j * 10, 1 + i * 5);
    cout << "         ";
    GoTo(5 + j * 10, 1 + i * 5 + 1);
    cout << "         ";
    GoTo(5 + j * 10, 1 + i * 5 + 2);
    cout << "         ";
    GoTo(5 + j * 10, 1 + i * 5 + 3);
    cout << "         ";
    GoTo(5 + j * 10, 1 + i * 5 + 4);
    cout << "         ";
}
void printBoard(pokemon**& board, int size)
{
    for (int i = 0; i < size + 1; i++)
    {
        for (int j = 0; j < size + 1; j++)
            if (board[i][j].status)
                printSquare(j, i, board[i][j].value);
    }
}
void printWon()
{
    GoTo(30, 10);
    SetColor(0, 14);
    cout << "   __     __          __          __         ";
    GoTo(30, 11);
    cout << "   \\ \\   / /          \\ \\        / /         ";
    GoTo(30, 12);

    cout << "    \\ \\_/ /__  _   _   \\ \\  /\\  / /__  _ __  ";
    GoTo(30, 13);
    SetColor(0, 15);
    cout << "     \\   / _ \\| | | |   \\ \\/  \\/ / _ \\| '_ \\ ";
    GoTo(30, 14);
    cout << "      | | (_) | |_| |    \\  /\\  / (_) | | | |";
    GoTo(30, 15);
    SetColor(0, 12);
    cout << "      |_|\\___/ \\__,_|     \\/  \\/ \\___/|_| |_|";
    SetColor(0, 7);
}
void playGame(pokemon**& board, int size)
{
    int current_x = 0, current_y = 0;
    int chosen_x = -1, chosen_y = -1;
    int pokemon_unchecked = size * size;
    while (!Hint_board(board, size))
    {
        UpdateBoard(board, size,size*size);
        /*system("pause");
        cout << "chua";*/
    //    printBoard(board, size);
    }
    printBoard(board, size);
//    if (!Hint_board(board, size)) cout << "cac";
//    UpdateBoard(board, size, size * size);
    SetColor(7, 0);
    printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
    SetColor(0, 7);
    int check = 0;
    while (pokemon_unchecked)
    {
        char c = _getch();
        c = tolower(c);
        int temp_x = current_x, temp_y = current_y;
        // printBoard(board, size);
        if (chosen_x != -1 && chosen_y != -1)
        {
            SetColor(9, 0);
            printSquare(chosen_x + 1, chosen_y + 1, board[chosen_y + 1][chosen_x + 1].value);
            SetColor(0, 7);
        }

        if (board[current_y + 1][current_x + 1].status == 0)
            printEmpty(current_x + 1, current_y + 1);

        switch (c)
        {
        case 'a':
            if (current_x - 1 >= 0)
                current_x--;

            if (board[temp_y + 1][temp_x + 1].status == 0)
                printEmpty(temp_x + 1, temp_y + 1);
            else
            {
                SetColor(0, 7);
                printSquare(temp_x + 1, temp_y + 1, board[temp_y + 1][temp_x + 1].value);
            }

            if (board[current_y + 1][current_x + 1].status != 0)
            {
                SetColor(7, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
            }

            break;
        case 'd':
            if (current_x + 1 <= size - 1)
                current_x++;
            if (board[temp_y + 1][temp_x + 1].status == 0)
                printEmpty(temp_x + 1, temp_y + 1);
            else
            {
                SetColor(0, 7);
                printSquare(temp_x + 1, temp_y + 1, board[temp_y + 1][temp_x + 1].value);
            }
            if (board[current_y + 1][current_x + 1].status != 0)
            {
                SetColor(7, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
            }

            break;
        case 's':
            if (current_y + 1 <= size - 1)
                current_y++;

            if (board[temp_y + 1][temp_x + 1].status == 0)
                printEmpty(temp_x + 1, temp_y + 1);
            else
            {
                SetColor(0, 7);
                printSquare(temp_x + 1, temp_y + 1, board[temp_y + 1][temp_x + 1].value);
            }

            if (board[current_y + 1][current_x + 1].status != 0)
            {
                SetColor(7, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
            }

            break;
        case 'w':
            if (current_y - 1 >= 0)
                current_y--;

            if (board[temp_y + 1][temp_x + 1].status == 0)
                printEmpty(temp_x + 1, temp_y + 1);
            else
            {
                SetColor(0, 7);
                printSquare(temp_x + 1, temp_y + 1, board[temp_y + 1][temp_x + 1].value);
            }

            if (board[current_y + 1][current_x + 1].status != 0)
            {
                SetColor(7, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
            }

            break;
        case 13:
            if (board[current_y + 1][current_x + 1].status)
            {
                SetColor(9, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
                break;
            }

            if (chosen_x != -1 && chosen_y != -1)

                if (Matching(board, size, current_y + 1, current_x + 1, chosen_y + 1, chosen_x + 1)
                    && board[current_y + 1][current_x + 1].value == board[chosen_y + 1][chosen_x + 1].value
                    && !(current_x == chosen_x && chosen_y == current_y))
                {
                    Sleep(400);
                    pokemon_unchecked -= 2; 
                    check = 1;
                    SetColor(2, 0);
                    printSquare(chosen_x + 1, chosen_y + 1, board[chosen_y + 1][chosen_x + 1].value);
                    printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                    SetColor(0, 7);
                    printf("%c", 7);

                    Sleep(200);
                    printEmpty(chosen_x + 1, chosen_y + 1);
                    printEmpty(current_x + 1, current_y + 1);

                    board[current_y + 1][current_x + 1].status = board[chosen_y + 1][chosen_x + 1].status = 0;
                    chosen_x = chosen_y = -1;
                    
                    SetColor(7, 0);
                    printEmpty(current_x + 1, current_y + 1);
                    SetColor(0, 7);
                    if (Hint_board(board, size))
                    {
                        UpdateBoard(board, size, pokemon_unchecked);
                    }
                }
                else
                {
                    Sleep(200);

                    SetColor(4, 0);
                    printSquare(chosen_x + 1, chosen_y + 1, board[chosen_y + 1][chosen_x + 1].value);
                    printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                    SetColor(0, 7);
                    printf("%c", 7);

                    Sleep(800);

                    printSquare(chosen_x + 1, chosen_y + 1, board[chosen_y + 1][chosen_x + 1].value);
                    SetColor(7, 0);
                    printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                    SetColor(0, 7);

                    chosen_x = chosen_y = -1;
                }
            else
            {
                chosen_x = current_x;
                chosen_y = current_y;
            }
            break;
        default:
            if (board[current_y + 1][current_x + 1].status != 0)
            {
                SetColor(7, 0);
                printSquare(current_x + 1, current_y + 1, board[current_y + 1][current_x + 1].value);
                SetColor(0, 7);
            }
            else
            {
                SetColor(7, 0);
                printEmpty(current_x + 1, current_y + 1);
                SetColor(0, 7);
            }
            break;
        }
    }
    system("cls");
    printWon();
}
void printPlaySquare()
{
    GoTo(45, 15);
    cout << " =================== ";
    GoTo(45, 16);
    cout << "|                   |";
    GoTo(45, 17);
    cout << "|        PLAY       |";
    GoTo(45, 18);
    cout << "|                   |";
    GoTo(45, 19);
    cout << " =================== ";
}
void printLeaderboardSquare()
{
    GoTo(45, 22);
    cout << " =================== ";
    GoTo(45, 23);
    cout << "|                   |";
    GoTo(45, 24);
    cout << "|    LEADERBOARD    |";
    GoTo(45, 25);
    cout << "|                   |";
    GoTo(45, 26);
    cout << " =================== ";
}
void printNameofGame()
{
    system("cls");
    GoTo(12, 3);
    cout << " _______ _            __  __       _       _     _                _____                      ";
    GoTo(12, 4);
    SetColor(0, 10);
    cout << "|__   __| |          |  \\/  |     | |     | |   (_)              / ____|                     ";
    GoTo(12, 5);
    cout << "   | |  | |__   ___  | \\  / | __ _| |_ ___| |__  _ _ __   __ _  | |  __  __ _ _ __ ___   ___ ";
    GoTo(12, 6);
    SetColor(0, 14);
    cout << "   | |  | '_ \\ / _ \\ | |\\/| |/ _` | __/ __| '_ \\| | '_ \\ / _` | | | |_ |/ _` | '_ ` _ \\ / _ \\";
    GoTo(12, 7);
    cout << "   | |  | | | |  __/ | |  | | (_| | || (__| | | | | | | | (_| | | |__| | (_| | | | | | |  __/";
    GoTo(12, 8);
    SetColor(0, 12);
    cout << "   |_|  |_| |_|\\___| |_|  |_|\\__,_|\\__\\___|_| |_|_|_| |_|\\__, |  \\_____|\\__,_|_| |_| |_|\\___|";
    GoTo(12, 9);
    cout << "                                                          __/ |                              ";
    GoTo(12, 10);
    cout << "                                                         |___/                               ";
}
void printStandardSquare()
{
    GoTo(45, 17);
    cout << " =================== ";
    GoTo(45, 18);
    cout << "|                   |";
    GoTo(45, 19);
    cout << "|      STANDARD     |";
    GoTo(45, 20);
    cout << "|                   |";
    GoTo(45, 21);
    cout << " =================== ";
}
void printDifficultSquare()
{
    GoTo(45, 24);
    cout << " =================== ";
    GoTo(45, 25);
    cout << "|                   |";
    GoTo(45, 26);
    cout << "|     DIFFICULT     |";
    GoTo(45, 27);
    cout << "|                   |";
    GoTo(45, 28);
    cout << " =================== ";
}
void printSizeSquare(int size)
{
    GoTo(45, 10 + (size - 4) * 3);
    cout << " =================== ";
    GoTo(45, 11 + (size - 4) * 3);
    cout << "|                   |";
    GoTo(45, 12 + (size - 4) * 3);
    cout << "|       " << size << " X " << size << "       |";
    GoTo(45, 13 + (size - 4) * 3);
    cout << "|                   |";
    GoTo(45, 14 + (size - 4) * 3);
    cout << " =================== ";
}
int ChooseSize()
{
    int choice = 1;
    SetColor(0, 11);
    GoTo(30, 5);
    cout << " ____ ____ _    ____ ____ ___    ____ _ ___  ____";
    GoTo(30, 6);
    cout << " [__  |___ |    |___ |     |     [__  |   /  |___ ";
    GoTo(30, 7);
    cout << " ___] |___ |___ |___ |___  |     ___] |  /__ |___ ";
    SetColor(7, 0);
    printSizeSquare(4);
    SetColor(0, 7);
    printSizeSquare(6);
    printSizeSquare(8);
    while (true)
    {
        if (choice == 1)
        {
            SetColor(7, 0);
            printSizeSquare(4);
            SetColor(0, 7);
            printSizeSquare(6);
            printSizeSquare(8);
        }
        if (choice == 2)
        {
            SetColor(7, 0);
            printSizeSquare(6);
            SetColor(0, 7);
            printSizeSquare(4);
            printSizeSquare(8);
        }
        if (choice == 3)
        {
            SetColor(7, 0);
            printSizeSquare(8);
            SetColor(0, 7);
            printSizeSquare(4);
            printSizeSquare(6);
        }
        char c = _getch();
        c = tolower(c);
        switch (c)
        {
        case 's':
            if (choice == 1)
            {
                SetColor(7, 0);
                printSizeSquare(6);
                SetColor(0, 7);
                choice = 2;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printSizeSquare(8);
                SetColor(0, 7);
                choice = 3;
                break;
            }
            if (choice == 3)
            {
                SetColor(7, 0);
                printSizeSquare(4);
                SetColor(0, 7);
                choice = 1;
                break;
            }
            break;
        case 'w':
            if (choice == 1)
            {
                SetColor(7, 0);
                printSizeSquare(8);
                SetColor(0, 7);
                choice = 3;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printSizeSquare(4);
                SetColor(0, 7);
                choice = 1;
                break;
            }
            if (choice == 3)
            {
                SetColor(7, 0);
                printSizeSquare(6);
                SetColor(0, 7);
                choice = 2;
                break;
            }
            break;
        case 13:
            if (choice == 1)
            {
                system("cls");
                return 4;
            };
            if (choice == 2)
            {
                system("cls");
                return 6;
            }
            if (choice == 3)
            {
                 system("cls");
                return 8;
            }
            break;
        }
    }
}
void LevelMenu(pokemon**& board, int& size)
{
    printNameofGame();

    SetColor(0, 9);
    GoTo(30, 12);
    cout << "____ ____ _    ____ ____ ___    _  _ ____ ___  ____ ";
    GoTo(30, 13);
    cout << "[__  |___ |    |___ |     |     |\\/| |  | |  \\ |___ ";
    GoTo(30, 14);
    cout << "___] |___ |___ |___ |___  |     |  | |__| |__/ |___ ";
    SetColor(0, 7);

    int choice = 1;
    SetColor(7, 0);
    printStandardSquare();
    SetColor(0, 7);
    printDifficultSquare();
    while (true)
    {
        char c = _getch();
        c = tolower(c);
        switch (c)
        {
        case 's':
            if (choice == 1)
            {
                SetColor(7, 0);
                printDifficultSquare();
                SetColor(0, 7);
                printStandardSquare();
                choice = 2;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printStandardSquare();
                SetColor(0, 7);
                printDifficultSquare();
                choice = 1;
            }
            break;
        case 'w':
            if (choice == 1)
            {
                SetColor(7, 0);
                printDifficultSquare();
                SetColor(0, 7);
                printStandardSquare();
                choice = 2;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printStandardSquare();
                SetColor(0, 7);
                printDifficultSquare();
                choice = 1;
            }
            break;
        case 13:
            if (choice == 1)
            {
                system("cls");
                size = ChooseSize();
                pokemon** board = GenerateBoard(size);
                playGame(board, size);
                return;
            };
            break;
        }
    }
}
void MenuGame(pokemon**& board, int size)
{
    printNameofGame();

    int choice = 1;

    while (true)
    {
        printPlaySquare();
        printLeaderboardSquare();
        if (choice == 1)
        {
            SetColor(7, 0);
            printPlaySquare();
            SetColor(0, 7);
            printLeaderboardSquare();
        }
        if (choice == 2)
        {
            SetColor(7, 0);
            printLeaderboardSquare();
            SetColor(0, 7);
            printPlaySquare();
        }
        char c = _getch();
        c = tolower(c);
        switch (c)
        {
        case 's':
            if (choice == 1)
            {
                SetColor(7, 0);
                printLeaderboardSquare();
                SetColor(0, 7);
                choice = 2;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printPlaySquare();
                SetColor(0, 7);
                choice = 1;
            }
            break;
        case 'w':
            if (choice == 1)
            {
                SetColor(7, 0);
                printLeaderboardSquare();
                SetColor(0, 7);
                choice = 2;
                break;
            }
            if (choice == 2)
            {
                SetColor(7, 0);
                printPlaySquare();
                SetColor(0, 7);
                choice = 1;
            }
            break;
        case 13:
            if (choice == 1)
            {
                system("cls");
                LevelMenu(board, size);
                return;
            }
            break;
        }
    }
}
int main()
{
    int size=0;
    SetWindowSize(80, 50);
    DisableResizeWindow();
    DisableCtrButton(0, 0, 1);
    SetScrollbar(0);
    ShowCur(0);
    SetConsoleTitleW(L"The Matching Game");

    pokemon** board;

    //playGame(board, size);
     MenuGame(board, size);

    delete[] board;

    system("pause");
    return 0;
}