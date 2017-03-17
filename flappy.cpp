#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<windows.h>

using namespace std;


// seperate coordinate function to use through the code
coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   

}


//**********************************
// border wall

class BorderWall {	
	int width, hieght;
	
public:
	BorderWall(int,int);
	void displayBorder();
};

BorderWall::BorderWall(int w, int h) {
	width = w;
	hieght = h;
}

void BorderWall::displayBorder() {
	// border 
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < hieght; j++) {
			if( (i==0) || (j==0) || (i==width-1) || (j==hieght-1) ) {
				coordinate(i, j);
				std::cout << "X";
			} 
		}
	}
}


//**********************************
// moving wall

class MovingWall {

	int xStart;
	int yStart;
	int width;
	int hieght;
	int xMoving;	
	
public:
	MovingWall(int, int, int, int);
	void setXMoving(); // refresh start location
	int setWall(int);
	void moveWall();
	int setGapLocation();
};

MovingWall::MovingWall(int x, int y, int w, int h) {
	xStart = x; // permanent start location
	yStart = y;
	width = w;
	hieght = h;
	xMoving = x; // refresh start location of wall
}

void MovingWall::setXMoving() {
	xMoving = xStart;
}

int MovingWall::setWall(int gapLocation) {
	moveWall();
	if(xMoving > 1) {
		// top wall
		for(int i = xMoving; i < width+xMoving; i++) {
			for(int j = yStart; j < gapLocation; j++) {
				coordinate(i, j);
				cout << "#";
			}
		}
		// bot wall
		for(int i = xMoving; i < width+xMoving; i++) {
			for(int j = gapLocation+5; j < hieght+yStart; j++) {
				coordinate(i, j);
				cout << "#";
			}
		}
	} else {
		setXMoving();
	}
	return xMoving;
}

void MovingWall::moveWall() {
	xMoving--;	
}

int MovingWall::setGapLocation() {
	return (rand()%12+4);
}


//**********************************
// the avatar

class Avatar {
	int xPos;
	int yPos;
	int borderW;
	int borderH;
	int gravity = 1; // change in y
//	int velocity = 0;
	
public:
	Avatar(int, int);
	int displayAvatar();
	void avatarGravity();
	void flyUp(int);
	int keyInput();
};

Avatar::Avatar(int borderW, int borderH) {
	borderW = borderW;
	borderH = borderH;
	xPos = (borderW/5);
	yPos = (borderH/3);
}

int Avatar::displayAvatar() {
	avatarGravity();
	flyUp(keyInput());
	coordinate(xPos, yPos);
	cout << ">>";
	return yPos;		
}

void Avatar::avatarGravity() {
	//velocity+=gravity;
	if(yPos >= borderH-2) {
		yPos = borderH-2;
	} 
	else if(yPos < 1){
		yPos=1;
	} else {
		yPos += gravity;	
	}
}

void Avatar::flyUp(int keyInput) {
	if(keyInput == 32) {
		yPos-=2;
	}
}

int Avatar::keyInput() {
	int key;
	if (_kbhit()) {
		key = _getch();
		return key;
	}
	return 0;
}


//**********************************
// start screen

class StartScreen {
	string choice[2] = {"1.Start", "2.Quit"};
	int picked = 0;
public:
	int options();
};

int StartScreen::options() {
	
	coordinate(10, 6);
	cout << "FLAPPY!";
	
	for(int i = 0; i < 2; i++) {
		coordinate(10, 8+i);
		cout << choice[i];
	}
	
	coordinate(10, 11);
	cout << ">> ";
	cin >> picked;
	return picked;
}


//**********************************
// game play

class GamePlay {
	int borderW = 28;
	int borderH = 25;
	int wallW = 3;
	int wallH = borderH-1;
	int wallXStart = borderW-wallW;
	int wallPos = borderW-wallW;
	int wallGap = 0;
	int flappyY = 0;
	int score = 0;
	int option = 0;
	int spacebar = 0;
	char cont;
	bool gameOn;
	bool keepPlaying;
	
public:
	void gameOver();
	void displayStart();
	void displayScore(int, int);
	void trackScore();
	bool collision();
	bool play();
};              

void GamePlay::gameOver() {	
	system("CLS");
	coordinate(0, 10);
	cout <<"\t***************\n"
		<<"\t   GAME OVER\n"
		<<"\t***************\n";
}

void GamePlay::displayStart() {
	if(option == 1) {
		gameOn = true;
		score = 0;
	}
	if(option == 2) {
		exit(0);
	}	
}

void GamePlay::displayScore(int x, int y) {
	coordinate(x, y);
	cout << "score: " << score;
}

void GamePlay::trackScore() {
	if(wallPos == borderW/6 ) {
		score++;
	}
}

bool GamePlay::collision() {
	if( (((borderW/5)+1 >= wallPos) && ((borderW/5)-wallW <= wallPos) ) && ((flappyY  < wallGap) || (flappyY > wallGap+5)) ) {
		gameOn = false;
			
		gameOver();				
		displayScore(32, 3);			
		coordinate(5, 15);
		cout << "continue (y)/(n)";
		coordinate(12, 16);
		cin >> cont;
		system("CLS");
		if(cont=='y' || cont=='Y') {
			return true;
		}else{
			return false;
		}			
	}
}

bool GamePlay::play() {	

	BorderWall border(borderW, borderH);
	MovingWall wall(borderW-wallW, 1, wallW, wallH);
	Avatar avatar(borderW, borderH);
	StartScreen start;
	
	border.displayBorder();
	option = start.options();

	displayStart();

	do {
		coordinate(2, 14);
		cout << "Press \"SPACEBAR\" to fly!\n";
		spacebar = avatar.keyInput();
	} while (spacebar != 32);

	
	do {		
		system("CLS");				
	
		if(wallPos == wallXStart) {
			wallGap = wall.setGapLocation();
		}
		
		flappyY = avatar.displayAvatar();
		border.displayBorder();
		wallPos = wall.setWall(wallGap);		
		keepPlaying = collision();
		trackScore();		
		displayScore(32, 3);
		
		for(int delay = 1; delay <= 50000000; delay++);
	} while (gameOn);
	return keepPlaying;
}


//**********************************
// main()

int main() {
	srand(time(0));
	
	bool keepPlaying = false;	   
	GamePlay newGame;
	
	do {
		keepPlaying = newGame.play();
		
	} while (keepPlaying);

	return 0;
}

