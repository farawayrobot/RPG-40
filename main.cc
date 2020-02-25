#include "map.h"
#include <fstream>
#include <unistd.h>

const int MAX_FPS = 90; //Cap frame rate
const unsigned int TIMEOUT = 10; //Milliseconds to wait for a getch to finish
const int UP = 65; //Key code for up arrow
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;

//Turns on full screen text mode
void turn_on_ncurses() {
	initscr();//Start curses mode
	start_color(); //Enable Colors if possible
	init_pair(1, COLOR_WHITE, COLOR_BLACK); //Set up some color pairs
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_BLUE, COLOR_BLACK);
	clear();
	noecho();
	cbreak();
	timeout(TIMEOUT); //Set a max delay for key entry
}

//Exit full screen mode - also do this if you ever want to use cout or gtest or something
void turn_off_ncurses() {
	clear();
	endwin(); // End curses mode
	system("clear");
}
//scans tile player is on and displays a description
void description(char tileDescriptor){
	if (tileDescriptor == Map::TREASURE){
		mvprintw(Map::DISPLAY +2,0,"BOOTY!"); 
	}
	else if(tileDescriptor == Map::MONSTER){
        mvprintw(Map::DISPLAY +2,0,"ARRG!");
	}
	else if (tileDescriptor == Map::WATER){
        mvprintw(Map::DISPLAY +2,0,"GLUB GLUB GLUB!");
	}
	else {
		mvprintw(Map::DISPLAY +2,0,"Empty land");
	}
}

bool collide(int posX, int posY, Map collision){
	char tile;
	tile = collision.getTile(posX,posY);
//	mvprintw(Map::DISPLAY +2,0,"%c", tile);
	if (tile == Map::WALL){
		return true;
	} else{
		return false;
	}
}

bool isWater(int posX, int posY, Map water){
    char tile;
    tile = water.getTile(posX,posY);
    if (tile == Map::WATER){
        return true;
    } else {
        return false;
    }
}

bool isTreasure(int posX, int posY, Map treasure){
	char tile;
	tile = treasure.getTile(posX,posY);
	if (tile == Map::TREASURE){
		return true;
	} else{
		return false;
	}
}

bool isMonster(int posX, int posY, Map battle){
	char tile;
	tile = battle.getTile(posX,posY);
	if (tile == Map::MONSTER){
		return true;
	} else {
		return false;
	}
}

bool isNpc(int posX, int posY, Map people){
    char tile;
    tile = people.getTile(posX,posY);
    if (tile == Map::NPC){
        return true;
    } else {
        return false;
    }
}

void titleScreen(){
	while(true){
		cout << "     Title Screen!\n     Please enter player name\n";
		string name;
		cin >> name;
		if (cin) break;
	}
}

// shopkeeper function

// Combat function

// dialouge function

// inventory function

int main() {
//	game variables
	char descriptor = '\0';
	int playerHP = 0;
	int playerGP = 0;
	int npcNum = 0;
//game code starts here
	titleScreen();
	turn_on_ncurses();
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;//menu button etc need to be right here
//shopkeeper
		else if (ch == 's' || ch == 'S') {
			turn_off_ncurses();
			//shopkeeper function loop
			turn_on_ncurses();		
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i\n", x, y, playerHP, playerGP );
//			function to print tile description
			description(descriptor);
			refresh();
		}
//character movement	
		else if (ch == RIGHT) {
			x++;
			if (x >= Map::SIZE) x = Map::SIZE - 1; //Clamp value
		} else if (ch == LEFT) {
			x--;
			if (x < 0) x = 0;
		} else if (ch == UP) {
			y--;
			if (y < 0) y = 0;
		} else if (ch == DOWN) {
			y++;
			if (y >= Map::SIZE) y = Map::SIZE - 1; //Clamp value
		} else if (ch == ERR) { //No keystroke
			; //Do nothing
		}
//collsion with walls x and y will dectect if wall and then push char back to prior pos
		if (collide(x,y,map) == true){
			x = old_x;
			y = old_y;
			mvprintw(Map::DISPLAY +2,0,"This is a freaking wall");
		}
//Monster interaction starts combatloop
		else if (isMonster(x,y,map) == true) {
			//combat function loop
//			turn_off_ncurses();
			//	add asci monsters
//	do differnet actions, maybe weak attack and strong attack, defend, maybe a run option?   

	
/*	turn this back on to exit combat loop
			turn_on_ncurses();		
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i\n", x, y, playerHP, playerGP );
			description(descriptor); //function to set tile description
			refresh();
*/
			map.setTile(x,y,Map::TREASURE);
		}		
//treasure player will gain 1 gold 
		else if(isTreasure(x,y,map) == true) {
			playerGP++;
			descriptor = Map::TREASURE;
			map.setTile(x,y,Map::OPEN);
		}
//water player will get pushed back to old xy pos and lose 1 HP
		else if(isWater(x,y,map) == true) {
			descriptor = Map::WATER;
			playerHP--;
			x = old_x;
			y = old_y;
		}
//ADD NPC interaction loop
		else if(isNpc(x,y,map) == true){ 
			descriptor = Map::NPC;
			npcNum++;
// Turn this on to enter NPC loop
			turn_off_ncurses();
			clear();
			//NPC DIALOG function

			while (true) {
				int playerChoice = 0;
				cout << "1) quit 2) FIXME";
				cin >> playerChoice;
				if (playerChoice == 1) {
					break;
				}
				else {
					continue;
				}
			}
			x = old_x;
			y = old_y;
//  turn this back on to exit NPC interaction loop
//			drawOn(x,y,map,playerHP,playerGP,descriptor)
			turn_on_ncurses();
            clear();
            map.draw(x, y);
            mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i\n", x, y, playerHP, playerGP );
            description(descriptor); //function to set tile description
            refresh();


		}
		
		
		//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y or descriptor == Map::WATER) {
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i\n", x, y, playerHP, playerGP );
			description(descriptor); //function to set tile description
			refresh();
		}

		
		old_x = x;
		old_y = y;
		descriptor = '\0';
		usleep(1000000 / MAX_FPS);
		
	}

	turn_off_ncurses();

	}

