#include <iostream>
#include <vector>

// push to end
// pop from front
template <typename T>
class MY_Queue {
	public:
	struct Node {
		T data;
		Node* next;
		Node(T value) : data(value), next(nullptr) {}
	};

	Node* first_node;
	Node* last_node;
	size_t size;

	bool empty(){ return size == 0; }

	void push(T data){
		Node* new_node= new Node(data);
		if(empty()){
			first_node = new_node;
			last_node = new_node;
		} 
		new_node->next = nullptr;
		last_node->next = new_node;
		last_node = new_node;
		size += 1;
	}

	T top(){ 
		if(!empty()){
			return first_node->data;
		}
		return T();
	} 

	void pop(){
		Node* tmp = first_node;
		first_node = first_node->next;
		delete tmp;
		size -= 1;
	}

};
