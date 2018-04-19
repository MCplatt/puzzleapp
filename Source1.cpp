// CannyStill.cpp

#include<opencv2/opencv.hpp>
//#include "nodeclass.h"
#include<math.h>
#include<vector>
#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows
#include <algorithm>>

using namespace std;
using namespace cv;
//bool operator<(const node& a, const node& b);
//void nodeCalc(Mat &src, node &curr, node &prev, node &exit);

//Rough node class implimentation----------------------------------
struct node {
	int value;
	int Hval;
	int Pval;
	int currX;
	int currY;
	int prevX;
	int prevY;
	void operator=(const node& a);
};

bool operator<(const node& a, const node& b) {
	return (a.Hval + a.Pval) < (b.Hval + b.Pval);
}

bool operator<=(const node& a, const node& b) {
	return (a.Hval + a.Pval) <= (b.Hval + b.Pval);
}
bool operator>=(const node& a, const node& b) {
	return (a.Hval + a.Pval) >= (b.Hval + b.Pval);
}
bool operator!=(const node& a, const node& b){
	if ((a.currX == b.currX) && (a.currY == b.currY))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool operator==(const node& a, const node& b){
	if ((a.currX == b.currX) && (a.currY == b.currY))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void node::operator=(const node& a)
{
	this->currX = a.currX;
	this->currY = a.currY;
	this->Hval = a.Hval;
	this->prevX = a.prevX;
	this->prevY = a.prevY;
	this->Pval = a.Pval;
	this->value = a.value;
}

node openSmallest(vector<node> &vec,node end);
void openInsert(vector<node> &vec, node nod);
void openReplace(vector<node> &vec, node nod);
void openRemove(vector<node> &vec, node nod);

int main() {
	try {
		cv::Mat imgOriginal;        // input image
		cv::Mat imgGrayscale;       // grayscale of input image
		cv::Mat imgBlurred;         // intermediate blured image
		//cv::Mat imgCanny;           // Canny edge image

		imgOriginal = cv::imread("rlmaze.jpg");          // open image

		if (imgOriginal.empty()) {                                  // if unable to open image
			std::cout << "error: image not read from file\n\n";     // show error message on command line
			_getch();                                               // may have to modify this line if not using Windows
			return(0);                                              // and exit program
		}
		//std::cout << "grayscale" << std::endl;
		cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

		//cv::GaussianBlur(imgGrayscale,          // input image
		//	imgBlurred,                         // output image
		//	cv::Size(9, 9),                     // smoothing window width and height in pixels
		//	1.5);                               // sigma value, determines how much the image will be blurred
		//medianBlur(imgGrayscale, imgBlurred, 5);
		//cv::Canny(imgBlurred,           // input image
		//	imgCanny,                   // output image
		//	82,                         // low threshold
		//	164);                       // high threshold


		// declare windows
		cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgGrayscale", CV_WINDOW_NORMAL);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		//cv::namedWindow("canny", CV_WINDOW_NORMAL);
		//cv::namedWindow("blurred", CV_WINDOW_NORMAL);							// CV_WINDOW_AUTOSIZE is the default

		cv::imshow("imgOriginal", imgOriginal);     // show windows
		cv::imshow("imgGrayscale", imgGrayscale);
		//cv::imshow("canny", imgCanny);
		//cv::imshow("blurred", imgBlurred);
		/*Mat opened;
		int morph_size = 1;
		Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(-1, -1));
		morphologyEx(imgGrayscale, opened, MORPH_OPEN, element, Point(-1, -1), 2);
		cv::namedWindow("opened", CV_WINDOW_NORMAL);
		cv::imshow("opened", opened);*/

		//use the grea
		cv::Mat src = imgGrayscale;
		int count = 0;
		int thresh = 127;
		std::vector<int> widths;
		std::vector<int> heights;
		int RowIter = (src.rows / 300);
		int ColIter = (src.cols / 300);
		if (RowIter == 0)
		{
			RowIter = 1;
		}
		if (ColIter == 0)
		{
			ColIter = 1;
		}

		for (int x = 0; x < src.rows; (x += RowIter)) {
			//std::cout << "row" <<  std::endl;
			for (int y = 1; y < src.cols; (y += ColIter)) {
				//std::cout << int(src.at<uchar>(x, y)) << " ";
				if (int(src.at<uchar>(x, y)) < thresh) {
					count++;
				}
				if ((int(src.at<uchar>(x, y - 1)) <= thresh) && (int(src.at<uchar>(x, y)) > thresh)) {
					if (count == 0)
					{

					}
					else
					{
						widths.push_back(count);
						count = 0;
					}
					
				}

			}
		}
		count = 0;
		for (int x = 0; x < src.cols ; (x += ColIter)) {
			//std::cout << "row" << std::endl;
			for (int y = 1; y < src.rows; (y += RowIter)) {
				//std::cout << int(src.at<uchar>(y, x)) << " ";
				if (int(src.at<uchar>(y, x)) < thresh) {
					count++;
				}
				if ((int(src.at<uchar>(y - 1, x)) <= thresh) && (int(src.at<uchar>(y, x)) > thresh)) {
					if (count == 0)
					{

					}
					else
					{
						heights.push_back(count);
						count = 0;
					}
					
				}

			}
		}
		//cv::imshow("src", src);
		std::cout << "----------------------------Widths-------------------------      " << std::endl;
		int k = 0;
		int median = 0;
		int lowerQT = 0;
		for (k = 0; k < widths.size(); k++)
		{
			//std::cout << widths[k] << " ";
			median = median + widths[k];
			
		}
		median = median / widths.size();
		cout << endl;
		std::cout << "LOWER Q" << std::endl;
		int j = 0;
		int lowerSize = 0;
		for (j = 0; j < widths.size(); j++)
		{
			if (widths[j] < median)
			{
				std::cout << widths[j] << " ";
				lowerQT += widths[j];
				lowerSize += 1;
			}
		}
		std::cout << "   SizeLQ     " << widths.size()/2 << " " << endl;
		std::cout << "   Lower Quartile   " << lowerQT / lowerSize << " " << endl;
		std::cout << "   SizeM     " << widths.size() << " " << endl;
		std::cout << "   MEDIAN   "<< median << " " << endl;
		int Wmed = median;
		int WLQ = lowerQT / lowerSize;
		std::cout << "----------------------------Heights----------------------------" << std::endl;
		k = 0;
		median = 0;
		lowerQT = 0;
		for (k = 0; k < heights.size(); k++)
		{
		//	std::cout << heights[k] << " ";
			median = median + heights[k];

		}
		median = median / heights.size();
		cout << endl;
		std::cout << "LOWER Q" << std::endl;
		j = 0;
		lowerSize = 0;
		for (j = 0; j < heights.size(); j++)
		{
			if (heights[j] < median)
			{
				std::cout << heights[j] << " ";
				lowerQT += heights[j];
				lowerSize += 1;
			}
		}

		std::cout << "   SizeLQ     " << heights.size() / 2 << " " << endl;
		std::cout << "   Lower Quartile   " << lowerQT / lowerSize << " " << endl;
		std::cout << "   SizeM     " << heights.size() << " " << endl;
		std::cout << "   MEDIAN   " << median << " " << endl;
		
		int Hmed = median;
		int HLQ = lowerQT / lowerSize;
		//int hypo = sqrt(pow(Hmed, 2) + pow(Wmed, 2));
		//int area = ((Hmed*Wmed) / 2);
		int hypo = sqrt(pow(HLQ, 2) + pow(WLQ, 2));
		int area = ((HLQ*WLQ) / 2);
		int sideLen = (2 * area) / hypo;

		cout << "side length = " << sideLen << endl;
		sideLen = sideLen/3.5;
		Mat resized;



		

		resize(src, resized, cv::Size((src.rows / sideLen), (src.cols / sideLen)) , 1 / sideLen, 1 / sideLen, INTER_AREA);
		
		for (int x = 0; x < resized.rows; x++) {
			//std::cout << "row" <<  std::endl;
			for (int y = 0; y < resized.cols; y++) {
				//std::cout << int(src.at<uchar>(x, y)) << " ";
				if (int(resized.at<uchar>(x, y)) < 127) {
					resized.at<uchar>(x, y) = 0;
				}
				else if (int(resized.at<uchar>(x, y)) >= 127) {
					resized.at<uchar>(x, y) = 255;
				}
			}
		}
	
		cv::imshow("resized", resized);
		imwrite("result.jpg", resized);

		
		//start node set to (318,301), pval 0
		//create end---------------------------
		node end;
		end.currX = 307;
		end.currY = 440;
		end.Pval = 0;
		end.Hval = 0;
		end.value = 255;
		end.prevX = 0;
		end.prevY = 0;
		//set start value-----------------------
		node start;
		start.currX = 252;
		start.currY = 267;
		start.Pval = 0;
		start.Hval = abs(max((start.currX - end.currX), (start.currY - end.currX)));
		start.value = int(resized.at<uchar>(start.currX, start.currY)); //assumed to be 255 already
		start.prevX = 318;
		start.prevY = 301;

		node curr = start;
		node next;
		int nextx;
		int nexty;

		
		//A* Algorithm ------------------------------------
		//loops from node to node till the current node is the end node

		vector<node> closed; //nodes that have been touched
		vector<node> open; // adjacent nodes that have been discovered
	    //while c is not at the end node
		while (end != curr) 
		{
			/*if ((curr.currX == 575) && (curr.currY == 446))
			{int pie = 0;}  stops a certain node*/

			//surrounding nodes added to open vector F(x(y))
			for (int i = -1; i < 2; i++)
			{
				if (((curr.currX + i) == -1) || ((curr.currX + i) > resized.cols))// keeps from out of bounds error on x axis using cols(?)
				{
					continue;
				}
				for (int y = -1; y < 2; y++)
				{
					if (((curr.currY + i) == -1) || ((curr.currY + i) > resized.rows))// keeps from out of bounds error on y axis using rows
					{
						continue;
					}
					if ((i == 0) && (y == 0)) //prevent calc of current node
					{
						continue;
					}
					//discover next node------------------------------------------ in its own function?
					nextx = curr.currX + i;
					nexty = curr.currY + y;
					next.currX = nextx;
					next.currY = nexty;
					next.Pval = curr.Pval + 1;
					next.Hval = abs(nextx - end.currX) + abs(nexty - end.currY);
					if (next.Hval > curr.Hval)
				    {
						next.Hval = (next.Hval * 1.5);
					}
					next.value = int(resized.at<uchar>(nextx, nexty)); 
					next.prevX = curr.currX;
					next.prevY = curr.currY;

					//add to the a.star vectors----------------------
					if (next.value == 100 || next.value == 0) //100 - closed: no action, < 100 - Black Tile: no action
					{
						continue;
					}
				    if (next.value == 200)//200 - has been discovered, is it better?
					{
						openReplace(open, next);
					}
					else//mark as discovered(200),  add to open list
					{
						resized.at<uchar>(nextx, nexty) = 200;
						openInsert(open, next);
					}
					
				};
			};
			//close out current node(100), find next node from open vector(openSmallest)
			resized.at<uchar>(curr.currX, curr.currY) = 100;
			closed.insert(closed.begin(), curr);
			curr = openSmallest(open, end);
			openRemove(open, curr);
			//Debug----cout << curr.currX << ", " << curr.currY << "  Hval:" << curr.Hval << "  Pval:" << curr.Pval << endl;     
			
		}
		imwrite("result.jpg", resized);
		cv::imshow("resized", resized);
		
		int closeX = end.currX;
		int closeY = end.currY;

		for (int i = (int(closed.size()) - 1); i != 0; i--)
		{
			
			src.at<uchar>(closeX * sideLen, closeY * sideLen) = 0;
			if (i = (int(closed.size()) - 1))
			{
				closeX = closed[i].prevX;
				closeY = closed[i].prevY;
			}
			else if ((closed[i].currX == closeX) && (closed[i].currY == closeY))
			{
				cout << "X: " << closeX << "   Y: " << closeY << "    Path: "<< closed[i].Pval << endl;
				closeX = closed[i].prevX;
				closeY = closed[i].prevY;
			}
		}
		cv::imshow("Final", src);
		cv::waitKey(0);                 // hold windows open until user presses a key
		return(0);
	}
	catch (cv::Exception & e)
	{
		std::cerr << e.msg << std::endl; // output exception message
		cv::waitKey(0);
	}
}
//VECTOR OPEN<node>  MEMBER FUNCTIONS--------------------------------------

//find smallest (Hval + Pval) in the open vector - O(n)
node openSmallest(vector<node> &vec, node end) 
{
	node smallest = vec[0];
	for(int i = 0; i < int(vec.size()); i++)
	{
		if (vec[i] <= smallest)
		{
			if (vec[i].Hval < smallest.Hval)//Tie Breaker: chose the smallest Hval among smallest nodes
			{
				smallest = vec[i];
			}
		}
	}
	return smallest;
}

//iterate throw open<node>, find matching (x,y) value, delete it- O(n)
void openRemove(vector<node> &vec, node nod)
{
	for (vector<node>::iterator itr = vec.begin(); itr != vec.end(); itr++)
	{
		if ((itr->currX ==  nod.currX) && (itr->currY == nod.currY))
		{
			vec.erase(itr);
			return;
		}
	}
}

//loop through open<node>, find matching (x,y) value, replace- O(n)
void openReplace(vector<node> &vec, node nod)
{
	for (int i = 0; i < int(vec.size()); i++)
	{
		if ((vec[i].currX == nod.currX) && (vec[i].currY == nod.currY))//find node at equal coordinate
		{
			if ((vec[i].Pval + vec[i].Hval) <= (nod.Pval + nod.Hval))
			{
				vec[i] = nod;
				return;
			}
			else
			{
				return;
			}
		}
	}
}
void openInsert(vector<node> &vec, node nod)//insert nodes in open<node>, least -> greatest (Hval + Pval)
{
	if (int(vec.size()) == 0)
	{
		vec.push_back(nod);
		return;
	}
	else if (int(vec.size()) > 0)
	{
		for (vector<node>::iterator itr = vec.begin(); itr != vec.end(); itr++)
		{
			if ((nod.Hval + nod.Pval) <= (itr->Hval + itr->Pval))
			{
				vec.insert(itr, nod);
				return;
			}
		}
		vec.push_back(nod);
		return;
		
	}
}

/*
void nodeCalc(Mat &src, node &curr,node &prev, node &exit)
{
	if (curr.value == -2 || curr.value == 0)//if already closed(-2) or wall (0) do nothing
	{
		
	}
	else if (curr.value == 255 || curr.value == -1)// if open space(255) or calculated open(-1)
	{
		curr.value = -1;  //set as calculated open
		int dx = std::abs(curr.currX - exit.currX);//calc x hueristic
		int dy = std::abs(curr.currY - exit.currY);//calc y heuristic
		curr.Hval = std::max(dx, dy);// furthest axis is the h val( because every direction is an equal move cost)
		curr.Pval = prev.Pval + 1; //calculate the path from the previous node
		curr.prevX = prev.currX;//link to prev node for end retrace
		curr.prevY = prev.currY;
	}
	
}
*/