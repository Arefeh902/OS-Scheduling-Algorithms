#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "my_queue.h"
#include "process.h"
using namespace std;

MY_Queue<Process> fill_job_queue(){

    MY_Queue<Process> job_queue;

	string fname="tmp.csv";

	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (fname, ios::in);
	if(file.is_open()){
		while(getline(file, line)){
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			
			vector<int> bursts;
			for(int i=2; i<row.size(); i++)
				bursts.push_back(stoi(row[i]));
			Process tmp = Process(stoi(row[0]), stoi(row[1]), bursts);
			job_queue.push(tmp);
		}
	}
	else cout<<"Could not open the file\n";

    return job_queue;
}