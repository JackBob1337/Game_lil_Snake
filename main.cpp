#include <iostream>
#include <deque>
#include <raylib.h>
#include <raymath.h>
#include "main_menu.h"

using namespace std;

Color green = {173, 204, 96, 255};

Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;
double lastUpdateTime = 0;

enum GameState
{
	MAIN_MENU,
	GAMEPLAY
};

int GetScreenWidth()
{
	return 2 * offset + cellSize * cellCount;
}

int GetScreenHeight()
{
	return 2 * offset + cellSize * cellCount;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake
{
	
public:
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
	Vector2 direction = { 1, 0 };
	bool addSegment = false;
	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++)
		{
			int x = body[i].x;
			int y = body[i].y;
			Rectangle segments = Rectangle{static_cast<float>(offset + x*cellSize), static_cast<float>(offset + y * cellSize), 
				static_cast<float>(cellSize), static_cast<float>(cellSize)};
			DrawRectangleRounded(segments, 0.5f, 6, darkGreen);
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
		}
	}

	void Reset()
	{
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		direction = { 1, 0 };
	}
};

class Food
{

public:
	Vector2 position;
	Texture2D texture;
	
	Food(deque<Vector2> snakeBody)
	{
		Image image = LoadImage("C:/Users/vyalk/Desktop/Snake/Apple.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomePos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomePos(deque<Vector2> snakeBody)
	{

		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}
		return position;

	}
};

class Game
{
public:
	Food food = Food(snake.body);
	Snake snake = Snake();
	bool running = true;
	int score = 0;

	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{
		if (running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdge();
			CheckCollisionWithTail();
		}
	}

	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomePos(snake.body);
			snake.addSegment = true;
			score++;
		}
	}

	void CheckCollisionWithEdge()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1)
		{
			GameOver();
		}
		if (snake.body[0].y == cellCount || snake.body[0].y == -1)
		{
			GameOver();
		}
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomePos(snake.body);
		running = false;
		score = 0;
	}

	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (ElementInDeque(snake.body[0], headlessBody))
		{
			GameOver();
		}
	}
};

int main()
{
	InitWindow(GetScreenWidth(), GetScreenHeight(), "lil Snake");

	SetTargetFPS(60);

	Game game;
	GameState gameState = MAIN_MENU;

	

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		switch (gameState)
		{
		case MAIN_MENU:
		{
			MenuAction action = DrawMainMenu();
			if (action == PLAY)
			{
				gameState = GAMEPLAY;
			}
			
			if (action == EXIT)
			{
				CloseWindow();
				return 0;
			}
		}
			break;
		case GAMEPLAY:
		{
			if (eventTriggered(0.2))
			{
				game.Update();
			}

			if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1)
			{
				game.snake.direction = { 0, -1 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1)
			{
				game.snake.direction = { 0, 1 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1)
			{
				game.snake.direction = { -1, 0 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1)
			{
				game.snake.direction = { 1, 0 };
				game.running = true;
			}

			//Drawing
			ClearBackground(green);
			DrawRectangleLinesEx(Rectangle{ static_cast<float>(offset - 5), static_cast<float>(offset - 5),
				float(cellSize * cellCount + 10),  float(cellSize * cellCount + 10) }, 5, darkGreen);
			DrawText("lil Snake", offset - 5, 20, 40, darkGreen);
			DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
			game.Draw();
		}break;
			
		default:
			break;
		}

		

		EndDrawing();
	}

	CloseWindow();

}