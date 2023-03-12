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
			int state; //0: 비어있음, 1: 블럭만 쌓여있음, 2: 물만 쌓여있음, 3: 블럭과 물이 함께 쌓여있음 
			int num_of_block; //블럭이 얼마나 쌓여있는가
			int num_of_water; //물이 얼마나 쌓여있는가
			bool can_waterfall; //물이 떨어질 수 있는 곳인가
		}Field;
		Field* field;

		int start_time;
		int score; //점수
		int game_over; //게임오버 플래그

		int selection_area; //현재 선택한 영역
		int selection_water; //물이 떨어질 영역
		int num_of_area = WIDTH / BLOCK_SIZE; //총 영역의 개수
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
