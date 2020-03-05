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

bool isKajiit(int posX, int posY, Map shop){
    char tile;
    tile = shop.getTile(posX,posY);
    if (tile == Map::KAJIIT){
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

// this function just displays the monster inside of the combat function
bool encounter(int playerLvl) {
       	char combatChoice = '\0';
		system("clear");
        string mobScreen;
		if (playerLvl == 1) {
			ifstream inFile("output/mob3.txt");
			while (getline(inFile,mobScreen,'\n')){ 
			cout << mobScreen << endl;
			}
		} else if (playerLvl == 2) {
			ifstream inFile("output/mob4.txt");
			while (getline(inFile,mobScreen,'\n')){
				cout << mobScreen << endl;
			}
				
		} else if (playerLvl == 3) {
            ifstream inFile("output/mob1.txt");
			while (getline(inFile,mobScreen,'\n')){ 
				cout << mobScreen << endl;
			}
		} else if (playerLvl == 4) {
            ifstream inFile("output/mob2.txt");
			while (getline(inFile,mobScreen,'\n')){ 
				cout << mobScreen << endl;
			}
		} else if (playerLvl == 5) {
            ifstream inFile("output/mob6.txt");
			while (getline(inFile,mobScreen,'\n')){ 
				cout << mobScreen << endl;
			}
		} else if (playerLvl > 5) {
           	ifstream inFile("output/mob5.txt");
			while (getline(inFile,mobScreen,'\n')){ 
				cout << mobScreen << endl;
			}
		}
		cin >> combatChoice;
        if (combatChoice == 'r' || combatChoice == 'R'){
            return false;
        }
        else if (combatChoice == 'c' || combatChoice == 'C'){
			return true;
        }
		return true;
}

// might need to balance combat HP/damage numbers but the system is complete
// Combat function
bool combat(int playerLvl,int& playerLives, int& playerHP, int playerDP, int sword, int& magic ) {
	srand(time(0));
	int outcome = 0;
	int playerAtk = 0;
	int mobAtk = 0;
	int mobHP = 0;
	string mobDescription;

	if (playerLvl == 1){
		mobDescription = "The monster charges KYK tackling the small goblin!\n";//wolf
		mobHP = 30;
	}

	if (playerLvl == 2) {
		mobDescription = "The monster lashes out with it's sharp claws!\n";//bear
		mobHP = 50;
	}
	if (playerLvl == 3) {
		mobDescription = "The monster unleasing it's true power!\n";//skeleton
		mobHP = 70;
	}
	if (playerLvl == 4) { 
		mobDescription = "The monster bites The Goblin King with it's oversized maw!\n";//bat
		mobHP = 90;
	}
	if (playerLvl == 5) {
		mobDescription = "The monster lashes out with spectral energies!\n";//skull
		mobHP = 120;
	}
	if (playerLvl > 5 && sword == 5) {
		mobDescription = "The monster strikes out with magical weapons!\n";//zombi
		mobHP = 160;
	}
	if (encounter(playerLvl) == false) {
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
		//player inputs
		if (combatChoice == 'A'|| combatChoice == 'a') {
			combatChoice = '\0';
			playerAtk =(rand() % 10 + playerLvl) * sword;
			mobAtk = (rand() % (10 + 4))  * playerLvl;
			swordDescription (sword);
		}
		else if (combatChoice == 'D' || combatChoice == 'd') {
			playerAtk = 0;
			mobAtk = ((rand() % 8 + 1) * playerLvl) - playerDP;
			cout << "Defend! Kyk deals no damage!\n";
			armorDescription (playerDP);
		}

		else if (combatChoice == 'R' || combatChoice == 'r'){
			playerAtk =(rand() % 20) * playerLvl;
			mobAtk = (rand() % 30 + 1) * playerLvl;
			cout << "Kyk starts twitching and squeeling. He drops his swords, hands shaking so bad. Suddenly he leaps, biting at the monsters face!\n";
		}

		else if (combatChoice == 'M' || combatChoice == 'm'){
			playerAtk =(rand() % 30) * magic * sword;
			mobAtk = (rand() % 10 + 1) * playerLvl;
			if (rand() % 4 == 0) {
				playerHP -= playerAtk / playerLvl;
				magic--;
				cout << "Magickal Backlash Scorches Kyk!";
			}
			magicDescription(magic);
		}
		//player damage resolution and mob turn
		if (playerAtk < 1) playerAtk = 0;
		cout << "Kyk Dealt " << playerAtk << " Damage!\n";
		mobHP -= playerAtk;
		if (mobHP < 1) break;
		if (mobAtk < 1) mobAtk = 0;
		cout << mobDescription;
		cout << "The monster dealt " <<  mobAtk << " Damage!\n";
		playerHP -= mobAtk;
		cout << "PRESS C TO CONTINUE OR R TO RUNAWAY\nSACRIFICING HALF OF YOUR HEALTH:\n";
		char combatChoice2;
		cin >> combatChoice2;
		if (combatChoice2 == 'R' || combatChoice2 == 'r'){
			playerHP /= 2;
			return false;
		}
		else if (combatChoice2 != 'R' || combatChoice2 != 'r') {
			system("clear");
		}
			
		if (playerHP < 1){
			system("clear");
			usleep(10000);
			cout << "You have been slain by a foul monster!\n\n Sacrifice a fraction of your soul to to the shrunken warlock's head, PRESS L TO LIVE, or PRESS Q to QUIT!";
			cout << "You only have " << playerLives << " Soul Fragments left.";
			char temp = '\0';
			cin >> temp;
			if (temp != 'q' || temp != 'Q'){
				playerHP = 10 * playerLives;
				playerLives--;
				break;
			}
			else {
				return 0;
			}

		}
	

		if (mobHP < 1)	{
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
void dialog (int& npcNum, int& playerCrown,int& solved) {
	while (true) {
		char diagChoice;
		string temp;
		if (npcNum == 1) {
			ifstream inFile("output/NPC1a.txt");
			while (getline(inFile,temp,'\n')){
				cout << temp << endl;
			}
			cin >> diagChoice;
			if (diagChoice == 'q' || diagChoice == 'Q') break;
			system("clear");
			ifstream inFile2("output/NPC1b.txt");
			while (getline(inFile2,temp,'\n')) {
				cout << temp << endl;
			}
			string password;
			string guess;
			if ((rand() % 3) == 0) {
				password = "rock";
			}
			if ((rand() % 3) == 1) {
				password = "paper";
			}
			if ((rand() % 3) == 2) {
				password = "scissors";
			}
			cout << "ENTER PASSWORD:";
			cin >> guess;
			if (guess == password) {
				system("clear");
				ifstream inFile3("output/NPC1c.txt");
				while (getline(inFile3,temp,'\n')) {
					cout << temp << endl;
				}
				playerCrown++;
				solved = 1;
			}
			else {
				system("clear");
				ifstream inFile4("output/NPC1d.txt");
				while (getline(inFile4,temp,'\n')) {
					cout << temp << endl;
				}
				solved = 0;
			}
			cin >> diagChoice;
			break;

		}
		else if (npcNum == 2) {
			int playerGuess = 0;
			int NPCGuess = 0;
			int win = 0;
			ifstream inFile4("output/NPC2a.txt");
			while (getline(inFile4,temp,'\n')) {
				cout << temp << endl;
			}
			cin >> diagChoice;
			system("clear");
			ifstream inFile5("output/NPC2b.txt");
			while (getline(inFile5,temp,'\n')) {
				cout << temp << endl;
			}
			cin >> playerGuess;
			NPCGuess = (rand() % 10) + 1;
			if (!cin||playerGuess > 10) {
				cout << "That's against the rules!\n";
			}
			int playerDistance = abs(win - playerGuess);
			int NPCDistance = abs(win - NPCGuess);
			if (playerDistance < NPCDistance) {
				system("clear");
				ifstream inFile6("output/NPC2c.txt");
				while (getline(inFile6,temp,'\n')) {
					cout << temp << endl;
		   		}	
				playerCrown++;
				solved = 1;
				cin >> diagChoice;
				break;
			} else {
				system("clear");
				ifstream inFile7("output/NPC2d.txt");
				while (getline(inFile7,temp,'\n')) {
					cout << temp << endl;
				}
				solved = 0;
				cin >> diagChoice;
				break;	
			}
		}
		else if (npcNum == 3) {
			string playerGuess;
			string answer = "silence";
			ifstream inFile8("output/NPC3a.txt");
			while (getline(inFile8,temp,'\n')) {
				cout << temp << endl;
			}
			cin >> diagChoice;
			system("clear");
			ifstream inFile9("output/NPC3b.txt");
			while (getline(inFile9,temp,'\n')) {
				cout << temp << endl;
			}
			cout << "ENTER GUESS:";
			cin >> playerGuess;
	
			if (playerGuess == answer) {
				system("clear");
				ifstream inFile10("output/NPC3c.txt");
				while (getline(inFile10,temp,'\n')) {
					cout << temp << endl;
		   		}	
				playerCrown++;
				solved = 1;
				cin >> diagChoice;
				break;
			} else {
				system("clear");
				ifstream inFile11("outcome/NPC3d.txt");
				while (getline(inFile11,temp,'\n')) {
					cout << temp << endl;
				}
				solved = 0;
				cin >> diagChoice;
				break;	
			}
		}
		else if (npcNum == 4) {
			string playerGuess;
			string answer = "clock";
			ifstream inFile12("output/NPC4a.txt");
			while (getline(inFile12,temp,'\n')) {
				cout << temp << endl;
			}
			cin >> diagChoice;
			system("clear");
			ifstream inFile13("output/NPC4b.txt");
			while (getline(inFile13,temp,'\n')) {
				cout << temp << endl;
			}
			cout << "ENTER GUESS:";
			cin >> playerGuess;
	
			if (playerGuess == answer) {
				system("clear");
				ifstream inFile14("output/NPC4c.txt");
				while (getline(inFile14,temp,'\n')) {
					cout << temp << endl;
		   		}	
				playerCrown++;
				solved = 1;
				cin >> diagChoice;
				break;
			} else {
				system("clear");
				ifstream inFile11("outcome/NPC3d.txt");
				while (getline(inFile11,temp,'\n')) {
					cout << temp << endl;
				}
				solved = 0;
				cin >> diagChoice;
				break;	
			}
		}
		else if (npcNum == 5) {
			string playerGuess;
			string answer = "bob";
			ifstream inFile16("output/NPC5a.txt");
			while (getline(inFile16,temp,'\n')) {
				cout << temp << endl;
			}
			cin >> diagChoice;
			system("clear");
			ifstream inFile17("output/NPC5b.txt");
			while (getline(inFile17,temp,'\n')) {
				cout << temp << endl;
			}
			cout << "ENTER GUESS:";
			cin >> playerGuess;
	
			if (playerGuess == answer) {
				system("clear");
				ifstream inFile18("output/NPC5c.txt");
				while (getline(inFile18,temp,'\n')) {
					cout << temp << endl;
		   		}	
				playerCrown++;
				solved = 1;
				cin >> diagChoice;
				break;
			} else {
				system("clear");
				ifstream inFile11("outcome/NPC3d.txt");
				while (getline(inFile11,temp,'\n')) {
					cout << temp << endl;
				}
				solved = 0;
				cin >> diagChoice;
				break;	
			}
		}
		else if (npcNum == 6) {
			
		}
		cin >> diagChoice;
    	if (diagChoice == 'q' || diagChoice == 'Q'){
   			diagChoice = '\0';
			break;
    	}
	}

}

//shopkeeper function
//Kajiit has wares if you have coin
void shopkeeping(int& playerLives, int& playerHP,int& playerGP,int& playerDP,int& playerSword, int& magic) {
	string temp;
	char selector;
	int price = 0, swordPrice = 0, armorPrice = 0, magicPrice = 0, healthPrice = 10, livesPrice = 0;
	swordPrice = (5 *(playerSword + 1));
	armorPrice = (3 * (playerDP + 1));
	magicPrice = (10 * (magic +1));
	livesPrice = (10 + (playerGP / 3));

	ifstream inFile("output/shop1.txt");
	while (getline(inFile,temp,'\n')) {
		cout << temp << endl;
	}
	inFile.close();
	cout << "KAJIIT HAS WARES IF YOU HAVE COIN\n";
	
	cin >> selector;
	if (selector != 'q' || selector != 'Q') {
		while (true) {
			system("clear");
			inFile.open("output/shop1.txt");

			while (getline(inFile,temp,'\n')) {
				cout << temp << endl;
			}
			inFile.close();
			cout << "PLAYER GOLD:" << playerGP << endl;
			cout << "1) A SWORD UPGRADE IS " << swordPrice << ":\n";
			cout << "2) An ARMOR UPGRADE IS " << armorPrice << ":\n";
			cout << "3) A MAGICK ITEM IS " << magicPrice << ":\n";
			cout << "4) A SOUL FRAGMENT IS " << livesPrice << ":\n";
			cout << "5) SOME HEALTH IS " << healthPrice << ":\n";
			cout << "ENTER YOUR SELECTION:\n" << "Q TO QUIT\n";
			int selector2 = 0;
			int sel = 0;
			cin >> selector2;
			if (selector2 == 1) {
				if (playerGP < swordPrice) {
					cout << "YOU DON'T HAVE ENOUGH!\n";
					usleep(1000000);
					continue;
				}
				playerSword++;
				playerGP -= swordPrice;
				cout << "YOU BOUGHT A NEW SWORD!\n";
			} else if (selector2 == 2) {
				if (playerGP < armorPrice) {
					cout << "YOU DON'T HAVE ENOUGH!\n";
					usleep(1000000);
					continue;
				}
				playerDP++;
				playerGP -= armorPrice;
				cout << "YOU BOUGHT SOME NEW CLOTHES!\n";
			} else if (selector2 == 3) {
				if (playerGP < magicPrice) {
					cout << "YOU DON'T HAVE ENOUGH!\n";
					usleep(1000000);
					continue;
				}
				magic++;
				playerGP -= magicPrice;
				cout << "YOU BOUGHT SOME NEW MAGIC ITEMS!\n";
			} else if (selector2 == 4) {
				if (playerGP < livesPrice) {
					cout << "YOU DON'T HAVE ENOUGH!\n";
					usleep(1000000);
					continue;
				}
				playerLives++;
				playerGP -= livesPrice;
				cout << "YOU BOUGHT A NEW SOUL FRAGMENT! BUT WHOSE\n";

			} else if (selector2 == 5) {
				if (playerGP < healthPrice) {
					cout << "YOU DON'T HAVE ENOUGH!\n";
					usleep(1000000);
					continue;
				}
				playerGP -= healthPrice;
				playerHP += 20;
				cout << "YOU BOUGHT A BIT OF HEALTH!\n";
			} else {
				break;
			}
			selector2 = '\0';
		}
	} else {
		selector = '\0';
		usleep(1000000);
		}
}



//ASCII CAT

// inventory function
//swords, 1 = bareknuckles, 2 = Dull Rusty Blade, 3 = Steel Short Sword, 4 = Cursed blade 'Misery's Edge',5 = Susan
//armor, 1 = shirtless, 2 = leather shirt, 3 = Chain Mail Shirt, 4 = cloak of agility, 5 = Bracers of Shadow Kin

//magic 0 = no magic, 1 = Ring of Finger Guns, 2 = Amulet of Noxious Burp, 3 = Belt of Fire Flatulance, 4 = Shoe of Giant Size, 5 = Bob the Wizard's wand. 

// Display number of Souls in the shrunken warlock's head.

void inventory(int playerLives,int playerCrown, int playerDP, int playerSword, int magic) {
//	while (true) {
		string temp;
		char exit;
		string inventoryScreen;
		ifstream inFile("output/item.txt");
		while (getline(inFile,inventoryScreen,'\n')) {
			cout << inventoryScreen << endl;
		}
		cout << "KYK HAS " << playerLives << " SOUL FRAGMENTS!\n\n";
		cout << "KYK HAS COLLECTED " << playerCrown << " CROWNS OUT OF 5!\n\n";
	//swords
		if (playerSword == 1) {
			cout << "KYK HAS HIS BARE HANDS!\n";
		}
		else if (playerSword == 2) {
			cout << "KYK HAS A RUSTED SWORD!\n";
		}
		else if (playerSword == 3) {
			cout << "KYK HAS A FINELY CRAFTED STEEL SHORT SWORD!\n";
		}
		else if (playerSword == 4) {
			cout << "KYK HAS A CURSED BLADE THAT WEEPS CONSTANTLY!\n";
		}
		else if (playerSword == 5) {
			cout << "KYK HAS SUSAN! HIS FAVORITE SWORD EVER!\n";
		}
		cout << endl;
	//armor
		if (playerDP == 1) {
			cout << "NO SHIRT NO SHOES\n";
		}
		else if (playerDP == 2) {
			cout << "A LEATHER SHIRT!\n";
		}
		else if (playerDP == 3) {
			cout << "A FINELY CRAFTED CHAINMAIL SHIRT 'JUST A BIT BIG'!\n";
		}
		else if (playerDP == 4) {
			cout << "KYK DONS A LIVING MAGICAL CLOAK THAT PROTECTS HIM EVERY MOMENT!\n";
		}
		else if (playerDP == 5) {
			cout << "KYK HAS THE VERY SHADOWS OF THE LAND WRAPPED AROUND HIM!\n";
		}
		cout << endl;
	// magic
		if (magic == 0) {
			cout << "NO MAGICKS!\n";
		}
		else if (magic == 1) {
			cout << "KYK WEARS A GOLDEN RING OF FINGERGUNS!\n";
		}
		else if (magic == 2) {
			cout << "AROUND HIS NECK A SICKLY GREEN GEMS HANGS FROM A SILVER CHAIN!\n";
		}
		else if (magic == 3) {
			cout << "KYK HAS A THICK LEATHER BELT WITH A GOLDEN FIRBALL BELT BUCKLE!\n";
		}
		else if (magic == 4) {
			cout << "A OVERLY LARGE SHOE!\n";
		}
		else if (magic == 5) {
			cout << "A PLAIN WOODEN WAND!\n";
		}
		cout << endl << "PRESS Q TO QUIT:";	
		cin >> exit;
//		break;
//	}
}


// main
int main() {
//	game variables
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
	int solved = 0;
//game code starts here
	turn_on_ncurses();
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;//menu button etc need to be right here
//inventory
		else if (ch == 'i' || ch == 'I') {
			turn_off_ncurses();
			//inventory function
			inventory(playerLives, playerCrown, playerDP, playerSword, magic);
//		cout << "make a selection 1) Lives 2)HP 3)DP 4)sword 5)magic 6)GP\n";
			drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);
//		cout << "make a selection 1) Lives 2)HP 3)DP 4)sword 5)magic 6)GP\n";
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
			string titleScreen;
			ifstream inFileTS("output/titlescreen1.txt");
            while (getline(inFileTS,titleScreen,'\n')){
				cout << titleScreen << endl;
            }
			char startGame;
			cin >> startGame;
			system("clear");
			ifstream inFileTS2("output/titlescreen2.txt");
			while (getline(inFileTS2,titleScreen,'\n')){
				cout << titleScreen << endl;
			}
			cout << "\n       Press C to continue...\n:";
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
//shopkeeper
		else if (isKajiit(x,y,map) == true) {
			turn_off_ncurses();
			//shopkeeper function
			shopkeeping(playerLives, playerHP, playerGP, playerDP, playerSword, magic);
			x = old_x;
			y = old_y;
			drawOn(x,y,map, playerHP, playerGP, playerLvl, descriptor);
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
				playerHP = 20 + (playerLvl * 10);
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
					dialog(npcNum, playerCrown,solved);
					break;
				}	
				else if (npcNum == 2) {
					dialog(npcNum, playerCrown,solved);
					break;
				}
				else if (npcNum == 3) {
					dialog(npcNum, playerCrown,solved);
					break;
				}
				else if (npcNum == 4) {
					dialog(npcNum, playerCrown,solved);
					break;
				}
				else if (npcNum == 5) {
					dialog(npcNum, playerCrown,solved);
					break;
				}
				/*
				else if (npcNum == 6) {
					dialog(npcNum, playerCrown,solved);
					break;
				}
				else if (npcNum == 7) {
					dialog(npcNum, playerCrown,solved);
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
				*/
  	 		}
		if (solved == 1) {
			map.setTile(x,y,Map::OPEN);
		}
		if (solved == 0) {
			npcNum--;
		}
		solved = 0;
		x = old_x;
		y = old_y;
// redraws map after NPC function ends
		drawOn(x,y,map,playerHP,playerGP,playerLvl,descriptor);
		}
//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y or descriptor == Map::WATER or descriptor == Map::MONSTER or descriptor == Map::WALL or descriptor == Map::KAJIIT) {
			clear();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i Health: %i Gold: %i Lvl: %i\n", x, y, playerHP, playerGP, playerLvl);
			description(descriptor); //function to set tile description
			refresh();
		}
//need to add a win check and win screen
		if (playerCrown == 5) {
			char quitButton = '\0';
			string wonScreen;
            turn_off_ncurses();
            system("clear");
			ifstream inFile("output/gamewon.txt");
			while (getline(inFile,wonScreen,'\n')){
				cout << wonScreen << endl;
			}
			cin >> quitButton;
           	break;
		}

//Game over check and game over screen
		if (playerHP < 1) {
			char quitButton = '\0';
			string lostScreen;
            turn_off_ncurses();
            system("clear");
			ifstream inFile("output/gamelost.txt");
			while (getline(inFile,lostScreen,'\n')){
				cout << lostScreen << endl;
			}
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
