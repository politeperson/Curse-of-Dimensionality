//#include "../build/octree.h"
#include "octree.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
int main()
{
	//// some tests, with floats
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
	if (Object.good())
	{
		// we start reading the .obj file
		while (getline(Object, line))
		{
			if (line[0] == 'v')
			{ // if we read a vertex of a file
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
		for (int i = 0; i < points.size(); ++i)
		{
			x = points[i].x;
			y = points[i].y;
			z = points[i].z;
			//cout << x << " " << y << " " << z << " " << endl;
			mytree.insert(x, y, z);
		}

		// making some tests
		cout << (mytree.find(-4.22943, 15.6584, 4.11027) ? "Found" : "Not found") << endl;
		cout << (mytree.find(-5.70826, 21.6563, 6.26488) ? "Found" : "Not found") << endl;

		mytree.remove(-4.22943, 15.6584, 4.11027);
		// the point now is not found
		cout << (mytree.find(-4.22943, 15.6584, 4.11027) ? "Found" : "Not found") << endl;
	}
	else
	{
		cout << "The file is not on the directory\n";
	}

	system("PAUSE");
	return 0;
}