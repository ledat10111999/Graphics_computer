
#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"
#include <stdlib.h>
#include <iostream>
#include <sys/timeb.h>
#include <sys/utime.h>
#include <string>

//Game Speed
int FPS = 40; // khung hình
int SPEED = 0; // tốc độ

//Game Track
int start = 0; // 
int gv = 0; // game over bằng 1 sẽ dừng game
int level = 0; // level người chơi level càng cao tốc độ càng nhanh

//Game Score
int score = 0;

//Form move track
int roadDivTop = 0; // OY vạch kẻ giữa thứ nhất
int roadDivMdl = 0; // OY vạch kẻ giữ thứ 2
int roadDivBtm = 0; // OY vạch kẻ giữa thứ 3

//For Card Left / RIGHT
int lrIndex = 0; // Ox player

//Bot xe
int car1 = 0; //Bot 1 OY
int lrIndex1 = 0; //OX

int lrIndex2 = 25; //Bot 2 OX
int car2 = 70; //bot 2 OY



//For Display TEXT
const int font1 = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
const int font3 = (int)GLUT_BITMAP_8_BY_13;


//hàm viết font chữ trên Windown

void renderBitmapString(float x, float y, void* font, const char* string) {
	const char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
//lấy milli giây
int getMilliSecondCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}
 
void sleep(int sleepTime) {
	int count = 0;
	int beginSleep = getMilliSecondCount();
	while (getMilliSecondCount() - beginSleep < sleepTime) {
		count++;
	}
}


void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0); // clear black
}

