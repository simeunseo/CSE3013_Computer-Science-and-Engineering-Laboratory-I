#include "ofApp.h"
#include <ctime> 

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(WIDTH, HEIGHT); //화면 크기 설정
	
	text.load("NotoSansMonoCJKkr-Regular.otf", 12);
	text_time.load("NotoSansMonoCJKkr-Regular.otf", 40);
	text_gameover.load("NotoSansMonoCJKkr-Regular.otf", 50);
	
	initGame();
}

void ofApp::initGame() {
	ofBackground(255); //화면 초기화
	srand((unsigned int)time(NULL));
	
	//area 초기화
	for (int i = 0; i < num_of_area; i++) {
		area[i] = i * BLOCK_SIZE;
	}
	//field 메모리 할당 및 초기화
	if (!field) {
		field = new Field[sizeof(Field)*num_of_area];
	}
	for (int i = 0; i < num_of_area; i++) {
		field[i].num_of_block = 0;
		field[i].num_of_water = 0;
		field[i].state = 0;
		field[i].can_waterfall = 0;
	}

	//flag 초기화
	//block build phase부터 시작한다
	block_flag = 1;
	water_flag = 0;
	new_water_flag = 0;

	//변수 초기화
	score = 0;
	game_over = 0;

	selection_area = 0;
	selection_water = 0;
	remain_block_num = BLOCK_LIMIT_NUM;

	waterfall_time_flag = 0;
	waterfall_time = 0;

	//3개의 블럭을 랜덤으로 쌓아두고 시작한다
	for (int i = 0; i < 3; i++) {
		int random_selection = (double)rand() / RAND_MAX * num_of_area;
		field[random_selection].state = 1;
		field[random_selection].num_of_block++;
	}
	start_time = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (score == SCORE_GOAL) { //score가 목표 점수에 도달하면
		game_over = -1;//success
		ofSetColor(ofColor::forestGreen);
		ofDrawRectangle(120, 160, 360, 130);
		ofSetColor(255);
		text_gameover.drawString("SUCCESS", 180, 230);
		text.drawString("replay (R)	quit(Q)", 210, 265);
	}
	
	if (game_over == 1) { //game over시
		ofSetColor(ofColor::indianRed);
		ofDrawRectangle(120, 160, 360, 160);
		ofSetColor(255);
		text_gameover.drawString("GAME OVER", 145, 230);
		text.drawString(gameover_reason, 145, 265);
		text.drawString("replay (R)	quit(Q)", 210, 300);
	}
	else if (game_over != -1) { //아직 game over가 아닐시
		remain_time = TIME_LIMIT - (ofGetElapsedTimeMillis() - start_time) / 1000; //타이머 작동
		if (remain_time == -1) { //타이머가 끝났을 시
			remain_time = 0; //0으로 다시 보정
			gameover_reason = "time out";
			game_over = 1;
		}

		//화면상에 남은 블럭 수와 점수 표시
		ofSetColor(ofColor::darkGrey);
		text.drawString("remaining blocks", 10, 20);
		text.drawString("your score", 10, 45);
		ofSetColor(ofColor::forestGreen);
		text.drawString(to_string(remain_block_num), 150, 20);
		text.drawString(to_string(score), 100, 45);
		ofSetColor(ofColor::indianRed);
		text_time.drawString(to_string(remain_time), WIDTH - 60, 50);

		//제한선 그리기
		ofSetColor(ofColor::indianRed);

		//선택할 수 있는 영역(darkGrey)과 선택한 영역(red) 그리기
		ofDrawLine(0, 115, WIDTH, 115);
		ofSetColor(ofColor::darkGrey);
		ofFill();
		for (int i = 0; i < num_of_area; i++) {
			if (selection_area == i)
				ofSetColor(ofColor::red);
			else
				ofSetColor(ofColor::darkGrey);
			ofDrawRectangle(area[i], HEIGHT, BLOCK_SIZE, -5);

			//지금까지 쌓인 블럭 그리기
			ofSetColor(ofColor::dimGrey);
			if (field[i].state == 1 || field[i].state == 3) {
				for (int j = 0; j < field[i].num_of_block; j++) {
					ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - (j + 1) * BLOCK_SIZE - 5, BLOCK_SIZE, BLOCK_SIZE);
				}
			}
		}

		

		//waterfall phase
		if (water_flag == 1 && new_water_flag == 1) {
			//물이 떨어질 지점에 화살표 그리기
			ofSetColor(ofColor::deepSkyBlue);
			int Xpos = selection_water * BLOCK_SIZE + BLOCK_SIZE * 0.5;
			ofDrawLine(Xpos, 65, Xpos, 90);
			ofDrawTriangle(Xpos - 5, 90, Xpos + 5, 90, Xpos, 105);

			//물 그리기
			for (int i = 0; i < num_of_area; i++) {
				if (field[i].state == 2) { //물만 있는 area라면
					if (i == selection_water) { //물이 떨어질 area는 떨어지기 전 상태까지만 그린다
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water - 1));
					}
					else {
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * field[i].num_of_water);
					}
				}
				else if (field[i].state == 3) { //블럭과 물이 둘다 있는 area라면
					if (i == selection_water) { //물이 떨어질 area는 떨어지기 전 상태까지만 그린다
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5 - field[i].num_of_block*BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water - 1));
					}
					else {
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5 - field[i].num_of_block*BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE * field[i].num_of_water);
					}
				}
			}

			//물이 차오르는 과정을 보여준다
			if (waterfall_time != BLOCK_SIZE) {
				waterfall_time++;
				ofDrawRectangle(selection_water*BLOCK_SIZE, HEIGHT - ((numOfBlockWater(selection_water) - 1) * BLOCK_SIZE) - 5, BLOCK_SIZE, -waterfall_time);
			}
			else {
				ofDrawRectangle(selection_water*BLOCK_SIZE, HEIGHT - ((numOfBlockWater(selection_water) - 1) * BLOCK_SIZE) - 5, BLOCK_SIZE, -BLOCK_SIZE);
			}
		}

		//물이 다 찼다면 이 상태를 반영하여 다시 물을 그린다
		if (waterfall_time == 0) {
			ofSetColor(ofColor::deepSkyBlue);
			for (int i = 0; i < num_of_area; i++) {
				if (field[i].state == 2) { //물만 있는 area라면 맨 아래부터 차있는 지점까지 사각형을 그린다
					ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water));
				}
				else if (field[i].state == 3) { //블럭과 물이 둘다 있는 area라면 블럭이 끝나는 지점부터 물이 차있는 지점까지 사각형을 그린다
					ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - field[i].num_of_block*BLOCK_SIZE - 5, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water));
				}
			}

		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'Q' || key == 'q') {
		delete[] field;
		ofExit(0);
	}

	if (game_over == 0) {
		//game over가 아닐때만 오른쪽, 왼쪽, 위쪽 key를 받아들이도록 한다
		if (key == OF_KEY_RIGHT) { //오른쪽 키를 누르면
			block_flag = 1; //block build phase
			water_flag = 0;
			waterfall_time = 0;
			selection_area++; //선택 area를 오른쪽으로 옮긴다
			if (selection_area == num_of_area) //끝으로가면
				selection_area = 0; //처음부터 다시 시작한다
		}
		if (key == OF_KEY_LEFT) { //왼쪽 키를 누르면
			block_flag = 1; //block build phase
			water_flag = 0;
			waterfall_time = 0;
			selection_area--; //선택 area를 왼쪽으로 옮긴다
			if (selection_area < 0) //처음으로 가면
				selection_area = num_of_area - 1; //끝부터 다시 시작한다
		}
		if (key == OF_KEY_UP) { //위쪽 키를 누르면
			if (field[selection_area].num_of_block + 1 == BLOCK_LIMIT_HEIGHT) { //블럭이 제한선 위로 쌓인다면
				gameover_reason = "you exceeded the limit line";
				game_over = 1;
			}
			else if (field[selection_area].state == 0 || field[selection_area].state == 1) { //블럭을 쌓을 영역이 비어있거나 블럭만 쌓인 영역이라면 블럭 쌓기가 가능하다
				block_flag = 1; //block build phase
				water_flag = 0;
				waterfall_time = 0;
				new_water_flag = 0;

				if (field[selection_area].state != 1) //처음 쌓는 영역이라면
					field[selection_area].state = 1; //필드 상태를 블럭(1)으로 세팅한다

				//블럭을 쌓는다
				field[selection_area].num_of_block++;

				//블럭의 높이를 5이상 쌓는다면 점수를 5점 깎는다
				if (field[selection_area].num_of_block > 4) {
					score -= 5;
				}

				remain_block_num--; //남아있는 블럭의 수를 줄인다
				if (remain_block_num == -1) { //남아있는 블럭의 수가 없다면
					remain_block_num = 0;
					gameover_reason = "you used all blocks";
					game_over = 1;
				}

				//물이 쌓일 수 있는 위치 초기화
				for (int i = 0; i < num_of_area; i++) {
					field[i].can_waterfall = 0;
				}

				//물이 쌓일 수 있는 위치 찾기
				int left_block = -1;
				int i = 0;
				while (i < num_of_area) { //마지막 area까지 탐색한다
					if (left_block == -1) { //왼쪽 블럭이 없다면
						if (field[i].state == 1) { //이 area에 블럭이 있다면
							left_block = i; //이 블럭을 왼쪽 블럭으로 취한다
						}
					}
					else { //왼쪽 블럭이 있다면
						if (numOfBlockWater(i) <= numOfBlockWater(i - 1)) { //이 area에 쌓인 블럭의 수가 바로 전 블럭의 수보다 적다면
							i++; //오른쪽 블럭으로 삼을수 없으므로, 다음 area로 넘어가서 탐색을 계속한다.
							continue;
						}
						else if (numOfBlockWater(i) != 0) { //이 area에 쌓인 블럭의 수가 바로 전 블럭의 수보다 크거나 같으면서 0은 아니라면
							if (i - left_block == 1) { //이 area의 블럭과 왼쪽 블럭이 바로 붙어있는 블럭이라면, 즉 둘 사이에 공간이 없다면
								left_block = i; //이 area의 블럭을 왼쪽 블럭으로 취하여
								i++; //다음 area로 넘어가서 탐색을 계속한다
								continue;
							}
							else if (i - left_block > 1) { //이 area의 블럭과 왼쪽 블럭 사이에 공간이 있다면
								for (int j = left_block + 1; j < i; j++) {
									if (numOfBlockWater(j) == numOfBlockWater(i - 1) && numOfBlockWater(j) < numOfBlockWater(left_block)) {
										field[j].can_waterfall = 1; //그 사이의 area들을 물이 떨어질 수 있는 곳으로 설정한다
										new_water_flag = 1;
									}
								}
								left_block = i; //이 area의 블럭을 왼쪽 블럭으로 취하여
								i++; //다음 area로 넘어가서 탐색을 계속한다
								continue;
							}
						}
					}
					i++;
				}

				if (new_water_flag == 0) { //물이 쌓일 수 있는 위치가 없다면
					gameover_reason = "there is no space to fill water";
					game_over = 1;
				}
				else {
					block_flag = 0;
					water_flag = 1; //waterfall phase로 전환

					do { //물을 쌓을 수 있는 위치 중 랜덤한 위치를 selection_water에 할당한다
						selection_water = (double)rand() / RAND_MAX * num_of_area;
					} while (!field[selection_water].can_waterfall);

					if (field[selection_water].state == 0 || field[selection_water].state == 2) { //원래 아무것도 없었거나 물만 있던 area라면
						field[selection_water].state = 2; //state 2: 물만 쌓여있음
					}
					else if (field[selection_water].state == 1) { //원래 블럭이 있던 area라면
						field[selection_water].state = 3; //state 3 : 블럭과 물이 함께 쌓여있음
					}

					field[selection_water].num_of_water++; //쌓인 물의 수를 추가한다
					score += 10; //점수를 10점 얻는다
				}
			}
			else { //블럭을 놓을 수 없는 위치
				gameover_reason = "you put the block where you can't put it";
				game_over = 1;
			}
		}
	}
	else { //game over 또는 success시
		if (key == 'R' || key == 'r') {
			initGame();
		}
	}

}

int ofApp::numOfBlockWater(int i) {
	return field[i].num_of_block + field[i].num_of_water;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
