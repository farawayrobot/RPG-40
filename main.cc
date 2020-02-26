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
//redraws ncurses map
void drawOn(int x,int y,Map map,int playerHP,int playerGP,char descriptor){
	turn_on_ncurses();
	clear();
	map.draw(x, y);
	mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold %i\n", x, y, playerHP, playerGP);
	description(descriptor);
	refresh();
}
// booleans for tile scanning
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

bool isHero(int posX, int posY, Map hero){
	char tile;
	tile =hero.getTile(posX,posY);
	if (tile == Map::HERO) {
		return true;
	} else {
		return false;
	}
}


//shopkeeper function
//Kajiit has wares if you have coin
//ASCII CAT


// Combat function
//test loop
void combat() {
	while (true) {	
       	char diagChoice;
		cout << "_______________________________________________________\n_______________________________________________________\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n_______________________________________________________\n_______________________________________________________\n\nPress c to continue, r to runaway:";
		cin >> diagChoice;
		if (diagChoice == 'r' || diagChoice == 'R'){
			diagChoice = '\0';
			break;
		}
		else if (diagChoice == 'c' || diagChoice == 'C'){
			system("clear");
			//need to put combat game in here
			cout << "hit anything to exit";
			cin >> diagChoice;
			diagChoice = '\0';
			break;
		}
	}
}

// inventory function


//dialog function
//having to do this inside main.cc because of variable scope



