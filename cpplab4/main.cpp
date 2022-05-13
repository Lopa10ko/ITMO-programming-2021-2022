#include <iostream>
#include "windows.h"
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;
enum COLOR_TYPE {
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
    COLOR_WHITE,
	COLOR_ORANGE,
};
int W_0 = 0, W_1 = 0, W_2 = 0, W_3 = 0, W_4 = 0, W_5 = 0;
int O_0 = 0, O_1 = 0, O_2 = 0, O_3 = 0, O_4 = 0, O_5 = 0;
int G_0 = 0, G_1 = 0, G_2 = 0, G_3 = 0, G_4 = 0, G_5 = 0;
int R_0 = 0, R_1 = 0, R_2 = 0, R_3 = 0, R_4 = 0, R_5 = 0;
int B_0 = 0, B_1 = 0, B_2 = 0, B_3 = 0, B_4 = 0, B_5 = 0;
int Y_0 = 0, Y_1 = 0, Y_2 = 0, Y_3 = 0, Y_4 = 0, Y_5 = 0;
char CUBESTR_0[] = "WWWWWWWWW", CUBESTR_1[] = "OOOOOOOOO", CUBESTR_2[] = "GGGGGGGGG";
char CUBESTR_3[] = "RRRRRRRRR", CUBESTR_4[] = "BBBBBBBBB", CUBESTR_5[] = "YYYYYYYYY";
void DRAW(Mat &img, int x, int y, int COLOR_TYPE) {
	Scalar sc;
	string COLOR_NAME;
	switch(COLOR_TYPE) {
		case COLOR_RED:
			sc = Scalar(0,0,255);
			COLOR_NAME = "R";
			break;
		case COLOR_BLUE:
			sc = Scalar(255,0,0);
			COLOR_NAME = "B";
			break;
		case COLOR_GREEN:
			sc = Scalar(0,255,0);
			COLOR_NAME = "G";
			break;
		case COLOR_YELLOW:
			sc = Scalar(0,255,255);
			COLOR_NAME = "Y";
			break;
		case COLOR_WHITE:
			sc = Scalar(255,255,0);
			COLOR_NAME = "W";
			break;
		case COLOR_ORANGE:
			sc = Scalar(0,150,150);
			COLOR_NAME = "O";
			break;
        default : break;
	}
	circle(img, Point(x, y), 10, sc, 3);
    putText(img, COLOR_NAME + "(" + std::to_string(x) + "," + std::to_string(y) + ")" ,Point(x, y),1,1,Scalar(255,255,255));
}
void thresholdImg(const Mat& FRAME, const Scalar& scLow, const Scalar& scHigh) {
    Mat imgHSV;
	cvtColor(FRAME, imgHSV, COLOR_BGR2HSV);
	Mat imgThresholded;
	inRange(imgHSV, scLow, scHigh, imgThresholded);
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
    imshow("test", imgThresholded);
}
void detect_stickers(Mat &FRAME, Mat DRAW_FRAME, int &iLastX, int &iLastY, int COLOR_TYPE, int h, int w, int sides_num) {
	Scalar scLow, scHigh;
	string COLOR_NAME;
    // settings
    // scLow = (LowH, LowS, LowV)
    // scHigh = (HighH, HighS, HighV)
	switch (COLOR_TYPE) {
        case COLOR_WHITE:
            scLow = Scalar(0,0,0);
            scHigh = Scalar(179,76,255);
            COLOR_NAME = "W";
            break;
        case COLOR_GREEN:
            scLow = Scalar(39,47,56);
            scHigh = Scalar(103,173,255);
            COLOR_NAME = "G";
            break;
        case COLOR_ORANGE:
            scLow = Scalar(4,50,0);
            scHigh = Scalar(27,255,255);
            COLOR_NAME = "O";
            break;
    	case COLOR_RED:
    		scLow = Scalar(126,103,0);
    		scHigh = Scalar(179,255,211);
    		COLOR_NAME = "R";
    		break;
    	case COLOR_BLUE:
    		scLow = Scalar(74,161,82);
    		scHigh = Scalar(130,255,255);
    		COLOR_NAME = "B";
			break;
    	case COLOR_YELLOW:
    		scLow = Scalar(21,35,103);
    		scHigh = Scalar(41,255,255);
    		COLOR_NAME = "Y";
			break;
        default : break;
	}
    Mat imgHSV;
	cvtColor(FRAME, imgHSV, COLOR_BGR2HSV);
	Mat imgThresholded;
	inRange(imgHSV, scLow, scHigh, imgThresholded);
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
//    imshow(COLOR_NAME, imgThresholded);
    switch (sides_num) {
        case 0:
            if (COLOR_NAME == "W" && W_0 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: W - FIRST");
                if (W_0 != 1) W_0 = 1;
            }
            else if (COLOR_NAME == "O" && O_0 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (O_0 != 1) {
                    O_0 = 1;
                    if (p1->x == 255) CUBESTR_0[0] = 'O';
                    if (p2->x == 255) CUBESTR_0[1] = 'O';
                    if (p3->x == 255) CUBESTR_0[2] = 'O';
                    if (p4->x == 255) CUBESTR_0[3] = 'O';
                    if (p6->x == 255) CUBESTR_0[5] = 'O';
                    if (p7->x == 255) CUBESTR_0[6] = 'O';
                    if (p8->x == 255) CUBESTR_0[7] = 'O';
                    if (p9->x == 255) CUBESTR_0[8] = 'O';
                }
            }
            else if (COLOR_NAME == "G" && G_0 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (G_0 != 1){
                    G_0 = 1;
                    if (p1->x == 255) CUBESTR_0[0] = 'G';
                    if (p2->x == 255) CUBESTR_0[1] = 'G';
                    if (p3->x == 255) CUBESTR_0[2] = 'G';
                    if (p4->x == 255) CUBESTR_0[3] = 'G';
                    if (p6->x == 255) CUBESTR_0[5] = 'G';
                    if (p7->x == 255) CUBESTR_0[6] = 'G';
                    if (p8->x == 255) CUBESTR_0[7] = 'G';
                    if (p9->x == 255) CUBESTR_0[8] = 'G';
                }
            }
            else if (COLOR_NAME == "R" && R_0 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (R_0 != 1) {
                    R_0 = 1;
                    if (p1->x == 255) CUBESTR_0[0] = 'R';
                    if (p2->x == 255) CUBESTR_0[1] = 'R';
                    if (p3->x == 255) CUBESTR_0[2] = 'R';
                    if (p4->x == 255) CUBESTR_0[3] = 'R';
                    if (p6->x == 255) CUBESTR_0[5] = 'R';
                    if (p7->x == 255) CUBESTR_0[6] = 'R';
                    if (p8->x == 255) CUBESTR_0[7] = 'R';
                    if (p9->x == 255) CUBESTR_0[8] = 'R';
                }
            }
            else if (COLOR_NAME == "B" && B_0 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (B_0 != 1) {
                    B_0 = 1;
                    if (p1->x == 255) CUBESTR_0[0] = 'B';
                    if (p2->x == 255) CUBESTR_0[1] = 'B';
                    if (p3->x == 255) CUBESTR_0[2] = 'B';
                    if (p4->x == 255) CUBESTR_0[3] = 'B';
                    if (p6->x == 255) CUBESTR_0[5] = 'B';
                    if (p7->x == 255) CUBESTR_0[6] = 'B';
                    if (p8->x == 255) CUBESTR_0[7] = 'B';
                    if (p9->x == 255) CUBESTR_0[8] = 'B';
                }
            }
            else if (COLOR_NAME == "Y" && Y_0 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (Y_0 != 1) {
                    Y_0 = 1;
                    if (p1->x == 255) CUBESTR_0[0] = 'Y';
                    if (p2->x == 255) CUBESTR_0[1] = 'Y';
                    if (p3->x == 255) CUBESTR_0[2] = 'Y';
                    if (p4->x == 255) CUBESTR_0[3] = 'Y';
                    if (p6->x == 255) CUBESTR_0[5] = 'Y';
                    if (p7->x == 255) CUBESTR_0[6] = 'Y';
                    if (p8->x == 255) CUBESTR_0[7] = 'Y';
                    if (p9->x == 255) CUBESTR_0[8] = 'Y';
                }
            }
            break;
        case 1:
            if (COLOR_NAME == "W" && W_1 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (W_1 != 1) {
                    W_1 = 1;
                    if (p1->x == 255) CUBESTR_1[0] = 'W';
                    if (p2->x == 255) CUBESTR_1[1] = 'W';
                    if (p3->x == 255) CUBESTR_1[2] = 'W';
                    if (p4->x == 255) CUBESTR_1[3] = 'W';
                    if (p6->x == 255) CUBESTR_1[5] = 'W';
                    if (p7->x == 255) CUBESTR_1[6] = 'W';
                    if (p8->x == 255) CUBESTR_1[7] = 'W';
                    if (p9->x == 255) CUBESTR_1[8] = 'W';
                }
            }
            else if (COLOR_NAME == "O" && O_1 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: O - SECOND");
                if (O_1 != 1) O_1 = 1;
            }
            else if (COLOR_NAME == "G" && G_1 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (G_1 != 1) {
                    G_1 = 1;
                    if (p1->x == 255) CUBESTR_1[0] = 'G';
                    if (p2->x == 255) CUBESTR_1[1] = 'G';
                    if (p3->x == 255) CUBESTR_1[2] = 'G';
                    if (p4->x == 255) CUBESTR_1[3] = 'G';
                    if (p6->x == 255) CUBESTR_1[5] = 'G';
                    if (p7->x == 255) CUBESTR_1[6] = 'G';
                    if (p8->x == 255) CUBESTR_1[7] = 'G';
                    if (p9->x == 255) CUBESTR_1[8] = 'G';
                }
            }
            else if (COLOR_NAME == "R" && R_1 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (R_1 != 1) {
                    R_1 = 1;
                    if (p1->x == 255) CUBESTR_1[0] = 'R';
                    if (p2->x == 255) CUBESTR_1[1] = 'R';
                    if (p3->x == 255) CUBESTR_1[2] = 'R';
                    if (p4->x == 255) CUBESTR_1[3] = 'R';
                    if (p6->x == 255) CUBESTR_1[5] = 'R';
                    if (p7->x == 255) CUBESTR_1[6] = 'R';
                    if (p8->x == 255) CUBESTR_1[7] = 'R';
                    if (p9->x == 255) CUBESTR_1[8] = 'R';
                }
            }
            else if (COLOR_NAME == "B" && B_1 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (B_1 != 1) {
                    B_1 = 1;
                    if (p1->x == 255) CUBESTR_1[0] = 'B';
                    if (p2->x == 255) CUBESTR_1[1] = 'B';
                    if (p3->x == 255) CUBESTR_1[2] = 'B';
                    if (p4->x == 255) CUBESTR_1[3] = 'B';
                    if (p6->x == 255) CUBESTR_1[5] = 'B';
                    if (p7->x == 255) CUBESTR_1[6] = 'B';
                    if (p8->x == 255) CUBESTR_1[7] = 'B';
                    if (p9->x == 255) CUBESTR_1[8] = 'B';
                }
            }
            else if (COLOR_NAME == "Y" && Y_1 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (Y_1 != 1) {
                    Y_1 = 1;
                    if (p1->x == 255) CUBESTR_1[0] = 'Y';
                    if (p2->x == 255) CUBESTR_1[1] = 'Y';
                    if (p3->x == 255) CUBESTR_1[2] = 'Y';
                    if (p4->x == 255) CUBESTR_1[3] = 'Y';
                    if (p6->x == 255) CUBESTR_1[5] = 'Y';
                    if (p7->x == 255) CUBESTR_1[6] = 'Y';
                    if (p8->x == 255) CUBESTR_1[7] = 'Y';
                    if (p9->x == 255) CUBESTR_1[8] = 'Y';
                }
            }
            break;
        case 2:
            if (COLOR_NAME == "W" && W_2 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (W_2 != 1) {
                    W_2 = 1;
                    if (p1->x == 255) CUBESTR_2[0] = 'W';
                    if (p2->x == 255) CUBESTR_2[1] = 'W';
                    if (p3->x == 255) CUBESTR_2[2] = 'W';
                    if (p4->x == 255) CUBESTR_2[3] = 'W';
                    if (p6->x == 255) CUBESTR_2[5] = 'W';
                    if (p7->x == 255) CUBESTR_2[6] = 'W';
                    if (p8->x == 255) CUBESTR_2[7] = 'W';
                    if (p9->x == 255) CUBESTR_2[8] = 'W';
                }
            }
            else if (COLOR_NAME == "O" && O_2 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (O_2 != 1) {
                    O_2 = 1;
                    if (p1->x == 255) CUBESTR_2[0] = 'O';
                    if (p2->x == 255) CUBESTR_2[1] = 'O';
                    if (p3->x == 255) CUBESTR_2[2] = 'O';
                    if (p4->x == 255) CUBESTR_2[3] = 'O';
                    if (p6->x == 255) CUBESTR_2[5] = 'O';
                    if (p7->x == 255) CUBESTR_2[6] = 'O';
                    if (p8->x == 255) CUBESTR_2[7] = 'O';
                    if (p9->x == 255) CUBESTR_2[8] = 'O';
                }
            }
            else if (COLOR_NAME == "G" && G_2 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: G - THIRD");
                if (G_2 != 1) G_2 = 1;
            }
            else if (COLOR_NAME == "R" && R_2 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (R_2 != 1) {
                    R_2 = 1;
                    if (p1->x == 255) CUBESTR_2[0] = 'R';
                    if (p2->x == 255) CUBESTR_2[1] = 'R';
                    if (p3->x == 255) CUBESTR_2[2] = 'R';
                    if (p4->x == 255) CUBESTR_2[3] = 'R';
                    if (p6->x == 255) CUBESTR_2[5] = 'R';
                    if (p7->x == 255) CUBESTR_2[6] = 'R';
                    if (p8->x == 255) CUBESTR_2[7] = 'R';
                    if (p9->x == 255) CUBESTR_2[8] = 'R';
                }
            }
            else if (COLOR_NAME == "B" && B_2 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (B_2 != 1) {
                    B_2 = 1;
                    if (p1->x == 255) CUBESTR_2[0] = 'B';
                    if (p2->x == 255) CUBESTR_2[1] = 'B';
                    if (p3->x == 255) CUBESTR_2[2] = 'B';
                    if (p4->x == 255) CUBESTR_2[3] = 'B';
                    if (p6->x == 255) CUBESTR_2[5] = 'B';
                    if (p7->x == 255) CUBESTR_2[6] = 'B';
                    if (p8->x == 255) CUBESTR_2[7] = 'B';
                    if (p9->x == 255) CUBESTR_2[8] = 'B';
                }
            }
            else if (COLOR_NAME == "Y" && Y_2 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (Y_2 != 1) {
                    Y_2 = 1;
                    if (p1->x == 255) CUBESTR_2[0] = 'Y';
                    if (p2->x == 255) CUBESTR_2[1] = 'Y';
                    if (p3->x == 255) CUBESTR_2[2] = 'Y';
                    if (p4->x == 255) CUBESTR_2[3] = 'Y';
                    if (p6->x == 255) CUBESTR_2[5] = 'Y';
                    if (p7->x == 255) CUBESTR_2[6] = 'Y';
                    if (p8->x == 255) CUBESTR_2[7] = 'Y';
                    if (p9->x == 255) CUBESTR_2[8] = 'Y';
                }
            }
            break;
        case 3:
            if (COLOR_NAME == "W" && W_3 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (W_3 != 1) {
                    W_3 = 1;
                    if (p1->x == 255) CUBESTR_3[0] = 'W';
                    if (p2->x == 255) CUBESTR_3[1] = 'W';
                    if (p3->x == 255) CUBESTR_3[2] = 'W';
                    if (p4->x == 255) CUBESTR_3[3] = 'W';
                    if (p6->x == 255) CUBESTR_3[5] = 'W';
                    if (p7->x == 255) CUBESTR_3[6] = 'W';
                    if (p8->x == 255) CUBESTR_3[7] = 'W';
                    if (p9->x == 255) CUBESTR_3[8] = 'W';
                }
            }
            else if (COLOR_NAME == "O" && O_3 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (O_3 != 1) {
                    O_3 = 1;
                    if (p1->x == 255) CUBESTR_3[0] = 'O';
                    if (p2->x == 255) CUBESTR_3[1] = 'O';
                    if (p3->x == 255) CUBESTR_3[2] = 'O';
                    if (p4->x == 255) CUBESTR_3[3] = 'O';
                    if (p6->x == 255) CUBESTR_3[5] = 'O';
                    if (p7->x == 255) CUBESTR_3[6] = 'O';
                    if (p8->x == 255) CUBESTR_3[7] = 'O';
                    if (p9->x == 255) CUBESTR_3[8] = 'O';
                }
            }
            else if (COLOR_NAME == "G" && G_3 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (G_3 != 1) {
                    G_3 = 1;
                    if (p1->x == 255) CUBESTR_3[0] = 'G';
                    if (p2->x == 255) CUBESTR_3[1] = 'G';
                    if (p3->x == 255) CUBESTR_3[2] = 'G';
                    if (p4->x == 255) CUBESTR_3[3] = 'G';
                    if (p6->x == 255) CUBESTR_3[5] = 'G';
                    if (p7->x == 255) CUBESTR_3[6] = 'G';
                    if (p8->x == 255) CUBESTR_3[7] = 'G';
                    if (p9->x == 255) CUBESTR_3[8] = 'G';
                }
            }
            else if (COLOR_NAME == "R" && R_3 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: R - FOURTH");
                if (R_3 != 1) R_3 = 1;
            }
            else if (COLOR_NAME == "B" && B_3 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (B_3 != 1){
                    B_3 = 1;
                    if (p1->x == 255) CUBESTR_3[0] = 'B';
                    if (p2->x == 255) CUBESTR_3[1] = 'B';
                    if (p3->x == 255) CUBESTR_3[2] = 'B';
                    if (p4->x == 255) CUBESTR_3[3] = 'B';
                    if (p6->x == 255) CUBESTR_3[5] = 'B';
                    if (p7->x == 255) CUBESTR_3[6] = 'B';
                    if (p8->x == 255) CUBESTR_3[7] = 'B';
                    if (p9->x == 255) CUBESTR_3[8] = 'B';
                }
            }
            else if (COLOR_NAME == "Y" && Y_3 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (Y_3 != 1){
                    Y_3 = 1;
                    if (p1->x == 255) CUBESTR_3[0] = 'Y';
                    if (p2->x == 255) CUBESTR_3[1] = 'Y';
                    if (p3->x == 255) CUBESTR_3[2] = 'Y';
                    if (p4->x == 255) CUBESTR_3[3] = 'Y';
                    if (p6->x == 255) CUBESTR_3[5] = 'Y';
                    if (p7->x == 255) CUBESTR_3[6] = 'Y';
                    if (p8->x == 255) CUBESTR_3[7] = 'Y';
                    if (p9->x == 255) CUBESTR_3[8] = 'Y';
                }
            }
            break;
        case 4:
            if (COLOR_NAME == "W" && W_4 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (W_4 != 1) {
                    W_4 = 1;
                    if (p1->x == 255) CUBESTR_4[0] = 'W';
                    if (p2->x == 255) CUBESTR_4[1] = 'W';
                    if (p3->x == 255) CUBESTR_4[2] = 'W';
                    if (p4->x == 255) CUBESTR_4[3] = 'W';
                    if (p6->x == 255) CUBESTR_4[5] = 'W';
                    if (p7->x == 255) CUBESTR_4[6] = 'W';
                    if (p8->x == 255) CUBESTR_4[7] = 'W';
                    if (p9->x == 255) CUBESTR_4[8] = 'W';
                }
            }
            else if (COLOR_NAME == "O" && O_4 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (O_4 != 1) {
                    O_4 = 1;
                    if (p1->x == 255) CUBESTR_4[0] = 'O';
                    if (p2->x == 255) CUBESTR_4[1] = 'O';
                    if (p3->x == 255) CUBESTR_4[2] = 'O';
                    if (p4->x == 255) CUBESTR_4[3] = 'O';
                    if (p6->x == 255) CUBESTR_4[5] = 'O';
                    if (p7->x == 255) CUBESTR_4[6] = 'O';
                    if (p8->x == 255) CUBESTR_4[7] = 'O';
                    if (p9->x == 255) CUBESTR_4[8] = 'O';
                }
            }
            else if (COLOR_NAME == "G" && G_4 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (G_4 != 1) {
                    G_4 = 1;
                    if (p1->x == 255) CUBESTR_4[0] = 'G';
                    if (p2->x == 255) CUBESTR_4[1] = 'G';
                    if (p3->x == 255) CUBESTR_4[2] = 'G';
                    if (p4->x == 255) CUBESTR_4[3] = 'G';
                    if (p6->x == 255) CUBESTR_4[5] = 'G';
                    if (p7->x == 255) CUBESTR_4[6] = 'G';
                    if (p8->x == 255) CUBESTR_4[7] = 'G';
                    if (p9->x == 255) CUBESTR_4[8] = 'G';
                }
            }
            else if (COLOR_NAME == "R" && R_4 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (R_4 != 1){
                    R_4 = 1;
                    if (p1->x == 255) CUBESTR_4[0] = 'R';
                    if (p2->x == 255) CUBESTR_4[1] = 'R';
                    if (p3->x == 255) CUBESTR_4[2] = 'R';
                    if (p4->x == 255) CUBESTR_4[3] = 'R';
                    if (p6->x == 255) CUBESTR_4[5] = 'R';
                    if (p7->x == 255) CUBESTR_4[6] = 'R';
                    if (p8->x == 255) CUBESTR_4[7] = 'R';
                    if (p9->x == 255) CUBESTR_4[8] = 'R';
                }
            }
            else if (COLOR_NAME == "B" && B_4 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: B - FIFTH");
                if (B_4 != 1) B_4 = 1;
            }
            else if (COLOR_NAME == "Y" && Y_4 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (Y_4 != 1){
                    Y_4 = 1;
                    if (p1->x == 255) CUBESTR_4[0] = 'Y';
                    if (p2->x == 255) CUBESTR_4[1] = 'Y';
                    if (p3->x == 255) CUBESTR_4[2] = 'Y';
                    if (p4->x == 255) CUBESTR_4[3] = 'Y';
                    if (p6->x == 255) CUBESTR_4[5] = 'Y';
                    if (p7->x == 255) CUBESTR_4[6] = 'Y';
                    if (p8->x == 255) CUBESTR_4[7] = 'Y';
                    if (p9->x == 255) CUBESTR_4[8] = 'Y';
                }
            }
            break;
        case 5:
            if (COLOR_NAME == "W" && W_5 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (W_5 != 1) {
                    W_5 = 1;
                    if (p1->x == 255) CUBESTR_5[0] = 'W';
                    if (p2->x == 255) CUBESTR_5[1] = 'W';
                    if (p3->x == 255) CUBESTR_5[2] = 'W';
                    if (p4->x == 255) CUBESTR_5[3] = 'W';
                    if (p6->x == 255) CUBESTR_5[5] = 'W';
                    if (p7->x == 255) CUBESTR_5[6] = 'W';
                    if (p8->x == 255) CUBESTR_5[7] = 'W';
                    if (p9->x == 255) CUBESTR_5[8] = 'W';
                }
            }
            else if (COLOR_NAME == "O" && O_5 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (O_5 != 1) {
                    O_5 = 1;
                    if (p1->x == 255) CUBESTR_5[0] = 'O';
                    if (p2->x == 255) CUBESTR_5[1] = 'O';
                    if (p3->x == 255) CUBESTR_5[2] = 'O';
                    if (p4->x == 255) CUBESTR_5[3] = 'O';
                    if (p6->x == 255) CUBESTR_5[5] = 'O';
                    if (p7->x == 255) CUBESTR_5[6] = 'O';
                    if (p8->x == 255) CUBESTR_5[7] = 'O';
                    if (p9->x == 255) CUBESTR_5[8] = 'O';
                }
            }
            else if (COLOR_NAME == "G" && G_5 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (G_5 != 1) {
                    G_5 = 1;
                    if (p1->x == 255) CUBESTR_5[0] = 'G';
                    if (p2->x == 255) CUBESTR_5[1] = 'G';
                    if (p3->x == 255) CUBESTR_5[2] = 'G';
                    if (p4->x == 255) CUBESTR_5[3] = 'G';
                    if (p6->x == 255) CUBESTR_5[5] = 'G';
                    if (p7->x == 255) CUBESTR_5[6] = 'G';
                    if (p8->x == 255) CUBESTR_5[7] = 'G';
                    if (p9->x == 255) CUBESTR_5[8] = 'G';
                }
            }
            else if (COLOR_NAME == "R" && R_5 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (R_5 != 1){
                    R_5 = 1;
                    if (p1->x == 255) CUBESTR_5[0] = 'R';
                    if (p2->x == 255) CUBESTR_5[1] = 'R';
                    if (p3->x == 255) CUBESTR_5[2] = 'R';
                    if (p4->x == 255) CUBESTR_5[3] = 'R';
                    if (p6->x == 255) CUBESTR_5[5] = 'R';
                    if (p7->x == 255) CUBESTR_5[6] = 'R';
                    if (p8->x == 255) CUBESTR_5[7] = 'R';
                    if (p9->x == 255) CUBESTR_5[8] = 'R';
                }
            }
            else if (COLOR_NAME == "B" && B_5 != 1) {
                auto* p7 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w - 80);
                auto* p8 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w);
                auto* p9 = imgThresholded.ptr<Point3_<uchar>>(h + 80, w + 80);
                auto* p4 = imgThresholded.ptr<Point3_<uchar>>(h, w - 80);
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                auto* p6 = imgThresholded.ptr<Point3_<uchar>>(h, w + 80);
                auto* p1 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w - 80);
                auto* p2 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w);
                auto* p3 = imgThresholded.ptr<Point3_<uchar>>(h - 80, w + 80);
                if (B_5 != 1){
                    B_5 = 1;
                    if (p1->x == 255) CUBESTR_5[0] = 'B';
                    if (p2->x == 255) CUBESTR_5[1] = 'B';
                    if (p3->x == 255) CUBESTR_5[2] = 'B';
                    if (p4->x == 255) CUBESTR_5[3] = 'B';
                    if (p6->x == 255) CUBESTR_5[5] = 'B';
                    if (p7->x == 255) CUBESTR_5[6] = 'B';
                    if (p8->x == 255) CUBESTR_5[7] = 'B';
                    if (p9->x == 255) CUBESTR_5[8] = 'B';
                }
            }
            else if (COLOR_NAME == "Y" && Y_5 != 1) {
                auto* p5 = imgThresholded.ptr<Point3_<uchar>>(h, w);
                if (!p5->x) throw std::invalid_argument("scan is incorrect: Y - SIXTH");
                if (Y_5 != 1) Y_5 = 1;
            }
            break;
        default: break;
    }
	Moments frame_moments = moments(imgThresholded);
    double fM01 = frame_moments.m01, fM10 = frame_moments.m10, fArea = frame_moments.m00;
	if (fArea > 10000) {
		int posX = (int)(fM10 / fArea), posY = (int)(fM01 / fArea);
		if (iLastX >= 0 && iLastY >= 0 && posX >= w - 130 && posY >= h - 130 && posX <= w + 130 && posY <= h + 130) {
//            DRAW(DRAW_FRAME, posX, posY, COLOR_TYPE);
        }
		iLastX = posX;
		iLastY = posY;
	}
	FRAME += DRAW_FRAME;
}

