#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "../data_structures/my_queue.h"
#include "../data_structures/my_heap.h"
#include "../process.h"
using namespace std;


vector<int> NULL_V;
Process NULL_PROCESS = Process();

class SJF_Scheduler {
	public:
	int num_of_process;	

    MY_Queue<Process> job_queue;
	
    My_Max_Heap<Process> ready_queue;
	Process running;

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

	SJF_Scheduler(MY_Queue<Process> job_queue) : num_of_process(0), running(NULL_PROCESS), using_io(NULL_PROCESS),
				last_dispatch_time(0), cpu_usage_time(0), turn_around_time_sum(0),
				waiting_time_sum(0), last_terminated_time(0), response_time_sum(0) {
					this->job_queue = job_queue;
					file.open("SJF.txt");  
				}

	void admit(int t){
		Process p = job_queue.top();
		job_queue.pop();

		this->ready_queue.push(p, -1*p.bursts[p.index_of_burst]);
		p.set_state(READY);
		p.last_entered_ready_queue = t;
		
		num_of_process += 1;

		file << "time=" << t << ", process_id=" << p.process_id << ", action=admit" << endl; 
		printf("time=%d: Admited process %d\n", t, p.process_id);
	}

	void dispatch(int t){
		
		running = ready_queue.top();
		ready_queue.pop();

		running.set_state(RUNNING);
		waiting_time_sum += t - running.last_entered_ready_queue;

		last_dispatch_time = t;
		file << "time=" << t << ", process_id=" << running.process_id << ", action=dispatch" << endl; 
		printf("time=%d: Dispatched process %d\n", t, running.process_id);
	}

	void preempt(int t){
		file << "time=" << t << ", process_id=" << running.process_id << ", action=preept" << endl; 
		printf("time=%d: Preempted process %d\n", t, running.process_id);
		running.set_state(READY);
		ready_queue.push(running, -1*running.bursts[running.index_of_burst]);

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
		
		ready_queue.push(using_io, -1*using_io.bursts[using_io.index_of_burst]);
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
