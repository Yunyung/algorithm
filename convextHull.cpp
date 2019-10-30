#include<iostream>
#include<cmath>
#include<stack>
#include<fstream>

#define PI 3.1415926
using namespace std;

struct Node {
	double x;
	double y;
	double angle;
	int id;
};


// function declaration
double GetDistance(double x1, double y1, double x2, double y2);
double GetAngle(double x1, double y1, double x2, double y2);
int orientation(Node a, Node b, Node c);
Node next_node(stack<Node> &S);
Node after_next_node(stack<Node> &S);
int next_idx(stack<int> &I);
void qs(Node *node, int first, int last);
void check_qs(Node *node, int first, int last);
void divide_node(Node *node, int first, int last);
void merge_dc(Node *node, int first, int last);


// global varible  
int n = 0; // The number of Nodes 
stack<int> I; 
stack<Node> S; 
stack<Node> CCW; 
int Total_num_node; // The number of convexhull point
bool exist[2000]; 

int main() {
	fstream file;
	file.open("test1.txt", ios::in); // open file
	
	if (!file) { // if file open succesfully 
		cout << "Open File Error" << endl;
		exit(1); 
	}
	
	double node_x, node_y;
	int first, last;

	Node *node;
	node = new Node[2000]; 
	
	if (!node) { 
		cout << "Memory Allocation Error!" << endl;
		exit(1);
	}
	
	while (file >> node_x >> node_y) { 
		node[n].x = node_x; // store each node
		node[n].y = node_y;
		node[n].id = n + 1; // given node id
		n += 1;
	}

	cout << "Input point : \n";
	for (int i = 0;i < n;i++)
	{
		cout << node[i].id << ". " << "(" << node[i].x << ", " << node[i].y << ")"  << endl;
	}
	cout << "\n";
	cout << "--------------------------------------------------------\n";
	file.close(); // close file
	for (int i = 0; i <= 2000; i++)
	    exist[i] = true;
	
	first = 0;
	last = n - 1;
	
	qs(node, first, last);
	check_qs(node, first, last);
	divide_node(node, first, last);
	
	system("pause");
	return 0;
}



double GetDistance(double x1, double y1, double x2, double y2) { // cal distance
	double x, y, dist;
	
	x = x2 - x1;
	y = y2 - y1;
	dist = x * x + y * y;
	
	return sqrt(dist);
}

double GetAngle(double x1, double y1, double x2, double y2) { // calculate angle
	double x, y, angle;
	
	x = x2 - x1;
	y = y2 - y1;
	
	if (x == 0) 
		angle = 180.0 / 2; // 90 
	else 
		angle = atan(fabs(y / x)) * 180.0 / PI; 
	
	if (x < 0 && y >= 0) 
		angle = 180.0 - angle;
	else if (x < 0 && y < 0)  
		angle = 180.0 + angle;
	else if (x > 0 && y <= 0) 
		angle = 2 * 180.0 - angle;
	
	return angle;
}

int orientation(Node a, Node b, Node c) { // collinear or CW or CCW
	double cross;
	
	cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	
	if (cross == 0) // collinear 
		return 0;
	else if (cross < 0) // CW 
		return 1;
	else // if (value > 0) CCW
		return 2;
}

Node next_node(stack<Node> &S) { 
	Node c = S.top(); 
	S.pop(); 
	Node b = S.top(); 
	S.push(c); 
	
	return b;
}

Node after_next_node(stack<Node> &S) { 
	Node c = S.top(); 
	S.pop(); 
	Node b = S.top(); 
	S.pop(); 
	Node a = S.top(); 
	
	S.push(b); 
	S.push(c); 
	
	return a;
}

int next_idx(stack<int> &I) {
	int c = I.top();
	I.pop(); 
	int b = I.top(); 
	I.push(c); 
	
	return b;
}


// quick sort for sorting
void qs(Node *node, int first, int last) {
	double split = node[first].x;
	int i = first + 1, j = last;
	
	while (i <= j) { 
		while (node[i].x <= split && i <= last)
			i++;
		while (node[j].x >= split && j >= (first + 1))
			j--;
		
		if (i < j) { // swap
			swap(node[i].x, node[j].x);
			swap(node[i].y, node[j].y);
			swap(node[i].id, node[j].id);
		}
	}
	
	swap(node[first].x, node[j].x); // swap first and target
	swap(node[first].y, node[j].y);
	swap(node[first].id, node[j].id);
	
	if (first < (j - 1))
		qs(node, first, j - 1); 
	if ((j + 1) < last)
		qs(node, j + 1, last);  
}

