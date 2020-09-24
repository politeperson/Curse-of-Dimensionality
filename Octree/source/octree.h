#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
using namespace std;

#define TopLeftFront 0	   // octant(+,+,+)
#define TopRightFront 1	   // octant(+,-,+)
#define BottomRightFront 2 // octant(+,-,-)
#define BottomLeftFront 3  // octant(+,+,-)
#define TopLeftBack 4	   // octant(-,+,+)
#define TopRightBack 5	   // octant(-,-,+)
#define BottomRightBack 6  // octant(-,-,-)
#define BottomLeftBack 7   // octant(-,+,-)

template <class T>
class Point
{
public:
	T x, y, z;
	Point<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	bool operator==(Point<T> &p)
	{
		return (x == p.x && y == p.y && z == p.z);
	}

	bool operator!=(Point<T> &p)
	{
		return !(p == (*this));
	}

	~Point<T>() {}
};

template <class T>
class Node
{
public:
	Point<T> *corner;
	T h, w, d;		 // h->heigth:(z), w->width:(y), d->depth:(x)
	Node<T> *father; // the father of the node
	vector<Node<T> *> children;
	short type; // type of our node: 0(empty), 1 (internal), 2 (full)

	Node<T>() : type(0) {} // an empty node

	Node<T>(T a, T b, T c, T _h, T _w, T _d, short t = 0, Node<T> *f = NULL)
	{ // we create an octant
		h = _h;
		w = _w;
		d = _d;							// the heigth, width and depth
		corner = new Point<T>(a, b, c); // the lower corner of the cube
		father = f;						// the father is assigned
		type = t;						// the type of my node

		if (type == 1)
		{
			divideOctant(); // we divide the space in 8 octants
		}
	}

	void divideOctant()
	{
		// children will be empty nodes by default, and the father are ourself
		type = 1;				  // we convert to an internal node
		children.assign(8, NULL); // eigth children with value NULL
		T midx = (corner->x + (corner->x + d)) / 2;
		T midy = (corner->y + (corner->y + w)) / 2;
		T midz = (corner->z + (corner->z + h)) / 2;

		children[TopLeftFront] = new Node<T>(midx, midy, midz,
											 h / 2, w / 2, d / 2, 0, this);
		children[TopRightFront] = new Node<T>(midx, corner->y, midz,
											  h / 2, w / 2, d / 2, 0, this);
		children[BottomRightFront] = new Node<T>(midx, corner->y, corner->z,
												 h / 2, w / 2, d / 2, 0, this);
		children[BottomLeftFront] = new Node<T>(midx, midy, corner->z,
												h / 2, w / 2, d / 2, 0, this);
		children[TopLeftBack] = new Node<T>(corner->x, midy, midz,
											h / 2, w / 2, d / 2, 0, this);
		children[TopRightBack] = new Node<T>(corner->x, corner->y, midz,
											 h / 2, w / 2, d / 2, 0, this);
		children[BottomRightBack] = new Node<T>(corner->x, corner->y, corner->z,
												h / 2, w / 2, d / 2, 0, this);
		children[BottomLeftBack] = new Node<T>(corner->x, midy, corner->z,
											   h / 2, w / 2, d / 2, 0, this);
		return;
	}

	// this function checks if a point is outside the volume of my cube
	bool insideBound(T a, T b, T c)
	{
		if (a < corner->x || a > corner->x + d || b < corner->y || b > corner->y + w || c < corner->z || c > corner->z + h)
			return 0;
		return 1;
	}

	short getOctant(T x, T y, T z)
	{
		// Otherwise perform binary search
		// for each ordinate
		T midx = (corner->x + (corner->x + d)) / 2;
		T midy = (corner->y + (corner->y + w)) / 2;
		T midz = (corner->z + (corner->z + h)) / 2;
		short pos = -1;
		// Deciding the position
		// where to move
		if (x <= midx)
		{
			if (y <= midy)
			{
				if (z <= midz)
					pos = BottomRightBack;
				else
					pos = TopRightBack;
			}
			else
			{
				if (z <= midz)
					pos = BottomLeftBack;
				else
					pos = TopLeftBack;
			}
		}
		else
		{
			if (y <= midy)
			{
				if (z <= midz)
					pos = BottomRightFront;
				else
					pos = TopRightFront;
			}
			else
			{
				if (z <= midz)
					pos = BottomLeftFront;
				else
					pos = TopLeftFront;
			}
		}
		return pos;
	}

	// return true, if our voxel can be divided into much more little voxels
	bool stopCondition()
	{
		T cond = 10;
		return w * h * d >= cond; // if the volume is greater than condition, then we can still divide
	}

