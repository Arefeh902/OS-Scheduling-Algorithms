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
Process NULL_PROCESS = Process();

class Scheduler {
	public:
	int num_of_process;	

	MY_Queue<Process> ready_queue;
	Process running;

	MY_Queue<Process> io_queue;
	Process using_io;


	int cpu_usage_time;
	int turn_around_time_sum;
	int waiting_time_sum;
	int last_terminated_time;
	int response_time_sum;


	MY_Queue<Process> terminated_queue;

	Scheduler() : num_of_process(0), running(NULL_PROCESS), using_io(NULL_PROCESS),
				cpu_usage_time(0), turn_around_time_sum(0), waiting_time_sum(0),
				last_terminated_time(0), response_time_sum(0) {}

	void admit(int t){
		Process p = job_queue.top();
		job_queue.pop();

		this->ready_queue.push(p);
		p.set_state(READY);
		
		num_of_process += 1;

		printf("time=%d: Admited process %d\n", p.process_id, t);
	}

	void dispatch(int t){
		running = ready_queue.top();
		ready_queue.pop();

		running.set_state(RUNNING);

		printf("time=%d: Dispatched process %d\n", t, running.process_id);
	}

	void preempt(int t){
		printf("time=%d:Preempted process %d\n", t, running.process_id);
		running.set_state(READY);
		ready_queue.push(running);
		running = NULL_PROCESS;
	}

	void io_request(int t){
		printf("time=%d: IO requested by process %d\n", t, running.process_id);
		running.set_state(WAITING);
		if(using_io == NULL_PROCESS){
			using_io = running;
		}else{
			io_queue.push(running);
		}
		running = NULL_PROCESS;
	}

	void io_completion(int t){
		printf("time=%d: IO usage of process %d completed\n", t, using_io.process_id);
		ready_queue.push(using_io);
		using_io.set_state(READY);
		using_io = NULL_PROCESS;
		if(!io_queue.empty()){
			using_io = io_queue.top();
			printf("time=%d: IO given to process %d\n", t, using_io.process_id);
			io_queue.pop();
		}
	}

	void terminate(int t){
		printf("time=%d: process %d terminated\n", t, running.process_id);
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