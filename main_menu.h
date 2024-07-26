//main_menu.h
#ifndef MAIN_MENU.H
#define MAIN_MENU_H

#include<raylib.h>

enum MenuAction
{
	NONE,
	PLAY,
	EXIT
};

MenuAction DrawMainMenu();

#endif // !MAIN_MENU.H
