#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "../data_structures/my_queue.h"
#include "../process.h"

#define MAX_QUEUE_NUM 10

using namespace std;


vector<int> NULL_V;
Process NULL_PROCESS = Process();

class MLFQ_Scheduler {
	public:
	int num_of_process;	

	MY_Queue<Process> job_queue;

	MY_Queue<Process> ready_queue[MAX_QUEUE_NUM];
	Process running;
	int ready_queue_levels;
	vector<int> level_qt;

	MY_Queue<Process> io_queue;
	Process using_io;
	
	float last_dispatch_time;
	float cpu_usage_time;

	float waiting_time_sum;

	float turn_around_time_sum;
	float last_terminated_time;
	float response_time_sum;
	ofstream file;

	MY_Queue<Process> terminated_queue;

	MLFQ_Scheduler(MY_Queue<Process> job_queue, int levels, vector<int> qts) : num_of_process(0), running(NULL_PROCESS), using_io(NULL_PROCESS),
				last_dispatch_time(0), cpu_usage_time(0), turn_around_time_sum(0),
				waiting_time_sum(0), last_terminated_time(0), response_time_sum(0) {
				this->job_queue = job_queue;
				this->ready_queue_levels = levels;
				this->level_qt = qts;
				cout << "####################################" << endl;
				file.open("MLFQ.txt");
				}

	void admit(int t){
		Process p = job_queue.top();
		job_queue.pop();

		p.queue_level = 0;
		ready_queue[0].push(p);
		p.set_state(READY);
		p.last_entered_ready_queue = t;
		
		num_of_process += 1;

		file << "time=" << t << ", process_id=" << p.process_id << ", action=admit" << endl; 
		printf("time=%d: Admited process %d\n", p.process_id, t);
	}

	void dispatch(int t){
		int i=0;
		while(i<ready_queue_levels && ready_queue[i].empty()) i++;
		running = ready_queue[i].top();
		ready_queue[i].pop();

		running.set_state(RUNNING);
		waiting_time_sum += t - running.last_entered_ready_queue;

		last_dispatch_time = t;

		file << "time=" << t << ", process_id=" << running.process_id << ", action=dispatch" << endl; 
		printf("time=%d: Dispatched process %d for queue %d\n", t, running.process_id, i);
	}

	void preempt(int t){
		file << "time=" << t << ", process_id=" << running.process_id << ", action=preept" << endl; 

		running.set_state(READY);
		running.bursts[running.index_of_burst] -= t - last_dispatch_time;
		running.queue_level += 1;
		ready_queue[running.queue_level].push(running);

		printf("time=%d: Preempted process %d to queue %d\n", t, running.process_id, running.queue_level);

		running.last_entered_ready_queue = t;
		cpu_usage_time += t - last_dispatch_time;

		running = NULL_PROCESS;
	}

	void io_request(int t){
		file << "time=" << t << ", process_id=" << running.process_id << ", action=io_request" << endl; 
		printf("time=%d: IO requested by process %d\n", t, running.process_id);
		running.set_state(WAITING);

		cpu_usage_time += t - last_dispatch_time;

		if(running.index_of_burst == 1){
			response_time_sum += t - running.arrival_time;
		}

		if(using_io == NULL_PROCESS){
			using_io = running;
			file << "time=" << t << ", process_id=" << using_io.process_id << ", action=io_givento" << endl; 
			printf("time=%d: IO given to process %d\n", t, using_io.process_id);
		}else{
			io_queue.push(running);
		}

		running = NULL_PROCESS;
	}

	void io_completion(int t){
		file << "time=" << t << ", process_id=" << using_io.process_id << ", action=io_complete" << endl; 
		printf("time=%d: IO usage of process %d completed\n", t, using_io.process_id);
		
		using_io.queue_level = 0;
		ready_queue[using_io.queue_level].push(using_io);
		using_io.set_state(READY);
		running.last_entered_ready_queue = t;

		using_io = NULL_PROCESS;
		if(!io_queue.empty()){
			using_io = io_queue.top();
			file << "time=" << t << ", process_id=" << using_io.process_id << ", action=io_givento" << endl; 
			printf("time=%d: IO given to process %d\n", t, using_io.process_id);
			io_queue.pop();
		}
	}

	void terminate(int t){
		file << "time=" << t << ", process_id=" << running.process_id << ", action=terminate" << endl; 
		printf("time=%d: process %d terminated\n", t, running.process_id);
		
		running.set_state(TERMINATED);
		turn_around_time_sum += t - running.arrival_time;
		terminated_queue.push(running);

		cpu_usage_time += t - last_dispatch_time;

		
		running = NULL_PROCESS;
		num_of_process -= 1;

		last_terminated_time = t;
	}

};
