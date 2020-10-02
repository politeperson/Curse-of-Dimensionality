#include <GL/glut.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
/*
void dibujar()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);

	glVertex2d(20,20);

	glEnd();
	glFlush();


}

void iniciar() 
{
	glClearColor(0, 0, 1, 1);

	glPointSize(5);

	glColor3i(1, 0, 0);

	glOrtho(800, 0, 600, 0, -1, 1);
}*/

//#define CWHITE 1.0, 1.0, 1.0


double rotX = 0.0;
double rotY = 0.0;

/*
class Point
{
public:
	double x, y, z;
	Point() {}
	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Point(Point& P) {
		this->x = P.x;
		this->y = P.y;
		this->z = P.z;
	}
	
	Point operator -(Point A) {
		Point Ans(x - A.x, y - A.y, z - A.z);
		return Ans;
	}
	~Point() {

	}
};
*/

/*
struct Octant {
	Point starP, size;
	Octant(Point boundMin, Point boundMax) 
	{
		starP = boundMin;
		size = boundMax - boundMin;
	}
	void show() {
		glPushMatrix();
		glColor3d(CWHITE);
		glScaled(size.x, size.y, size.z);
		glTranslated(starP.x, starP.y, starP.z);
		glutSolidCube(1.0);
		glPopMatrix();
	}
};*/


#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
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
	Point<T>() {}
	Point<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	bool operator==(Point<T>& p)
	{
		return (x == p.x && y == p.y && z == p.z);
	}

	bool operator!=(Point<T>& p)
	{
		return !(p == (*this));
	}

	~Point<T>() {}
};

template <class T>
class Cube
{
public:
	T h, w, d;
	Point<T> corner;
	Cube<T>(T a, T b, T c, T _h, T _w, T _d) {
		h = _h;
		w = _w;
		d = _d;
		corner.x = a;
		corner.y = b;
		corner.z = c;
	}
	Cube<T>() {}
	void show() {
		glPushMatrix();
		glColor3d(1.0, 1.0, 1.0);
		glScaled(h, w, d);
		glTranslated(corner.x, corner.y, corner.z);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	~Cube() {}
};



template <class T>
class Node
{
public:
	Point<T>* corner;
	T h, w, d;		 // h->heigth:(z), w->width:(y), d->depth:(x)
	Node<T>* father; // the father of the node
	vector<Node<T>*> children;
	short type; // type of our node: 0(empty), 1 (internal), 2 (full)

	Node<T>() : type(0) {} // an empty node

	Node<T>(T a, T b, T c, T _h, T _w, T _d, short t = 0, Node<T> * f = NULL)
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

	~Node() {
		delete corner;
	}
};

template <class T>
class Octree
{
public:
	Node<T>* root;
	vector<Point<T>> pointsData; // only to remove all the node of my tree

	Octree(T a, T b, T c, T h, T w, T d, short t = 1)
	{ // my root always have to start with bounds
		root = NULL;
		if (h <= 0 || w <= 0 || d <= 0)
		{
			cout << "the cube must have volume, try other parameters" << endl;
			return;
		}
		root = new Node<T>(a, b, c, h, w, d, t, NULL); // by deafult root does not have a father
	}

	bool find(T a, T b, T c)
	{
		Node<T>** p;
		return findVal(a, b, c, p);
	}

