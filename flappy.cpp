#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<windows.h>

using namespace std;

//**********************************
// border wall

class BorderWall {	
	int width, hieght;
	
public:
	BorderWall(int,int);
	void coordinate(int,int);
	void displayBorder();

};


BorderWall::BorderWall(int w, int h) {
	width = w;
	hieght = h;
}

void BorderWall::coordinate(int x,int y){
     COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
}

void BorderWall::displayBorder() {
	// border 
	for(int i = 1; i <= width; i++) {
		for(int j = 1; j <= hieght; j++) {
			if( (i==1) || (j==1) || (i==width) || (j==hieght) ) {
				coordinate(i, j);
				std::cout << "X";
			} //else {
			//	coordinate(i, j);
			//	cout << "";
		//	}			
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

	
	
public:
	MovingWall(int, int, int, int);
	void coordinate(int ,int);
	void setWall(int);
	void moveWall();
	int setGapLocation();
};

MovingWall::MovingWall(int x, int y, int w, int h) {
	xStart = x;
	yStart = y;
	width = w;
	hieght = h;
}

void MovingWall::coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
}

void MovingWall::setWall(int gapLocation) {
	moveWall();
	for(int i = xStart; i < width+xStart; i++) {
		for(int j = yStart; j < gapLocation; j++) {
			coordinate(i, j);
			cout << "#";
		}
	}	
	for(int i = xStart; i < width+xStart; i++) {
		for(int j = gapLocation+5; j < hieght+yStart; j++) {
			coordinate(i, j);
			cout << "#";
		}
	}
}

void MovingWall::moveWall() {
	xStart--;	
}

int MovingWall::setGapLocation() {
	return (rand()%10+5);
}





//**********************************
// the avatar

class Avatar {
	int xPos;
	int yPos;
	int borderW;
	int borderH;
	int gravity = 1;
//	int velocity = 0;
	
public:
	Avatar(int, int);
	void coordinate(int, int);
	void displayAvatar();
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

void Avatar::coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
}

void Avatar::displayAvatar() {
	avatarGravity();
	coordinate(xPos, yPos);
	cout << ">><>";
		
}

void Avatar::avatarGravity() {
	//velocity+=gravity;
	//************** change to variables
	if(yPos >= 23 ) {
		yPos = 23;
	} 
	else if(yPos < 2){
		yPos=2;
	} else {
		yPos += gravity;
	
	}
}

void Avatar::flyUp(int keyInput) {
	if(keyInput == 32) {
		yPos-=3;
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
// game play

class GamePlay {
	int borderW = 40;
	int borderH = 24;
	int wallW = 2;
	int wallH = borderH-1;
	int wallXStart = borderW-wallW;
	int walls[3] = {0};
	
public:
	GamePlay();
	void movingWalls();
	void play();	
	
};

GamePlay::GamePlay() {
	
	BorderWall border(borderW, borderH);
	MovingWall wall(wallXStart, 1, wallW, wallH);
	Avatar avatar(borderW, borderH);
	
}


void GamePlay::play() {

	
	BorderWall border(borderW, borderH);
	MovingWall wall(wallXStart, 1, wallW, wallH);
	Avatar avatar(borderW, borderH);
	

	
	do {
		
		
		
		
		system("cls");
		
		for(int i = 0; i < 3; i++) {
			if(walls[i] == 0) {
				walls[i] = wall.setGapLocation();
			}
		}
		
		
	/*
		wallXStart--;
		
		if(wallXStart==1) {
			wallXStart = borderW-wallW;
		}
		
*/
		wall.setWall(walls[0]);
		border.displayBorder();
		avatar.displayAvatar();
		avatar.flyUp(avatar.keyInput());

		
		for(int delay = 1; delay <= 60000000; delay++);
	
		} while (wallXStart > 1);	
}

int main() {
    srand(time(0));	
    
	GamePlay newGame;
	
	newGame.play();
	

	

	return 0;
}

