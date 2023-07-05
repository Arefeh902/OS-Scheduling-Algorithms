#include <iostream>
#include <vector>

using namespace std;

enum ProcessState {NEW, READY, RUNNING, WAITING, TERMINATED};

class Process {
	ProcessState state;
	int process_id;
	int arrival_time;
	vector<int> process_bursts;
	int index_of_burst;

	public:
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

	 bool operator==(const Process& other) const {
        return process_id == other.process_id;
    }
};
