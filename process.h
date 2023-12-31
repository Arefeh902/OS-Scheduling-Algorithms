#pragma once

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
	int last_entered_ready_queue;
	int queue_level;
	
	Process(){
		this->state = NEW;
		this->process_id = 0;
		this->arrival_time = 0;

		vector<int> v;
		this->bursts = v;
		this->index_of_burst = 0;
		this->num_of_bursts = 0;
		this->last_entered_ready_queue = 0;
		this->queue_level = -1;
	}

	Process(int process_id, int arrival_time, vector<int> bursts){
		this->state = NEW;
		this->process_id = process_id;
		this->arrival_time = arrival_time;

		this->bursts = bursts;
		this->index_of_burst = 0;
		this->num_of_bursts = 3;
		this->last_entered_ready_queue = 0;;
		this->queue_level = -1;
	}

	void set_state(ProcessState state){
		this->state = state;
	}

	bool operator==(const Process& other) const {
		return process_id == other.process_id;
	}
	
	bool operator!=(const Process& other) const {
		return process_id != other.process_id;
	}

	bool operator<(const Process& other) const {
		return process_id < other.process_id;
	}

	bool operator>(const Process& other) const {
		return process_id > other.process_id;
	}

	friend std::ostream& operator<<(std::ostream& os, const Process& obj) {
        os << obj.process_id;
        return os;
    }
};