	~Node() {}
};

template <class T>
class Octree
{
public:
	Node<T> *root;
	vector<Point<T>> pointsData; // only to remove all the node of my tree

	Octree(T a, T b, T c, T h, T w, T d, short t = 1)
	{ // my root always have to start with bounds
		root = NULL;
		if (h <= 0 || w <= 0 | d <= 0)
		{
			cout << "the cube must have volume, try other parameters" << endl;
			return;
		}
		root = new Node<T>(a, b, c, h, w, d, t, NULL); // by deafult root does not have a father
	}

	bool find(T a, T b, T c)
	{
		Node<T> **p;
		return findVal(a, b, c, p);
	}

	// this function checks if a point exist on the tree
	bool findVal(T a, T b, T c, Node<T> **&p)
	{
		if (!root)
		{ // the root is NULL, so the tree is empty
			return 0;
		}
		else if (!root->insideBound(a, b, c))
		{
			// the point is outside the hole cube volume, so we cannot find it
			return 0;
		}

		short pos = -1;
		for (p = &root; *p && (*p)->type != 0 && (*p)->type != 2;)
		{
			pos = (*p)->getOctant(a, b, c);
			p = &((*p)->children[pos]);
		}
		// remenber type 2, says the node is full
		return (*p)->type == 2; // if the node is not full, then it does not exist on the tree
	}

	bool insert(T a, T b, T c)
	{
		Node<T> **p; // the pointer to the insertion node
		// the point already exist on the tree or is outside the bound, hence we dont need to insert it
		if (findVal(a, b, c, p) || !root->insideBound(a, b, c))
		{
			return 0;
		}
		// then we have p, pointing to the pointer of the destination point
		int pos = -1;
		// true if we can divide, false if not
		while ((*p)->stopCondition())
		{
			(*p)->divideOctant(); // we divide our space in 8 octants
			// now we move p down the tree
			pos = (*p)->getOctant(a, b, c); // we obtain the octant, to move p pointer
			p = &((*p)->children[pos]);		// we move the p pointer to the position that correspond in the octant
		}

		bool fullSpace = 1; // this is a flag to check if all the children are full
		Node<T> **q = &((*p)->father);
		(*p)->type = 2; // p now is full
		for (int i = 0; i < (*q)->children.size(); ++i)
		{
			if ((*q)->children[i]->type != 2)
			{
				fullSpace = 0;
				break;
			}
		}

		// if the hole space is full, then we can set q space to full
		// and delete the children
		if (fullSpace)
		{
			(*q)->type = 2;			// q is now of type full
			(*q)->children.clear(); // and we clean all the vector of children
		}
		// we push the point to the dataPoints, for later destruction
		pointsData.push_back(Point<T>(a, b, c));
		return 1;
	}
	bool remove(T a, T b, T c)
	{
		Node<T> **p;
		if (!findVal(a, b, c, p) || !root->insideBound(a, b, c))
		{
			// node is not on the tree, or is out of bound
			return 0;
		}
		short pos = -1;
		Node<T> **q;
		// 1st case, the octant selected, can be partitioned
		// we do the descent on the tree
		while ((*p)->stopCondition())
		{
			(*p)->divideOctant();
			(*p)->type = 1; // p is now a node of type internal
			for (int i = 0; i < (*p)->children.size(); ++i)
			{
				(*p)->children[i]->type = 2; // all the children of my node will be full nodes
			}
			pos = (*p)->getOctant(a, b, c);
			p = &((*p)->children[pos]);
		}
		(*p)->type = 0; // the node will be empty
		// 2nd case, the octant is the last point of another octant
		// now we will do the upward on the tree
		q = &((*p)->father);
		bool emptySpace = 1; // check if the hole space of the father of p is empty
		// while q is not the root, and the space is empty
		while ((*q) != root && emptySpace)
		{
			for (int i = 0; i < (*q)->children.size(); ++i)
			{
				if ((*q)->children[i]->type != 0)
				{
					emptySpace = 0;
					break;
				}
			}
			// if the hole space is empty
			if (emptySpace)
			{
				// we delete all the childs of q, and set q to empty
				(*q)->children.clear(); // and we clean all the vector of children
				// the parent is of type empty
				(*q)->type = 0;
				q = &((*q)->father); // q points to its father
			}
		}

		return 1;
	}

	~Octree()
	{
		T x, y, z;
		for (int i = 0; i < pointsData.size(); ++i)
		{
			x = pointsData[i].x;
			y = pointsData[i].y;
			z = pointsData[i].z;
			remove(x, y, z);
		}
		pointsData.clear();
		root->children.clear();
		delete root;
	}
};
