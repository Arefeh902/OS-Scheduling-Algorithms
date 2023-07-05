#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "my_queue.h"
#include "process.h"
using namespace std;

MY_Queue<Process> job_queue;
vector<int> NULL_V;
Process NULL_PROCESS = Process(0, 0, NULL_V);

template <typename T>
class Scheduler {
	public:
	int num_of_process;

	T ready_queue;
	Process running;

	MY_Queue<Process> io_queue;
	Process using_io;

	MY_Queue<Process> terminated_queue;

	Scheduler() : num_of_process(0), running(NULL_PROCESS), using_io(NULL_PROCESS) {}

	void admit(int t){
		Process p = job_queue.top();
		job_queue.pop();

		ready_queue.push(p);
		p.set_state(READY);
		
		num_of_process += 1;

		printf("Admited process %d at time=%d", p.process_id, t);
	}

	void dispatch(int t){
		running = ready_queue.top();
		ready_queue.pop();

		running.set_state(RUNNING);

		printf("Dispatched process %d at time=%d", running.process_id, t);
	}

	void preempt(int t){
		printf("Preempted process %d at time=%d", running.process_id, t);
		running.set_state(READY);
		ready_queue.push(running);
		running = NULL_PROCESS;
	}

	void io_request(int t){
		printf("IO requested by process %d at time=%d", running.process_id, t);
		running.set_state(WAITING);
		io_queue.push(running);
		if(using_io == NULL_PROCESS)
			using_io = running;
		running = NULL_PROCESS;
	}

	void io_completion(int t){
		printf("IO usage of process %d completed at time=%d", using_io.process_id, t);
		ready_queue.push(using_io);
		using_io.set_state(READY);
		using_io = NULL_PROCESS;
		if(!io_queue.empty()){
			using_io = io_queue.top();
			printf("IO given to process %d at time=%d", using_io.process_id, t);
			io_queue.pop();
		}
	}

	void terminate(int t){
		printf("process %d terminated at time=%d", running.process_id, t);
		running.set_state(TERMINATED);
		terminated_queue.push(running);
		running = NULL_PROCESS;
		num_of_process -= 1;
	}

};

void fill_job_queue(){

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

}