	// this function checks if a point exist on the tree
	bool findVal(T a, T b, T c, Node<T>**& p)
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
		Node<T>** p; // the pointer to the insertion node
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
		Node<T>** q = &((*p)->father);
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
		Node<T>** p;
		if (!findVal(a, b, c, p) || !root->insideBound(a, b, c))
		{
			// node is not on the tree, or is out of bound
			return 0;
		}
		short pos = -1;
		Node<T>** q;
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
	// a function that returns my cubes of type 2 in the tree
	void BFS(vector<Cube<T>> &Cubes) {
		queue<Node<T>*> Q;
		if (!root) {
			return;// we return an empty list of cubes
		}
		//Node<T>(T a, T b, T c, T _h, T _w, T _d, short t = 0, Node<T> * f = NULL)
		Q.push(root);
		T x, y, z;
		while (!Q.empty())
		{
			for(int i = 0; i < Q.front()->children.size(); ++i)
			{
				if (Q.front()->children[i]->type != 0) {
					if (Q.front()->children[i]->type == 2)// a full cube
					{
						x = Q.front()->corner->x;
						y = Q.front()->corner->y;
						z = Q.front()->corner->z;
						Cubes.push_back(Cube<T>(x, y, z, Q.front()->h, Q.front()->w, Q.front()->d));
					}
					else {// an internal node
						Q.push(Q.front()->children[i]);
					}
				}
			}
			Q.pop();
		}
		return;
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


//////////////////////////////////////////////////
vector<Cube<float>> Cubes;
///////////////////////////////////////////////////

void init(void) {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glClearColor(0.2, 0.0, 0.0, 1.0);

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	GLfloat lightColor0[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat lightPos0[] = { 5.5f, 7.5f, 5.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	glRotated(rotX, 1.0, 0.0, 0.0);
	glRotated(rotY, 0.0, 1.0, 0.0);


	/*glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glScaled(1, 1, 1);
	glTranslated(3, 3, 3);
	glutSolidCube(0.5);
	glPopMatrix();*/
	
	for (auto& cube : Cubes) {
		cube.show();
	}

	glDisable(GL_COLOR_MATERIAL);
	glFlush();
	glutSwapBuffers();
}

void keyboard(int key, int x, int y) {
	rotX = rotY = 0.0;
	if (key == GLUT_KEY_RIGHT) rotY = 1.5;
	else if (key == GLUT_KEY_LEFT) rotY = -1.5;
	else if (key == GLUT_KEY_UP) rotX = 1.5;
	else if (key == GLUT_KEY_DOWN) rotX = -1.5;

	glutPostRedisplay();
}

void runOPENGL(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 740);
	glutCreateWindow("Octree Visualization");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutMainLoop();
}


/*
int main(int argc, char *args[])
{
	string windowTitle = "puntos openGL";

	glutInit(&argc, args);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(800, 600);

	glutInitWindowPosition(50,50);

	glutCreateWindow(windowTitle.c_str());

	glutDisplayFunc(dibujar);

	iniciar();

	glutMainLoop();

	return 0;
}*/



int main(int argc, char** argv)
{
	//Octree<float> tree(1.0, 1.0, 1.0, 5.0, 5.0, 5.0);

	//tree.insert(2.0, 3.0, 2.5);
	//tree.insert(4.1, 3.4, 1.2);
	//tree.insert(7.6, 3.0, 2.5);
	//tree.insert(3.5, 4.3, 2.1);

	//cout << (tree.find(3.5, 4.3, 2.0) ? "Found" : "Not found") << endl;
	//cout << (tree.find(7.0, 4.3, 2.1) ? "Found" : "Not found") << endl;
	//cout << (tree.find(2.0, 3.0, 2.5) ? "Found" : "Not found") << endl;
	//cout << (tree.find(3.5, 3.5, 3.5) ? "Found" : "Not found") << endl;

	//tree.remove(2.0, 3.0, 2.5);

	//cout << (tree.find(2.0, 3.0, 2.5) ? "Found" : "Not found") << endl;

	
	
	// we will read the .obj file
	string line;
	stringstream ss;
	vector<Point<float>> points;
	float x, y, z;
	float minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0; // this are the points that will be our bounds of the cube
	char c;
	ifstream Object("teddy.obj");
	if (Object.good()) {
		// we start reading the .obj file
		while (getline(Object, line)) {
			if (line[0] == 'v') { // if we read a vertex of a file
				ss << line;
				ss >> c >> x >> y >> z;
				// we obtain the point corner bounds of the cube
				minx = min(minx, x);
				maxx = max(maxx, x);
				miny = min(miny, y);
				maxy = max(maxy, y);
				minz = min(minz, z);
				maxz = max(maxz, z);
				points.push_back(Point<float>(x, y, z));
				ss.clear();
			}
		}
		// after reading the file
		cout << "bottom right back corner: " << minx << " " << miny << " " << minz << endl;
		cout << "top left front corner: " << maxx << " " << maxy << " " << maxz << endl;
		float h = maxz - minz;
		float w = maxy - miny;
		float d = maxx - minx;

		Octree<float> mytree(minx, miny, minz, h, w, d); // creating the octree
		// we insert all the points on the tree
		for (int i = 0; i < points.size(); ++i) {
			x = points[i].x;
			y = points[i].y;
			z = points[i].z;
			//cout << x << " " << y << " " << z << " " << endl;
			mytree.insert(x, y, z);
		}

		//// making some tests
		//cout << (mytree.find(-4.22943, 15.6584, 4.11027) ? "Found" : "Not found") << endl;
		//cout << (mytree.find(-5.70826, 21.6563, 6.26488) ? "Found" : "Not found") << endl;

		//mytree.remove(-4.22943, 15.6584, 4.11027);
		//// the point now is not found
		//cout << (mytree.find(-4.22943, 15.6584, 4.11027) ? "Found" : "Not found") << endl;
		
		mytree.BFS(Cubes);

		/*for (int i = 0; i < Cubes.size(); ++i) {
			cout << Cubes[i].corner.x << " ";
			cout << Cubes[i].corner.y << " ";
			cout << Cubes[i].corner.z << " ";
			cout << endl;
		}*/


		runOPENGL(argc, argv);
		Cubes.clear();
	}
	else {
		cout << "The file is not on the directory\n";
	}
	

	system("PAUSE");

	return 0;
}