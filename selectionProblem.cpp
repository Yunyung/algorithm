#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;


int find_Kth_smallest(vector<double> elements, int k)
{
	if (elements.size() <= 5) // if the number of elements is smaller or equal than 5, solve by brute method
	{
		sort(elements.begin(), elements.end());
		return elements[k - 1];
	}

	int group_num = ceil(elements.size() / 5.0);
	vector< vector<double> > groups_element(group_num);

	int group_index = 0;
	for (int i = 0;i < elements.size();i++)
	{
		groups_element[group_index].push_back(elements[i]);
		if (groups_element[group_index].size() == 5)
			group_index++;
	}

	while(groups_element[group_index].size() < 5)
		groups_element[group_index].push_back(numeric_limits<double>::max()); // max double num

	// sort each group
	for (int i = 0;i < groups_element.size();i++)
		sort(groups_element[i].begin(), groups_element[i].end());


	// Create the median list for each group to find the median of median
	vector<double> eachGroup_Medians;
	for (int i = 0;i < groups_element.size();i++)
	{
		eachGroup_Medians.push_back(groups_element[i][2]);
	}


	// find the median of median recursively
	double mid_element = find_Kth_smallest(eachGroup_Medians, (eachGroup_Medians.size() + 1) / 2);

	vector<double> s1, s2, s3;
	for (int i = 0;i < elements.size();i++)
	{
		if (elements[i] < mid_element)
			s1.push_back(elements[i]);
		else if (elements[i] == mid_element)
			s2.push_back(elements[i]);
		else 
			s3.push_back(elements[i]);
	}


	if (k <= s1.size())
		return find_Kth_smallest(s1, k);
	else if (k <= (s1.size() + s2.size()))
		return s2[0];
	else 
		return find_Kth_smallest( s3, k - (s1.size() + s2.size()) );



}



int main()
{
	// file preprocess
	string fname;
	cout << "Enter your txt file name (extendtion(.txt) is not needed to input) : ";
	cin >> fname;
	fname += ".txt";

	fstream file;
	file.open(fname.c_str(), ios::in);


	if (!file){
		cout << "Open file error" << endl;
		exit(1);
	}
	// read elements from file
	vector<double> elements;
	double temp;
	while (file >> temp){
		elements.push_back(temp);
	}

	while (1)
	{
		int k;

		cout << "Find k'th smallest element, enter your 'k' value : ";
		cin >> k;
		if (k < 1 || k > elements.size())
		{
			cout << "'k' value out of range!";
			continue;
		}
		cout << "The " << k << "'th smallest is -> " << find_Kth_smallest(elements, k) << "\n";
	}
		
}