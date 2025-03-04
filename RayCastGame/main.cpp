#include <iostream>
#include <math.h>
#include <windows.h>

#define PI 3.14159265358979323846

#define MAP_WIDTH 10
#define MAP_HEIGHT 7
#define SCREEN_DISTANCE 1
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 31
#define MAX_RENDER_DISTANCE 3
#define WALL_HEIGHT 10

#define RAY_STEP 0.01

#define PLAYER_STEP 0.001
#define PLAYER_ROTATION 0.1

const char GRADIENT_SYMBOLS[] = ".,-!+*=#$";

char WorldMap[MAP_HEIGHT][MAP_WIDTH + 1] = {
		"##########",
		"#........#",
		"#..#..#..#",
		"#........#",
		"#..#..#..#",
		"#........#",
		"##########"
};


char ScreenImage[SCREEN_HEIGHT][SCREEN_WIDTH];

double PlayerX = MAP_WIDTH / 2;
double PlayerY = MAP_HEIGHT / 2;

// angle between player's perspective and Ox axis
double PlayerAngle = 0;
double PlayerFOV = 60;
const double DELTA_ALPHA = PlayerFOV / SCREEN_WIDTH;
const double DELTA_DISTANCE = 0.01;

double Min(double a, double b)
{
	return (a > b) ? b : a;
}

double Max(double a, double b)
{
	return (a > b) ? a : b;
}

int Min(int a, int b)
{
	return (a > b) ? b : a;
}

int Max(int a, int b)
{
	return (a > b) ? a : b;
}

/// <summary>
/// Метод, возвращающий длину символьного массива
/// </summary>
/// <param name="array"> Массив символов </param>
/// <returns> Размер массива </returns>
int Length(const char* array)
{
	int length = 0;
	for (int i = 0; array[i]; i++)
	{
		length++;
	}
	return length;
}

/// <summary>
/// Clear screen buffer method
/// </summary>
void ClearScreen()
{
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			ScreenImage[i][j] = ' ';
		}
	}
}

/// <summary>
/// Method for rendering floor
/// </summary>
void RenderFloor() 
{
	double screenBottomHalfHeight = SCREEN_HEIGHT / 2;
	int gradientLength = Length(GRADIENT_SYMBOLS);
	for (int x = 0; x < SCREEN_WIDTH; x++) 
	{
		for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) 
		{
			ScreenImage[y][x] = GRADIENT_SYMBOLS[(int)((y / screenBottomHalfHeight - 1) * gradientLength)];
		}
	}
}

void PutPlayerMarkOnTheMap()
{
	WorldMap[(int)PlayerY][(int)PlayerX] = 'p';
}

void RemovePlayerMarkOnTheMap()
{
	WorldMap[(int)PlayerY][(int)PlayerX] = '.';
}

/// <summary>
/// Check if the dot is on the game map
/// </summary>
/// <param name="x"> Dot X </param>
/// <param name="y"> Dot Y </param>
/// <returns> true or false </returns>
bool CheckIfDotOnTheMap(int x, int y)
{
	if (x >= 0 && x < MAP_WIDTH)
	{
		if (y >= 0 && y < MAP_HEIGHT)
		{
			return true;
		}

		return false;
	}

	return false;
}

/// <summary>
/// Check if the dot is on the game map
/// </summary>
/// <param name="x"> Dot X </param>
/// <param name="y"> Dot Y </param>
/// <returns> true or false </returns>
bool CheckIfDotOnTheMap(double x, double y)
{
	if (x >= 0 && x < MAP_WIDTH)
	{
		if (y >= 0 && y < MAP_HEIGHT)
		{
			return true;
		}

		return false;
	}

	return false;
}

