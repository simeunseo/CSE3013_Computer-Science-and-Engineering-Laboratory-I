#pragma once

#include "ofMain.h"

#define WIDTH 600
#define HEIGHT 440
#define BLOCK_SIZE 40
#define BLOCK_LIMIT_NUM 30
#define BLOCK_LIMIT_HEIGHT 9
#define TIME_LIMIT 30
#define SCORE_GOAL 250

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void initGame();
		int numOfBlockWater(int i);

		int area[WIDTH / BLOCK_SIZE] = { 0, };

		typedef struct Field {
			int state; //0: �������, 1: ���� �׿�����, 2: ���� �׿�����, 3: ���� ���� �Բ� �׿����� 
			int num_of_block; //���� �󸶳� �׿��ִ°�
			int num_of_water; //���� �󸶳� �׿��ִ°�
			bool can_waterfall; //���� ������ �� �ִ� ���ΰ�
		}Field;
		Field* field;

		int start_time;
		int score; //����
		int game_over; //���ӿ��� �÷���

		int selection_area; //���� ������ ����
		int selection_water; //���� ������ ����
		int num_of_area = WIDTH / BLOCK_SIZE; //�� ������ ����
		int remain_block_num = BLOCK_LIMIT_NUM;
		int remain_time = TIME_LIMIT;

		bool block_flag; //block build phase
		bool water_flag; //waterfall phase
		bool new_water_flag; 
		bool waterfall_time_flag = 0;
		int waterfall_time = 0;

		

		ofTrueTypeFont text;
		ofTrueTypeFont text_time;
		ofTrueTypeFont text_gameover;
		string gameover_reason;
};
