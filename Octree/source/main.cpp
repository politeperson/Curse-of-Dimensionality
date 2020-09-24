// #include "../build/octree.h"
#include "octree.h"
using namespace std;
int main()
{
	// some tests, with floats
	Octree<float> tree(1.0, 1.0, 1.0, 5.0, 5.0, 5.0);

	tree.insert(2.0, 3.0, 2.5);
	tree.insert(4.1, 3.4, 1.2);
	tree.insert(7.6, 3.0, 2.5);
	tree.insert(3.5, 4.3, 2.1);

	cout << (tree.find(3.5, 4.3, 2.0) ? "Found" : "Not found") << endl;
	cout << (tree.find(7.0, 4.3, 2.1) ? "Found" : "Not found") << endl;
	cout << (tree.find(2.0, 3.0, 2.5) ? "Found" : "Not found") << endl;
	cout << (tree.find(3.5, 3.5, 3.5) ? "Found" : "Not found") << endl;

	tree.remove(2.0, 3.0, 2.5);

	cout << (tree.find(2.0, 3.0, 2.5) ? "Found" : "Not found") << endl;

	system("PAUSE");
	return 0;
}