/// <summary>
/// Method for rendering Screen Image
/// </summary>
void RenderWalls() 
{
	double minAngle = PlayerAngle - PlayerFOV / 2;
	int gradientSymbolsLength = Length(GRADIENT_SYMBOLS);
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		double angle = (minAngle + DELTA_ALPHA * i) / 180 * PI;
		double r = 0;
		double x = r * cos(angle) + PlayerX;
		double y = r * sin(angle) + PlayerY;

		while (CheckIfDotOnTheMap(x, y) && WorldMap[(int)y][(int)x] != '#' && r < MAX_RENDER_DISTANCE)
		{
			x = r * cos(angle) + PlayerX;
			y = r * sin(angle) + PlayerY;
			r += RAY_STEP;
		}

		if (WorldMap[(int)y][(int)x] == '#')
		{
			int projectedWallHeight = (SCREEN_DISTANCE * WALL_HEIGHT / r);
			for (int j = SCREEN_HEIGHT / 2; j > max(-1, SCREEN_HEIGHT / 2 - projectedWallHeight); j--)
			{
				ScreenImage[j][i] = GRADIENT_SYMBOLS[(int)(min(1, SCREEN_DISTANCE / r) * gradientSymbolsLength)];
			}
		}
	}
}

/// <summary>
/// Draw screen image in console
/// </summary>
void PrintScreen()
{
	//std::cout << SCREEN_WIDTH << ' ' << SCREEN_HEIGHT << std::endl;
	const int printLineLength = (SCREEN_WIDTH + 1) * SCREEN_HEIGHT;
	char printLine[printLineLength + 1];
	printLine[printLineLength] = '\0';

	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			printLine[y * (SCREEN_WIDTH + 1) + x] = ScreenImage[y][x];
		}
		printLine[(y + 1) * (SCREEN_WIDTH + 1) - 1] = '\n';
	}

	std::cout << printLine;
}

/// <summary>
/// Method that allow to show map and various player parameters on the screen
/// </summary>
void DrawInterface()
{
	for (int x = 0; x < Min(MAP_WIDTH, SCREEN_WIDTH); x++)
	{
		for (int y = 0; y < Min(MAP_HEIGHT, SCREEN_HEIGHT); y++)
		{
			ScreenImage[y][x] = WorldMap[y][x];
		}
	}
}


bool CheckIfPlayerCollideWithWall()
{
	if (WorldMap[(int)PlayerY][(int)PlayerX] == '#')
	{
		return true;
	}

	return false;
}

/// <summary>
/// Method for rotating player camera by an angle
/// </summary>
/// <param name="delta"> An angle to rotate player by</param>
void RotatePlayer(double delta)
{
	if (delta < 0)
	{
		PlayerAngle = PlayerAngle + delta;
		if (PlayerAngle < 0)
		{
			PlayerAngle = 360 + ((int)PlayerAngle % 360);
		}
		return;
	}
	double angleArgument = (PlayerAngle + delta);
	PlayerAngle = angleArgument - ((int)angleArgument / 360) * angleArgument;
}

/// <summary>
/// Move player method
/// </summary>
void MovePlayerFront()
{
	double angleInRadians = PlayerAngle / 180 * PI;
	PlayerX += cos(angleInRadians) * PLAYER_STEP;
	PlayerY += sin(angleInRadians) * PLAYER_STEP;
}

/// <summary>
/// Move player backwards
/// </summary>
void MovePlayerBackwards()
{
	double angleInRadians = PlayerAngle / 180 * PI;
	PlayerX -= cos(angleInRadians) * PLAYER_STEP;
	PlayerY -= sin(angleInRadians) * PLAYER_STEP;
}

/// <summary>
/// Method for controlling player by pressing keys
/// </summary>
void HandleKeyPress()
{
	RemovePlayerMarkOnTheMap();

	if (GetKeyState(VK_UP) & 0x8000) 
	{
		MovePlayerFront();
		if (!CheckIfDotOnTheMap(PlayerX, PlayerY) || CheckIfPlayerCollideWithWall())
		{
			MovePlayerBackwards();
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		MovePlayerBackwards();
		if (!CheckIfDotOnTheMap(PlayerX, PlayerY) || CheckIfPlayerCollideWithWall())
		{
			MovePlayerFront();
		}
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		RotatePlayer(-PLAYER_ROTATION);
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		RotatePlayer(PLAYER_ROTATION);
	}

	PutPlayerMarkOnTheMap();
}

bool GameRunning = true;

int main() 
{	
	while (GameRunning) 
	{
		//system("cls");
		//std::cout << PlayerAngle << std::endl;
		//std::cout << PlayerX << ' ' << PlayerY << std::endl;
		ClearScreen();
		RenderFloor();
		RenderWalls();
		DrawInterface();
		PrintScreen();

		HandleKeyPress();
	}

	return 0;
}
