#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
using namespace std;


const int ROW_SIZE = 30, COL_SIZE = 60;
int  exit_door_1;
int player_row, player_col;
int bullet_row, bullet_col;
int limit = 0, score, life = 3, level = 1;
int obstacle_numbers, enemy_count;
bool player_fire = false, game = true;
char fire_direction;
int enemy_row = 4, enemy_col = 20;
int soloman_row, soloman_col;
int soloman_bullet_row, soloman_bullet_col;
bool IsSolomanFiring = true;
int obstacle_row, obstacle_col;
int start_r = 3, end_r = start_r + 4;  // obstacles
int extra_loop = 0;
bool IsEnemyDown[7] = { false };
int enemy_bullet_row[7] = { 0 };      //enemy bullet row positions
int enemy_bullet_col[7] = { 0 };      //enemy bullet colimn positions
int enemy_bullet_direction[7] = { 0 };    //enemy bullet directions for each enemy
bool is_enemy_fire[6][4] = { false };     // storing 6 eemies firing behaviour for 4 different directions just to avoid buffer overflow
int fire_count = 1;
int level_speed;
int messile_count = 0;
string player_name;
int player_score;
string name[10];
int scores[10];
int score_count = 0;
int obstacles_position[7][2] = { '\0' };
int messile_target = 0;


void hideCursor();
void showCursor();
void moveCursorToTop();
void initializeMap(char map[][COL_SIZE]);
void boundary(char map[][COL_SIZE]);
void print_map(char map[][COL_SIZE]);
void obs_enemy_count();
void initialize_player(char map[][COL_SIZE]);
void player(char map[][COL_SIZE]);
void player_movement(char map[][COL_SIZE], char direction);
bool player_foot_fire_check(char map[][COL_SIZE]);
void life_check(char map[][COL_SIZE]);
void fire_check(char map[][COL_SIZE], int enemy_position[][2]);
void bullet(char map[][COL_SIZE], char direction);
void print_bullet(char map[][COL_SIZE]);
void messile(char map[][COL_SIZE]);
void initialize_obstacles(char map[][COL_SIZE], int obstacles_position[][2]);
void obstacles(char map[][COL_SIZE], int obstacles_position[][2]);
void initialize_enemy(char map[][COL_SIZE], int enemy_position[][2]);
void enemy(char map[][COL_SIZE], int enemy_position[][2]);
void enemy_fire(char map[][COL_SIZE], int enemy_position[][2]);
bool stop_enemy_bullet(int i);
void clear_enemy_bullet(char map[][COL_SIZE]);
void enemy_fire_check(char map[][COL_SIZE]);
bool positionOccupied(char map[][COL_SIZE], int enemy_new_row, int enemy_new_col);
void moveTowardsObstacle(char map[][COL_SIZE], int enemy_position[][2]);
void every_movement(char map[][COL_SIZE], char direction);
void initialize_soloman(char map[][COL_SIZE]);
void soloman(char map[][COL_SIZE]);
void soloman_movement(char map[][COL_SIZE]);
void soloman_fire(char map[][COL_SIZE], int& soloman_bullet_row, int& soloman_bullet_col);
void soloman_fire_check(char map[][COL_SIZE], int soloman_bullet_row, int soloman_bullet_col);
void displayScoreboard();
void display_menu(int selected);
void levelExchangeScreen();
void GameOver();
void pause_screen();
void exit_screen();
void update_score();
void input_name();
void scoreboard();
bool level_check(char map[][COL_SIZE]);
void initialization(char map[][COL_SIZE], int enemy_position[][2], int obstacles_position[][2]);
void gameloop();
void menu();

//---------------------------------------- MAP AND LEVELS -----------------------------------------------------------------//

void hideCursor() {    // hide the cursor
	cout << "\033[?25l" << flush;
}
void showCursor() {	  // Show the cursor
	cout << "\033[?25h" << flush;
}
void moveCursorToTop() { // Move the cursor to the top-left corner
	cout << "\033[H" << flush;
}
void initializeMap(char map[][COL_SIZE]) {

	for (int i = 0; i < ROW_SIZE; i++) {       // initializiing map with space
		for (int j = 0; j < COL_SIZE; j++) {
			map[i][j] = ' ';
		}
	}  // initializing boundary
	for (int i = 0; i < ROW_SIZE; i++) {    //left and right side
		map[i][0] = '|';
		map[i][COL_SIZE - 1] = '|';
	}
	for (int i = 0; i < COL_SIZE; i++) {   //up and down side
		if (i < exit_door_1 || i > exit_door_1 + 8) {   //exit door condition 
			map[0][i] = '-';
		}
		map[ROW_SIZE - 1][i] = '-';
	}
	map[0][0] = '*';
	map[ROW_SIZE - 1][COL_SIZE - 1] = '*';
	map[ROW_SIZE - 1][0] = '*';
	map[0][COL_SIZE - 1] = '*';
}
void boundary(char map[][COL_SIZE]) {
	for (int i = 0; i < ROW_SIZE; i++) {
		map[i][0] = '|';
		map[i][COL_SIZE - 1] = '|';
	}

	for (int i = 0; i < COL_SIZE; i++) {
		if (i < exit_door_1 || i > exit_door_1 + 8) {
			map[0][i] = '-';
		}
		map[ROW_SIZE - 1][i] = '-';
	}

	map[0][0] = '*';
	map[ROW_SIZE - 1][COL_SIZE - 1] = '*';
	map[ROW_SIZE - 1][0] = '*';
	map[0][COL_SIZE - 1] = '*';
}
void print_map(char map[][COL_SIZE]) {
	ostringstream buffer;
	moveCursorToTop();
	buffer << "\n\n";
	for (int i = 0; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			if (j == 0)
				buffer << "\t\t\t\t\t\t";
			buffer << map[i][j];
		}
		buffer << endl;
	}
	cout << "\033[H" << buffer.str();
	buffer.str("");
	buffer.clear();
}
void obs_enemy_count() {
	
	srand(time(0));

	int door_row = 4, door_col = COL_SIZE - 8;
	exit_door_1 = door_row + rand() % (door_col - door_row + 1);

	messile_count = 0;

	if (level == 1) {
		obstacle_numbers = 3;
		enemy_count = 2;
		level_speed = 100;
		
	}
	else if (level == 2 || level == 3) {
		obstacle_numbers = 4;
		enemy_count = 4;
		if (level == 2)
			level_speed = 85;
		else
			level_speed = 80;
	}
	else if (level == 4) {
		obstacle_numbers = 5;
		enemy_count = 5;
		level_speed = 75;
	}
	else if (level == 5) {
		obstacle_numbers = 6;
		enemy_count = 6;
		level_speed = 70;
	}
}

