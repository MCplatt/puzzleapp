#include <opencv2/opencv.hpp>
#include<math.h>
#include<vector>
#include<iostream>
//#include<conio.h>

using namespace std;
using namespace cv;

class Node                      // begin declaration of the class
{
	public:                      // begin public section
		Node(int xpos, int ypos, Mat &src);       // constructor
		Node(Mat &src);
		Node(const Node& copy_from); //copy constructor
		Node& operator=(const Node& copy_from); //copy assignment
		~Node();                    // destructor

		int GetValue() const;        // accessor function
		void SetValue(int val);      // accessor function
		void moveUp();
		void moveDown();
		void moveRight();
		void moveLeft();
		void moveUpRight();
		void moveDownRight();
		void moveUpLeft();
		void moveDownLeft();
	private:                      // begin private section
		int value;                // member variable
		int posx;
		int posy;
		cv::Mat matrix;
};