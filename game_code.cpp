#include"iostream"
#include"time.h"
#include"termios.h"
#include"sys/ioctl.h"
#include"unistd.h"

using namespace std;

bool game_over;
const int width = 30;
const int height = 30;
int head_x, head_y, fruit_x, fruit_y, score;
enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
direction dir;
int tail_x[100], tail_y[100];
int len_tail;
int obstacle1_x = width/4;
int obstacle1_y = height/4;
int obstacle2_x = 3 * obstacle1_x;
int obstacle2_y = 3 * obstacle1_y;
int obstacle3_x = obstacle1_x;
int obstacle3_y = 2 * obstacle1_y;
int obstacle4_x = 3 * obstacle1_x;
int obstacle4_y = 2 * obstacle1_y;

void enable_raw_mode()
{
	termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
	termios term;
	tcgetattr(0, & term);
	term.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &term);
}

bool kbhit()
{
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	return byteswaiting > 0;
}

void start_game()
{
	game_over = false;
	dir = STOP;

	head_x = width/2;
	head_y = height/2;

	srand(time(0));
	fruit_x = rand() % (width-1);
	fruit_y = rand() % (height-1);

	score = 0;
}

void build_display()
{
	system("clear");
	
	for(int i = 0; i < width+1; i++)
		cout << "#";
	cout << endl;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(j == 0 || j == width-1)
				cout << "#";
			if(i == head_y && j == head_x)
				cout << "O";
			else if(i == fruit_y && j == fruit_x)
				cout << "F";
			else			
			{
				bool tail_print = false;
				for(int k = 0; k < len_tail; k++)
				{
					if(tail_x[k] == j && tail_y[k] == i)
					{
						cout << "o";
						tail_print = true;
					}
				}
				if(!tail_print)
				{	
					if(i == obstacle1_y && (j == obstacle1_x-2 || j == obstacle1_x-1 || j == obstacle1_x || j == obstacle1_x+1 || j == obstacle1_x+2))
						cout << "#";
					else if((i == obstacle2_y-2 || i == obstacle2_y-1 || i == obstacle2_y || i == obstacle2_y+1 || i == obstacle2_y+2) && j == obstacle2_x)
						cout << "#";
					else if(i == obstacle4_y && (j == obstacle4_x-2 || j == obstacle4_x-1 || j == obstacle4_x || j == obstacle4_x+1 || j == obstacle4_x+2))
						cout << "#";
					else if((i == obstacle3_y-2 || i == obstacle3_y-1 || i == obstacle3_y || i == obstacle3_y+1 || i == obstacle3_y+2) && j == obstacle3_x)
						cout << "#";
					else 
		 				cout << " ";
				}
			}
		}
		cout << endl;
	}

	for(int i = 0; i < width+1; i++)
		cout << "#";
	cout << endl;
	cout << "Score = " << score << endl;
}

void gamer_moves()
{
	enable_raw_mode();

	if(kbhit())
	{
		switch(getchar())
		{
			case 'a': 
				dir = LEFT;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 'w':
				dir = UP;
				break;
			case 's':
				dir = DOWN;		
				break;
			case 'x':
				game_over = true;
				break;
		}
	}

	disable_raw_mode();
	tcflush(0, TCIFLUSH);
}

void game_run()
{
	int prev_x = tail_x[0];
	int prev_y = tail_y[0];
	int prev_2x, prev_2y;
	tail_x[0] = head_x;
	tail_y[0] = head_y;

	for(int i = 1; i < len_tail; i++)
	{
		prev_2x = tail_x[i];
		prev_2y = tail_y[i];
		tail_x[i] = prev_x;
		tail_y[i] = prev_y;
		prev_x = prev_2x;
		prev_y = prev_2y;
	}

	switch(dir)
	{
		case LEFT:
			head_x--;
			break;
		case RIGHT:
			head_x++;
			break;
		case UP:
			head_y--;
			break;
		case DOWN:
			head_y++;
			break;
		default: 
			break;
	}
	if(head_x > width || head_x < 0 || head_y > height || head_y < 0)
		game_over = true;

	if((head_y == obstacle2_y-2 || head_y == obstacle2_y-1 || head_y == obstacle2_y || head_y == obstacle2_y+1 || head_y == obstacle2_y+2) && head_x == obstacle2_x)
		game_over = true;

	else if(head_y == obstacle1_y && (head_x == obstacle1_x-2 || head_x == obstacle1_x-1 || head_x == obstacle1_x || head_x == obstacle1_x+1 || head_x == obstacle1_x+2))
		game_over = true;

	else if((head_y == obstacle3_y-2 || head_y == obstacle3_y-1 || head_y == obstacle3_y || head_y == obstacle3_y+1 || head_y == obstacle3_y+2) && head_x == obstacle3_x)
		game_over = true;

	else if(head_y == obstacle4_y && (head_x == obstacle4_x-2 || head_x == obstacle4_x-1 || head_x == obstacle4_x || head_x == obstacle4_x+1 || head_x == obstacle4_x+2))
		game_over = true;
	
	for(int i = 0; i < len_tail; i++)
	{
		if(tail_x[i] == head_x && tail_y[i] == head_y)
			game_over = true;
	}

	if(head_x == fruit_x && head_y == fruit_y)
	{
		score = score + 10;
		srand(time(0));
		fruit_x = rand() % (width-1);
		fruit_y = rand() % (height-1);
		len_tail++;
	}
}

int main()
{	
	char ch;
	system("clear");
	cout << "...WELCOME TO THE SNAKE GAME...\n\n";
	cout << "PRESS THE FOLLOWING KEYS FOR THE MOVEMENT\n";
	cout << "a: LEFT\nd: RIGHT\nw: UP\ns: DOWN\n\n";
	cout << "THE RULES FOR THE GAME ARE AS FOLLOWS:\n";
	cout << "1. WHEN THE HEAD OF THE SNAKE HITS ANY OF THE OBSTACLES OR THE BOUNDARY, YOU LOSE THE GAME.\n";
	cout << "2. WHEN THE HEAD OF THE SNAKE HITS ANY PART OF IT'S BODY, YOU LOSE THE GAME.\n";
	cout << "3. EATING A FRUIT FETCHES YOU A SCORE OF 10 POINTS.\n\n";
	cout << "GOOD LUCK!!! ENJOY!!!\n\n";
	cout << "PRESS ENTER TO BEGIN";
	getchar();

	start_game();
	while(!game_over)
	{
		build_display();
		gamer_moves();
		game_run();
		usleep(200000);
	}
	cout << "\n...GAME OVER...\n";
	cout << "BETTER LUCK NEXT TIME!!!\n\n";
	return 0;
}
