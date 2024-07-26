#include <raylib.h>
#include "main_menu.h"

extern int GetScreenHeight();
extern int GetScreenWidth();
extern Color green;
extern Color darkGreen;

MenuAction DrawMainMenu()
{
	ClearBackground(green);

	int screenWidth = GetScreenWidth();
	int screedHeight = GetScreenHeight();
	
	DrawText("Main Menu", screedHeight / 2 - MeasureText("Main Memu", 40) / 2, screedHeight / 2 - 100, 40, DARKGRAY);

	
	//Button Play
	Rectangle playButton = { static_cast<float>(screedHeight / 2 - MeasureText("Main Memu", 40) / 2), static_cast<float>(screenWidth / 2), 200, 50};
	DrawRectangleRec(playButton, darkGreen);
	DrawText("PLAY", playButton.x + 60, playButton.y + 15, 20, DARKGRAY);

	//Button Exit
	Rectangle exitButton = { static_cast<float>(screedHeight / 2 - MeasureText("Main Memu", 40) / 2), static_cast<float>(screenWidth / 2 + 70), 200, 50 };
	DrawRectangleRec(exitButton, darkGreen);
	DrawText("EXIT", exitButton.x + 60, exitButton.y + 15, 20, DARKGRAY);


	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePoint = GetMousePosition();
		if (CheckCollisionPointRec(mousePoint, playButton))
		{
			return PLAY;
		}

		if (CheckCollisionPointRec(mousePoint, exitButton))
		{
			return EXIT;
		}
	}
	return NONE;
}