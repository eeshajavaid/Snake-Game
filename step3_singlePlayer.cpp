#include <Windows.h>
#include <conio.h>
#include <time.h>
#include<iostream>
#include <iomanip>
#include <vector>

using namespace std;

#define Rows 70
#define Cols 70
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77 


void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}


struct Position
{
	int ri, ci;
	bool operator == (const Position P)const
	{
		return ri == P.ri && ci == P.ci;

	}
};

struct Snake
{
	int Score;
	vector<Position> Ps;
	char Sym;
	int Direction;
	int Leftkey;
	int Rightkey;
	int Upkey;
	int Downkey;

};

bool isValidFoodPos(Position& Fposition, Snake S)
{
	for (int i = 0; i < S.Ps.size(); i++)
	{
		if (S.Ps[i] == Fposition)
			return false;
	}
	return true;
}
void GenerateFood(Position& Fposition, Snake S, int Rows1, int Cols1)
{
	do
	{
		Fposition.ri = rand() % Rows1;
		Fposition.ci = rand() % Cols1;
		if (Fposition.ri == 0)
			Fposition.ri = Fposition.ri++;
		if (Fposition.ci == 0)
			Fposition.ci = Fposition.ci++;
	} while (!isValidFoodPos(Fposition, S));
}

void Init(Snake& S, Position& Fposition)
{
	S.Ps.resize(3);
	S.Ps[0].ri = Rows / 2;
	S.Ps[0].ci = Cols / 2;
	S.Ps[1].ri = Rows / 2;
	S.Ps[1].ci = Cols / 2 + 1;
	S.Ps[2].ri = Rows / 2;
	S.Ps[2].ci = Cols / 2 + 2;
	S.Score = 0;
	S.Sym = -37;
	S.Downkey = DOWNKEY;
	S.Upkey = UPKEY;
	S.Leftkey = LEFTKEY;
	S.Rightkey = RIGHTKEY;
	S.Direction = LEFT;

	GenerateFood(Fposition, S, Rows, Cols);
}

void UpdateDirection(Snake& S, int key)
{

	if (key == S.Leftkey && S.Direction != RIGHT)
	{
		S.Direction = LEFT;
	}
	else if (key == S.Rightkey && S.Direction != LEFT)
	{
		S.Direction = RIGHT;
	}
	else if (key == S.Upkey && S.Direction != DOWN)
	{
		S.Direction = UP;
	}
	else if (key == S.Downkey && S.Direction != UP)
	{
		S.Direction = DOWN;
	}
}

void DisplaySnake(const Snake& S, char s)
{
	for (int i = 0; i < S.Ps.size(); i++)
	{
		gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
		if (i == 0)
			cout << 'O';
		else
			cout << s;
	}
}

void KillSnake()
{
	system("cls");
	gotoRowCol(Rows / 2, Cols / 2);
	cout << "YOU LOST" << endl << endl;
}

bool SnakeMove(Snake& S, int Rows1, int Cols1)
{
	for (int i = S.Ps.size() - 1; i > 0; i--)
	{
		S.Ps[i] = S.Ps[i - 1];
	}
	switch (S.Direction)
	{
	case UP:
		S.Ps[0].ri--;
		if (S.Ps[0].ri == 0)
		{
			KillSnake();
			return false;
		}
		return true;
		//S.Ps[0].ri = Rows - 1;

		break;

	case DOWN:
		S.Ps[0].ri++;
		if (S.Ps[0].ri == Rows1)
		{
			KillSnake();
			return false;
		}
		return true;
		//S.Ps[0].ri = 0;
		break;

	case LEFT:
		S.Ps[0].ci--;
		if (S.Ps[0].ci == 0)
		{
			KillSnake();
			return false;
		}
		return true;
		//S.Ps[0].ci = Cols - 1;
		break;

	case RIGHT:
		S.Ps[0].ci++;
		if (S.Ps[0].ci == Cols1)
		{
			KillSnake();
			return false;
		}
		return true;
		//S.Ps[0].ci = 0;
		break;
	}
}