//--------------------------------------- PLAYER FUNCTIONS ----------------------------------------------------------------//

void initialize_player(char map[][COL_SIZE]) {
	srand(time(0));
	do {
		int random_row_start = (ROW_SIZE / 2) + 10, random_row_end = ROW_SIZE - 4;    //initializing the player
		int random_col_start = 4, random_col_end = COL_SIZE - 4;
		player_row = random_row_start + rand() % (random_row_end - random_row_start + 1);
		player_col = random_col_start + rand() % (random_col_end - random_col_start + 1);
	} while (!(map[player_row][player_col] == ' ' && map[player_row - 1][player_col] == ' ' && map[player_row][player_col - 1] == ' '     //space checking
		&& map[player_row][player_col + 1] == ' ' && map[player_row + 1][player_col + 1] == ' ' && map[player_row + 1][player_col - 1] == ' '));

}
void player(char map[][COL_SIZE]) {  //storing player in array
	map[player_row][player_col] = '|';
	map[player_row - 1][player_col] = 'O';
	map[player_row][player_col - 1] = '/';
	map[player_row][player_col + 1] = '\\';
	if (limit % 2 == 0) {
		map[player_row + 1][player_col - 1] = '/';  
		map[player_row + 1][player_col + 1] = '\\';
	}
	else {
		map[player_row + 1][player_col - 1] = '|';
		map[player_row + 1][player_col + 1] = '|';
	}
}
void player_movement(char map[][COL_SIZE], char direction) {

	if (direction == 72) // player upward direction
		player_row--;
	else if (direction == 80)  // player downward direction
		player_row++;
	if (direction == 75)  // player right direction
		player_col--;
	else if (direction == 77)  //player left direction
		player_col++;
	limit++;  // countr to move player foot

}
bool player_foot_fire_check(char map[][COL_SIZE]) {  // checking if there is space on ethan hunt foot indexes
	if ((map[player_row + 1][player_col - 1] != '/' && map[player_row + 1][player_col - 1] != '|')
		|| ((map[player_row + 1][player_col + 1] != '\\' && map[player_row + 1][player_col + 1] != '|')))
		return true;
	return false;

}
void life_check(char map[][COL_SIZE]) {  // checking ethan hunt colision with anything

	/*if (map[player_row - 2][player_col] != '^' && map[player_row][player_col - 2] != '<' &&
		map[player_row][player_col + 2] != '>')*/
	
		if (map[player_row - 1][player_col] != 'O' || map[player_row][player_col - 1] != '/'
			|| map[player_row][player_col + 1] != '\\' || map[player_row][player_col] != '|'
			|| player_foot_fire_check(map))
		{
			life--;  
			initialize_player(map); // again initializing ethan hunt
		}
	
}
void fire_check(char map[][COL_SIZE], int enemy_position[][2]) {

	switch (fire_direction) { // ethan hunt bullet colision with wall and boundaries
	case 'w':
		if (map[bullet_row + 1][bullet_col] != '^' && player_fire) {  
			player_fire = false;
		}
		break;
	case 's':
		if (map[bullet_row - 1][bullet_col] != 'v' && player_fire) {
			player_fire = false;
		}
		break;
	case 'd':

		if (map[bullet_row][bullet_col - 1] != '>' && player_fire) {
			player_fire = false;
		}
		break;
	case 'a':
		if (map[bullet_row][bullet_col + 1] != '<' && player_fire) {
			player_fire = false;
		}
		break;
	}
	

	for (int i = 0; i < enemy_count; i++) {

		enemy_row = enemy_position[i][0];
		enemy_col = enemy_position[i][1];

		if (IsEnemyDown[i] == false) {
			//checking if ethan hunt bullet hit any part of enemy
			if (((bullet_row == enemy_row + 1 && bullet_col == enemy_col) || (bullet_row == enemy_row && bullet_col == enemy_col - 1)
				|| (bullet_row == enemy_row && bullet_col == enemy_col + 1) || (bullet_row == enemy_row + 1 && bullet_col == enemy_col)
				|| (bullet_row == enemy_row + 1 && bullet_col == enemy_col - 1) || (bullet_row == enemy_row + 1 && bullet_col == enemy_col + 1))
				&& player_fire)
			{

				if (player_fire) 
					score += 5;   // adding 5 scores
				player_fire = false;  // stop ethan bullet
				IsEnemyDown[i] = true; // clear the enemy
				messile_target += 5;  

			}
		}
	}
}


//-------------------------------------- PLAYER BULLET --------------------------------------------------------------------//

void bullet(char map[][COL_SIZE], char direction) { //initializing the ethan bullet direction
	if (direction == 'w') {  // upward direction
		bullet_row = player_row - 2;
		bullet_col = player_col;
	}
	else if (direction == 's') {  //downward direction
		bullet_row = player_row + 2;
		bullet_col = player_col;
	}
	else if (direction == 'd') {  //left direction
		bullet_row = player_row;
		bullet_col = player_col + 2;
	}
	else if (direction == 'a') {  //right direction
		bullet_row = player_row;
		bullet_col = player_col - 2;
	}
	if (map[bullet_row][bullet_col] != ' ') 
		player_fire = false;
}
void print_bullet(char map[][COL_SIZE]) {  // moving the bullet
	if (player_fire) {
	
		switch (fire_direction) {

		case 'w': // moving upward

			map[bullet_row][bullet_col] = '^';
			if (map[bullet_row + 1][bullet_col] != 'O')
				if (bullet_row == 0) {
					player_fire = false;
				}
			bullet_row--;
			break;

		case 's':  // moving downward

			map[bullet_row][bullet_col] = 'v';
			if (bullet_row == ROW_SIZE - 1) {
				player_fire = false;
			}
			bullet_row++;
			break;

		case 'd':  // moving left

			map[bullet_row][bullet_col] = '>';
			if (map[bullet_row][bullet_col - 1] != '\\')
				if (bullet_col == COL_SIZE - 1) {
					player_fire = false;
				}
			bullet_col++;
			break;

		case 'a':  // moving right

			map[bullet_row][bullet_col] = '<';
			if (map[bullet_row][bullet_col + 1] != '/')
				if (bullet_col == 0) {
					player_fire = false;
				}
			bullet_col--;
			break;
		}
	}
}
void messile(char map[][COL_SIZE]) { // firing the messile
	srand(time(0));
	int messile_start = 0, messile_end = enemy_count - 1, i;
	do{
		 i = messile_start + rand() % (messile_end - messile_start + 1);
	} while (IsEnemyDown[i]);
	IsEnemyDown[i] = true;
	messile_target = 0;
	score += 5;
	messile_target += 5;
}

