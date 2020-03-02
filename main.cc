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
	else if (tileDescriptor == Map::WALL) {
		mvprintw(Map::DISPLAY +2,0,"AN ANCIENT AND CRUMBLING STONE WALL");
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


// this function just displays the monster inside of the combat function
bool encounter() {
       	char combatChoice = '\0';
		cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
        cout << "\n\n\n\n\n\n\n     ARRRG A SCARY MONSTER!!!\n\n\n\n\n\n\n\n\n\n\n\n";
        cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress c to continue, r to runaway:";
        cin >> combatChoice;
        if (combatChoice == 'r' || combatChoice == 'R'){
            return false;
        }
        else if (combatChoice == 'c' || combatChoice == 'C'){
			return true;
        }
		return true;
}

//magic 0 = no magic, 1 = Ring of Finger Guns, 2 = Amulet of Noxious Burp, 3 = Belt of Fire Flatulance, 4 = Shoe of Giant Size, 5 = Bob the Wizard's wand.
void magicDescription (int magic) {
	if (magic == 0) { 
		cout << "Kyk has no Magicks!\n";
	} else if (magic == 1) {
		cout << "Kyk raises both hands pointing. Carefully cocking each thumb back before firing burst of pure Ego into the Monster!\n";
	} else if (magic == 2) {
		cout << "Kyk reaches under his armor and rubs his belly, a deep growling starts in the Goblins Belly before Kyk belches a green cloud of smoke into the Monster!\n";	
	} else if (magic == 3) {
		cout << "Kyk turns around, putting his head between his legs and takes aim, farting a blast of pure inferno into the Monster!\n";
	} else if (magic == 4) {
		cout << "Kyk screams\"MAKE MY MONSTER GROW!\" and quickly grows several stories tall and steps on the Monster before shrinking back to his normal size.\n";
	} else if (magic == 5) {
		cout << "Kyk holds the plain wooden wand in two hands and carefully aim at the monster. Suddenly a old man appears and charges at the Monster slapping it with a shoe!\n";
	}
}

//swords, 1 = bareknuckles, 2 = Dull Rusty Blade, 3 = Steel Short Sword, 4 = Cursed blade 'Misery's Edge',5 = Susan
void swordDescription (int sword) {
    if (sword == 1) {
        cout << "Kyk says \"These fist! These fists are like weapons!\" before scratching at the monsters face.\n";
    } else if (sword == 2) {
        cout << "Kyk raises the dull rust red blade high swinging wildly in all directions getting many slashes into the Monster's side!\n";
    } else if (sword == 3) {
        cout << "With Kyk's great skill at the sword, the steel short sword makes quick and accurate strikes into the Monster!\n";
    } else if (sword == 4) {
        cout << "The pure black blade cries tears down it's cursed edge, as Kyk slashes at the Monster the Blades magic reaches out causing great emotional damage.\n";
    } else if (sword == 5) {
        cout << "The goblins knuckles go white, gripping the sword. Susan the Ancient Soul Blade burst into magnificent glory! Shining out Golden Light, Susan guides Kyk's hand into the perfect strike!\n";
    }

}
//armor, 1 = shirtless, 2 = leather shirt, 3 = Chain Mail Shirt, 4 = cloak of agility, 5 = Bracers of Shadow Kin  
void armorDescription (int playerDP) {
	    if (playerDP == 1) {
			cout << "Kyk tries as hard as he can to not get scratched, maimed, or brutalized.\n";
    } else if (playerDP == 2) {
		cout << "The thick leathers protect Kyk's rotund pot belly\n";
    } else if (playerDP == 3) {
		cout << "The oversized chain mail shirt, confuses the monster because they've never seen a Goblin in a dress before.\n";
    } else if (playerDP == 4) {
		cout << "The runecast skywhale leather cloak pushes the small goblin out of harm's way.\n";
    } else if (playerDP == 5) {
		cout << "Shadows rise from the corners of everything in the area, slowly forming living shadows of the Goblin King. They intercede the Monster's attack saving Kyk from most of the damage.\n";
    }
}

// might need to balance combat HP/damage numbers but the system is complete
// Combat function
bool combat(int playerLvl,int& playerLives, int& playerHP, int playerDP, int sword, int& magic ) {
	srand(time(0));
	int outcome = 0;
	int playerAtk = 0;
	int mobAtk = 0;
	int mobHP = 0;

	if (playerLvl == 1) mobHP = 30;
	if (playerLvl == 2) mobHP = 50;
	if (playerLvl == 3) mobHP = 130;
	if (playerLvl == 4) mobHP = 190;
	if (playerLvl == 5) mobHP = 220;
	if (playerLvl > 5 && sword == 5) mobHP = 400;

	if (encounter() == false) {
		return false;
	}
	system("clear");
	while(true) {
		char combatChoice;
		cout << "Player Health:" << playerHP << endl;
	//comment out mob HP before deployment
		cout << "Mob HP:" << mobHP << endl; 
		cout << "\nPress A for attack  D for defend  R for rabid attack M for magic:";
		cin >> combatChoice;
		if (combatChoice == 'A'|| combatChoice == 'a') {
			playerAtk =(rand() % 10 + playerLvl) * sword;
			swordDescription (sword);
			cout << "Kyk Dealt " << playerAtk << " Damage!\n";
			mobHP = mobHP - playerAtk;
			if (mobHP < 1) break;
			mobAtk = (rand() % 10 + 1) - playerDP;
			cout << "The monster dealt " <<  mobAtk << " Damage!\n";
			playerHP -= mobAtk;
			usleep(10000);
		}
		else if (combatChoice == 'D' || combatChoice == 'd') {
			playerAtk = 0;
			cout << "Defend! Kyk deals no damage!\n";
			mobHP -= playerAtk;
			mobAtk = (rand() % 10 + 1) - (playerDP * 4);
			cout << "The monster dealt " << mobAtk << " Damage!\n";
			playerHP = playerHP - mobAtk;
			usleep(10000);
		}
		else if (combatChoice == 'R' || combatChoice == 'r'){
			playerAtk =(rand() % 20) * playerLvl;
			cout << "Kyk starts twitching and squeeling. He drops his swords, hands shaking so bad. Suddenly he leaps, biting at the monsters face!\n";
			cout << "Kyk Dealt " << playerAtk << " Damage!\n";
			mobHP = mobHP - playerAtk;
			if (mobHP < 1) break;
			mobAtk = (rand() % 10 + 10) * 2;
			cout << "The monster dealt " <<  mobAtk << " Damage!\n";
			playerHP -= mobAtk;
			usleep(10000);
		}
		else if (combatChoice == 'M' || combatChoice == 'm'){
			playerAtk =(rand() % 30) * magic * sword;
			if (rand() % 4 == 0) {
				playerHP -= playerAtk / playerLvl;
				magic--;
				cout << "Magickal Backlash Scorches Kyk!";
			}
			magicDescription(magic);
			cout << "Kyk dealt " << playerAtk << " Damage!\n";
			mobHP = mobHP - playerAtk;
			if (mobHP < 1) break;
			mobAtk = (rand() % 10 + 1) - playerDP;
			cout << "The monster dealt " <<  mobAtk << " Damage!\n";
			playerHP -= mobAtk;
			usleep(10000);	
		}
		if (playerHP < 1){
			system("clear");
			cout << "You have been slain by a foul monster!\n\n Sacrifice a fraction of your soul to to the shrunken warlock's head, PRESS L TO LIVE, or PRESS Q to QUIT!";
			char temp = '\0';
			cin >> temp;
			if (temp == 'l' || temp == 'l'){
				playerHP = 10;
				break;
			}
			else {
				return 0;
			}
		}

		else if (mobHP < 1)	{
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

//print from file loop
void dialog (int npcNum) {
	while (true) {
		char diagChoice;
		string temp;
		string temp2;
		cout << setw(60) << setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
		if (npcNum == 1) {
	/*		ifstream inFile("NPC1.txt");
			while (getline(inFile,temp,'\n')){
				temp += temp + " ";	
				}
			cout << temp;
			*/
			} 	
		else if (npcNum == 2) {
			ifstream inFile("NPC2.txt");
		}
		else if (npcNum == 3) {
			ifstream inFile("NPC3.txt");
		}
		else if (npcNum == 4) {
			ifstream inFile("NPC4.txt");
		}
		else if (npcNum == 5) {
			ifstream inFile("NPC5.txt");
		}
		else if (npcNum == 6) {
			ifstream inFile("NPC6.txt");
		}
		else if (npcNum == 7) {
			ifstream inFile("NPC7.txt");
		}
		else if (npcNum == 8) {
			ifstream inFile("NPC8.txt");
		}
		else if (npcNum == 9) {
			ifstream inFile("NPC9.txt");
		}
		else if (npcNum == 10) {
			ifstream inFile("NPC10.txt");
		}
		cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
		cout << endl << "Press q to quit\n";
		cin >> diagChoice;
    	if (diagChoice == 'q' || diagChoice == 'Q'){
   			diagChoice = '\0';
			break;
    	}
	}

}

//shopkeeper function
//Kajiit has wares if you have coin
//ASCII CAT

// inventory function
//swords, 1 = bareknuckles, 2 = Dull Rusty Blade, 3 = Steel Short Sword, 4 = Cursed blade 'Misery's Edge',5 = Susan
//armor, 1 = shirtless, 2 = leather shirt, 3 = Chain Mail Shirt, 4 = cloak of agility, 5 = Bracers of Shadow Kin

//magic 0 = no magic, 1 = Ring of Finger Guns, 2 = Amulet of Noxious Burp, 3 = Belt of Fire Flatulance, 4 = Shoe of Giant Size, 5 = Bob the Wizard's wand. 

// Display number of Souls in the shrunken warlock's head.

// main
int main() {
//	game variables
	string playerName = "";
	char descriptor = '\0';
	int playerLvl = 1;
	int playerLives = 5;// this is soul fragments.
	int playerHP = 20 + (playerLvl * 10);
	int playerDP = 1;
// player defense points starts at 0. player buys better armor and this goes up 
	int playerSword = 1;
	int magic = 0;
// player sword starts at 1. player buys better weapons this goes up by 1
	int playerGP = 0;
	int mobKills = 0;
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
			cout << "\n\n\t\tWELCOME TO THE GRAND ADVENTURES\n                        OF\t              KYK THE GOBLIN KING\n\n\n\n\t        Please enter player name to start\n";
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << "Name:";
			cin >> playerName;
			system("clear");
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
			cout << "\n  LONG LONG AGO IN THE DISTANT LAND OF LEVIATHAN IN\n  THE WILD AND CHAOTIC NORTH THERE WAS A SMALL GOBLIN\n  NAMED KYK. SMALL EVEN AMOUNGST OTHER GOBLINS KYK WAS\n  ONCE RIDICULED BY EVERYONE, BUT NO LONGER BECAUSE\n  KYK THROUGH SURE TENACITY AND A STRONG BITE BECAME\n  THE KING OF ALL GOBLIN KIND. HIS SMALLER THAN\n  AVERAGE HEAD MADE THE BURDEN OF WEARING ALL FIVE\n  CROWNS OF THE ONCE SEPERATE GOBLIN NATIONS IMPOSSIBLE,\n  SO UPON HIS CORINATION KYK HAD ALL FIVE CROWNS\n  FORGED INTO A GOLDEN CHAIN HE WORE AROUND HIS NECK.\n\n  AFTER SEVERAL WEEKS KYK GREW TIRED OF HIS KINGLY\n  DUTIES AND WANDERED OFF TO FIND SOME ADVENTURE.\n  AND ADVENTURE HE DID FIND! SLAYING DRAGONS, ELVES,\n  DWARFS, HOOMANS, EVEN A GIANT! NO BEAST OR FOE\n  COULD STAND BEFORE THE WHIRLING BLADES OF THE KING\n  OF ALL GOBLINS. BUT THEN ONE NIGHT AT A TAVERN\n  KYK LOST EVERYTHING HE OWNED IN A GAME OF CARDS\n  TO A SHREWED AND CALCULATING KAJIIT.\n\n\n  NOW WE JOIN OUR GOBLIN KING THE MORNING AFTER.\n  IT'S YOUR JOB AS THE PLAYER TO COLLECT MONEY TO BUY BACK\n  HIS POSSESSIONS FROM THE DEVIOUS KAJIIT,\n  AND FIND THE FIVE GOLBIN CROWNS HE HID IN THE\n  WILDERNESS OF THE NORTH!\n\n";
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
			descriptor = Map::WALL;
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
//			drowned++;
			x = old_x;
			y = old_y;
//			if (drowned = 5) {
//				playerGP -= 5;
//	 		    mvprintw(Map::DISPLAY +2,0,"Stop drowning you dummy! I'm gonna take this! Stay out of my water!"}		
//			}
		}
//Monster interaction starts combatloop then sets tile to treasure and pushes player back to old pos
		else if (isMonster(x,y,map) == true) {
			//combat function loop
			turn_off_ncurses();
//combat function
			if (combat(playerLvl, playerLives, playerHP, playerDP, playerSword, magic) == true){
				mobKills++;
				map.setTile(x,y,Map::TREASURE);
			}
			else {
				map.setTile(x,y,Map::OPEN);
			}
			if (mobKills == 6) {
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
					dialog(npcNum);
					break;
				}	
				else if (npcNum == 2) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 3) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 4) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 5) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 6) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 7) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 8) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 9) {
					dialog(npcNum);
					break;
				}
				else if (npcNum == 10) {
					dialog(npcNum);
					break;
				}
  	 		}
		map.setTile(x,y,Map::OPEN);
		x = old_x;
		y = old_y;
// redraws map after NPC function ends
		drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);
		}
//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y or descriptor == Map::WATER or descriptor == Map::MONSTER or descriptor == Map::WALL) {
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
			cout << "\n\n\n  " << playerName << " Won!\n" << "CRY ALOUD BOLD AND PROUD OF WHERE I'VE BEEN\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress e to END:";
			cin >> quitButton;
           	break;
		}

//Game over check and game over screen
		if (playerHP < 1) {
			char quitButton = '\0';
			turn_off_ncurses();
			system("clear");
			cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl;
			cout << "\n\n\n  " << playerName << " Died!\n" <<"ONCE INVINCILBLE NOW THE ARMOR'S WEARING THIN HEAVY SHIELD DOWN\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            cout << setw(60) <<  setfill('_') << "_" << endl << setw(60) << setfill('_') << "_" << endl << endl <<"\nPress e to END:";
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