void DisplayFood(Position Fposition, char Fsym)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Fsym == '*')
		SetConsoleTextAttribute(hConsole, 12);
	else
		SetConsoleTextAttribute(hConsole, 10);
	gotoRowCol(Fposition.ri, Fposition.ci);
	cout << Fsym;
	SetConsoleTextAttribute(hConsole, 15);
}

bool FoodEaten(Position Fposition, Snake S)
{
	return Fposition == S.Ps[0];
}

void ExtendSnake(Snake& S, Position Tposition)
{
	S.Ps.push_back(Tposition);
}

void ShrinkSize(Snake& S)
{
	S.Ps.resize(S.Ps.size() - 1);
	S.Ps.shrink_to_fit();
}

int main() {

	int Hr = 0; int min = 0; int sec = 0;
	Snake S;
	char Food = '*';
	Position Fposition, TPosition;
	int j;
	for (int i = 0; i <= Rows; i++)
	{
		for (j = 0; j <= Cols; j++)
		{
			if (i == 0 || i == Rows)
				cout << "-";
			else if (j == 0 || j == Cols)
				cout << "|";
			else
				cout << " ";

		}
		cout << endl;
	}

	Init(S, Fposition);
	DisplayFood(Fposition, Food);
	DisplaySnake(S, S.Sym);
	bool bonusfood = false;
	TPosition = S.Ps[S.Ps.size() - 1];
	int i = 1;
	int Rows1 = Rows;
	int Cols1 = Cols;
	bool increaseSpeed = false , ShrinkBoard = false;
	bool play;
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == 224)
			{
				key = _getch();
				UpdateDirection(S, key);
			}
		}


		sec += 1;
		if (sec == 60)
		{
			sec = 0;
			min += 1;
			if (min == 60)
			{
				min = 0;
				Hr += 1;
			}
		}
		gotoRowCol(72, 0);
		cout << "\r" << setw(2) << Hr << ":" << setw(2) << min << ":" << setw(2) << sec << endl;
		gotoRowCol(73, 0);
		cout << "\r" << "Score : " << S.Score;

		DisplaySnake(S, ' ');
		TPosition = S.Ps[S.Ps.size() - 1];
		if(!ShrinkBoard)
			play = SnakeMove(S, Rows1,Cols1);
		else
		{
			ShrinkBoard = false;
			
			play = SnakeMove(S,Rows1, Cols1);
		}

		if (!play)
			break;
		if (FoodEaten(Fposition, S))
		{
			i++;
			if (bonusfood)
			{
				S.Score += 10;
				//ShrinkSize(S); 
				//increaseSpeed = true;
				/*ShrinkBoard = true;
				for (int i = 0; i < S.Ps.size(); i++)
				{
					S.Ps[i].ri = S.Ps[i].ri / 2;
					S.Ps[i].ci = S.Ps[i].ci / 2;
				}
				system("cls");
				if (Rows1 > 2 && Cols1 > 2)
				{
					Rows1 = Rows / 2;
					Cols1 = Cols / 2;
				}
				for (int i = 0; i <= Rows1; i++)
				{
					for (j = 0; j <= Cols1; j++)
					{
						if (i == 0 || i == Rows1)
							cout << "-";
						else if (j == 0 || j == Cols1)
							cout << "|";
						else
							cout << " ";

					}
					cout << endl;
				}
				*/

			}
			else
			{
				increaseSpeed = false;
				S.Score++;
				ExtendSnake(S, TPosition);
			}

			GenerateFood(Fposition, S, Rows1, Cols1);
			if (i % 5 == 0)
			{
				DisplayFood(Fposition, '$');
				bonusfood = true;
			}
			else
			{
				DisplayFood(Fposition, Food);
				bonusfood = false;
			}
		}
		gotoRowCol(75, 0);
		cout << "Current size of Snake: " << S.Ps.size();
		DisplaySnake(S, S.Sym);
		if (!increaseSpeed)
			Sleep(100);
		else
			Sleep(50);

	}
	while (true);

	return 0;
}