//-------------------------------------- OBSTACLES ----------------------------------------------------------------------//

void initialize_obstacles(char map[][COL_SIZE], int obstacles_position[][2]) {
	srand(time(0));
	start_r = 4, end_r = start_r + 4;
	int start_c = 7, end_c = COL_SIZE - 7;

	for (int i = 0; i <= obstacle_numbers; i++) {
		obstacle_row = start_r + rand() % (end_r - start_r + 1);
		obstacle_col = start_c + rand() % (end_c - start_c + 1);
		if (map[obstacle_row][obstacle_col] == ' ' && map[obstacle_row][obstacle_col - 1] == ' '
			&& map[obstacle_row][obstacle_col + 1] == ' ' && map[obstacle_row][obstacle_col + 2] == ' '
			&& map[obstacle_row][obstacle_col - 2] == ' ' && map[obstacle_row][obstacle_col + 3] == ' '
			&& map[obstacle_row][obstacle_col - 3] == ' ') {

			obstacles_position[i][0] = obstacle_row;
			obstacles_position[i][1] = obstacle_col;

			if (level == 1) {
				start_r += 7;
				end_r = ROW_SIZE - 4;
			}
			else if (level == 2 || level == 3) {
				start_r += 6;
				end_r = start_r + 4;
			}
			else if (level == 4) {
				start_r += 4;
				end_r = start_r + 3;
			}
			else if (level == 5) {
				start_r = 5;
				end_r = ROW_SIZE - 2;
			}
		}
		else {
			i--;
		}
	}
}
void obstacles(char map[][COL_SIZE], int obstacles_position[][2]) {


	for (int i = 0; i < obstacle_numbers; i++) {

		obstacle_row = obstacles_position[i][0];
		obstacle_col = obstacles_position[i][1];

		map[obstacle_row][obstacle_col] = '-';
		map[obstacle_row][obstacle_col - 1] = '-';
		map[obstacle_row][obstacle_col + 1] = '-';
		map[obstacle_row][obstacle_col + 2] = '-';
		map[obstacle_row][obstacle_col - 2] = '[';
		map[obstacle_row][obstacle_col + 3] = ']';
		if (level >= 3) {
			map[obstacle_row][obstacle_col - 2] = '-';
			map[obstacle_row][obstacle_col - 3] = '[';
		}
	}
}

//------------------------------------ ENEMY FUNCTIONS ------------------------------------------------------------------//

