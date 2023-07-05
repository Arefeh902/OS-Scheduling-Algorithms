#include <iostream>
#include <vector>

#include <my_queue.h>

using namespace std;

enum ProcessState {NEW, READY, RUNNING, WAITING, TERMINATED};

class Process {
	ProcessState state;
	int process_id;
	int arrival_time;
	vector<int> process_bursts;
	int index_of_burst;

	Process(int process_id, int arrival_time, vector<int> process_bursts){
		this->state = NEW;
		this->process_id = process_id;
		this->arrival_time = arrival_time;

		this->process_bursts = process_bursts;; 
		this->index_of_burst = 0;
	}

	void set_state(ProcessState state){
		this->state = state;
	}
};

MY_Queue<Process> job_queue;

template <typename T>
class Scheduler {
	int num_of_process;

	T ready_queue;
	Process running;

	MY_Queue<Process> io_queue;
	Process using_io;

	MyClass() : num_of_process(0), running(nullptr), using_io(nullptr) {}

	void admit(int t){
		Process p = job_queue.top();
		job_queue.pop();

		ready_queue.push(p);
		p.set_state(READY);

		num_of_process += 1;

		printf("Admited process %d at time=%d", p->process_id, t);
	}

	Process dispatch(int t){
		running = ready_queue.top();
		ready_queue.pop();

		running.set_state(RUNNING);

		printf("Dispatched process %d at time=%d", running->process_id, t);
	}

	void preempt(int t){
		printf("Preempted process %d at time=%d", running->process_id, t);
		running.set_state(READY);
		ready_queue.push(running);
		running = NULL;
	}

	void io_request(Process p, int t){
		printf("IO requested by process %d at time=%d", running->process_id, t);
		p.set_state(WAITING);
		io_queue.push(p);
		if(using_io == NULL)
			using_io = p;
	}

	void io_completion(int t){
		printf("IO usage of process %d completed at time=%d", using_io->process_id, t);
		ready_queue.push(using_io);
		using_io.set_state(READY);
		using_io = NULL;
		if(!io_queue.empty()){
			using_io = io_queue.top();
			printf("IO given to process %d at time=%d", using_io->process_id, t);
			io_queue.pop();
		}
	}

	void terminate(int t){
		printf("process %d terminated at time=%d", running->process_id, t);
		running.set_state(TERMINATED);
		running = NULL;
		num_of_process -= 1;
	}

};



int main(){

}