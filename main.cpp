/*
Example platformer with ncurses library
Made by aceinet 2022-present

Github: github.com/Aceinet
*/
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

float gravity = 1;
/*Gravity 1 - Normal
Gravity 2 - Medium
Gravity 3 - High
*/

char textures[] = {
	'#', // Player texture
	'+', // Platform texture
	'='  // Ground texture
};

bool debug = false; // Debug ( Press '0' key to enable )
bool algrd = false; // Always ground
bool nclip = false; // No-Clip ( allows to phase through the bottom of platforms)

int main(){
	int y_pos, x_pos;

	int velocity = 0;

	int cy, cx, c;

	bool onground = false; // Onground - Prevents double-jumps

	initscr(); // Initalizing screen
	clear();   // Clearing screen
	refresh(); // Refreshing screen

    getmaxyx(stdscr, cy, cx); // Get max terminal resolution
	
	int platforms[][2] = { // Platforms coords ( 1st value is Y Coord, second is X Coord )
		{cy-5, 5},
		{cy-5, 6},
		
		{cy-7, 10},
		{cy-7, 11},

		{cy-10, 10},
		{cy-10, 11},

		{cy-13, 15},
		{cy-13, 16},
		{cy-13, 17}
	};

    curs_set(0);           // Hiding cursor
    noecho();              // Disable input feedback
	nodelay(stdscr, TRUE); // Disabling waiting for input in getch()
    keypad(stdscr, TRUE);  // Allows to use arrow keys

	y_pos = cy-2;  //
	x_pos = 5; // Spawn positions

	while(true){
    	getmaxyx(stdscr, cy, cx);

		int sz = 0;

    	for(auto l:platforms){ // Get max platforms count
    		sz++;
    	}
	
		clear();
		for(int i=0;i<cx+1;i++){   // Drawing ground
			mvprintw(cy-1, i, "%c", textures[2]);
		}

		for(int j=0;j<sz;j++){ // Drawing platforms
			mvprintw(platforms[j][0], platforms[j][1], "%c", textures[1]);
		}

		if(debug == true){ // Debug
			mvprintw(0, 0, "VEL: %d GRD: %d GRV: %f X: %d Y: %d", velocity, onground, gravity, x_pos, y_pos);
		}

		if(velocity > 0){ // Checking if player jumped
			y_pos--;      // Making player position higher
			onground = false;
			for(int k=0;k<sz;k++){ // Checking if player touched the bottom of platform
				if(platforms[k][0] == y_pos){ // Checking positions
					if(platforms[k][1] == x_pos){ 
						if(nclip == false){   // If noclip is on, then just ignore collision
							y_pos++;
							velocity = 1;
							onground = true;
						}
					}
				} else if(platforms[k][0] == y_pos+1){ // Checking if player standing on a platform
					if(platforms[k][1] == x_pos){
						velocity = 1;
					}
				}
			}
			velocity--;
			
		} else { // If player not jumped, then we checking collision
			bool ispl = false; // ispl - Is player standing on platform
			
			for(int h=0;h<sz;h++){              // Platforms
				if(platforms[h][0] == y_pos+1){ // Checking positions
					if(platforms[h][1] == x_pos){
						onground = true;
						ispl = true;
						velocity = 0;
					}
				}
			}

			if(ispl == false){        // if player isn't standing on platform
				if(y_pos < cy-2){     // if player standing on ground
					onground = false;
					y_pos++;
				} else {
					onground = true;
				}
			}
		}

		// If always ground is turned on, then we enabling onground
		if(algrd == true){
			onground = true;
		}

		// Making that player cannot escape terminal borders
		if(x_pos == -1){
			x_pos++;
		} else if(x_pos == cx){
			x_pos--;
		} else if(y_pos == -1){
			y_pos++;
		}

		// Drawing player
		mvprintw(y_pos, x_pos, "%c", textures[0]);

		refresh();
		usleep(0.1 * 1e+6); // Making some delay
		c = getch();

		if(c == 'q'){ // If pressed 'q' then exit
			break;
		} else if(c == '0'){ // Switching debug mode
			if(debug == false){
				debug = true;
			} else {
				debug = false;
			}
		} else if(c == 27){ // 27 - Escape key, for testing menu
			if(debug == true){
				nodelay(stdscr, FALSE);
				mvprintw(1, 0, "Menu");
				mvprintw(2, 0, "[1] High gravity");
				mvprintw(3, 0, "[2] Normal gravity");
				mvprintw(4, 0, "[3] Always on-ground");
				mvprintw(5, 0, "[4] Disable Always on-ground");
				mvprintw(6, 0, "[5] No-Clip");
				mvprintw(7, 0, "[6] Disable No-Clip");
				int mc = getch();
				if(mc == '1'){
					gravity = 4;
				} else if(mc == '2'){
					gravity = 1;
				} else if(mc == '3'){
					algrd = true;
				} else if(mc == '4'){
					algrd = false;
				} else if(mc == '5'){
					nclip = true;
				} else if(mc == '6'){
					nclip = false;
				}
			}
			nodelay(stdscr, TRUE);
		} else if(c == KEY_LEFT){ // Player moving
			x_pos--;
		} else if(c == KEY_RIGHT){
			x_pos++;
		} else if(c == KEY_UP){ // Jump
			if(onground == true){
				velocity = gravity*6;
			}
		}
	}

	endwin(); // Ending ncurses initalization

	return 0; // Exiting with code 0
}