int main() {
//	game variables
	string playerName = "";
	char descriptor = '\0';
	int playerHP = 10;
	int playerGP = 0;
	int playerLVL = 0;
	int playerCrown = 0;
	int npcNum = 0;
//game code starts here
	turn_on_ncurses();
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;//menu button etc need to be right here
//shopkeeper
		else if (ch == 's' || ch == 'S') {
//			turn_off_ncurses();
			//shopkeeper function
//			drawOn(x,y,map,playerHP,playerGP,descriptor);
		}
//inventory
		else if (ch == 'i' || ch == 'I') {
//			turn_off_ncurses();
			//inventory function
//			drawOn(x,y,map,playerHP,playerGP,descriptor);
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
// Hero starts off on Hero tile, this forces a title screen to appear on game start up
		if (isHero(x,y,map) == true) {
			turn_off_ncurses();
// this changes the map tile so that the player can't reaccess the title page
			map.setTile(x,y,Map::OPEN);
			// line is 55 underscores
			cout << "_______________________________________________________\n_______________________________________________________\n\n          WELCOME TO THE GRAND ADVENTURES\n                        OF\n               KYK THE GOBLIN KING\n\n\n\n        Please enter player name to start\n\n_______________________________________________________\n_______________________________________________________\n\nName:";
			cin >> playerName;
			system("clear");
			cout << "_______________________________________________________\n_______________________________________________________\n\nLONG LONG AGO IN THE DISTANT LAND OF LEVIATHAN IN\nTHE WILD AND CHAOTIC NORTH THERE WAS A SMALL GOBLIN\nNAMED KYK. SMALL EVEN AMOUNGST OTHER GOBLINS KYK WAS\nONCE RIDICULED BY EVERYONE, BUT NO LONGER BECAUSE\nKYK THROUGH SURE TENACITY AND A STRONG BITE BECAME\nTHE KING OF ALL GOBLIN KIND. HIS SMALLER THAN\nAVERAGE HEAD MADE THE BURDEN OF WEARING ALL FIVE\nCROWNS OF THE ONCE SEPERATE GOBLIN NATIONS IMPOSSIBLE,\nSO UPON HIS CORINATION KYK HAD ALL FIVE CROWNS\nFORGED INTO A GOLDEN CHAIN HE WORE AROUND HIS NECK.\n\nAFTER SEVERAL WEEKS KYK GREW TIRED OF HIS KINGLY\nDUTIES AND WANDERED OFF TO FIND SOME ADVENTURE.\nAND ADVENTURE HE DID FIND! SLAYING DRAGONS, ELVES,\nDWARFS, HOOMANS, EVEN A GIANT! NO BEAST OR FOE\nCOULD STAND BEFORE THE WHIRLING BLADES OF THE KING\nOF ALL GOBLINS. BUT THEN ONE NIGHT AT A TAVERN\nKYK LOST EVERYTHING HE OWNED IN A GAME OF CARDS\nTO A SHREWED AND CALCULATING KAJIIT. NOW WE JOIN OUR\nGOBLIN KING THE MORNING AFTER TRYING TO BUY\nBACK HIS POSSESSIONS FROM THE DEVIOUS KAJIIT.\n_______________________________________________________\n_______________________________________________________\n\n";
			cout << "       Press C to continue...\n:";
			char startGame;
			cin >> startGame;
			if (startGame != 'c' || startGame != 'c') {
				system("clear");
				return 0;
			}			
			drawOn(x,y,map,playerHP,playerGP,descriptor);
		}
//collsion with walls x and y will dectect if wall and then push char back to prior pos
		if (collide(x,y,map) == true){
			x = old_x;
			y = old_y;
			mvprintw(Map::DISPLAY +2,0,"This is a freaking wall");
		}
//Monster interaction starts combatloop then sets tile to treasure and pushes player back to old pos
		else if (isMonster(x,y,map) == true) {
			//combat function loop
			turn_off_ncurses();
//add asci monsters
//do differnet actions, maybe weak attack and strong attack, defend, maybe a run option? 
//combat function
			combat();
			map.setTile(x,y,Map::TREASURE);
			x = old_x;
			y = old_y;
//redraws map after combat ends 
			drawOn(x,y,map,playerHP,playerGP,descriptor);		
		}		
//treasure player will gain 1 gold after moving over the tile
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
// NPC interaction
		else if(isNpc(x,y,map) == true){
			npcNum++;
// clears map to enter NPC interaction
			turn_off_ncurses();
			//NPC DIALOG function
			while (true) {
       			if (npcNum == 1) {
					char diagChoice;
					cout << "_______________________________________________________\n_______________________________________________________\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n_______________________________________________________\n_______________________________________________________\n\nPress c to continue:";
					cin >> diagChoice;
					if (diagChoice != 'c' || diagChoice != 'C') {
						break;
					}
					else {
						continue;
					}
				}
					
       			if (npcNum == 2) {
           				combat();
						break;
				}
       			if (npcNum == 3) {
       					combat();
						break;
				}
				if (npcNum == 4) {
           				combat();
						break;
				}
       			if (npcNum == 5) {
       					combat();
						break;
				}
				if (npcNum == 6) {
           				combat();
						break;
				}
       			if (npcNum == 7) {
       					combat();
						break;
				}
   				if (npcNum == 8) {
					combat();
						break;
				}
       			if (npcNum == 9) {
       					combat();
						break;
				}
   				if (npcNum == 10) {
           				combat();
						break;
				}
				else {
					break;
				}
  	 		 }
		map.setTile(x,y,Map::OPEN);
		x = old_x;
		y = old_y;
// redraws map after NPC function ends
		drawOn(x,y,map,playerHP,playerGP,descriptor);
		}


		//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y or descriptor == Map::WATER or descriptor == Map::MONSTER) {
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i\n", x, y, playerHP, playerGP );
			description(descriptor); //function to set tile description
			refresh();
		}
//need to add a win check and win screen
//Game over check and game over screen
		if (playerHP < 1) {
			turn_off_ncurses();
			system("clear");
			cout << "\n\n\n                  " << playerName << "\n\n               YOU DIED!\n\n               GAME OVER\n\n";
			usleep(1000000);
			break;
		}

//clean up section
		old_x = x;
		old_y = y;
		descriptor = '\0';
		usleep(1000000 / MAX_FPS);
		
	}

	turn_off_ncurses();
}
