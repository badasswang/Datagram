#include <iostream>

#include "msg_list.h"

msg_list::msg_list(){
    front = back = NULL;
}

void msg_list::display() {
	msg_list_node *tmp;  int i;
	
	if(front==NULL) {
		cout << "** List is empty. **\n";
		return;
	}
	
	tmp = front;  i = 1;
	while(tmp!=NULL) {
		cout << "Datagram " << i++ << ":  \n";
		(tmp->d)->display();
		cout << "\n";
		tmp = tmp->next;
	}
	
}

void msg_list::append(datagram* x){
    msg_list_node* addOne = new msg_list_node;
    addOne->d = x;
    addOne->next = NULL;
    (*back).next = addOne;
    back = addOne;
}