void initialize_enemy(char map[][COL_SIZE], int enemy_position[][2]) {

	for (int i = 0; i < enemy_count; i++) {
		IsEnemyDown[i] = false;
	}

	srand(time(0));
	int start_r = 4, end_r = (ROW_SIZE / 2) - 2;
	int start_c = 3, end_c = COL_SIZE - 5;
	for (int i = 0; i < enemy_count; i++) {

		enemy_row = start_r + rand() % (end_r - start_r + 1);
		enemy_col = start_c + rand() % (end_c - start_c + 1);
		if (map[enemy_row][enemy_col] == ' ' &&
			map[enemy_row][enemy_col - 1] == ' ' && map[enemy_row][enemy_col + 1] == ' ' &&
			map[enemy_row + 1][enemy_col] == ' ' && map[enemy_row + 1][enemy_col - 1] == ' ' &&
			map[enemy_row + 1][enemy_col + 1] == ' ') {

			enemy_position[i][0] = enemy_row;
			enemy_position[i][1] = enemy_col;
		}
		else {
			i--;
		}
	}
}
void enemy(char map[][COL_SIZE], int enemy_position[][2]) {

	for (int i = 0; i < enemy_count; i++) {
		enemy_row = enemy_position[i][0];
		enemy_col = enemy_position[i][1];
		if (!IsEnemyDown[i]) {
			map[enemy_row][enemy_col] = '|';
			map[enemy_row][enemy_col - 1] = '[';
			map[enemy_row][enemy_col + 1] = ']';
			map[enemy_row + 1][enemy_col] = 'V';
			map[enemy_row + 1][enemy_col - 1] = '[';
			map[enemy_row + 1][enemy_col + 1] = ']';
		}
	}
}
void enemy_fire(char map[][COL_SIZE], int enemy_position[][2]) {


	for (int i = 0; i < enemy_count; i++) {
		enemy_row = enemy_position[i][0];
		enemy_col = enemy_position[i][1];

		
		if (!IsEnemyDown[i]) {

			// checks if player row or column becomes equal to enemyrow or column
			if ((enemy_row == player_row || enemy_row == player_row - 1 || enemy_row == player_row + 1) && enemy_col > player_col && !is_enemy_fire[i][0]) {   //row is same and player_col less than enemy_col
				enemy_bullet_row[i] = enemy_row;
				enemy_bullet_col[i] = enemy_col - 1;
				enemy_bullet_direction[i] = 1;  //right direction
				is_enemy_fire[i][0] = true;                     // storing from which side of enemy, bulllet is fired
			}
			else if ((enemy_row == player_row || enemy_row == player_row - 1 || enemy_row == player_row + 1) && enemy_col < player_col && !is_enemy_fire[i][1]) {   //row is same and player_col greater than enemy_col
				enemy_bullet_row[i] = enemy_row;
				enemy_bullet_col[i] = enemy_col + 1;
				enemy_bullet_direction[i] = 2;  //left direction
				is_enemy_fire[i][1] = true;                   // storing from which side of enemy, bulllet is fired
			}
			else if ((enemy_col == player_col || enemy_col == player_col - 1 || enemy_col == player_col + 1) && enemy_row > player_row && !is_enemy_fire[i][2]) {  //column is same and player_row less than enemy_row
				enemy_bullet_row[i] = enemy_row ;
				enemy_bullet_col[i] = enemy_col;
				enemy_bullet_direction[i] = 3;  //up direction
				is_enemy_fire[i][2] = true;                  // storing from which side of enemy, bulllet is fired
			}
			else if ((enemy_col == player_col || enemy_col == player_col - 1 || enemy_col == player_col + 1) && enemy_row < player_row && !is_enemy_fire[i][3]) { //column is same and player_row greater than enemy_row
				enemy_bullet_row[i] = enemy_row + 1;
				enemy_bullet_col[i] = enemy_col;
				enemy_bullet_direction[i] = 4;  //down direction
				is_enemy_fire[i][3] = true;                 // storing from which side of enemy, bulllet is fired
			}
		}
		 /*Setting tha new bullet position on the map */
		if (enemy_bullet_row[i] >= 0 && enemy_bullet_row[i] < ROW_SIZE &&
			enemy_bullet_col[i] >= 0 && enemy_bullet_col[i] < COL_SIZE) {
			if (enemy_bullet_direction[i] == 1 && is_enemy_fire[i][0]) {           // Update bullet movement based on direction.  moving bullet right
				enemy_bullet_col[i]--;
				map[enemy_bullet_row[i]][enemy_bullet_col[i]] = '*';
				if (enemy_bullet_col[i] < 0) {
					is_enemy_fire[i][0] = false;         // if bullet reaches boundary, changing the undex
				}
			}
			else if (enemy_bullet_direction[i] == 2 && is_enemy_fire[i][1]) {
				enemy_bullet_col[i]++;                   //moving bullet left
				map[enemy_bullet_row[i]][enemy_bullet_col[i]] = '*';
				if (enemy_bullet_col[i] >= COL_SIZE) {
					is_enemy_fire[i][1] = false;        // if bullet reaches boundary, changing the undex
				}
			}
			else if (enemy_bullet_direction[i] == 3 && is_enemy_fire[i][2]) {
				enemy_bullet_row[i]--;                  //moving bullet up
				map[enemy_bullet_row[i]][enemy_bullet_col[i]] = '*';
				if (enemy_bullet_row[i] < 0 || map[enemy_bullet_row[i]][enemy_bullet_col[i]] != '*') {
					is_enemy_fire[i][2] = false;       // if bullet reaches boundary, changing the undex
				}
			}
			else if (enemy_bullet_direction[i] == 4 && is_enemy_fire[i][3]) {
				enemy_bullet_row[i]++;                  //moving bullet down
				map[enemy_bullet_row[i]][enemy_bullet_col[i]] = '*';
				if (enemy_bullet_row[i] >= ROW_SIZE) {
					is_enemy_fire[i][3] = false;        // if bullet reaches boundary, changing the undex
				}
			}
		}
	}
}
bool stop_enemy_bullet(int i) {

	return ((enemy_bullet_row[i] == player_row - 1 && enemy_bullet_col[i] == player_col)  // enemy bullet equals player'x head 'O'
		|| (enemy_bullet_row[i] == player_row && enemy_bullet_col[i] == player_col - 1)  // enemy bullet equals player's right hand '/'
		|| (enemy_bullet_row[i] == player_row && enemy_bullet_col[i] == player_col + 1)  // enemy bullet equals player's left hand '\'
		|| (enemy_bullet_row[i] == player_row && enemy_bullet_col[i] == player_col)      // enemy bullet equals player;s body '|'
		|| (enemy_bullet_row[i] == player_row + 1 && enemy_bullet_col[i] == player_col - 1)   // enemy bullet equals player's riht foot '/' or '|'
		|| (enemy_bullet_row[i] == player_row + 1 && enemy_bullet_col[i] == player_col + 1)); // enemy bullet equals player's left foot '\' or '|'
}
void clear_enemy_bullet(char map[][COL_SIZE]) {

	for (int i = 0; i < enemy_count; i++) {
		if (stop_enemy_bullet(i) && !IsEnemyDown[i]) {
			// Reset bullet firing
			if (enemy_bullet_direction[i] == 1)
				is_enemy_fire[i][0] = false;
			else if (enemy_bullet_direction[i] == 2)
				is_enemy_fire[i][1] = false;
			else if (enemy_bullet_direction[i] == 3)
				is_enemy_fire[i][2] = false;
			else if (enemy_bullet_direction[i] == 4) {
				is_enemy_fire[i][3] = false;
			}
		}
	}

}
void enemy_fire_check(char map[][COL_SIZE]) {

	for (int i = 0; i < enemy_count; i++) {


		if (!IsEnemyDown[i]) {
			if (enemy_bullet_direction[i] == 4 && is_enemy_fire[i][3] && (enemy_bullet_row[i] + 1 == bullet_row || enemy_bullet_row[i] == bullet_row) && enemy_bullet_col[i] == bullet_col && player_fire) {
				is_enemy_fire[i][3] = false;   player_fire = false;
			}
			else if (enemy_bullet_direction[i] == 3 && is_enemy_fire[i][2] && (enemy_bullet_row[i] - 1 == bullet_row || enemy_bullet_row[i] == bullet_row) && enemy_bullet_col[i] == bullet_col && player_fire) {
				is_enemy_fire[i][2] = false;   player_fire = false;
			}
			else if (enemy_bullet_direction[i] == 2 && is_enemy_fire[i][1] && enemy_bullet_row[i] == bullet_row && (enemy_bullet_col[i] == bullet_col || enemy_bullet_col[i] + 1 == bullet_col) && player_fire) {
				is_enemy_fire[i][1] = false;   player_fire = false;
			}
			else if (enemy_bullet_direction[i] == 1 && is_enemy_fire[i][0] && enemy_bullet_row[i] == bullet_row && (enemy_bullet_col[i] == bullet_col || enemy_bullet_col[i] - 1 == bullet_col) && player_fire) {
				is_enemy_fire[i][0] = false;   player_fire = false;
			}

			if (enemy_bullet_direction[i] == 4 && is_enemy_fire[i][3] && map[enemy_bullet_row[i]][enemy_bullet_col[i]] != '*') {
				is_enemy_fire[i][3] = false;  
			}
			else if (enemy_bullet_direction[i] == 3 && is_enemy_fire[i][2] && map[enemy_bullet_row[i]][enemy_bullet_col[i]] != '*') {
				is_enemy_fire[i][2] = false;
			}
			else if (enemy_bullet_direction[i] == 2 && is_enemy_fire[i][1] && map[enemy_bullet_row[i]][enemy_bullet_col[i]] != '*') {
				is_enemy_fire[i][1] = false;
			}
			else if (enemy_bullet_direction[i] == 1 && is_enemy_fire[i][0] && map[enemy_bullet_row[i]][enemy_bullet_col[i]] != '*') {
				is_enemy_fire[i][0] = false;
			}
		}
	}
	
}
bool positionOccupied(char map[][COL_SIZE], int enemy_new_row, int enemy_new_col)
{	
	if (map[enemy_new_row][enemy_new_col] == ' ' && map[enemy_new_row][enemy_new_col - 1] == ' '
		&& map[enemy_new_row][enemy_new_col + 1] == ' ' && map[enemy_new_row + 1][enemy_new_col] == ' '
		&& map[enemy_new_row + 1][enemy_new_col - 1] == ' ' && map[enemy_new_row + 1][enemy_new_col + 1] == ' ')
		return true;
	return false;

}
void moveTowardsObstacle(char map[][COL_SIZE], int enemy_position[][2])
{

	for (int i = 0; i < enemy_count; i++) {

		enemy_row = enemy_position[i][0];
		enemy_col = enemy_position[i][1];

		int enemy_new_row = enemy_row;
		int enemy_new_col = enemy_col;

		if (rand() % 3 == 0)
		{
			int enemy_movement_direction = rand() % 4;
			switch (enemy_movement_direction) {
			case 0: if (enemy_row > 1) enemy_new_row--; break;
			case 1: if (enemy_row < ROW_SIZE - 3) enemy_new_row++; break;
			case 2: if (enemy_col > 2) enemy_new_col--; break;
			case 3: if (enemy_col < COL_SIZE - 3) enemy_new_col++; break;
			}
		}
		else
		{
			
			if (enemy_col > player_col && enemy_col > 1) enemy_new_col--;
			else if (enemy_col < player_col && enemy_col < COL_SIZE - 3) enemy_new_col++;
			if (enemy_row > player_row && enemy_row > 1) enemy_new_row--;
			else if (enemy_row < player_row && enemy_row < ROW_SIZE - 3) enemy_new_row++;
			
		}

		if (positionOccupied(map, enemy_new_row, enemy_new_col) && enemy_new_row > 2 && enemy_new_row < ROW_SIZE - 2 && enemy_new_col > 2 && enemy_new_col < COL_SIZE - 2)
		{
			enemy_row = enemy_new_row;
			enemy_col = enemy_new_col;

			enemy_position[i][0] = enemy_row;
			enemy_position[i][1] = enemy_col;

		}

	}


}

