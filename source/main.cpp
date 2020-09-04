#include <iostream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;
typedef long long ll;
// random numbers generator
random_device rd;
mt19937::result_type seed = rd() ^ ((mt19937::result_type)
                                        chrono::duration_cast<chrono::seconds>(
                                            chrono::system_clock::now().time_since_epoch())
                                            .count() +
                                    (mt19937::result_type)
                                        chrono::duration_cast<chrono::microseconds>(
                                            chrono::high_resolution_clock::now().time_since_epoch())
                                            .count());

// gets the euclidian distance of two vectors
ll euclidian_distance(vector<int> &a, vector<int> &b)
{
    ll result = 0;
    for (int i = 0; i < a.size(); i++)
        result += ((1ll * a[i]) - (1ll * b[i])) * ((1ll * a[i]) - (1ll * b[i])); // (ai - bi)^2
    return result;
}
// construct set of size "n", with vectors of dimension "k", with random integers.
void construct_VD(vector<vector<int>> &VD, int k, int n)
{
    mt19937 gen(seed);
    uniform_int_distribution<unsigned> distrib(1, 10000); // random values are from 1 to 10000
    vector<int> temp;                                     //temporal vector
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
            temp.push_back((int)distrib(gen)); // adding random values to our vector
        VD.push_back(temp);                    // adding a vector to our set of vectors
        temp.clear();                          //clear the temporal vector
    }
    return;
}
// print a set of vectors
void printSetVectors(vector<vector<int>> &VD)
{
    for (int i = 0; i < VD.size(); i++)
    {
        for (int j = 0; j < VD[i].size(); j++)
            cout << VD[i][j] << " ";
        cout << endl;
    }
    return;
}

void compare(vector<vector<int>> &VD)
{
    for (int i = 0; i < VD.size(); i++)
        for (int j = i + 1; j < VD.size(); j++)
            euclidian_distance(VD[i], VD[j]);
}
int main()
{
    vector<vector<int>> VD4;
    vector<vector<int>> VD6;
    vector<vector<int>> VD8;
    vector<vector<int>> VD10;
    vector<vector<int>> VD12;
    vector<vector<int>> VD18;
    vector<vector<int>> VD20;
    int cardinality = 25000;
    construct_VD(VD4, 4, cardinality);
    construct_VD(VD6, 6, cardinality);
    construct_VD(VD8, 8, cardinality);
    construct_VD(VD10, 10, cardinality);
    construct_VD(VD12, 12, cardinality);
    construct_VD(VD18, 18, cardinality);
    construct_VD(VD20, 20, cardinality);
    // printSetVectors(VD4);

    auto t1 = chrono::high_resolution_clock::now();
    compare(VD4);
    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD4[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD6);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD6[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD8);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD8[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD10);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD10[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD12);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD12[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD18);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD18[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    t1 = chrono::high_resolution_clock::now();
    compare(VD20);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Duration VD20[" << cardinality << " elementos]: " << duration << " ms" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}