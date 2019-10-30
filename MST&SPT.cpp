#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <stack>
using namespace std;

class Node
{
public:
	string status;
	string path = "";
	int misplace;
	Node(string status, string path, int misplace)
	{
		this->status = status;
		this->path = path;
		this->misplace = misplace;
	}


	int ToSequence()
	{
		int result = 0;
		for (int i = 0;i < status.length();i++)
			result = result * 10 + status[i];
		return result;
	}
};

bool HillClimbingSortCompare(Node x, Node y)
{
	return x.misplace >= y.misplace;
}

int calMisplace(string s, Node end)
{
	int full = 9;
	int correct = 0;
	for (int i = 0;i < 9;i++)
	{
		if (s[i] == end.status[i])
		{
			correct++;
		}
	}
	return (9 - correct);
}

vector<Node> GetNext(Node now, Node end)
{
	int zero_index = now.status.find("0", 0); // find the location of empty space(sign -> 0)
	int col = zero_index % 3;
	int row = zero_index / 3;

	vector<Node> nextPush;
	string next;

	if (row != 0) // enable 0 swap with Top
	{
		next = now.status;
		swap(next[zero_index], next[zero_index - 3]); // move
		Node n(next, now.path + "D", calMisplace(next, end));
		nextPush.push_back(n);      // add new puzzle state
	}
	if (col != 2) // enable 0 swap with right
	{
		next = now.status;
		swap(next[zero_index], next[zero_index + 1]); // move
		Node n(next, now.path + "L", calMisplace(next, end));
		nextPush.push_back(n);      // add new puzzle state
	}

	if (row != 2) // enable 0 swap with Bottom
	{
		next = now.status;
		swap(next[zero_index], next[zero_index + 3]); // move
		Node n(next, now.path + "U", calMisplace(next, end));
		nextPush.push_back(n);      // add new puzzle state
	}
	if (col != 0) // enable 0 swap with left
	{
		next = now.status;
		swap(next[zero_index], next[zero_index - 1]); // move
		Node n(next, now.path + "R", calMisplace(next, end));
		nextPush.push_back(n);      // add new puzzle state
	}

	return nextPush;
}


Node Solver(string source, string goal)
{

	Node end(goal, "", 0); // goal status
	Node start(source, "", calMisplace(source, end)); // start status
	stack<Node> stackN;
	stackN.push(start);				 // push initial state to stack

	set<int> status_set;             // record all puzzle status have been checked. Use STL -> set for O(logN) for fast search
	status_set.insert(start.ToSequence());  // insert first status to set


	int endStatus = end.ToSequence();
	while (stackN.size() > 0)
	{
		Node now = stackN.top();	// pop the element at top of the stack
		stackN.pop();
		// cout << "pop-> status : " << now.status << ", path = " << now.path << ", misplace = " << now.misplace << endl;
		if (now.ToSequence() == endStatus)	// compare whether the top element reach the goal status
		{
			return now;
		}

		vector<Node> nextPath = GetNext(now, end);
		// To using Hill climbing method, namely -> local greedy method, choose the local best status min(f(n)) -> f(n) = level + misplace
		// first sort the next path by f(n)
		sort(nextPath.begin(), nextPath.end(), HillClimbingSortCompare);
		for (int i = 0;i < nextPath.size();i++)
		{
			int sign = nextPath[i].ToSequence();
			if (!status_set.count(sign))		// if this puzzle status not in set(never checked), push into stack by f(n) oder
			{
				// cout << "add node " << nextPath[i].status << ", misplace = " << nextPath[i].misplace << endl;
				stackN.push(nextPath[i]);
				status_set.insert(sign);
			}
		}

	}

	cout << "No solution";
	exit(1);



}

void printStep(string initialStatus, Node p)
{
	string next = initialStatus;
	string path = p.path;
	for (int i = 0;i < p.path.length();i++)
	{
		int zero_index = next.find("0", 0); // find the location of empty space(sign -> 0)
		char direction = path[i];
		cout << "Step" << i << " : "; 
		switch(direction)
		{
			case 'U':
				cout << next[zero_index + 3];
				cout << " UP" << endl;
				swap(next[zero_index], next[zero_index + 3]); // move
			break;
			case 'R':
				cout << next[zero_index - 1];
				cout << " Right" << endl;
				swap(next[zero_index], next[zero_index - 1]); // move
			break;
			case 'D':
				cout << next[zero_index - 3];
				cout << " Down" << endl;
				swap(next[zero_index], next[zero_index - 3]); // move
			break;
			case 'L':
				cout << next[zero_index + 1];
				cout << " Left" << endl;
				swap(next[zero_index], next[zero_index + 1]); // move
			break;
		}

	}
	cout << "\nTotal path length by Hill climbing : " << p.path.length();
}

int main()
{
	// Get input
	cout << "Input your initial puzzle status: (Ex: 2 8 3 1 0 4 7 6 5):";
	string initialStatus;
	int temp;
	for (int i = 0;i < 9;i++)
	{
		cin >> temp;
		initialStatus += to_string(temp);
	} 
	cout << "Initial puzzle : " << initialStatus << endl;
	cout << "Goal puzzle : " << "123804765" << endl;
	cout << "--------------------------------------------------------------" << endl;
 	Node p = Solver(initialStatus, "123804765");
	printStep(initialStatus, p);
}
