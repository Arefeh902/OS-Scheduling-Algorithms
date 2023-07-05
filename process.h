#include <iostream>
#include <vector>

using namespace std;

enum ProcessState {NEW, READY, RUNNING, WAITING, TERMINATED};

class Process {
	public:
	ProcessState state;
	int process_id;
	int arrival_time;
	vector<int> bursts;
	int index_of_burst;
	int num_of_bursts;

	public:
	Process(int process_id, int arrival_time, vector<int> bursts){
		this->state = NEW;
		this->process_id = process_id;
		this->arrival_time = arrival_time;

		this->bursts = bursts;
		this->index_of_burst = 0;
		this->num_of_bursts = 3;
	}

	void set_state(ProcessState state){
		this->state = state;
	}

	 bool operator==(const Process& other) const {
        return process_id == other.process_id;
    }
};
