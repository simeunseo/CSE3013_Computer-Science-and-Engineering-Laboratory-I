#include "ofApp.h"
#include <ctime> 

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(WIDTH, HEIGHT); //ȭ�� ũ�� ����
	
	text.load("NotoSansMonoCJKkr-Regular.otf", 12);
	text_time.load("NotoSansMonoCJKkr-Regular.otf", 40);
	text_gameover.load("NotoSansMonoCJKkr-Regular.otf", 50);
	
	initGame();
}

void ofApp::initGame() {
	ofBackground(255); //ȭ�� �ʱ�ȭ
	srand((unsigned int)time(NULL));
	
	//area �ʱ�ȭ
	for (int i = 0; i < num_of_area; i++) {
		area[i] = i * BLOCK_SIZE;
	}
	//field �޸� �Ҵ� �� �ʱ�ȭ
	if (!field) {
		field = new Field[sizeof(Field)*num_of_area];
	}
	for (int i = 0; i < num_of_area; i++) {
		field[i].num_of_block = 0;
		field[i].num_of_water = 0;
		field[i].state = 0;
		field[i].can_waterfall = 0;
	}

	//flag �ʱ�ȭ
	//block build phase���� �����Ѵ�
	block_flag = 1;
	water_flag = 0;
	new_water_flag = 0;

	//���� �ʱ�ȭ
	score = 0;
	game_over = 0;

	selection_area = 0;
	selection_water = 0;
	remain_block_num = BLOCK_LIMIT_NUM;

	waterfall_time_flag = 0;
	waterfall_time = 0;

	//3���� ���� �������� �׾Ƶΰ� �����Ѵ�
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
	if (score == SCORE_GOAL) { //score�� ��ǥ ������ �����ϸ�
		game_over = -1;//success
		ofSetColor(ofColor::forestGreen);
		ofDrawRectangle(120, 160, 360, 130);
		ofSetColor(255);
		text_gameover.drawString("SUCCESS", 180, 230);
		text.drawString("replay (R)	quit(Q)", 210, 265);
	}
	
	if (game_over == 1) { //game over��
		ofSetColor(ofColor::indianRed);
		ofDrawRectangle(120, 160, 360, 160);
		ofSetColor(255);
		text_gameover.drawString("GAME OVER", 145, 230);
		text.drawString(gameover_reason, 145, 265);
		text.drawString("replay (R)	quit(Q)", 210, 300);
	}
	else if (game_over != -1) { //���� game over�� �ƴҽ�
		remain_time = TIME_LIMIT - (ofGetElapsedTimeMillis() - start_time) / 1000; //Ÿ�̸� �۵�
		if (remain_time == -1) { //Ÿ�̸Ӱ� ������ ��
			remain_time = 0; //0���� �ٽ� ����
			gameover_reason = "time out";
			game_over = 1;
		}

		//ȭ��� ���� �� ���� ���� ǥ��
		ofSetColor(ofColor::darkGrey);
		text.drawString("remaining blocks", 10, 20);
		text.drawString("your score", 10, 45);
		ofSetColor(ofColor::forestGreen);
		text.drawString(to_string(remain_block_num), 150, 20);
		text.drawString(to_string(score), 100, 45);
		ofSetColor(ofColor::indianRed);
		text_time.drawString(to_string(remain_time), WIDTH - 60, 50);

		//���Ѽ� �׸���
		ofSetColor(ofColor::indianRed);

		//������ �� �ִ� ����(darkGrey)�� ������ ����(red) �׸���
		ofDrawLine(0, 115, WIDTH, 115);
		ofSetColor(ofColor::darkGrey);
		ofFill();
		for (int i = 0; i < num_of_area; i++) {
			if (selection_area == i)
				ofSetColor(ofColor::red);
			else
				ofSetColor(ofColor::darkGrey);
			ofDrawRectangle(area[i], HEIGHT, BLOCK_SIZE, -5);

			//���ݱ��� ���� �� �׸���
			ofSetColor(ofColor::dimGrey);
			if (field[i].state == 1 || field[i].state == 3) {
				for (int j = 0; j < field[i].num_of_block; j++) {
					ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - (j + 1) * BLOCK_SIZE - 5, BLOCK_SIZE, BLOCK_SIZE);
				}
			}
		}

		

		//waterfall phase
		if (water_flag == 1 && new_water_flag == 1) {
			//���� ������ ������ ȭ��ǥ �׸���
			ofSetColor(ofColor::deepSkyBlue);
			int Xpos = selection_water * BLOCK_SIZE + BLOCK_SIZE * 0.5;
			ofDrawLine(Xpos, 65, Xpos, 90);
			ofDrawTriangle(Xpos - 5, 90, Xpos + 5, 90, Xpos, 105);

			//�� �׸���
			for (int i = 0; i < num_of_area; i++) {
				if (field[i].state == 2) { //���� �ִ� area���
					if (i == selection_water) { //���� ������ area�� �������� �� ���±����� �׸���
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water - 1));
					}
					else {
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * field[i].num_of_water);
					}
				}
				else if (field[i].state == 3) { //���� ���� �Ѵ� �ִ� area���
					if (i == selection_water) { //���� ������ area�� �������� �� ���±����� �׸���
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5 - field[i].num_of_block*BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water - 1));
					}
					else {
						ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5 - field[i].num_of_block*BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE * field[i].num_of_water);
					}
				}
			}

			//���� �������� ������ �����ش�
			if (waterfall_time != BLOCK_SIZE) {
				waterfall_time++;
				ofDrawRectangle(selection_water*BLOCK_SIZE, HEIGHT - ((numOfBlockWater(selection_water) - 1) * BLOCK_SIZE) - 5, BLOCK_SIZE, -waterfall_time);
			}
			else {
				ofDrawRectangle(selection_water*BLOCK_SIZE, HEIGHT - ((numOfBlockWater(selection_water) - 1) * BLOCK_SIZE) - 5, BLOCK_SIZE, -BLOCK_SIZE);
			}
		}

		//���� �� á�ٸ� �� ���¸� �ݿ��Ͽ� �ٽ� ���� �׸���
		if (waterfall_time == 0) {
			ofSetColor(ofColor::deepSkyBlue);
			for (int i = 0; i < num_of_area; i++) {
				if (field[i].state == 2) { //���� �ִ� area��� �� �Ʒ����� ���ִ� �������� �簢���� �׸���
					ofDrawRectangle(i*BLOCK_SIZE, HEIGHT - 5, BLOCK_SIZE, -BLOCK_SIZE * (field[i].num_of_water));
				}
				else if (field[i].state == 3) { //���� ���� �Ѵ� �ִ� area��� ���� ������ �������� ���� ���ִ� �������� �簢���� �׸���
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
		//game over�� �ƴҶ��� ������, ����, ���� key�� �޾Ƶ��̵��� �Ѵ�
		if (key == OF_KEY_RIGHT) { //������ Ű�� ������
			block_flag = 1; //block build phase
			water_flag = 0;
			waterfall_time = 0;
			selection_area++; //���� area�� ���������� �ű��
			if (selection_area == num_of_area) //�����ΰ���
				selection_area = 0; //ó������ �ٽ� �����Ѵ�
		}
		if (key == OF_KEY_LEFT) { //���� Ű�� ������
			block_flag = 1; //block build phase
			water_flag = 0;
			waterfall_time = 0;
			selection_area--; //���� area�� �������� �ű��
			if (selection_area < 0) //ó������ ����
				selection_area = num_of_area - 1; //������ �ٽ� �����Ѵ�
		}
		if (key == OF_KEY_UP) { //���� Ű�� ������
			if (field[selection_area].num_of_block + 1 == BLOCK_LIMIT_HEIGHT) { //���� ���Ѽ� ���� ���δٸ�
				gameover_reason = "you exceeded the limit line";
				game_over = 1;
			}
			else if (field[selection_area].state == 0 || field[selection_area].state == 1) { //���� ���� ������ ����ְų� ���� ���� �����̶�� �� �ױⰡ �����ϴ�
				block_flag = 1; //block build phase
				water_flag = 0;
				waterfall_time = 0;
				new_water_flag = 0;

				if (field[selection_area].state != 1) //ó�� �״� �����̶��
					field[selection_area].state = 1; //�ʵ� ���¸� ��(1)���� �����Ѵ�

				//���� �״´�
				field[selection_area].num_of_block++;

				//���� ���̸� 5�̻� �״´ٸ� ������ 5�� ��´�
				if (field[selection_area].num_of_block > 4) {
					score -= 5;
				}

				remain_block_num--; //�����ִ� ���� ���� ���δ�
				if (remain_block_num == -1) { //�����ִ� ���� ���� ���ٸ�
					remain_block_num = 0;
					gameover_reason = "you used all blocks";
					game_over = 1;
				}

				//���� ���� �� �ִ� ��ġ �ʱ�ȭ
				for (int i = 0; i < num_of_area; i++) {
					field[i].can_waterfall = 0;
				}

				//���� ���� �� �ִ� ��ġ ã��
				int left_block = -1;
				int i = 0;
				while (i < num_of_area) { //������ area���� Ž���Ѵ�
					if (left_block == -1) { //���� ���� ���ٸ�
						if (field[i].state == 1) { //�� area�� ���� �ִٸ�
							left_block = i; //�� ���� ���� ������ ���Ѵ�
						}
					}
					else { //���� ���� �ִٸ�
						if (numOfBlockWater(i) <= numOfBlockWater(i - 1)) { //�� area�� ���� ���� ���� �ٷ� �� ���� ������ ���ٸ�
							i++; //������ ������ ������ �����Ƿ�, ���� area�� �Ѿ�� Ž���� ����Ѵ�.
							continue;
						}
						else if (numOfBlockWater(i) != 0) { //�� area�� ���� ���� ���� �ٷ� �� ���� ������ ũ�ų� �����鼭 0�� �ƴ϶��
							if (i - left_block == 1) { //�� area�� ���� ���� ���� �ٷ� �پ��ִ� ���̶��, �� �� ���̿� ������ ���ٸ�
								left_block = i; //�� area�� ���� ���� ������ ���Ͽ�
								i++; //���� area�� �Ѿ�� Ž���� ����Ѵ�
								continue;
							}
							else if (i - left_block > 1) { //�� area�� ���� ���� �� ���̿� ������ �ִٸ�
								for (int j = left_block + 1; j < i; j++) {
									if (numOfBlockWater(j) == numOfBlockWater(i - 1) && numOfBlockWater(j) < numOfBlockWater(left_block)) {
										field[j].can_waterfall = 1; //�� ������ area���� ���� ������ �� �ִ� ������ �����Ѵ�
										new_water_flag = 1;
									}
								}
								left_block = i; //�� area�� ���� ���� ������ ���Ͽ�
								i++; //���� area�� �Ѿ�� Ž���� ����Ѵ�
								continue;
							}
						}
					}
					i++;
				}

				if (new_water_flag == 0) { //���� ���� �� �ִ� ��ġ�� ���ٸ�
					gameover_reason = "there is no space to fill water";
					game_over = 1;
				}
				else {
					block_flag = 0;
					water_flag = 1; //waterfall phase�� ��ȯ

					do { //���� ���� �� �ִ� ��ġ �� ������ ��ġ�� selection_water�� �Ҵ��Ѵ�
						selection_water = (double)rand() / RAND_MAX * num_of_area;
					} while (!field[selection_water].can_waterfall);

					if (field[selection_water].state == 0 || field[selection_water].state == 2) { //���� �ƹ��͵� �����ų� ���� �ִ� area���
						field[selection_water].state = 2; //state 2: ���� �׿�����
					}
					else if (field[selection_water].state == 1) { //���� ���� �ִ� area���
						field[selection_water].state = 3; //state 3 : ���� ���� �Բ� �׿�����
					}

					field[selection_water].num_of_water++; //���� ���� ���� �߰��Ѵ�
					score += 10; //������ 10�� ��´�
				}
			}
			else { //���� ���� �� ���� ��ġ
				gameover_reason = "you put the block where you can't put it";
				game_over = 1;
			}
		}
	}
	else { //game over �Ǵ� success��
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
