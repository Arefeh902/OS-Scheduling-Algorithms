 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
 
using namespace std;
 
int main()
{
	string fname="tmp.csv";
 
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (fname, ios::in);
	if(file.is_open()){
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			
			vector<int> bursts;
			for(int i=2; i<row.size(); i++)
				bursts.push_back(stoi(row[i]));


			content.push_back(row);
		}
	}
	else
		cout<<"Could not open the file\n";
 
	return 0;
}