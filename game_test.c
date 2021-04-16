#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

char map[20][20]; //map[y][x]
const int start_x = 4, start_y = 3, max_x = 10, max_y = 20;
int con_x, con_y, is_stop, type_id[2], first = 0;
char blocks = 'O';
int line = 0, score = 0;
// 0:��� 1:���� 2:L�� 3:�� �� 4:T�� 

const int shape[7][4][4] = {
	{
		{1,1,0,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0}
	},
	{
		{1,1,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{0,0,0,0}
	},
	{
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,0,0,0},
		{1,1,0,0},
		{1,0,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	}
};

int long_type[4][4];


int is_touch_down(); //�P�_�O�_�IĲ�U���� 
int is_touch_wall(char); //�P�_�O�_�IĲ���k��� 
int is_gameover();//�P�_�O�_�C������ 
int is_draw_able(int);//�P�_�_�l�I�O�_��e��� 
char to_block(int); //��ư}�C��ϧ� 
void re_fresh(); //��s�e�� 
void blocks_check(); //�P�_���O�_�񺡮��� 
void down_fill(int); //������V�U��  
void blocks_fall(); //����۵M���U 
void blocks_move(char); //���ʤ�� 
void new_blocks_move(char); // �s�����ʤ�� 
void move_set(char); //���ʤ��(���) 
void rotate(int[4][4]);//��V
void fix_place();//��V��ե���m 
void set_blocks(int);//�]�w��ܤ�� 
void clear_old_blocks();//�M���¤�� 
void draw_blocks(int,int);
void ending();

//======== ��V���U 
int is_wall(int[4][4]);//�O�_�K����
int is_top(int[4][4]); 
int is_rotate_able(); 
//======== ��V���U

int main(){
	srand(time(0));
	char c;
	int i, j, t, game_over = 0;
	for (i = 0 ; i < max_y ; i++)
		for (j = 0;  j < max_x ; j++)
			map[i][j] = ' ';
	while (!game_over){
		if (is_gameover())
			break;
		if (!first){
			type_id[0] = rand() % 7;
			type_id[1] = rand() % 7;
			first = !first;
		}
		else{
			type_id[0] = type_id[1];
			type_id[1] = rand() % 7;
		}
		con_y = start_y;
		if (!is_draw_able(start_x)){
			for (i = 0 ; i < max_x ; i++){
				if (is_draw_able(i)){
					con_x = i;
					break;
				}
			}
			if (i == max_x)
				break;
		}
		else
			con_x = start_x;
		
		set_blocks(type_id[0]);
		draw_blocks(0,0);
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

int is_gameover(){
	int i, j;
	for (i = 0 ; i < max_x ; i++){
		if (map[0][i] != ' ')
			return 1;
	}
	return 0;
}

int is_draw_able(int x){
	int i, j;
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (long_type[i][j] == 1 && map[3 - i][j + x] != ' ')
				return 0;
		}
	}
	return 1;
}

void re_fresh(){
	system("CLS");
	int i, j, k, z = 0;
	for (i = 0 ; i < max_y ; i++){
		for (j = 0 ; j < max_x ; j++){
			printf("%c", map[i][j]);
		}
		printf("|");
		if (i == 1)
			printf("     �U�@�Ӥ���O:");
		else if (i == 2 || i == 3 || i == 4 || i == 5){
			printf("     ");
			for (k = 0 ; k < 4 ; k++){
				to_block(shape[type_id[1]][3 - z][k]);
			}
			z++;
		}
		else if (i == 8)
			printf("     �A�H�d��%d���u", line);
		else if (i == 9)
			printf("     �o��:%d��", score);
		else if (i == 11)
			printf("     W:�����̩ܳ���");
		else if (i == 12)
			printf("     A:����  S:�U��  D:�k��");
		else if (i == 13)
			printf("     �ť���:����");
		printf("\n");
	}
	for (i = 0 ; i < max_x ; i++)
		printf("=");
	printf("=\n");
}

void blocks_check(){
	int i, j, ct = 0;
	for (i = 0 ; i < max_y ; i++){
		for (j = 0 ; j < max_x ; j++){
			if (map[i][j] == ' ')
				break;
		}
		if (j == max_x){
			down_fill(i);
			line++;
			ct++;
		}
	}
	switch (ct){
		case 1:{
			score += 40;
			break;
		}
		case 2:{
			score += 100;
			break;
		}
		case 3:{
			score += 300;
			break;
		}
		case 4:{
			score += 1200;
			break;
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
//����ʤ��
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
		case ' ':
			clear_old_blocks();
			if (is_rotate_able()){
				rotate(long_type);
				fix_place(long_type);
				draw_blocks(0,0);
				re_fresh();
			}
			else{
				draw_blocks(0,0);
				re_fresh();
			}
			break;
		case 'W':
		case 'w':
			while(!is_stop)
				blocks_fall();
			re_fresh();
			break;
	}
}
//����۵M���U 
void blocks_fall(){
	int i, j;
	//if (map[con_y + 1][con_x] == ' '){
	if (is_touch_down() && con_y < max_y){
		clear_old_blocks();
		draw_blocks(1,0);
		con_y++;
	}
	else
		is_stop = 1;
	re_fresh();
}

void ending(){
	system("CLS");
	printf("~~GAME OVER~~\n�A�`�@�����F%d���u\n�`����%d��", line, score);
}


//��V 
void rotate(int arr[4][4]){
	int i, j;
	int brr[4][4];
	for (i = 0 ; i < 4 ; i++)
		for (j = 0 ; j < 4 ; j++)
			brr[i][j] = arr[3-j][i];
	for (i = 0 ; i < 4 ; i++)
		for (j = 0 ; j < 4 ; j++)
			arr[i][j] = brr[i][j];
}
//��V��ե���m 
void fix_place(){
	int i, j;
	while (is_wall(long_type)){	
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 3 ; j++){
				long_type[i][j] = long_type[i][j + 1];
			}
		}
		for (i = 0 ; i < 4 ; i++)
			long_type[i][3] = 0;
	}
	/*
	while (is_top(long_type)){
		for (i = 0 ; i < 3 ; i++){
			for (j = 0 ; j < 4 ; j++){
				long_type[i][j] = long_type[i + 1][j];
			}
		}
		for (i = 0 ; i < 4 ; i++)
			long_type[3][i] = 0;
	}
	*/
}
//�O�_������� 
int is_rotate_able(){
	int i, j;
	int arr[4][4];
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			arr[i][j] = long_type[3-j][i];
		}
	}
	//=====================
	while (is_wall(arr)){
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 3 ; j++){
				arr[i][j] = arr[i][j + 1];
			}
		}
		for (i = 0 ; i < 4 ; i++)
			arr[i][3] = 0;
	}
	//========================
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (arr[i][j] == 1 && map[con_y - i][con_x + j] != ' ')
				return 0;
		}
	}
	return 1;
}

