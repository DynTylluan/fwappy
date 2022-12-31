#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "pipeheightgen.h"

int flappymoved;

void* flappymv(void * arg){
	while(1){
		getch();
		flappymoved++;
	}
}

int flappyclscheck(int y, int a){	//Cowwision Check
	int b = 34 - (27-a);
	if(y <= a || y >= b){
		printw("flappypipea");
		return 1;
	}
	return 0;
}

void mainmenu(){
	wbkgd(stdscr,COLOR_PAIR(6));
	WINDOW * mainmenu = newwin(32,36,1,1);
	wbkgd(mainmenu,COLOR_PAIR(7));
	wattron(mainmenu,COLOR_PAIR(7));
	box(mainmenu,0,0);
	mvwprintw(mainmenu,1,1,"FWAPPY");
	mvwprintw(mainmenu,2,1,"A Wibwe Fwappy-type");
	mvwprintw(mainmenu,3,1,"game by ACT, edited by Owwy :P");
	mvwprintw(mainmenu,5,1,"PWESS ANY KEY TO BEGIN");
	mvwprintw(mainmenu,8,1,"Vewsion 1.1");
	mvwprintw(mainmenu,9,1,"Copywight 2022 ACT :3");
	mvwprintw(mainmenu,10,1,"Wicensed undew the");
	mvwprintw(mainmenu,11,1,"GNU GPW 3.0 >w<");
	mvwprintw(mainmenu,12,1,"github.com/act17/flappy");
	mvwprintw(mainmenu,13,1,"github.com/dyntylluan/fwappy");
	refresh(); wrefresh(mainmenu);
	getch();
}

int main(){

	//Init NCurses
	initscr();
	noecho();
	cbreak();
	start_color();
	
	//Init time
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 83333333;				//Gamespeed. Evewy 0.83 seconds a fwame iws pewfowmed;
							//Gamespeed iws 15 fps.
	//Init Cowows
	init_pair(1,COLOR_BLUE,COLOR_BLUE);		//Sky cowow
	init_pair(2,COLOR_WHITE,COLOR_WHITE);		//Gwound cowow
	init_pair(3,COLOR_YELLOW,COLOR_YELLOW);		//Fwappy cowow
	init_pair(4,COLOR_GREEN,COLOR_GREEN);		//Pipe cowow
	init_pair(5,COLOR_BLACK,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_CYAN);		//Cowow fow main menu
	init_pair(7,COLOR_BLACK,COLOR_WHITE);		//and gawme ovew scween.

	//Main menu function (added aftew gawme was devewoped.)
	mainmenu();

	//Init windows
	WINDOW * sky = newwin(34,72,1,0);
	WINDOW * ground = newwin(1,72,35,0);
	WINDOW * roof = newwin(1,72,0,0);
	WINDOW * flappy = newwin(1,1,18,6);

	//Init PThread
	pthread_t flappymove;

	//Appwy cowows tuwu window
	wbkgd(stdscr,COLOR_PAIR(5));
	wbkgd(sky,COLOR_PAIR(1)); wbkgd(ground,COLOR_PAIR(2));
	wbkgd(flappy,COLOR_PAIR(3)); wbkgd(roof,COLOR_PAIR(2));
	wattron(ground,COLOR_PAIR(5));
	refresh(); wrefresh(sky); wrefresh(ground); wrefresh(roof); wrefresh(flappy);

	int flappyhit = 0;		//Numewic twack if fwappy iws hit.
	int pipeheight,pipebheight;	//vawue of the height of a new pipe awnd 'compwiment' tuwu pipeheight's height.
	int score = 0;			//Numewic twack of pipes passed.
	int pipemoves = 1;		//Numewic count of pipe movements tuwu see if the pipe shouwd be deweted.
	int flappyheight = 18;		//Numewic twack of fwappy's height.

	do{				//Thiws woop wiww end whewn fwappy iws hit.
			pipeheight = pipeheightgenerator();
			WINDOW * pipea = newwin(pipeheight,1,4,71);
			WINDOW * pipeb = newwin(28-pipeheight,1,1,71);
			wbkgd(pipea,COLOR_PAIR(4));	wbkgd(pipeb,COLOR_PAIR(4));
			wrefresh(pipea);		wrefresh(pipeb);

			pipebheight = 34 - (27-pipeheight);
	while(1){

		if(pipemoves == 72){
			wbkgd(pipea,COLOR_PAIR(1));	wbkgd(pipeb,COLOR_PAIR(1));
			wrefresh(pipea);		wrefresh(pipeb);
			delwin(pipea);			delwin(pipeb);
			pipemoves = 1;
			break;
		}
		
		wbkgd(pipea,COLOR_PAIR(1));	wrefresh(pipea);		//Thiws 'ewases' the pipe's pwescence onscween.
		mvwin(pipea,1,71-pipemoves);	wbkgd(pipea,COLOR_PAIR(4));	//Thiws makes the pipe appeaw in a new position.
		wrefresh(pipea);
		
		wbkgd(pipeb,COLOR_PAIR(1));	wrefresh(pipeb);
		mvwin(pipeb,pipebheight,71-pipemoves);	wbkgd(pipeb,COLOR_PAIR(4));
		wrefresh(pipeb);
		
		pipemoves++;
		flappyheight++;							//Thiws decweases fwappy's height by owne.

		flappymoved = pthread_create(&flappymove, NULL, flappymv, NULL);
		nanosleep(&ts,&ts);
		pthread_cancel(flappymove);

		if(flappymoved == 1){						//If usew made an input duwing thiws fwame...
			flappyheight = flappyheight - 6;			//Waise fwappy's positon by 5 units.	
		}

		wbkgd(flappy,COLOR_PAIR(1));	wrefresh(flappy);
		mvwin(flappy,flappyheight,6);	wbkgd(flappy,COLOR_PAIR(3));
		wrefresh(flappy);
		
		if(flappyheight == 1 || flappyheight == 36){			//If fwappy hits the woof ow gwound,
			flappyhit = 1;						//Teww pwogwam fwappy was hit,
			break;							//Awnd bweak the gawme woop.
		}	
		if(pipemoves == 66){						//If pipes awe at fwappy's x-axis
			flappyhit = flappyclscheck(flappyheight,pipeheight);	//Pewfowm a cowwision check.
			if(flappyhit != 0)					//If fwappy was hit,
				break;						//Bweak the gawme woop.
			score++;						//Ewse, add 1 tuwu the scowe.
		}

	}
	}while(flappyhit == 0);
	clear();
	WINDOW * gameoverwin = newwin(32,36,1,1);
	wbkgd(stdscr,COLOR_PAIR(6));
	wbkgd(gameoverwin,COLOR_PAIR(7));
	wattron(gameoverwin,COLOR_PAIR(7));
	box(gameoverwin,0,0);
	
	mvwprintw(gameoverwin,2,2,"GAME OVEW :<");
	mvwprintw(gameoverwin,3,2,"You wost ;-;");
	mvwprintw(gameoverwin,5,2,"Youw scowe:");
	mvwprintw(gameoverwin,6,2,"   %d",score);
	mvwprintw(gameoverwin,9,2,"PWESS ANY");
	mvwprintw(gameoverwin,10,2,"KEY TO EXIT");
	
	refresh(); wrefresh(gameoverwin);
	getch();

	endwin();
	return 0;
}