//----------------------------------------------------------------------------------------------------------------------//

void every_movement(char map[][COL_SIZE], char direction) {
	if ((direction == 'w' || direction == 's' || direction == 'a' || direction == 'd') && !player_fire) {
		fire_direction = direction;
		player_fire = true;
		bullet(map, direction);

	}
	
	else if (direction == 32 && messile_count < enemy_count && messile_target > 10) {
		for (int i = 0; i < enemy_count; i++) {
			if (!IsEnemyDown[i]) {
				messile(map);
				messile_count++;
				break;
			}
		}
		
	}
	else {
			if (direction == -32) // Arrow keys generate extended codes
			direction = _getch();
		player_movement(map, direction);

	}
}

//------------------------------ SOLOMAN FUNCTIONS----------------------------------------------------------------------//

void initialize_soloman(char map[][COL_SIZE]) {
	int start_r = 3, end_r = (ROW_SIZE / 2) - 2;
	int start_c = 3, end_c = COL_SIZE - 5;
	for (int i = 0; i < 1; i++) {
		soloman_row = start_r + rand() % (end_r - start_r + 1);
		soloman_col = start_c + rand() % (end_c - start_c + 1);

		if (map[soloman_row][soloman_col] == ' ' && map[soloman_row][soloman_col - 1] == ' '
			&& map[soloman_row][soloman_col + 1] == ' ' && map[soloman_row][soloman_col - 2] == ' '
			&& map[soloman_row][soloman_col + 2] == ' ' && map[soloman_row][soloman_col - 3] == ' '
			&& map[soloman_row][soloman_col + 3] == ' ' && map[soloman_row + 1][soloman_col - 1] == ' '
			&& map[soloman_row + 1][soloman_col + 1] == ' ' && map[soloman_row - 1][soloman_col - 1] == ' '
			&& map[soloman_row - 1][soloman_col + 1] == ' ' && map[soloman_row - 1][soloman_col] == ' '
			&& map[soloman_row + 1][soloman_col] == ' ') {

			map[soloman_row][soloman_col] = 'X';
			map[soloman_row][soloman_col - 1] = '[';
			map[soloman_row][soloman_col + 1] = ']';
			map[soloman_row][soloman_col - 2] = ']';
			map[soloman_row][soloman_col + 2] = '[';
			map[soloman_row][soloman_col - 3] = '[';
			map[soloman_row][soloman_col + 3] = ']';
			map[soloman_row + 1][soloman_col - 1] = '[';
			map[soloman_row + 1][soloman_col + 1] = ']';
			map[soloman_row - 1][soloman_col - 1] = '[';
			map[soloman_row - 1][soloman_col + 1] = ']';

		}
		else {
			i--;
		}

	}


}
void soloman(char map[][COL_SIZE])
{
	map[soloman_row][soloman_col] = 'X';
	map[soloman_row][soloman_col - 1] = '[';
	map[soloman_row][soloman_col + 1] = ']';
	map[soloman_row][soloman_col - 2] = ']';
	map[soloman_row][soloman_col + 2] = '[';
	map[soloman_row][soloman_col - 3] = '[';
	map[soloman_row][soloman_col + 3] = ']';
	map[soloman_row + 1][soloman_col - 1] = '[';
	map[soloman_row + 1][soloman_col + 1] = ']';
	map[soloman_row - 1][soloman_col - 1] = '[';
	map[soloman_row - 1][soloman_col + 1] = ']';
}
void soloman_movement(char map[][COL_SIZE]) {
	srand(time(0));
	int soloman_dx = 0;
	int soloman_dy = 7;
	int soloman_direction = ' ';
	int moves[8][2] = {
	   {1, 0},  // Move down
	   {-1, 0}, // Move up
	   {0, 1},  // Move right9
	   {0, -1}, // Move left
	   {1, -1}, // Move diagonally down-left
	   {1, 1},  // Move diagonally down-right
	   {-1, -1}, // Move diagonally up-left
	   {-1, 1},  // Move diagonally up-right 
	};
	soloman_direction =  soloman_dx + rand() % (soloman_dy - soloman_dx + 1);   // Generate a random direction

	int next_row = soloman_row + moves[soloman_direction][0];       // Get movement deltas
	int next_col = soloman_col + moves[soloman_direction][1];

	if (next_row >= 2 && next_row < ROW_SIZE - 2 &&       // Check if the next position is valid
		next_col >= 5 && next_col < COL_SIZE - 5)
	{
		soloman_row = next_row;       // Update Solomon position
		soloman_col = next_col;
	}
}
void soloman_fire(char map[][COL_SIZE], int& soloman_bullet_row, int& soloman_bullet_col) {
	if (fire_count == 1) {
		if (soloman_bullet_row > 0)
			soloman_bullet_row--;
		if (soloman_bullet_row == 0 || !IsSolomanFiring) {
			soloman_bullet_row = soloman_row + 2;
			soloman_bullet_col = soloman_col;
			fire_count = 2;
			IsSolomanFiring = true;
		}
	}
	else if (fire_count == 2) {
		if (soloman_bullet_row < ROW_SIZE - 1)
			soloman_bullet_row++;
		if (soloman_bullet_row == ROW_SIZE - 1 || !IsSolomanFiring) {
			soloman_bullet_row = soloman_row;
			soloman_bullet_col = soloman_col - 4;
			fire_count = 3;
			IsSolomanFiring = true;
		}
	}
	else if (fire_count == 3) {
		if (soloman_bullet_col > 0)
			soloman_bullet_col--;
		if (soloman_bullet_col == 0 || !IsSolomanFiring) {
			soloman_bullet_row = soloman_row;
			soloman_bullet_col = soloman_col + 4;
			fire_count = 4;
			IsSolomanFiring = true;
		}
	}
	else if (fire_count == 4) {
		if (soloman_bullet_col < COL_SIZE - 1)
			soloman_bullet_col++;
		if (soloman_bullet_col == COL_SIZE - 1 || !IsSolomanFiring) {
			soloman_bullet_row = soloman_row - 2;
			soloman_bullet_col = soloman_col;
			fire_count = 1;
			IsSolomanFiring = true;
		}
	}
	if (soloman_bullet_row >= 0 && soloman_bullet_row < ROW_SIZE &&
		soloman_bullet_col >= 0 && soloman_bullet_col < COL_SIZE )
	{
		map[soloman_bullet_row][soloman_bullet_col] = '*';

	}
}
void soloman_fire_check(char map[][COL_SIZE], int soloman_bullet_row, int soloman_bullet_col) {

	if ((bullet_row == soloman_bullet_row || bullet_row + 1 == soloman_bullet_row) && bullet_col == soloman_bullet_col && player_fire) {
		player_fire = false;  IsSolomanFiring = false;
	}
	else if ((bullet_row == soloman_bullet_row || bullet_row - 1 == soloman_bullet_row) && bullet_col == soloman_bullet_col && player_fire) {
		player_fire = false;  IsSolomanFiring = false;
	}
	else if (bullet_row == soloman_bullet_row && (bullet_col == soloman_bullet_col || bullet_col + 1 == soloman_bullet_col) && player_fire) {
		player_fire = false;  IsSolomanFiring = false;
	}
	else if (bullet_row == soloman_bullet_row && (bullet_col == soloman_bullet_col || bullet_col - 1 == soloman_bullet_col) && player_fire) {
		player_fire = false;  IsSolomanFiring = false;
	}

	if (map[soloman_bullet_row][soloman_bullet_col] != '*'
		|| (soloman_bullet_row == player_row - 1 && soloman_bullet_col == player_col)  // soloman bullet equals player'x head 'O'
		|| (soloman_bullet_row == player_row && soloman_bullet_col == player_col - 1)  // soloman bullet equals player's right hand '/'
		|| (soloman_bullet_row == player_row && soloman_bullet_col == player_col + 1)  // soloman bullet equals player's left hand '\'
		|| (soloman_bullet_row == player_row && soloman_bullet_col == player_col)      // soloman bullet equals player;s body '|'
		|| (soloman_bullet_row == player_row + 1 && soloman_bullet_col == player_col - 1)   // soloman bullet equals player's riht foot '/' or '|'
		|| (soloman_bullet_row == player_row + 1 && soloman_bullet_col == player_col + 1))  // soloman bullet equals player's left foot '\' or '|'
	{
		IsSolomanFiring = false;
	}

}