void startGame() {
	//road
	glColor3f(0.414, 0.412, 0.412); // màu xám
	glBegin(GL_POLYGON);
	glVertex2f(20, 0);
	glVertex2i(20, 100);
	glVertex2i(80, 100);
	glVertex2i(80, 0);
	glEnd();
	//Road Left Border
	glColor3f(1.000, 1.000, 1.000); // màu trắng
	glBegin(GL_POLYGON);
	glVertex2f(20, 0);
	glVertex2f(20, 100);
	glVertex2f(23, 100);
	glVertex2f(23, 0);
	glEnd();
	//Road Right Border
	glColor3f(1.000, 1.000, 1.000);
	glBegin(GL_POLYGON);
	glVertex2f(77, 0);
	glVertex2f(77, 100);
	glVertex2f(80, 100);
	glVertex2f(80, 0);
	glEnd();

	//Road Middel Border
	//TOP
	glColor3f(1.000, 1.000, 0.000); // màu vàng
	glBegin(GL_POLYGON);
	glVertex2f(48, roadDivTop + 80);
	glVertex2f(48, roadDivTop + 100);
	glVertex2f(52, roadDivTop + 100);
	glVertex2f(52, roadDivTop + 80);
	glEnd();

	roadDivTop--;
	if (roadDivTop < -100) { // nếu vạch kẻ giữa đi quá màn hình set lại vạch kẻ
		roadDivTop = 20;
		score++;
	}
	//Midle
	glColor3f(1.000, 1.000, 1.000); // màu trắng
	glBegin(GL_POLYGON);
	glVertex2f(48, roadDivMdl + 40);
	glVertex2f(48, roadDivMdl + 60);
	glVertex2f(52, roadDivMdl + 60);
	glVertex2f(52, roadDivMdl + 40);
	glEnd();
	roadDivMdl--; // nếu vạch kẻ ở giữa nhỏ hơn kích thước màn hình thì set lại vị trí
	if (roadDivMdl < -60) {
		roadDivMdl = 60;
		score++;
	}
	//Bottom
	glColor3f(1.000, 1.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(48, roadDivBtm + 0);
	glVertex2f(48, roadDivBtm + 20);
	glVertex2f(52, roadDivBtm + 20);
	glVertex2f(52, roadDivBtm + 0);
	glEnd();
	roadDivBtm--;
	if (roadDivBtm < -20) // nếu vạch kẻ ở dưới nhỏ hơn khích thước màn hình thì set lại vị trí
	{
		roadDivBtm = 100;
		score++;
	}
	//Print Score
	char buffer[50];
	sprintf_s(buffer, "SCORE: %d", score);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95, (void*)font3, buffer);
	//Speed Print
	char buffer1[50];
	sprintf_s(buffer1, "SPEED:%dKm/h", SPEED + 40);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95 - 2, (void*)font3, buffer1);
	//level Print
	if (score % 50 == 0) {
		int last = score / 50;
		if (last != level) {
			level = score / 50;
			FPS = FPS - 2;

		}
	}
	char level_buffer[50];
	sprintf_s(level_buffer, "LEVEL: %d", level);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95 - 4, (void*)font3, level_buffer);
	//MAIN car
	  //Bánh xe trước
	//lrIndex là số khai báo trước, ban đầu là 0
	glColor3f(0.000, 0.000, 0.000); //	màu đen
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex + 26 - 2, 5);
	glVertex2f(lrIndex + 26 - 2, 7);
	glVertex2f(lrIndex + 30 + 2, 7);
	glVertex2f(lrIndex + 30 + 2, 5);
	glEnd();

	//Bánh xe sau
	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex + 26 - 2, 1);
	glVertex2f(lrIndex + 26 - 2, 3);
	glVertex2f(lrIndex + 30 + 2, 3);
	glVertex2f(lrIndex + 30 + 2, 1);
	glEnd();
	//Thân xe
	glColor3f(0.678, 1.000, 0.184);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex + 26, 1);
	glVertex2f(lrIndex + 26, 8);
	glColor3f(0.000, 0.545, 0.545);
	glVertex2f(lrIndex + 28, 10);
	glVertex2f(lrIndex + 30, 8);
	glVertex2f(lrIndex + 30, 1);
	glEnd();

	//Bot 1
	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 4);
	glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 6);
	glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 6);
	glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 4);
	glEnd();

	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex1 + 26 - 2, car1 + 100);
	glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 2);
	glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 2);
	glVertex2f(lrIndex1 + 30 + 2, car1 + 100);
	glEnd();

	glColor3f(1.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex1 + 26, car1 + 100);
	glVertex2f(lrIndex1 + 26, car1 + 100 - 7);
	glVertex2f(lrIndex1 + 28, car1 + 100 - 9);
	glVertex2f(lrIndex1 + 30, car1 + 100 - 7);
	glVertex2f(lrIndex1 + 30, car1 + 100);
	glEnd();
	
	car1--; // cho bot Chạy về phía trước theo trục OY
	if (car1 < -100) // nếu bot car 1 nhỏ hơn khích thước màn hình thì set về vị trí ban đầu
	{
		int botCarPosX[5] = { 0,12,16,18,20 }; // mảng lưu trữ các vị trí OX mới của bot 1
		car1 = 0;
		lrIndex1 = botCarPosX[rand() % 5]; // random vị trí mới của bot
	}
	//check kill bot car1 
	std::cout << car1 <<"\n";
	if ((abs(lrIndex - lrIndex1) < 8) && (car1 + 100 < 10)) { // nếu OX player - OX bot1 < 8px và OY bot1 + OY player thì thua
		start = 0; // trả về trạng thái xuất phát
		gv = 1; // hiện bảng game over

	}
	//BOT car 2
	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 4);
	glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 6);
	glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 6);
	glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 4);
	glEnd();
	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex2 + 26 - 2, car2 + 100);
	glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 2);
	glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 2);
	glVertex2f(lrIndex2 + 30 + 2, car2 + 100);
	glEnd();
	glColor3f(0.294, 0.000, 0.510);
	glBegin(GL_POLYGON);
	glVertex2f(lrIndex2 + 26, car2 + 100);
	glVertex2f(lrIndex2 + 26, car2 + 100 - 7);
	glVertex2f(lrIndex2 + 28, car2 + 100 - 9);
	glVertex2f(lrIndex2 + 30, car2 + 100 - 7);
	glVertex2f(lrIndex2 + 30, car2 + 100);
	glEnd();
	car2--;
	if (car2 < -100) {
		int botCarPosX[5] = { 25,32,36,40,45 }; // mảng lưu trữ các vị trí OX mới của bot 2
		car2 = 0;
		lrIndex2 = botCarPosX[rand() % 5]; // random vị trí mới của bot
	}
	//KIll check car2
	if ((abs(lrIndex - lrIndex2) < 8) && (car2 + 100 < 10)) {
		start = 0;
		gv = 1;
	}

}
void update() {
	int beginFrame = getMilliSecondCount();
	glutPostRedisplay(); // thông báo cho chương trình vẽ lại
	int timeDriff = getMilliSecondCount() - beginFrame;
	if (timeDriff < FPS) // 50 khung hình / giây
	{
		sleep(FPS -(SPEED/4)  - timeDriff);
	}

}
void firstDesign() {
	//Road backgrond
	//Road Backgound
	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(0, 55);
	glVertex2f(100, 55);
	glColor3f(0.604, 0.804, 0.196);
	glVertex2f(100, 50 - 50);
	glVertex2f(0, 50 - 50);
	glEnd();

	//Road Design In Front Page
	glColor3f(00, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(32 - 2 + 21, 55);
	glVertex2f(32 + 58, 50 - 50);
	glVertex2f(32 - 22, 50 - 50);
	glEnd();

	//Road Midle
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(32 - 2 + 21, 55);
	glVertex2f(50 + 2, 50 - 50);
	glVertex2f(50 - 2, 50 - 50);
	glEnd();

	//Road Sky
	glColor3f(0.000, 0.749, 1.000);
	glBegin(GL_POLYGON);
	glVertex2f(100, 100);
	glVertex2f(0, 100);
	glColor3f(0.686, 0.933, 0.933);
	glVertex2f(0, 55);
	glVertex2f(100, 55);
	glEnd();

	//Hill 1
	glColor3f(0.235, 0.702, 0.443);
	glBegin(GL_TRIANGLES);
	glVertex2f(20, 55 + 10);
	glVertex2f(20 + 7, 55);
	glVertex2f(0, 55);
	glEnd();

	//Hill 2
	glColor3f(0.000, 0.502, 0.000);
	glBegin(GL_TRIANGLES);
	glVertex2f(20 + 15, 55 + 12);
	glVertex2f(20 + 20 + 10, 55);
	glVertex2f(0 + 10, 55);
	glEnd();

	//Hill 4
	glColor3f(0.235, 0.702, 0.443);
	glBegin(GL_TRIANGLES);
	glVertex2f(87, 55 + 10);
	glVertex2f(100, 55);
	glVertex2f(60, 55);
	glEnd();

	//Hill 3
	glColor3f(0.000, 0.502, 0.000);
	glBegin(GL_TRIANGLES);
	glVertex2f(70, 70);
	glVertex2f(90, 55);
	glVertex2f(50, 55);
	glEnd();


	//Tree Left
		//Bottom
	glColor3f(0.871, 0.722, 0.529);
	glBegin(GL_TRIANGLES);
	glVertex2f(11, 55);
	glVertex2f(12, 55 - 10);
	glVertex2f(10, 55 - 10);
	glEnd();
	//Up
	glColor3f(0.133, 0.545, 0.133);
	glBegin(GL_TRIANGLES);
	glVertex2f(11, 55 + 3);
	glVertex2f(12 + 3, 55 - 3);
	glVertex2f(10 - 3, 55 - 3);
	glEnd();

	//Menu tùy chọn
	//main menu
	glColor3f(0.098, 0.098, 0.439);
	glBegin(GL_POLYGON);
	glVertex2f(32 - 4, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 - 15 + 10);
	glVertex2f(32 - 4, 50 - 15 + 10);
	glEnd();
	//border trên
	glColor3f(00, 0, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(32 - 4, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 + 4 + 10);
	glVertex2f(32 - 4, 50 + 4 + 10);
	glEnd();
	//boder trái
	glBegin(GL_POLYGON);
	glVertex2f(32 + 45, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 + 5 + 10);
	glVertex2f(32 + 46, 50 - 15 + 10);
	glVertex2f(32 + 45, 50 - 15 + 10);
	glEnd();
	//border phải
	glBegin(GL_POLYGON);
	glVertex2f(32 - 4, 50 - 14 + 10);
	glVertex2f(32 + 46, 50 - 14 + 10);
	glVertex2f(32 + 46, 50 - 15 + 10);
	glVertex2f(32 - 4, 50 - 15 + 10);
	glEnd();
	//border dưới
	glBegin(GL_POLYGON);
	glVertex2f(32 - 4, 50 + 5 + 10);
	glVertex2f(32 - 5, 50 + 5 + 10);
	glVertex2f(32 - 5, 50 - 15 + 10);
	glVertex2f(32 - 4, 50 - 15 + 10);
	glEnd();

	//Thông tin + luật chơi
	if (gv == 1) { // trong trạng thái game over
		glColor3f(1.000, 0.000, 0.000);
		renderBitmapString(35, 60 + 10, (void*)font1, "GAME OVER");
		glColor3f(1.000, 0.000, 0.000);
		char buffer2[50];
		sprintf_s(buffer2, "Your Score is : %d", score);
		renderBitmapString(33, 60 - 4 + 10, (void*)font1, buffer2);
	}
	glColor3f(1.000, 1.000, 0.000);
	renderBitmapString(30, 80, (void*)font1, " 2D Car Race ");

	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(30, 50 + 10, (void*)font2, "Press SPACE to START");
	renderBitmapString(30, 50 - 3 + 10, (void*)font2, "Press ESC to Exit");

	glColor3f(1.000, 1.000, 1.000);
	renderBitmapString(30, 50 - 6 + 10, (void*)font3, "Press UP to increase Speed");
	renderBitmapString(30, 50 - 8 + 10, (void*)font3, "Press DOWN to decrease Speed");
	renderBitmapString(30, 50 - 10 + 10, (void*)font3, "Press RIGHT to turn Right");
	renderBitmapString(30, 50 - 12 + 10, (void*)font3, "Press LEFT to turn Left");

}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); // clear all in buffer
	if (start == 1) {
		glClearColor(0.000, 0.392, 0.000, 1);
		startGame();
	}
		
	else {
			glClearColor(0.000, 0.392, 0.000, 1);
			firstDesign();
	}
	
	glFlush(); // Đẩy ra buffer
	glutSwapBuffers();// hoán đổi vị trí 2 buffer optional

}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -1, 1); // góc nhìn
	glMatrixMode(GL_MODELVIEW);
}
void processKeys(unsigned char key, int x, int y) {

	switch (key)
	{
	case ' ': // chọn dấu cách sẽ khởi khởi động game 
		if (start == 0) {// phòng khi người chơi bấm space khi đang chơi game
			start = 1;
			gv = 0;
			FPS = 40;
			roadDivTop = 0;
			roadDivMdl = 0;
			roadDivBtm = 0;
			lrIndex = 0;
			car1 = 0;
			lrIndex1 = 0;
			car2 = +35;
			lrIndex2 = 0;
			score = 0;
			level = 0;
			SPEED = 0;
		}
		break;

	case 27: // ESC đóng màn hinh
		exit(0);
		break;
	default:
		break;
	}
}
//sự kiện cho bàn phím di chuyển
void spec_key(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_DOWN://bắt sự kiện cho phím xuống
			SPEED -= 2; // giảm 2 khung hình trên giây
			if (SPEED <=0) { SPEED = 1; }// Nếu speed nhỏ hơn 0 thì không giảm nữa
		break;
	case GLUT_KEY_UP:// bắt sự kiện cho phím lên
		SPEED += 2; // tăng thêm 2 khung hình / giây
		if (SPEED >=120) SPEED = 120; // nếu SPEED vượt quá 120km/h thì dừng
		break;
	case GLUT_KEY_RIGHT: // bắt sự kiện cho phím phải
		if (lrIndex <= 44) {
			lrIndex = lrIndex + (FPS / 10); // tăng OX xe player 
			if (lrIndex > 44) {
				lrIndex = 45;
			}
		}
		break;
	case GLUT_KEY_LEFT:
		if (lrIndex >= 0) {
			lrIndex = lrIndex - (FPS / 10);
			if (lrIndex < 0) {
				lrIndex = -1;
			}
		}
		break;

	default:
		break;
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // Khởi tọa glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);// Khởi tạo chế độ single buffer, hệ màu RGBA
	glutInitWindowSize(500, 650);
	glutInitWindowPosition(200, 20);
	glutCreateWindow("Car Game");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(spec_key);
	glutKeyboardFunc(processKeys);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;
}
