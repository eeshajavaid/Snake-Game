#include <Windows.h>
#include <conio.h>
#include <time.h>
#include<iostream>
#include <iomanip>
#include <vector>

using namespace std;

int Rows = 70;
int Cols = 70;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77 

#define SUPKEY 87
#define SDOWNKEY 83
#define SLEFTKEY 65
#define SRIGHTKEY 68 



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

bool isValidFoodPos(Position& Fposition, Snake S, Snake S2, vector<Position> Ps)
{
	for (int i = 0; i < S.Ps.size(); i++)
	{
		if (S.Ps[i] == Fposition)
			return false;
	}

	for (int i = 0; i < S2.Ps.size(); i++)
	{
		if (S2.Ps[i] == Fposition)
			return false;
	}

	for (int i = 0; i < Ps.size(); i++)
	{
		if (Ps[i] == Fposition)
			return false;
	}
	return true;
}
void GenerateFood(Position& Fposition, Snake S, Snake S2, vector<Position> Ps)
{
	do
	{
		Fposition.ri = rand() % Rows;
		Fposition.ci = rand() % Cols;
		if (Fposition.ri == 0)
			Fposition.ri += 1;
		if (Fposition.ci == 0)
			Fposition.ci += 1;
	} while (!isValidFoodPos(Fposition, S, S2, Ps));
}

void Init(Snake& S, Snake& S2, Position& Fposition, vector <Position> Ps)
{
	S.Ps.resize(3);
	S2.Ps.resize(3);

	S.Ps[0].ri = Rows / 2;
	S.Ps[0].ci = Cols / 2;
	S.Ps[1].ri = Rows / 2;
	S.Ps[1].ci = Cols / 2 + 1;
	S.Ps[2].ri = Rows / 2;
	S.Ps[2].ci = Cols / 2 + 2;

	S2.Ps[0].ri = Rows / 4;
	S2.Ps[0].ci = Cols / 4;
	S2.Ps[1].ri = Rows / 4;
	S2.Ps[1].ci = Cols / 4 + 1;
	S2.Ps[2].ri = Rows / 4;
	S2.Ps[2].ci = Cols / 4 + 2;

	S.Score = 0;
	S.Sym = -37;
	S.Downkey = DOWNKEY;
	S.Upkey = UPKEY;
	S.Leftkey = LEFTKEY;
	S.Rightkey = RIGHTKEY;
	S.Direction = LEFT;

	S2.Score = 0;
	S2.Sym = -37;
	S2.Downkey = SDOWNKEY;
	S2.Upkey = SUPKEY;
	S2.Leftkey = SLEFTKEY;
	S2.Rightkey = SRIGHTKEY;
	S2.Direction = LEFT;

	GenerateFood(Fposition, S, S2, Ps);
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
		cout << s;
	}
}

void GameEnd()
{
	system("cls");
	gotoRowCol(Rows / 2, Cols / 2);
	cout << "Game End" << endl << endl;
}

bool checkTileValidity(Snake S, vector<Position> Ps)
{
	for (int i = 0; i < Ps.size(); i++)
	{
		if (S.Ps[0] == Ps[i])
			return false;
	}
	return true;
}

bool SnakeMove(Snake& S, Snake& S2, vector <Position> Ps)
{
	for (int i = S.Ps.size() - 1; i > 0; i--)
	{
		S.Ps[i] = S.Ps[i - 1];
	}
	if (S.Ps[0] == S2.Ps[S2.Ps.size() - 1])
	{
		S.Score -= 10;
	}

	if (S.Ps[0] == S2.Ps[0])
		return false;
	switch (S.Direction)
	{
	case UP:
		S.Ps[0].ri--;
		if (S.Ps[0].ri == 0)
		{
			return false;
		}
		else
			return checkTileValidity(S, Ps);
		//S.Ps[0].ri = Rows - 1;

		break;

	case DOWN:
		S.Ps[0].ri++;
		if (S.Ps[0].ri == Rows)
		{
			return false;
		}
		else
			return checkTileValidity(S, Ps);
		//S.Ps[0].ri = 0;
		break;

	case LEFT:
		S.Ps[0].ci--;
		if (S.Ps[0].ci == 0)
		{
			return false;
		}
		else
			return checkTileValidity(S, Ps);
		//S.Ps[0].ci = Cols - 1;
		break;

	case RIGHT:
		S.Ps[0].ci++;
		if (S.Ps[0].ci == Cols)
		{
			return false;
		}
		else
			return checkTileValidity(S, Ps);
		//S.Ps[0].ci = 0;
		break;
	}


}



void DisplayFood(Position Fposition, char Fsym)
{
	gotoRowCol(Fposition.ri, Fposition.ci);
	cout << Fsym;
}

bool FoodEaten(Position Fposition, Snake S)
{
	return Fposition == S.Ps[0];
}

void ExtendSnake(Snake& S, Position Tposition)
{
	S.Ps.push_back(Tposition);
}

