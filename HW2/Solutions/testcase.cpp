#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int maze[14][8] = { {1, 0, 1, 1, 1, 1, 1, 1},
					{1, 0, 1, 0, 0, 0, 0, 1},
					{1, 0, 0, 0, 1, 1, 0, 1},
					{1, 0, 1, 1, 1, 0, 0, 1},
					{1, 1, 1, 0, 1, 1, 0, 1},
					{1, 0, 0, 0, 0, 1, 0, 1},
					{1, 0, 1, 1, 1, 1, 0, 1},
					{1, 0, 1, 0, 0, 0, 0, 1},
					{1, 0, 1, 1, 1, 1, 0, 1},
					{1, 0, 1, 0, 0, 0, 0, 1},
					{1, 0, 1, 1, 0, 1, 0, 0},
					{1, 0, 0, 1, 1, 1, 0, 1},
					{1, 1, 0, 0, 0, 0, 0, 1},
					{1, 1, 1, 1, 1, 1, 1, 1} };

int main() {
	Mat mazeImage(140, 80, CV_8UC3);
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 8; j++)
		{
			if (maze[i][j])
				for (int k = 10 * i; k < 10 * i + 10; k++)
					for (int h = j * 10; h < j * 10 + 10; h++)
						mazeImage.ptr<Vec3b>(k)[h] = {0, 0, 0};
			else
				for (int k = 10 * i; k < 10 * i + 10; k++)
					for (int h = j * 10; h < j * 10 + 10; h++)
						mazeImage.ptr<Vec3b>(k)[h] = { 255, 255, 255 };
		}

	imshow("test", mazeImage);
	waitKey(0);
	imwrite("1113341-input.png", mazeImage);
}