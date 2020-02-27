#include "map.h"
#include <fstream>
#include <unistd.h>
#include <ios>
#include <iomanip>

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
	else if (tileDescriptor == Map::WATER){
        mvprintw(Map::DISPLAY +2,0,"GLUB GLUB GLUB! KYK HATES SWIMMING");
	}
	else {
		mvprintw(Map::DISPLAY +2,0,"DESOLATE AND BARREN LAND STRETCHES ALL AROUND YOU");
	}
}
//redraws ncurses map
void drawOn(int x,int y,Map map,int playerHP,int playerGP,int playerLvl,char descriptor){
	turn_on_ncurses();
	clear();
	map.draw(x, y);
	mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i Lvl: %i\n" , x, y, playerHP, playerGP, playerLvl);
	description(descriptor);
	refresh();
}
// booleans for tile scanning
bool collide(int posX, int posY, Map collision){
	char tile;
	tile = collision.getTile(posX,posY);
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
	tile = hero.getTile(posX,posY);
	if (tile == Map::HERO) {
		return true;
	} else {
		return false;
	}
}

// Combat function
bool combat(int playerLvl, int& playerHP, int playerDP, int sword) {
	while (true) {	
       	char combatChoice;
		srand(time(0));
		cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl; 
		cout << "\n\n\n\n\n\n\n     ARRRG A SCARY MONSTER!!!\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to continue, r to runaway:";
		cin >> combatChoice;
		if (combatChoice == 'r' || combatChoice == 'R'){
			combatChoice = '\0';
			return false;
		}
		else if (combatChoice == 'c' || combatChoice == 'C'){
			combatChoice = '\0';
			system("clear");
		}
			//need to put combat game in here
			
		int outcome = 0;
		int playerAtk = 0;
		int mobAtk = 0;
		int mobHP = 1;
		while (true) {
			char combatChoice;
			cout << "Player Health:" << playerHP << endl;
//comment out mob HP before deployment
			cout << "Mob HP:" << mobHP << endl;
     		cout << "\nPress A for attack or D for defend:";
			cin >> combatChoice;
			if (combatChoice == 'A'|| combatChoice == 'a') {
				playerAtk =(rand() % 10 + playerLvl) + sword;
				cout << "Kyk Dealt " << playerAtk << " Damage!\n";
				mobHP = mobHP - playerAtk;
				mobAtk = (rand() % 10 + 1) - playerDP;
				cout << "The monster dealt " <<  mobAtk << " Damage!\n";
				playerHP -= mobAtk;
			}
			else if (combatChoice == 'D' || combatChoice == 'd') {
				playerAtk = 0;
            	cout << "Defend! Kyk deals no damage!\n";
           		mobHP -= playerAtk;
           	 	mobAtk = (rand() % 10 + 1) - (playerDP * 2);
           	 	cout << "The monster dealt " << mobAtk << " Damage!\n";
            	playerHP = playerHP - mobAtk;
			}
			if (playerHP < 1){
				system("clear");
				break;
			}
			else if (mobHP < 1) {
				system("clear");
				break;
			}
		}
		if (playerHP < 1) {
			return false;
		}
		else {
			return true;
		}
	}

}
	

//shopkeeper function
//Kajiit has wares if you have coin
//ASCII CAT

// inventory function