//------------------------------ DISPLAY SCREENS -----------------------------------------------------------------------//
void displayScoreboard() {
	
	cout << "\t\t\t\t\t\t|" << "\033[36m" << "**********************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t|""\033[36m" << "*" << "\033[0m" << "--------------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t|""\033[36m" << "*" << "\033[0m" << "|  SCORE : " << left << setw(12) << score << "LIFE : " << left << setw(14) << life << "LEVEL : " << left << setw(3) << level << "|\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t|""\033[36m" << "*" << "\033[0m" << "--------------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t|" << "\033[36m" << "**********************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t*----------------------------------------------------------*" << endl;
	if(messile_target > 10)
		cout << "\t\t\t\t\t\t..................... MISSILE LOADED ......................." << endl;
	else
		cout << "\t\t\t\t\t\t............................................................" << endl;
}
void display_menu(int selected) {
	ostringstream buffer;
	buffer << "\n\n\n\n";
	buffer << "\t\t\t\t\t\t*=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t|                     " << "\033[36m" << "MISSION IMPOSSIBLE  " << "\033[0m" << "              |" << endl;
	buffer << "\t\t\t\t\t\t*=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t|                                                       |" << endl;
	string options[] = { "   NEW GAME   ", " INSTRUCTIONS ", "   RECORDS    ", "    EXIT      " };
	for (int i = 0; i < 4; i++) {
		if (i == selected) {
			buffer << "\033[46m\033[30m" << "\t\t\t\t\t\t|\t\t\t" << left << setw(14) << options[i] << "\t\t\t|" << "\033[0m" << endl;
			buffer << "\t\t\t\t\t\t|                                                       |" << endl;
		}
		else {
			buffer << "\t\t\t\t\t\t|\t\t\t" << options[i] << "\t\t\t|" << endl;
			buffer << "\t\t\t\t\t\t|                                                       |" << endl;
		}

	}
	buffer << "\t\t\t\t\t\t|=======================================================|" << endl;
	buffer << "\t\t\t\t\t\t|" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t*=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t|              " << "\033[36m" << "PRESS 'ENTER KEY' TO SELECT" << "\033[0m" << "              |" << endl;
	buffer << "\t\t\t\t\t\t*=======================================================*\n" << endl;
	buffer << "\t\t\t\t\t\t .............. PRESS 'W' OR 'S' TO MOVE ..............." << endl;
	cout << "\033[H" << buffer.str();
}
void levelExchangeScreen() {
	PlaySound(TEXT("level_change.wav"), NULL, SND_FILENAME | SND_ASYNC);
	ostringstream buffer;
	buffer << "\n\n\n\n\n\n";
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|\t\t        " << "\033[36m" << "LEVEL " << level << " COMPLETED " << "\033[0m" << "\t\t  |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |  SCORE : " << left << setw(14) << score << "LIFE : " << left << setw(13) << life << "LEVEL : " << left << setw(3) << level << "|" << endl;
	buffer << "\t\t\t\t\t\t    ---------------------------------------------------------\n" << endl;

	cout << "\033[H" << buffer.str();
	Sleep(1500);
	buffer.str("");      // Clear the contents of the buffer
	buffer.clear();      // Clear any error flags (important for reuse)
	system("cls");
	if (level == 5) {
		PlaySound(TEXT("game_win.wav"), NULL, SND_FILENAME | SND_ASYNC);
		buffer << "\n\n\n\n\n\n";
		buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
		buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                      " << "\033[36m" << "YOU WON!.." << "\033[0m" << "                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
		buffer << "\t\t\t\t\t\t    |  SCORE : " << left << setw(14) << score << "LIFE : " << left << setw(13) << life << "LEVEL : " << left << setw(3) << level << "|" << endl;
		buffer << "\t\t\t\t\t\t    ---------------------------------------------------------\n" << endl;
		buffer << "\t\t\t\t\t\t    ................ RETURNING TO MAIN MENU ................." << endl;
		cout << "\033[H\033[2J" << buffer.str();
		player_score = score;
		update_score();
		Sleep(2000);
		system("pause");
		system("cls");
		menu();

	}
	else if (level < 5) {
		level++;
		score += 10;
		messile_target += 10;
		ostringstream buffer;
		buffer << "\n\n\n\n\n\n";
		buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
		buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|\t\t        " << "\033[36m" << "STARTING LEVEL " << level << "\033[0m" << "\t\t  |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
		buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
		buffer << "\t\t\t\t\t\t    |  SCORE : " << left << setw(14) << score << "LIFE : " << left << setw(13) << life << "LEVEL : " << left << setw(3) << level << "|" << endl;
		buffer << "\t\t\t\t\t\t    ---------------------------------------------------------\n" << endl;
		cout << "\033[H\033[2J" << buffer.str();
	}
	system("pause");
	system("cls");

}
void GameOver() {

	PlaySound(TEXT("game_over.wav"), NULL, SND_FILENAME | SND_ASYNC);

	ostringstream buffer;
	buffer << "\n\n\n\n\n\n";
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                     " << "\033[36m" << "GAME OVER!.." << "\033[0m" << "                  |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |  SCORE : " << left << setw(14) << score << "LIFE : " << left << setw(13) << life << "LEVEL : " << left << setw(3) << level << "|" << endl;
	buffer << "\t\t\t\t\t\t    ---------------------------------------------------------\n" << endl;
	buffer << "\t\t\t\t\t\t    ................ RETURNING TO MAIN MENU ................." << endl;
	cout << "\033[H" << buffer.str();
	Sleep(2000);
	system("cls");
}
void pause_screen() {
	int input = '\0';
	ostringstream buffer;
	buffer << "\n\n\n\n\n\n";
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                     " << "\033[36m" << "GAME PAUSED" << "\033[0m" << "                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |  SCORE : " << left << setw(14) << score << "LIFE : " << left << setw(13) << life << "LEVEL : " << left << setw(3) << level << "|" << endl;
	buffer << "\t\t\t\t\t\t    ---------------------------------------------------------\n" << endl;
	buffer << "\t\t\t\t\t\t    .................... PRESS 'Q' TO QUIT ..................\n" << endl;
	buffer << "\t\t\t\t\t\t    ............... PRESS ANY KEY TO CONTINUE................" << endl;
	cout << "\033[H" << buffer.str();
	input = _getch();
	if (input == 'q' || input ==  'Q') {
		system("cls");
		menu();
	}
}
void exit_screen() {
	ostringstream buffer;
	buffer << "\n\n\n\n\n\n";
	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                 " << "\033[36m" << "THANKS FOR PLAYING" << "\033[0m" << "                |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    *=======================================================*\n" << endl;
	buffer << "\t\t\t\t\t\t    ........................ GOOD BYE ......................." << endl;
	cout << "\033[H" << buffer.str();
}

// -----------------------------  FILE HANDLING ------------------------------------------------------------------------//

void update_score() {

	ifstream fin;
	fin.open("record.txt");
	if (!fin.is_open())
		cout << "File not Found.." << endl;
	else
	{
		while( score_count < 10 && fin >> name[score_count] >> scores[score_count]) {
			score_count++;
		}
		fin.close();
	}
	
	if (score_count == 10) {
		int min = 0;
		for (int i = 0; i < 10; i++) {
			if (scores[i] < scores[min]) {
				min = i;
			}
		}
		// Replace the lowest score if the new score is higher
		if (player_score > scores[min]) {
			name[min] = player_name;
			scores[min] = player_score;
		}
	}
	else {
		if (score_count < 10) {
			name[score_count] = player_name;
			scores[score_count] = player_score;
			score_count++;
		}
		
	}

	// Sort scores in descending order using bubble sort
	for (int i = 0; i < score_count - 1; i++) {
		for (int j = 0; j < score_count - i - 1; j++) {
			if (scores[j] < scores[j + 1]) {
				swap(scores[j], scores[j + 1]);
				swap(name[j], name[j + 1]);
			}
		}
	}

	
	ofstream fout;

	fout.open("record.txt");
	if (!fout.is_open())
		cout << "File Not Found..";
	else {
		for (int i = 0; i < score_count; ++i) {
			fout << name[i] << " " << scores[i] << "\n";
		}
	}
	fout.close();
}
void input_name() {
	ostringstream buffer;
	buffer << "\n\n\n\n\n\n";

	buffer << "\t\t\t\t\t\t    *=======================================================*" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|         ENTER YOUR NAME :                         |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                                                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	buffer << "\t\t\t\t\t\t    *=======================================================*\n" << endl;
	buffer << "\t\t\t\t\t\t    ........................................................." << endl;
	cout << "\033[H" << buffer.str();
	cout << "\033[H" << "\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\t\t\t\t\t :  ";
	showCursor();
	cin >> player_name;
	hideCursor();
	system("cls");
}
void scoreboard() {
	string NAME;
	int SCORE;
	
	cout << "\n\n\n\n\n\n";

	cout << "\t\t\t\t\t\t    *=======================================================*" << endl;
	cout << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|          NAME           |          SCORE          |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|===================================================|" << "\033[36m" << "*" << "\033[0m" << "|" << endl;

	ifstream fin("record.txt");
	if (!fin.is_open()) {
		cout << "File not Found.." << endl;
	}
	else {
		for (;fin >> NAME >> SCORE;) { // Read and check for success
			cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << left << setw(11) << "|" << left << setw(15) << NAME << setw(12) << "|" << left << setw(14) << SCORE << "|" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
			cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "-----------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
		}
	}
	fin.close();
	cout << "\t\t\t\t\t\t    |" << "\033[36m" << "*******************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    *=======================================================*" << endl;
}
bool level_check(char map[][COL_SIZE]) {
	if (player_row == 0) {
		return true;
	}
	return false;
}
void instructions() {

	cout << "\t\t\t\t\t\t    *========================================================*" << endl;
	cout << "\t\t\t\t\t\t    |" << "\033[36m" << "********************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "------------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "|                     " << "\033[36m" << "INSTRUCTIONS" << "\033[0m" << "                   |" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |""\033[36m" << "*" << "\033[0m" << "------------------------------------------------------" << "\033[36m" << "*" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    |" << "\033[36m" << "********************************************************" << "\033[0m" << "|" << endl;
	cout << "\t\t\t\t\t\t    *========================================================*" << endl;
	cout << "\t\t\t\t\t\t    .........................................................." << endl;

	ifstream fin;
	string line;
	fin.open("instructions.txt");
	if (!fin.is_open()) {
		cout << "File Not Found " << endl;
	}
	else {
		while (!fin.eof()) {
			getline(fin, line);
			cout << "\t\t\t\t\t\t   ";
			cout << line << endl;
				if (fin.eof())
				break;
		}
	}
	fin.close();
	cout << "\t\t\t\t\t\t    .........................................................." << endl;
}

// -----------------------------------------------------------------------------------------------------------------//

void initialization(char map[][COL_SIZE], int enemy_position[][2], int obstacles_position[][2]) {
	bool IsFiring = true;
	player_fire = false;
	life = 3;
	game = true;
	IsFiring = false;
	bool IsEnemyDown[7] = { false };
	bool is_enemy_fire[6][4] = { false };
	obs_enemy_count();
	initializeMap(map);
	initialize_obstacles(map, obstacles_position);
	initialize_player(map);
	initialize_soloman(map);
	soloman_bullet_row = soloman_row - 2;
	soloman_bullet_col = soloman_col;
	initialize_enemy(map, enemy_position);
	print_map(map);
}
void gameloop() {
	
	messile_target = 0;
	int enemy_position[7][2] = { '\0' };
	 soloman_bullet_row = soloman_row - 2;
	 soloman_bullet_col = soloman_col;
	char map[ROW_SIZE][COL_SIZE], input_direction = '\0';
	int loop_counter = 0;
	level = 1;
	score = 0;
	input_name();
	while (level <= 5) {
		PlaySound(TEXT("background.wav"), NULL, SND_FILENAME | SND_ASYNC);
		initialization(map, enemy_position, obstacles_position);

		while (game && life > 0) {
			for (int i = 0; i < ROW_SIZE; i++) {
				for (int j = 0; j < COL_SIZE; j++) {
					map[i][j] = ' ';
				}
			}
			if (_kbhit()) {
	
				input_direction = _getch();
				input_direction = tolower(input_direction);
				if (input_direction == 27) {
					system("cls");
					PlaySound(NULL, 0, 0);
					pause_screen();
					system("cls");
					PlaySound(TEXT("background.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
				every_movement(map, input_direction);
			}

			loop_counter++;
			if (loop_counter == 2) {
				obstacles(map, obstacles_position);
				moveTowardsObstacle(map, enemy_position);
				loop_counter = 0;
			}
			player(map);
			soloman_fire(map, soloman_bullet_row, soloman_bullet_col);
			enemy_fire(map, enemy_position);
			enemy(map, enemy_position);
			clear_enemy_bullet(map);
			print_bullet(map); 
			obstacles(map, obstacles_position);                      
			soloman_movement(map);                                                              
			soloman(map);                                                                      
			soloman_fire_check(map, soloman_bullet_row, soloman_bullet_col);
			enemy_fire_check(map);
			fire_check(map, enemy_position);                                                       
			boundary(map);                                                                     
			life_check(map);
			print_map(map);       
			Sleep(level_speed);
			displayScoreboard();
			if (level_check(map)) {
				game = false;
				system("cls");
				PlaySound(NULL, 0, 0);
				levelExchangeScreen();
			}
		}
		if (life == 0) {
			for (int i = 0; i < ROW_SIZE; i++) {
				for (int j = 0; j < COL_SIZE; j++) {
					map[i][j] = ' ';
					if (i < 7 && j < 2)
						enemy_position[i][j] = '\0';
				}
			}
			system("cls");
			PlaySound(NULL, 0, 0);
			GameOver();
			level = 6;
			game = false;
		}
	}
	player_score = score;
	update_score();

}

void menu() {
	char input;
	int selected = 0;
	bool menu_loop = true;
	while (menu_loop) {
		moveCursorToTop();
		display_menu(selected);
		input = _getch();
		input = tolower(input);
		if (input == 'w') {
			PlaySound(TEXT("menu_change.wav"), NULL, SND_FILENAME | SND_ASYNC);
			selected--;
			if (selected < 0)
				selected = 3;
		}
		else if (input == 's') {
			PlaySound(TEXT("menu_change.wav"), NULL, SND_FILENAME | SND_ASYNC);
			selected++;
			if (selected > 3)
				selected = 0;
		}
		else if (input == '\r') {
			switch (selected) {

			case 0:
			{
				system("cls");
				gameloop();
				break;
			}
			case 1: {
				system("cls");
				instructions();
				system("pause");
				system("cls");
				break;
			}
			case 2: {
				system("cls");
				scoreboard();
				system("pause");
				system("cls");
				break;
			}
			case 3: {

				system("cls");
				menu_loop = false;
				exit_screen();
				Sleep(1500);
				break;
			}
				  if (!menu_loop)
					  break;

			}
		}
	}
}
int main() {
	hideCursor();
	menu();
	return 0;
}