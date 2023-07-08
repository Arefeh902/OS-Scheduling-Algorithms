#include <iostream>
#include <vector>
#include "../schedulers/MLFQ_Scheduler.h"
#include "../utils.h"

#define INF 1000000

using namespace std;

char get_min(int admit, int cpu, int io){
	if(admit <= cpu && admit <= io) return 'a';
	if(cpu <= io) return 'c';
	return 'i';
}

void MLFQ(){

    int num_of_levels = 3;
    vector<int> qts = {1, 3, INF};
	
	MLFQ_Scheduler scheduler = MLFQ_Scheduler(fill_job_queue(), num_of_levels, qts);
	int num_of_processes = scheduler.job_queue.size;
	scheduler.file << num_of_processes << endl;
	cout << num_of_processes << endl;
	
	int time = 0;
	int next_admit = scheduler.job_queue.top().arrival_time;
	int next_end_of_cpu_burst = INF;
	int next_end_of_io_burst = INF;
	Process* p = &NULL_PROCESS;
	while(scheduler.terminated_queue.size < num_of_processes){

		char mode = get_min(next_admit, next_end_of_cpu_burst, next_end_of_io_burst);
		cout << "==============================================" << endl;

		if(mode == 'a'){
			time = next_admit; 

			scheduler.admit(time);
			
			if(!scheduler.job_queue.empty()){
				next_admit = scheduler.job_queue.top().arrival_time;
			}else{
				next_admit = INF;
			}
		}
		
		
		else if(mode == 'c'){
			time = next_end_of_cpu_burst;

			p = &scheduler.running;

			if(p->bursts[p->index_of_burst] > scheduler.level_qt[p->queue_level]){
				scheduler.preempt(time);
			}
			
			else if(p->bursts[p->index_of_burst] <= scheduler.level_qt[p->queue_level]){
				if(p->index_of_burst < (p->num_of_bursts - 1)){
					p->index_of_burst += 1;

					if(scheduler.using_io == NULL_PROCESS)
						next_end_of_io_burst = time + p->bursts[p->index_of_burst];
					scheduler.io_request(time);
				}else{
					scheduler.terminate(time);
				}
			}

			
		}

		else if(mode == 'i'){
			time = next_end_of_io_burst;

			p = &scheduler.using_io;
			p->index_of_burst += 1;
			scheduler.io_completion(time);			
			
			if(scheduler.using_io != NULL_PROCESS){
				next_end_of_io_burst = time + scheduler.using_io.bursts[scheduler.using_io.index_of_burst];
			}else{
				next_end_of_io_burst = INF;
			}

			
		}

		if(scheduler.running == NULL_PROCESS){
			int i = 0;
            while(i<scheduler.ready_queue_levels && scheduler.ready_queue[i].empty()) i++;
			if(i < scheduler.ready_queue_levels){
				scheduler.dispatch(time);
				next_end_of_cpu_burst = min(time + scheduler.level_qt[scheduler.running.queue_level], time + scheduler.running.bursts[scheduler.running.index_of_burst]);
			}else{
				next_end_of_cpu_burst = INF;
			}
		}

	}

	cout << "==============================================" << endl;
	cout << "==============================================" << endl;
	cout << "utilization    : " << (scheduler.cpu_usage_time / scheduler.last_terminated_time) * 100 << endl;
	cout << "troughput      : " << num_of_processes / scheduler.last_terminated_time << endl;;
	cout << "waiting time   : " << scheduler.waiting_time_sum / num_of_processes << endl;
	cout << "turnaround time: " << scheduler.turn_around_time_sum / num_of_processes  << endl;
	cout << "response time  : " << scheduler.response_time_sum / num_of_processes << endl;

}


int main(){
	MLFQ();
}
