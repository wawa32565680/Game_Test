#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

char map[20][20]; //map[y][x]
const int start_x = 4, start_y = 0, max_x = 10, max_y = 20;
int con_x, con_y, is_stop;
char blocks = '#';

// 0:��� 1:���� 2:L�� 3:�� �� 4:T�� 

void re_fresh();//��s�e�� 
void blocks_check(); //�P�_���O�_�񺡮��� 
void down_fill(int); //������V�U��  
void blocks_fall(); //����۵M���U 
void blocks_move(char); //���ʤ�� 
void blocks_type(int);//ø�s���P�ϫ� 
void ending();

int main(){
	srand(time(0));
	char c;
	int i, j, t, game_over = 0, type_id;
	for (i = 0 ; i < max_y ; i++)
		for (j = 0;  j < max_x ; j++)
			map[i][j] = ' ';
	while (!game_over){
		if (map[start_y][start_x] != ' ')
			break;
		type_id = rand() % 5;
		map[start_y][start_x] = blocks;
		con_x = start_x;
		con_y = start_y;
		re_fresh();
		t = 20000;
		is_stop = 0;
		while (!is_stop && !game_over){
			t = 20000;
			while (t > 0){
				if (kbhit()){
					c = getch();
					if (c == 'Q' || c == 'q'){
						game_over = 1;
						break;
					}
					blocks_move(c);
					re_fresh();
					
				}
				t--;
			}
			blocks_fall();
		}
		blocks_check();
	}
	ending();
}

void re_fresh(){
	system("CLS");
	int i, j;
	for (i = 0 ; i < max_y ; i++){
		for (j = 0 ; j < max_x ; j++){
			printf("%c", map[i][j]);
		}
		printf("|");
		printf("\n");
	}
	for (i = 0 ; i < max_x ; i++)
		printf("=");
	printf("=\n");
}

void blocks_check(){
	int i, j;
	for (i = 0 ; i < max_y ; i++){
		for (j = 0 ; j < max_x ; j++){
			if (map[i][j] == ' ')
				break;
		}
		if (j == max_x){
			down_fill(i);
		}
	}
}

void down_fill(int pai){
	int i, j;
	for (i = pai ; i > 0 ; i--){
		for (j = 0 ; j < max_x ; j++){
			map[i][j] = map[i - 1][j];
		}
	}
}

void blocks_move(char way){
	switch (way){
		case 'A':
		case 'a':
			if (map[con_y][con_x - 1] == ' ' && con_x > 0){
				map[con_y][con_x - 1] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_x--;
			}
			break;
		case 'D':
		case 'd':
			if (map[con_y][con_x + 1] == ' ' && con_x < max_x - 1){
				map[con_y][con_x + 1] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_x++;
			}
			break;
		case 'S':
		case 's':
			if (map[con_y + 1][con_x] == ' ' && con_y < max_y){
				map[con_y + 1][con_x] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_y++;
			}
	}
}

void blocks_fall(){
	if (map[con_y + 1][con_x] == ' '){
		map[con_y + 1][con_x] = map[con_y][con_x];
		map[con_y][con_x] = ' ';
		con_y++;
	}
	else
		is_stop = 1;
	re_fresh();
}

void ending(){
	system("CLS");
	printf("~~GAME OVER~~\n");
}

void blocks_type(int type){
	int i, j;
	switch (type){
		case 0:{
			
			break;
		}
	}
}
