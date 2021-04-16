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

int box[4][4] = {
	{0,1,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
};

int long_type[4][4] = {
	{1,1,1,0},
	{1,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};


int is_touch_down(); //�P�_�O�_�IĲ�U���� 
int is_touch_wall(char); //�P�_�O�_�IĲ���k��� 
char to_block(int); //��ư}�C��ϧ� 
void re_fresh(); //��s�e�� 
void blocks_check(); //�P�_���O�_�񺡮��� 
void down_fill(int); //������V�U��  
void blocks_fall(); //����۵M���U 
void blocks_move(char); //���ʤ�� 
void new_blocks_move(char); // �s�����ʤ�� 
void move_set(char); //���ʤ��(���) 
void rotate(int[4][4]);
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
		//type_id = rand() % 5;
		con_x = start_x;
		con_y = start_y + 4;
		//map[con_y][con_x] = blocks;
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				map[con_y - i][con_x + j] = to_block(long_type[i][j]);
			}
		}
		re_fresh();
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
					new_blocks_move(c);
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
	//map[con_y][con_x] = 'x';
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

void new_blocks_move(char way){
	switch (way){
		case 'A':
		case 'a':
			if (is_touch_wall('l') && con_x > 0)
				move_set('l');
				break;
		case 'D':
		case 'd':
			if (is_touch_wall('r') && con_x < max_x - 1)
				move_set('r');
				break;
		case 'S':
		case 's':
			if (is_touch_down() && con_y < max_y)
				move_set('s');
				break;
	}
}

void blocks_move(char way){
	int i, j;
	
	
	switch (way){
		case 'A':
		case 'a':
			/*
			if (map[con_y][con_x - 1] == ' ' && con_x > 0){
				map[con_y][con_x - 1] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_x--;
			}
			*/
			if (is_touch_wall('l') && con_x > 0){
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j] = ' ';
					}
				}
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j - 1] = to_block(long_type[i][j]);
					}
				}
				con_x--;
			}
			break;
		case 'D':
		case 'd':
			/*
			if (map[con_y][con_x + 1] == ' ' && con_x < max_x - 1){
				map[con_y][con_x + 1] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_x++;
			}
			*/
			//if (map[con_y][con_x + 1] == ' ' && con_x < max_x - 1){
			if (is_touch_wall('r') && con_x < max_x - 1){
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j] = ' ';
					}
				}
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j + 1] = to_block(long_type[i][j]);
					}
				}
				con_x++;
			}
			break;
		case 'S':
		case 's':
			/*
			if (map[con_y + 1][con_x] == ' ' && con_y < max_y){
				map[con_y + 1][con_x] = map[con_y][con_x];
				map[con_y][con_x] = ' ';
				con_y++;
			}
			*/
			//if (map[con_y + 1][con_x] == ' ' && con_y < max_y){
			if (is_touch_down() && con_y < max_y){
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j] = ' ';
					}
				}
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i + 1][con_x + j] = to_block(long_type[i][j]);
					}
				}
				con_y++;
			}
			break;
		case ' ':
			
			break;
	}
	
}
//����۵M���U 
void blocks_fall(){
	int i, j;
	//if (map[con_y + 1][con_x] == ' '){
	if (is_touch_down() && con_y < max_y){
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1){
					map[con_y - i][con_x + j] = ' ';
				}
			}
		}
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1)
					map[con_y - i + 1][con_x + j] = to_block(long_type[i][j]);
			}
		}
		con_y++;
	}
	else
		is_stop = 1;
	/*
	if (map[con_y + 1][con_x] == ' '){
		map[con_y + 1][con_x] = map[con_y][con_x];
		map[con_y][con_x] = ' ';
		con_y++;
		
	}
	else
		is_stop = 1;
	re_fresh();
	*/
	re_fresh();
}

void ending(){
	system("CLS");
	printf("~~GAME OVER~~\n");
}


//��V 
void rotete(int arr[4][4]){
	int i, j;
	int brr[4][4];
	for (i = 0 ; i < 4 ; i++)
		for (j = 0 ; j < 4 ; j++)
			brr[i][j] = arr[3-j][i];
	for (i = 0 ; i < 4 ; i++)
		for (j = 0 ; j < 4 ; j++)
			arr[i][j] = brr[i][j];
}
//�O�_�IĲ�U���� 
int is_touch_down(){
	int i, j;
	int is_compare[4] = {0,0,0,0};
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (is_compare[j])
				continue;
			if (long_type[i][j] == 1){
				if (map[con_y - i + 1][con_x + j] != ' ')
					return 0;
				else
					is_compare[j] = 1;
			}
		}
	}
	return 1;
}
//�O�_�IĲ���k��� 
int is_touch_wall(char way){
	int i, j, x;
	int is_compare[4] = {0,0,0,0};
	if (way == 'L' || way == 'l')
		x = -1;
	else if (way == 'R' || way == 'r')
		x = 1;
		
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (long_type[i][j] == 1 && long_type[i][j + x] == 0){
				if (map[con_y - i][con_x + j + x] != ' ')
					return 0;
			}
		}
	}
	return 1;
	
	/*
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (is_compare[j])
				continue;
			if (long_type[i][j] == 1){
				if (map[con_y][con_x] != ' ')
					return 0;
				else
					is_compare[j] = 1;
			}
		}
	}
	return 1;
	*/
}
//���ʨ�� 
void move_set(char way){
	int x, i, j;
	if (way == 'L' || way == 'l' || way == 'R' || way == 'r'){
		if (way == 'L' || way == 'l')
			x = -1;
		else
			x = 1;
		for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j] = ' ';
					}
				}
				for (i = 0 ; i < 4 ; i++){
					for (j = 0 ; j < 4 ; j++){
						if (long_type[i][j] == 1)
							map[con_y - i][con_x + j + x] = to_block(long_type[i][j]);
					}
				}
				con_x = con_x + x;
	}
	else if (way == 'S' || way == 's'){
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1){
					map[con_y - i][con_x + j] = ' ';
				}
			}
		}
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1)
					map[con_y - i + 1][con_x + j] = to_block(long_type[i][j]);
			}
		}
		con_y++;
	}
}
//�}�C�������r�� 
char to_block(int x){
	if (x)
		printf("%c", blocks);
	else
		printf(" ");
}