void check_qs(Node *node, int first, int last) { 
	for (int i = first; i < last; i++)
		for (int j = i + 1; j < last + 1; j++)
			if (node[i].x == node[j].x && node[i].y > node[j].y) 
				swap(node[i], node[j]);
}

void merge_dc(Node *node, int first, int last) { // merge 
	int min_y = node[first].y; 
	int left_down = first; // the lowest and leftest
	
	for (int i = first + 1; i < last + 1; i++) { 
		if (node[i].y < min_y) { 
			min_y = node[i].y;
			left_down = i;
		}
		else if (node[i].y == min_y) 
			if (node[i].x < node[left_down].x)
				left_down = i;
	}
	swap(node[first], node[left_down]); 
	
	node[first].angle = 0.0;
	
	for (int i = first + 1; i < last + 1; i++) 
		if (exist[i] == true)
			node[i].angle = GetAngle(node[first].x, node[first].y, node[i].x, node[i].y);
	
	for (int i = first; i < last + 1; i++) { // sort by angle
		double min = 360.0;
		int k;
		
		if (exist[i] == true) {
			for (int j = i; j < last + 1; j++){
				if (exist[j] == true) {	
					if (node[j].angle < min) {
						min = node[j].angle;
						k = j;
					}
				}
			}
			swap(node[i], node[k]);
		}
	}
	for (int i = first + 1; i < last; i++)
		if (exist[i] == true)
			for (int j = i + 1; j < last + 1; j++)
				if (exist[j] == true) 
					if (node[i].angle == node[j].angle && GetDistance(node[first].x, node[first].y, node[i].x, node[i].y) > GetDistance(node[first].x, node[first].y, node[j].x, node[j].y))
						swap(node[i], node[j]);
	
	for (int i = first + 1; i < last + 1; i++) {
		while (i + 1 < last + 1 && orientation(node[first], node[i], node[i + 1]) == 0) { // 角度相同時，距離大的留著 
			exist[i] = false;
			i++;
		}
	}
	int many = 0; 
	
	for (int i = first; i < last + 1; i++) { 
		if (many == 2)
			break;
		
		if (exist[i] == true) { 
			if (exist[i] == true) {
				S.push(node[i]);
				I.push(i);
				many++;
			}
		}
	}
	for (int i = I.top() + 1; i < last + 1; i++) {
		if (exist[i] == true) {
			while (S.size() >= 2 && (orientation(next_node(S), S.top(), node[i]) == 1)) { 
				exist[I.top()] = false;
				S.pop(); // if cw, discard
				I.pop();
			}
			S.push(node[i]);
			I.push(i);
			
			// collinear, discard 
			if (orientation(after_next_node(S), next_node(S), node[i]) == 0) {
				exist[next_idx(I)] = false; 
				S.pop(); 
				I.pop();
				S.pop(); 
				I.pop();
				S.push(node[i]); 
				I.push(i);
			}
		}
	}
	Total_num_node = S.size();
	
	for (int i = 0; i < Total_num_node; i++) {
		CCW.push(S.top());
		S.pop();
	}  
	if ((first == 0 && last == n - 1) || (first == 0 && last == (n - 1) / 2) || (first == ((n - 1) / 2 + 1) && last == n - 1)) {
		if (first == 0 && last == (n - 1) / 2)
			cout << "Left -> Convex hull:" << endl;
		else if (first == ((n - 1) / 2 + 1) && last == n - 1)
			cout << "Right -> Convex hull:" << endl;
		else
			cout << "After Merge -> Convex hull:" << endl;
		
		Node arr[CCW.size()];
		int ccw_size = CCW.size(), j = 0;
		
		while(!CCW.empty()) {
			Node top = CCW.top();
			arr[j] = top;
			j++;
			CCW.pop();
		}
		int min = 1000, min_x;
		
		for (int i = 0; i < ccw_size; i++) {
			if (arr[i].x < min) {
				min = arr[i].x;
				min_x = i;
			}
		}
		for (int i = min_x; i < ccw_size; i++)
			cout << arr[i].id << ". (" << arr[i].x << ", " << arr[i].y << ")" << endl;
		for (int i = 0; i < min_x; i++)
			cout << arr[i].id << ". (" << arr[i].x << ", " << arr[i].y << ")" << endl;
		cout << "Total " << Total_num_node << " point" << endl << endl;
	}
	while(!CCW.empty())
		CCW.pop();
}

void divide_node(Node *node, int first, int last) { // divide
	int mid = (first + last) / 2; // median 
	
	if (first + 0 >= last || first + 1 >= last) // terminate bound
		return;
	if (first < mid)
		divide_node(node, first, mid); // left recursive
	if ((mid + 1) < last)
		divide_node(node, mid + 1, last); // right recursive
	
	merge_dc(node, first, last);  // merge process
}