void ShrinkSnake(Snake& S)
{
	if(S.Ps.size()>1)
	S.Ps.resize(S.Ps.size() - 1);
	S.Ps.shrink_to_fit();
}

void ShrinkBoard(Snake& S1, Snake& S2)
{
	if (Rows / 2 > S1.Ps.size() && Rows / 2 > S2.Ps.size())
	{
		Rows = Rows / 2;
		Cols = Cols / 2;
		for (int i = 0; i < S1.Ps.size(); i++)
		{
			S1.Ps[i].ri = S1.Ps[i].ri / 2;
			S1.Ps[i].ci = S1.Ps[i].ci / 2;
		}
		for (int i = 0; i < S2.Ps.size(); i++)
		{
			S2.Ps[i].ri = S2.Ps[i].ri / 2;
			S2.Ps[i].ci = S2.Ps[i].ci / 2;
		}
	}
}

void insertTile(Snake S1, Snake S2, Position Fposition, vector<Position> &insertedTiles)
{
	bool match = true;
	Position tile;
	do {
		tile.ri = rand() % Rows;
		tile.ci = rand() % Cols;
		for (int i = 0; i < S1.Ps.size(); i++)
		{
			if (S1.Ps[0] == tile)
			{
				match = true;
				break;
			}
			else
				match = false;
		}
		for (int i = 0; i < S1.Ps.size(); i++)
		{
			if (S1.Ps[0] == tile)
			{
				match = true;
				break;
			}
			else
				match = false;
		}
		if (tile == Fposition)
			match = true;
		else
			match = false;
	} while (match);
	

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 13);
		char ch = -37;
		gotoRowCol(tile.ri, tile.ci);
		insertedTiles.insert(insertedTiles.begin(), tile);
		cout << ch;
		SetConsoleTextAttribute(hConsole, 15);
}

int main() {

	vector <Position> insertedTiles;
	insertedTiles.resize(1);
	int Hr = 0; int min = 0; int sec = 0;
	Snake S, S2;
	char Food = '*';
	Position Fposition, TPosition, T2Position;
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
	bool aliveSnake1 = true, aliveSnake2 = true;
	Init(S, S2, Fposition, insertedTiles);
	DisplayFood(Fposition, Food);
	DisplaySnake(S, S.Sym);
	DisplaySnake(S2, S2.Sym);
	bool bonusfood = false;
	TPosition = S.Ps[S.Ps.size() - 1];
	T2Position = S2.Ps[S2.Ps.size() - 1];

	int i = 1;
	bool slowSpeed = false;

	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			Sleep(100);
			if (key == 224)
			{
				key = _getch();
				if (aliveSnake1)
					UpdateDirection(S, key);
			}
			if (key == 'A' || key == 'S' || key == 'W' || key == 'D')
				if (aliveSnake2)
					UpdateDirection(S2, key);
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
		cout << "\r" << "Score of PLAYER 1 : " << S.Score;

		gotoRowCol(74, 10);
		cout << "\r" << "Score OF PLAYER 2 : " << S2.Score;

		if (aliveSnake1)
		{
			DisplaySnake(S, ' ');
			TPosition = S.Ps[S.Ps.size() - 1];
		}
		if (aliveSnake2)
		{
			DisplaySnake(S2, ' ');
			T2Position = S2.Ps[S2.Ps.size() - 1];
		}

		if (aliveSnake1)
			aliveSnake1 = SnakeMove(S, S2, insertedTiles);
		if (aliveSnake2)
			aliveSnake2 = SnakeMove(S2, S, insertedTiles);

		if (!aliveSnake1 && !aliveSnake2)
			break;
		if (aliveSnake1)
		{
			if (FoodEaten(Fposition, S))
			{
				i++;
				if (bonusfood)
				{
					S.Score += 10;
					slowSpeed = true;
					//ShrinkSnake(S);
					/*ShrinkBoard(S, S2);
					system("cls");
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
					}*/
				}
				else
				{
					S.Score++;
					slowSpeed = false;
					ExtendSnake(S, TPosition);
				}

				GenerateFood(Fposition, S, S2, insertedTiles);
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
			DisplaySnake(S, S.Sym);
		}
		if (aliveSnake2)
		{
			if (FoodEaten(Fposition, S2))
			{
				i++;
				if (bonusfood)
				{
					S2.Score += 10;
					slowSpeed = true;
					//ShrinkSnake(S2);
					/*ShrinkBoard(S, S2);
					system("cls");
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
					}*/
				}
				else
				{
					S2.Score++;
					ExtendSnake(S2, TPosition);
					slowSpeed = false;
				}

				GenerateFood(Fposition, S, S2, insertedTiles);
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
			DisplaySnake(S2, S2.Sym);
			if (min % 3 == 0)
			{
			//	insertTile(S, S2, Fposition,insertedTiles);
				
			}
		}

		Sleep(100);
		if (slowSpeed)
			Sleep(1000);

	}
	while (true)
	{
		GameEnd();
	}

	return 0;
}