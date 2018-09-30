// CannyStill.cpp

#include<opencv2/opencv.hpp>
//#include "nodeclass.h"
#include<math.h>
#include<vector>
#include<iostream>
#include<conio.h>       // may have to modify this line if not using Windows
#include <algorithm>
using namespace std;
using namespace cv;
//bool operator<(const node& a, const node& b);
//void nodeCalc(Mat &src, node &curr, node &prev, node &exit);


void onMouse(int evt, int x, int y, int flags, void* param) {
	if (evt == CV_EVENT_LBUTTONDOWN) {
		std::vector<cv::Point>* ptPtr = (std::vector<cv::Point>*)param; 
		ptPtr->push_back(cv::Point(x, y));
	}
}

//Rough node class implimentation----------------------------------
struct node {
	int value;
	int Hval;//distance (dx + dy) between node and the end node
	int Pval;//number of nodes away from from start node
	int currX;
	int currY;
	int prevX;
	int prevY;
	void operator=(const node& a);
};

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

bool operator<(const node& a, const node& b) { //uses (Hval + Pval) to determine size
	return (a.Hval + a.Pval) < (b.Hval + b.Pval);
}
bool operator<=(const node& a, const node& b) {//uses (Hval + Pval) to determine size
	return (a.Hval + a.Pval) <= (b.Hval + b.Pval);
}
bool operator>=(const node& a, const node& b) {//uses (Hval + Pval) to determine size
	return (a.Hval + a.Pval) >= (b.Hval + b.Pval);
}
bool operator!=(const node& a, const node& b){//Uses (x,y) to determine identity
	if ((a.currX == b.currX) && (a.currY == b.currY))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool operator==(const node& a, const node& b){//Uses (x,y) to determine identity
	if ((a.currX == b.currX) && (a.currY == b.currY))
	{
		return true;
	}
	else
	{
		return false;
	}
}



node openSmallest(vector<node> &vec,node end);
void openInsert(vector<node> &vec, node nod);
void openReplace(vector<node> &vec, node nod);
void openRemove(vector<node> &vec, node nod);

int main() {
	try {
		cv::Mat imgOriginal;        // input image
		cv::Mat imgGrayscale;       // grayscale of input image
		cv::Mat imgMulty;
		cv::Mat imgEroded;
		//::Mat imgBlurred;         // intermediate blured image
		//cv::Mat imgCanny;           // Canny edge image

		imgOriginal = cv::imread("rlmaze.jpg");          // open image

		if (imgOriginal.empty()) {                                  // if unable to open image
			std::cout << "error: image not read from file\n\n";     // show error message on command line
			_getch();                                               // may have to modify this line if not using Windows
			return(0);                                              // and exit program
		}

		//Make image black&white, clean up and make clear (multy *2, erode 5X5)----------------------------------------------------------
		cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);
		cv::imshow("imgOriginal", imgOriginal);     // show windows

		cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

		//cv::namedWindow("imgGrayscale", CV_WINDOW_NORMAL);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		//cv::imshow("imgGrayscale", imgGrayscale);

		cv::multiply(imgGrayscale,2, imgMulty);                    //smooths out the image (doc scanning technique)

		//cv::namedWindow("imgMulty", CV_WINDOW_NORMAL);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		//cv::imshow("imgMulty", imgMulty);

		cv::imwrite("imgTest.png", imgMulty);

		int erosion_type = 1;
		Mat element = getStructuringElement(erosion_type,Size(5,5),Point(-1,-1));// increases size of black lines, makes them thicker
		erode(imgMulty, imgEroded, element);
		
		cv::imwrite("imgTest.png", imgEroded);
		//scan image for line size-------------------------------------------------
		cv::Mat src = imgEroded;
		int count = 0;
		int thresh = 127;
		std::vector<int> widths;
		std::vector<int> heights;
		int RowIter;
		int ColIter;
		int RowIterStart = 0;
		int ColIterStart = 0;
		int RowEnd;
		int ColEnd;
		if ((src.rows < 900) || (src.cols < 900))//if the image is less than 900x900 px scan whole pic
		{
			RowIter = (src.rows / 300);
			ColIter = (src.cols / 300);
			RowEnd = src.rows;
			ColEnd = src.cols;
			RowIterStart = 1;
			ColIterStart = 1;
		}
		else //if larger than 900x900, scan the middle 1/3 of the pic for 300 lines
		{
			RowIterStart = (src.rows / 3);
			ColIterStart = (src.cols / 3);
		    RowIter = (RowIterStart / 300);
			ColIter = (ColIterStart / 300);
			RowEnd = RowIterStart * 2;
			ColEnd = ColIterStart * 2;
		}
		
		if (RowIter == 0)
		{
			RowIter = 1;
		}
		if (ColIter == 0)
		{
			ColIter = 1;
		}
		//Scan Rows--------------------------------------------------------------------
		for (int y = RowIterStart; y < RowEnd; (y += RowIter)) {
			//std::cout << "row" <<  std::endl;
			for (int x = ColIterStart; x < ColEnd; (x += 1)) {
				//std::cout << int(src.at<uchar>(x, y)) << " ";
				if (int(src.at<uchar>(x, y)) < thresh) {
					count++;
				}
				if ((int(src.at<uchar>(x - 1 ,y)) <= thresh) && (int(src.at<uchar>(x, y)) > thresh)) {
					if (count == 0){}
					else
					{
						widths.push_back(count);
						count = 0;
					}
				}
			}
		}
		count = 0;
		//Scan Columns-------------------------------------------------------------
		for (int x = ColIterStart; x < ColEnd ; (x += ColIter)) {
			//std::cout << "row" << std::endl;
			for (int y = RowIterStart; y < RowEnd; (y += 1)) {
				//std::cout << int(src.at<uchar>(y, x)) << " ";
				if (int(src.at<uchar>(x, y)) < thresh) {
					count++;
				}
				if ((int(src.at<uchar>(x, y - 1)) <= thresh) && (int(src.at<uchar>(x, y)) > thresh)) {
					if (count == 0){}
					else
					{
						heights.push_back(count);
						count = 0;
					}
					
				}

			}
		}

		std::cout << "----------------------------Widths-------------------------      " << std::endl;
		std::cout << endl;
		int k = 0;
		int median = 0;
		int lowerQT = 0;
		for (k = 0; k < widths.size(); k++)
		{
			std::cout << widths[k] << " ";
			median = median + widths[k];
			
		}
		median = median / widths.size();//calculate the median
		
		std::cout << "LOWER Q" << std::endl;
		int lowerSize = 0;
		for (int j = 0; j < widths.size(); j++)
		{
			if (widths[j] < median)
			{
				std::cout << widths[j] << " ";
				lowerQT += widths[j];
				lowerSize += 1;
			}
		}
		std::cout << "   SizeLQ     " << widths.size()/2 << " " << endl;
		std::cout << "   Lower Quartile   " << lowerQT / lowerSize << " " << endl;//calculate the median of the lower half of the recorded widths (Lower Quartile)
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

		lowerSize = 0;
		for (int j = 0; j < heights.size(); j++)
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
		sideLen = sideLen + 1;
		Mat resized;



		

		resize(src, resized, cv::Size((src.cols / sideLen), (src.rows / sideLen)) , 1 / sideLen, 1 / sideLen, INTER_AREA);
		
		for (int x = 0; x < resized.rows; x++) {
			//std::cout << "row" <<  std::endl;
			for (int y = 0; y < resized.cols; y++) {
				//std::cout << int(src.at<uchar>(x, y)) << " ";
				if (int(resized.at<uchar>(x, y)) < 200) {
					resized.at<uchar>(x, y) = 0;
				}
				else if (int(resized.at<uchar>(x, y)) >= 200) {
					resized.at<uchar>(x, y) = 255;
				}
			}
		}
	
		//cv::imshow("resized", resized);
		//imwrite("result.jpg", resized);

		cv::namedWindow("Output Window", CV_WINDOW_NORMAL);
		vector<Point> points;
		cv::setMouseCallback("Output Window", onMouse, (void*)&points);
		int X, Y;
		bool flag = true;
		while (flag)
		{
			cv::imshow("Output Window", resized);
			waitKey(1);
			if (points.size() > 1) //we have 2 points
			{
				cout << "X and Y coordinates are given below" << endl;
				cout << points[0].x << '\t' << points[0].y << endl; 
				cout << points[1].x << '\t' << points[1].y << endl; 
				//resized.at<uchar>(points[0].y, points[0].x) = 100;
				//resized.at<uchar>(points[1].y, points[1].x) = 100;
				flag = false;
			}

		}
		
		//start node set to (318,301), pval 0
		//create end---------------------------
		node end;
		end.currX = points[1].y;
		end.currY = points[1].x;
		end.Pval = 0;
		end.Hval = 0;
		end.value = 255;
		end.prevX = 0;
		end.prevY = 0;
		//set start value-----------------------
		node start;
		start.currX = points[0].y;
		start.currY = points[0].x;
		start.Pval = 0;
		start.Hval = abs(max((start.currX - end.currX), (start.currY - end.currX)));
		start.value = int(resized.at<uchar>(start.currX, start.currY)); //assumed to be 255 already
		start.prevX = points[0].y;
		start.prevY = points[0].x;

		node curr = start;
		node next;
		int nextx;
		int nexty;
		bool openFlag = true;
		
		//A* Algorithm ------------------------------------
		//loops from node to node till the current node is the end node
		vector<node> closed; //nodes that have been touched
		vector<node> open; // adjacent nodes that have been discovered
	    //while c is not at the end node
		while (end != curr || openFlag) 
		{
			if ((curr.currX == 239) && (curr.currY == 1))
			{int pie = 0;} // stops a certain node

			//surrounding nodes added to open vector F(x(y))
			for (int i = -1; i < 2; i++)
			{
				if (((curr.currX + i) == -1) || ((curr.currX + i) >= resized.rows))// keeps from out of bounds error on x axis using cols(?)
				{
					continue;
				}
				for (int y = -1; y < 2; y++)
				{
					if (((curr.currY + i) == -1) || ((curr.currY + i) >= resized.cols))// keeps from out of bounds error on y axis using rows
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
					if (((i == -1) && (y == -1)) || ((i == 1) && (y == 1)) || ((i == -1) && (y == 1)) || ((i == 1) && (y == -1)))//make diagnal path cost a bit more
					{
						next.Pval = curr.Pval + 1.5;
					}
					else
					{
						next.Pval = curr.Pval + 1;
					}
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
			cout << curr.currX << ", " << curr.currY << "  Hval:" << curr.Hval << "  Pval:" << curr.Pval << endl;     
			cv::imshow("resized", resized);
			cv::waitKey(1);
		}
		
		imwrite("result.jpg", resized);
		int rowScaler = imgOriginal.rows / resized.rows;
		int colScaler = imgOriginal.cols / resized.cols;
		//Display the correct path------------------------------------------------------
		cv::namedWindow("Final", CV_WINDOW_NORMAL);
		int closeX = end.currY;
		int closeY = end.currX;
		//cv::waitKey(0);
		for (int i = 0; i < int(closed.size()); i++)//iterate through the closed points
		{
			points[0] = Point(closeX * colScaler , closeY * rowScaler);			
			if (i == 0)//for the first point initialize the point set 
			{
				
				closeX = closed[i].prevY;
				closeY = closed[i].prevX;
				points[1] = Point(closeX * colScaler, closeY * rowScaler);
				line(imgOriginal,points[0],points[1], Scalar(0, 0, 255) , sideLen ,0 );//draws a line
			}
			else if ((closed[i].currX == closeY) && (closed[i].currY == closeX))//when the (x,y) cords match the previous cordinates 
			{
				cout << "X: " << closeX << "   Y: " << closeY << "    Path: "<< closed[i].Pval << endl;
				closeX = closed[i].prevY;//update point set
				closeY = closed[i].prevX;
				points[1] = Point(closeX * colScaler, closeY * rowScaler);
				line(imgOriginal, points[0], points[1], Scalar(0, 0, 255), sideLen, 0);
				cv::imshow("Final", imgOriginal);
				cv::waitKey(1);
			}
			
		}
		
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
			if ((vec[i].Pval) > (nod.Pval))
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