// main
int main() {
//	game variables
	string playerName = "";
	char descriptor = '\0';
	int playerLvl = 0;
	int playerHP = 20 + (playerLvl * 2);
	int playerDP = 1;
// player defense points starts at 0. player buys better armor and this goes up 
	int playerSword = 1;
// player sword starts at 1. player buys better weapons this goes up by 1
	int playerGP = 0;
	int mobKills = 0;
// player lvl goes up after every 3 monsters defeated
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
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
			cout << "\n\n          WELCOME TO THE GRAND ADVENTURES\n                        OF\n               KYK THE GOBLIN KING\n\n\n\n        Please enter player name to start\n";
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << "Name:";
			cin >> playerName;
			system("clear");
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
			cout << "\n  LONG LONG AGO IN THE DISTANT LAND OF LEVIATHAN IN\n  THE WILD AND CHAOTIC NORTH THERE WAS A SMALL GOBLIN\n  NAMED KYK. SMALL EVEN AMOUNGST OTHER GOBLINS KYK WAS\n  ONCE RIDICULED BY EVERYONE, BUT NO LONGER BECAUSE\n  KYK THROUGH SURE TENACITY AND A STRONG BITE BECAME\n  THE KING OF ALL GOBLIN KIND. HIS SMALLER THAN\n  AVERAGE HEAD MADE THE BURDEN OF WEARING ALL FIVE\n  CROWNS OF THE ONCE SEPERATE GOBLIN NATIONS IMPOSSIBLE,\n  SO UPON HIS CORINATION KYK HAD ALL FIVE CROWNS\n  FORGED INTO A GOLDEN CHAIN HE WORE AROUND HIS NECK.\n\n  AFTER SEVERAL WEEKS KYK GREW TIRED OF HIS KINGLY\n  DUTIES AND WANDERED OFF TO FIND SOME ADVENTURE.\n  AND ADVENTURE HE DID FIND! SLAYING DRAGONS, ELVES,\n  DWARFS, HOOMANS, EVEN A GIANT! NO BEAST OR FOE\n  COULD STAND BEFORE THE WHIRLING BLADES OF THE KING\n  OF ALL GOBLINS. BUT THEN ONE NIGHT AT A TAVERN\n  KYK LOST EVERYTHING HE OWNED IN A GAME OF CARDS\n  TO A SHREWED AND CALCULATING KAJIIT.\n\n\n NOW WE JOIN OUR GOBLIN KING THE MORNING AFTER.\n  IT'S YOUR JOB AS THE PLAYER TO COLLECT MONEY TO BUY BACK\n  HIS POSSESSIONS FROM THE DEVIOUS KAJIIT,\n  AND FIND THE FIVE GOLBIN CROWNS HE HID IN THE\n  WILDERNESS OF THE NORTH!\n\n";
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
			cout << "\n       Press C to continue...\n:";
			char startGame;
			cin >> startGame;
			if (startGame != 'c' || startGame != 'c') {
				system("clear");
				return 0;
			}			
			drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);
		}
//collsion with walls x and y will dectect if wall and then push char back to prior pos
		if (collide(x,y,map) == true){
			x = old_x;
			y = old_y;
			mvprintw(Map::DISPLAY +2,0,"This is a freaking wall");
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
//Monster interaction starts combatloop then sets tile to treasure and pushes player back to old pos
		else if (isMonster(x,y,map) == true) {
			//combat function loop
			turn_off_ncurses();
//add asci monsters
//do differnet actions, maybe weak attack and strong attack, defend, maybe a run option? 
//combat function
			if (combat(playerLvl,playerHP, playerDP, playerSword) == true){
				mobKills++;
				map.setTile(x,y,Map::TREASURE);
			}
			else {
				map.setTile(x,y,Map::OPEN);
			}
			if (mobKills == 3) {
				playerLvl++;
				mobKills = 0;
			}
			x = old_x;
			y = old_y;
//redraws map after combat ends 
			drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);		
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
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice == 'c' || diagChoice == 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}	
				else if (npcNum == 2) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 3) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 4) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 5) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 6) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 7) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 8) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 9) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
				else if (npcNum == 10) {
           			char diagChoice;
					cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       				cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue, q to Quit:";
        			cin >> diagChoice;
       				if (diagChoice == 'q' || diagChoice == 'Q'){
            			diagChoice = '\0';
           				break;
       				}	
					system("clear");
					map.setTile(x,y,Map::OPEN);
					if (diagChoice != 'c' || diagChoice != 'C') {
						cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
       					cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        				cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to Continue:";
        				cin >> diagChoice;
					} else {
            			diagChoice = '\0';
           				break;
					}
				}
  	 		 }
		x = old_x;
		y = old_y;
// redraws map after NPC function ends
		drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);
		}
//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y or descriptor == Map::WATER or descriptor == Map::MONSTER) {
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i Lvl: %i\n", x, y, playerHP, playerGP, playerLvl);
			description(descriptor); //function to set tile description
			refresh();
		}
//need to add a win check and win screen
		if (playerCrown == 5) {
			char quitButton = '\0';
            turn_off_ncurses();
            system("clear");
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
            cout << "\n\n\n  " << playerName << "YOU WIN!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress q to Quit:";
			cin >> quitButton;
           	break;
		}

//Game over check and game over screen
		if (playerHP < 1) {
			char quitButton = '\0';
			turn_off_ncurses();
			system("clear");
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
            cout << "\n\n\n  " << playerName << "YOU DIED!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress q to Quit:";
			cin >> quitButton;
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
