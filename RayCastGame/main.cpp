#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846

#define WIDTH 11
#define HEIGHT 7
#define SCREEN_DISTANCE 1
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100
#define MAX_RENDER_DISTANCE 3

#define PLAYER_STEP 0.01

const char GRADIENT_SYMBOLS[] = "$#!=*+-,. ";

char WorldMap[HEIGHT][WIDTH] = {
		"##########",
		"#........#",
		"#........#",
		"#........#",
		"#........#",
		"#........#",
		"##########"
};

char ScreenImage[SCREEN_WIDTH][SCREEN_HEIGHT];

double PlayerX = WIDTH / 2;
double PlayerY = HEIGHT / 2;

// angle between player's perspective and Ox axis
double PlayerAngle = 0;
double PlayerFOV = 60;
const double DELTA_ALPHA = PlayerFOV / SCREEN_WIDTH;
const double DELTA_DISTANCE = 0.01;


/// <summary>
/// Method for endering floor
/// </summary>
void RenderFloor() 
{

}

/// <summary>
/// Method for rendering Screen Image
/// </summary>
void Render() 
{
	double minAngle = PlayerAngle - PlayerFOV / 2;
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		double angle = (minAngle + DELTA_ALPHA * i) / 180 * PI;
		double r = 0;
		double x = r * cos(angle) + PlayerX;
		double y = r * sin(angle) + PlayerY;

		while (r < MAX_RENDER_DISTANCE && WorldMap[(int)y][(int)x] != '#')
		{
			r += PLAYER_STEP;
		}

		
	}
}

bool GameRunning = true;

int main() 
{
	while (GameRunning) 
	{
		Render();
	}

	return 0;
}
