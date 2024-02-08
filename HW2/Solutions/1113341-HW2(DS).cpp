#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stack>
#include <iomanip>
using namespace std;
using namespace cv;

int moving[4][4] = { {0, -1}, {+1, 0}, {0, +1}, {-1, 0} };

struct valueInStack {
	int row;
	int col;
	int dir;
};

/*to determine which side is the exit at*/
int typeOfExit(int x, int y, int edge)
{
	return (x == edge - 1) ? 1 : /*right side*/
		(x == 0) ? 2 : /*left side*/
		(y == edge - 1) ? 3 : /*down side*/
		(y == 0) ? 4 : 0; /*top side. 0, if there is no exit*/
}

/*set the path of maze*/
void setPath(int** maze, stack<valueInStack> path, int x, int y, int exit)
{
	while (!path.empty())
	{
		maze[path.top().row][path.top().col] = 2;
		path.pop();
	}
	switch (exit)
	{
	case 1: /*if the exit is at the right side*/
		maze[y][x - 1] = 2;
		break;
	case 2: /*at left side*/
		maze[y][x + 1] = 2;
		break;
	case 3: /*at down side*/
		maze[y - 1][x] = 2;
		break;
	case 4: /*at top side*/
		maze[y + 1][x] = 2;
		break;
	}
}

/*run an algorithm to find the path of maze with stack*/
void findPath(int** maze, int edge)
{
	int mark[72][72]{}; /*if the point has been through, mark as 1*/
	stack<valueInStack> path;
	mark[1][2] = 1; /*the entry*/
	valueInStack tmp = { 1, 2, 2 }; /*the next step after entry must at the south*/
	path.push(tmp);
	while (true) /*the maze must have exit*/
	{
		tmp = path.top();
		path.pop();
		int x = tmp.col, y = tmp.row, d = tmp.dir;
		while (d < 4)
		{
			int next_x = x + moving[d][0], next_y = y + moving[d][1];
			int exitType = typeOfExit(next_x, next_y, edge);
			if (exitType) /*the exit is found*/
			{
				setPath(maze, path, next_x, next_y, exitType);
				return; /*back to main function to draw path*/
			}
			if (maze[next_y][next_x] && (!mark[next_y][next_x]))
			{
				mark[next_y][next_x] = 1;
				tmp.col = x, tmp.row = y, tmp.dir = d + 1;
				/*cout << x << ' ' << y << ' ' << d + 1 << endl;*/
				path.push(tmp);
				x = next_x, y = next_y; d = 0;
			}
			else
				d++;
		}
	}
}

/*convert input.png to a maze as 2d-array, whose size is (the size of png file) / 10 + 2
  because the width of both road and wall is 10px,
  which means the size of maze is actually (the size of png file) / 10;
  +2 to avoid overflow of maze when run algorithm at side*/
void makeMaze(Mat image, int** maze, int row, int col)
{
	for (int i = 0, m = 1; i < row; i += 10, m++)
		for (int j = 0, n = 1; j < col; j += 10, n++)
			maze[m][n] = image.ptr<Vec3b>(i)[j][0] ? 1 : 0; /*1 for road, 0 for wall*/

	findPath(maze, row / 10 + 2);
}

/*the maze has solved before this function, then draw the path with green on input.png*/
void drawPath(Mat input, int** maze, int edge)
{
	for (int i = 1; i < edge; i++)
	{
		for (int j = 1; j < edge; j++)
		{
			if (maze[i][j] == 2) /*should be BGR(0, 255, 0), road is white(255,255,255)*/
			{
				for (int m = (i - 1) * 10; m < 10 * (i - 1) + 10; m++)
				{
					for (int n = (j - 1) * 10; n < 10 * (j - 1) + 10; n++)
					{
						input.ptr<Vec3b>(m)[n] = {0, 255, 0};
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {

	Mat input = imread(argv[1], IMREAD_COLOR);
	//imshow("input", input);
	int width = input.rows, height = input.cols;
	int edge = height / 10 + 2;
	int** maze = new int* [edge];
	for (int i = 0; i < edge; i++)
		maze[i] = new int [edge] {};

	makeMaze(input, maze, height, width);

	/*for (int i = 0; i < edge; i++)
	{
		for (int j = 0; j < edge; j++)
			cout << right << setw(3) << maze[i][j] << " ";
		cout << endl;
	}*/
	drawPath(input, maze, edge);

	for (int i = 0; i < edge; i++)
		delete[] maze[i];
	delete[] maze;

	imshow("output", input);
	//cout << clock() << "ms\n";
	waitKey(0);
	imwrite(argv[2], input);
}