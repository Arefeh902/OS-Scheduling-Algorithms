#pragma once

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class My_Max_Heap {
public:
	struct Node {
		T data;
		int priority;
		Node(T value, int p) : data(value), priority(p) {}
	};

	size_t size;
	vector<Node> arr;

	My_Max_Heap(){
		this->size = 0;
	}

	bool empty() { return size == 0; }

	void push(T data, int priority) {
		arr.push_back(Node(data, priority));
		int tmp = size;
		while (tmp > 1 && arr[tmp].priority > arr[(tmp-1)/2].priority) {
			Node swap_helper = arr[(tmp-1)/2];
			arr[(tmp-1)/2] = arr[tmp];
			arr[tmp] = swap_helper;
			tmp = (tmp-1)/2;
		}
		size += 1;
	}

	T top() {
		if (!empty()) {
			return arr[0].data;
		}
		return T();
	}

	void pop() {
		arr[0] = arr[size-1];
		arr.pop_back();
		size -= 1;

		int tmp = 0;
		while (2*tmp + 2 < size) {
			int mx = arr[tmp*2+1].priority > arr[tmp*2+2].priority ? tmp*2+1 : tmp*2+2;
			if (arr[tmp].priority < arr[mx].priority) {
				Node swap_helper = arr[tmp];
				arr[tmp] = arr[mx];
				arr[mx] = swap_helper;
				tmp = mx;
			}
		}
		if(2*tmp+1 <= size && arr[2*tmp+1].priority > arr[tmp].priority){
			Node swap_helper = arr[tmp];
			arr[tmp] = arr[2*tmp+1];
			arr[2*tmp+1] = swap_helper;
			
		}
	}
};
