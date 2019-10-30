#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
class Pt
{
public:
    double x, y; // x,y coordinate
    int idx; // record input's order
    int rk;
    Pt (double x = -100, double y = -100, int idx = -100, int r = 0) { this->x = x; this->y = y; this->idx = idx; rk = r; }
};

bool sortByIdx (Pt p1, Pt p2){
    return (p1.idx < p2.idx);
}
void HeapSort(vector<Pt> &pt);
void buildMaxHeap(vector<Pt> &pt);
void MaxHeapify(vector<Pt> &pt, int root, int length);
int LeftNode(int);
int RightNode(int);

void DC_2D_ranking(vector<Pt> &pt, int l, int r);
void merge_2D(vector<Pt> &pt, int l, int m, int r);

int main()
{
    vector<Pt> pt;
    fstream file;
    file.open("test1.txt", ios::in);
    if (file.fail())
    {
        cout << "The file was not found" << endl;
        return 1;
    }
    // store all point in file
    double temp;
    int len;
    while (file >> temp){
        len = pt.size();
        pt.push_back(Pt());
        pt[len].x = temp;
        file >> pt[len].y;
        pt[len].idx = len;
    }

    file.close();

    HeapSort(pt);
    DC_2D_ranking(pt, 0, pt.size() - 1);
    sort(pt.begin(), pt.end(), sortByIdx);

    cout << fixed << setprecision(2);
    cout << "Complete:" << endl;
    double avgRank = 0.0;
    int maxRank = -1;
    int minRank = 2147483647;
    for (int i = 0;i < pt.size();i++){
        cout << "Point " << i << " : x = " << pt[i].x << " , y = " << pt[i].y << ", Rank = " << pt[i].rk << endl;
        if (maxRank < pt[i].rk)
            maxRank = pt[i].rk;
        if (minRank > pt[i].rk)
            minRank = pt[i].rk;
        avgRank += pt[i].rk;
    }
    avgRank /= pt.size();
    cout << "The number of input points :" << pt.size() << endl;
    cout << "Max Rank : " << maxRank << endl;
    cout << "Min Rank : " << minRank << endl;
    cout << "Average Rank: " << avgRank << endl;
    system("pause");
    return 0;
}

void DC_2D_ranking(vector<Pt> &pt, int l, int r){
    if (l >= r) return;
    int m = (l + r) / 2;
    DC_2D_ranking(pt, l, m);
    DC_2D_ranking(pt, m + 1, r);
    merge_2D(pt, l, m, r);
}

void merge_2D(vector<Pt> &pt, int l, int m, int r){
    int leftLine = l, rightLine = m + 1;
    int addedPoint = 0, accumulateRank = 0;
    Pt temp[r - l + 1];
    while (leftLine <= m && rightLine <= r)
    {

        if (pt[leftLine].y < pt[rightLine].y){
            temp[addedPoint++] = pt[leftLine++];
            accumulateRank++;
        }
        else{
            temp[addedPoint] = pt[rightLine++];
            temp[addedPoint].rk += accumulateRank;
            addedPoint++;
        }
    }

    while (leftLine <= m){ temp[addedPoint++] = pt[leftLine++]; }
    while (rightLine <= r){
            temp[addedPoint] = pt[rightLine++];
            temp[addedPoint].rk += accumulateRank;
            addedPoint++;
    }
    for (int i = 0;i < addedPoint;i++){
        swap(pt[l++], temp[i]);
    }

}
void HeapSort(vector<Pt> &pt){
    pt.insert(pt.begin(), Pt());
    buildMaxHeap(pt);
    int s = pt.size() - 1;

    for (int i = s;i >= 2;i--){
        swap(pt[1], pt[i]);
        s--;
        MaxHeapify(pt, 1, s);
    }
    pt.erase(pt.begin());

}

void buildMaxHeap(vector<Pt> &pt){
    for (int i = (pt.size()) / 2;i >= 1;i--)
        MaxHeapify(pt, i, pt.size() - 1);

}
void MaxHeapify(vector<Pt> &pt, int root, int length){
    int l = LeftNode(root),
        r = RightNode(root),
        largest = root;


    if (l <= length){
        if (pt[l].x > pt[root].x || (pt[l].x == pt[root].x && pt[l].y < pt[root].y))
            largest = l;

    }

    if (r <= length)
    {
        if (pt[r].x > pt[largest].x || (pt[r].x == pt[largest].x && pt[r].y < pt[largest].y))
            largest = r;
    }

    if (largest != root){
        swap(pt[root], pt[largest]);
        MaxHeapify(pt, largest, length);
    }

}

int LeftNode(int i){
    return 2 * i;
}

int RightNode(int i){
    return (2 * i) + 1;
}