int is_wall(int arr[4][4]){
	int i;
	for (i = 0 ; i < 4 ; i++)
		if (arr[i][0] == 1)
			break;
	return (i == 4);
}

int is_top(int arr[4][4]){
	int i;
	for (i = 0 ; i < 4 ; i++){
		if (arr[0][i] == 1)
			return 0;
	return 1;
	}
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
				if (map[con_y][con_x + x] != ' ')
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
		clear_old_blocks();
		/*
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1)
					map[con_y - i][con_x + j + x] = to_block(long_type[i][j]);
			}
		}
		*/
		draw_blocks(0,x);
			con_x = con_x + x;
	}
	else if (way == 'S' || way == 's'){
		clear_old_blocks();
		/*
		for (i = 0 ; i < 4 ; i++){
			for (j = 0 ; j < 4 ; j++){
				if (long_type[i][j] == 1)
					map[con_y - i + 1][con_x + j] = to_block(long_type[i][j]);
			}
		}
		*/
		draw_blocks(1,0);
		con_y++;
	}
}
//�]�w������� 
void set_blocks(int type){
	int i, j;
	for (i = 0 ; i < 4 ; i++)
		for (j = 0 ; j < 4 ; j++)
			long_type[i][j] = shape[type][i][j];
}
//�M���¤�� 
void clear_old_blocks(){
	int i, j;
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (long_type[i][j] == 1)
				map[con_y - i][con_x + j] = ' ';
		}
	}
}
//�e��� 
void draw_blocks(int y,int x){
	int i, j;
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){
			if (long_type[i][j] == 1){
				map[con_y - i + y][con_x + j + x] = to_block(long_type[i][j]);
			}
		}
	}
}

//�}�C�������r�� 
char to_block(int x){
	if (x)
		printf("%c", blocks);
	else
		printf(" ");
}