int main( int argc, char** argv ) {
	VideoCapture VC(0);
	if (!VC.isOpened()) throw std::invalid_argument("no cam source");
	namedWindow("Control", WINDOW_NORMAL);
    int iLowH = 0, iHighH = 179, iLowS = 0, iHighS = 255, iLowV = 0, iHighV = 255;
	createTrackbar("LowH", "Control", &iLowH, 179);
	createTrackbar("HighH", "Control", &iHighH, 179);
	createTrackbar("LowS", "Control", &iLowS, 255);
	createTrackbar("HighS", "Control", &iHighS, 255);
	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);
    iLowH = getTrackbarPos("LowH","Control");
    iHighH = getTrackbarPos("HighH","Control");
    iLowS = getTrackbarPos("LowS","Control");
    iHighS = getTrackbarPos("HighS","Control");
    iLowV = getTrackbarPos("LowV","Control");
    iHighV = getTrackbarPos("HighV","Control");
	int iLastX = -1, iLastY = -1;
	Mat temp_image;
    VC.read(temp_image);
    cv::Size size_image = temp_image.size();
    VideoWriter video_src("Video.avi", cv::VideoWriter::fourcc('D','I','V','3'), 20, size_image, true);
    if (!video_src.isOpened()) throw std::invalid_argument("codec failed");
    int sides_num = -1;
    while (true) {
    	Mat np_ones = Mat::zeros(temp_image.size(), CV_8UC3);
    	Mat frame;
        if (!VC.read(frame)) throw std::invalid_argument("no cam source");
        int w = size_image.width / 2, h = size_image.height / 2;
        thresholdImg(frame, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV));
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_WHITE, h, w, sides_num);
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_ORANGE, h, w, sides_num);
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_GREEN, h, w, sides_num);
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_RED, h, w, sides_num);
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_BLUE, h, w, sides_num);
        detect_stickers(frame, np_ones, iLastX, iLastY, COLOR_YELLOW, h, w, sides_num);
        rectangle(frame, Point(w - 40,h - 120), Point(w + 120,h + 120), Scalar(0,0,0), 2);
        rectangle(frame, Point(w - 120,h - 120), Point(w + 40,h + 120), Scalar(0,0,0), 2);
        rectangle(frame, Point(w - 120,h - 40), Point(w + 120,h + 40), Scalar(0,0,0), 2);
        imshow("Video", frame);
        video_src.write(frame);
		if (waitKey(30) >= 0) {
            sides_num++;
            if (sides_num == 6) {
                std::cout << CUBESTR_0 << std::endl;
                std::cout << CUBESTR_1 << std::endl;
                std::cout << CUBESTR_2 << std::endl;
                std::cout << CUBESTR_3 << std::endl;
                std::cout << CUBESTR_4 << std::endl;
                std::cout << CUBESTR_5 << std::endl;
                break;
            }
        };
	}
    return 0;
}