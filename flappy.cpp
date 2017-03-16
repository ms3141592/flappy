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
	void coordinate(int ,int);
	void setXMoving();
	int setWall(int);
	void moveWall();
	int setGapLocation();
};

MovingWall::MovingWall(int x, int y, int w, int h) {
	xStart = x;
	yStart = y;
	width = w;
	hieght = h;
	xMoving = x;
}

void MovingWall::coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
}

void MovingWall::setXMoving() {
	xMoving = xStart;
}

int MovingWall::setWall(int gapLocation) {
	moveWall();
	if(xMoving > 1) {
		for(int i = xMoving; i < width+xMoving; i++) {
			for(int j = yStart; j < gapLocation; j++) {
				coordinate(i, j);
				cout << "#";
			}
		}	
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
	int gravity = 1;
//	int velocity = 0;
	
public:
	Avatar(int, int);
	void coordinate(int, int);
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

void Avatar::coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
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
	//************** change to variables
	if(yPos >= 23 ) {
		yPos = 23;
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
		void coordinate(int, int);
		int options();
};

void StartScreen::coordinate(int x,int y){
    COORD pos = {x,y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);   
}

int StartScreen::options() {
	
	coordinate(10, 6);
	cout << "Flappy";
	for(int i = 0; i < 2; i++) {
		coordinate(10, 8+i);
		cout << choice[i];
	}
	
	coordinate(10, 13);
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
	char cont;
	bool gameOn;
	
public:
	bool play();
	void gameOver();
		
};              

void GamePlay::gameOver() {	
	cout << "\t***************\n"
		<<"\t    GAME OVER\n"
		<< "\t***************\n";
}

bool GamePlay::play() {	

	BorderWall border(borderW, borderH);
	MovingWall wall(borderW-wallW, 1, wallW, wallH);
	Avatar avatar(borderW, borderH);
	StartScreen start;
	gameOn = false;
	
	border.displayBorder();
	option = start.options();
	if(option == 1) {
		gameOn = true;
	}
	
	
	
	do {
		
		system("cls");
				
	
		if(wallPos == wallXStart) {
			wallGap = wall.setGapLocation();
		}

		
		wallPos = wall.setWall(wallGap);

				
		// collision
		if( (((borderW/5)+1 >= wallPos) && ((borderW/5)-wallW <= wallPos) ) && ((flappyY  < wallGap) || (flappyY > wallGap+5)) ) {
			gameOn = false;
			wall.coordinate(0, 10);
			gameOver();
			
			
			wall.coordinate(32, 3);
			cout << "score: " << score;
			
			wall.coordinate(5, 15);
			cout << "continue (y)/(n)";
			wall.coordinate(12, 16);
			cin >> cont;
			if(cont=='y' || cont=='Y') {

				
				
				system("cls");
				return true;
			}else{
				return false;
			}
			
		}
		if(wallPos == borderW/6 ) {
			score++;
		}
		
		
		wall.coordinate(32, 3);
		cout << "score: " << score;

		flappyY = avatar.displayAvatar();
		border.displayBorder();
		
		for(int delay = 1; delay <= 50000000; delay++);

		
	} while (gameOn);
	
}



//**********************************
// main()



int main() {
	bool keepPlaying = false;
	
    srand(time(0));	
    
	GamePlay newGame;
	
	do {
		keepPlaying = newGame.play();
		
	} while(keepPlaying);
	

	

	

	return 0;
}

