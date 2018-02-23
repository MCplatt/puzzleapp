#include "nodeclass.h"

// constructor of Node,
Node::Node()
{
	posx = 0;
	posy = 0;
	matrix = NULL;
	value = NULL;
}
Node::Node(int xpos,int ypos, Mat &src)
{
	posx = xpos;
	posy = ypos;
	matrix = src;
	value = int(src.at<uchar>(x, y));
}

//copy constructor for making a new copy of a Node
Node::Node(const Node& copy_from) {
	posx = copy_from.posx;
	posy = copy_from.posy;
	src = copy_from.matrix;
	value = copy_from.value;
}

//copy assignment for assigning a value from one Node to another
Node& Node::operator=(const Node& copy_from) {
	posx = copy_from.posx;
	posy = copy_from.posy;
	src = copy_from.matrix;
	value = copy_from.value;
}

// destructor, just an example
Node::~Node()
{
	src.release();
}

// GetAge, Public accessor function
// returns value of itsAge member
int Node::GetValue() const
{
	return value;
}

// Definition of SetAge, public
// accessor function
void Node::SetVlaue(int val)
{
	value = val;
}


void moveUp()
{
	value = int((*matrix).at<uchar>(posx, posy + 1));
	posy += 1;
}
void moveDown()
{
	value = int((*matrix).at<uchar>(posx, posy - 1));
	posy -= 1;
}
void moveRight()
{
	value = int((*matrix).at<uchar>(posx + 1, posy));
	posx += 1;
}
void moveLeft()
{
	value = int((*matrix).at<uchar>(posx - 1, posy));
	posx -= 1;
}
void moveUpRight()
{
	value = int((*matrix).at<uchar>(posx + 1, posy + 1));
	posx += 1;
	posy += 1;
}
void moveDownRight()
{
	value = int((*matrix).at<uchar>(posx + 1, posy - 1));
	posx += 1;
	posy -= 1;
}
void moveUpLeft()
{
	value = int((*matrix).at<uchar>(posx - 1, posy + 1));
	posx -= 1;
	posy += 1
}
void moveDownLeft()
{
	value = int((*matrix).at<uchar>(posx - 1, posy - 1));
	posx -= 1;
	posy -= 1;
}
