#include <stdio.h>
#include <stdlib.h>
#include "my_set.h"
#include <assert.h>


typedef struct node_t* Node;

struct node_t {
	MySetElement element;
	Node next;
	};

struct MySet_t{
	Node list;
	Node head;
	int set_size;
	copyMySetElements copyElement;
	freeMySetElements freeElement;
	compareMySetElements compareElements;
};
//inline or macro
static void Add_And_Link(Node *new_node, Node *next_node){
	(*new_node)->next = *next_node;
	*next_node = *new_node;
}

static MySetElement Remove_And_Return(Node *link, bool first){
	if (first) {
		MySetElement to_return = (*link)->element;
		Node tmp = *link;
		*link = (*link)->next;
		free(tmp);
		return to_return;
	}
	else {
		MySetElement to_return = (*link)->next->element;
		Node tmp = (*link)->next;
		(*link)->next = (*link)->next->next;
		free(tmp);
		return to_return;
	}

}

#define MY_SET_INNER_FOREACH(type,iterator,set) \
	for(type iterator = set->head ; \
		iterator; \
		iterator = iterator->next)

static Node listCreate(MySetElement element) {
	Node node = (Node) malloc(sizeof(struct node_t));
	if(!node) {
		return NULL;
	}
	node->element = element;
	node->next = NULL;
	return node;
}

static void listDestroy(MySet set) {
	Node ptr = set->head;
	while(ptr) {
		Node to_delete = ptr;
		ptr = ptr->next;
		set->freeElement(to_delete->element);
		free(to_delete);
	}
}

static MySetResult listInsertAfter(MySet set, MySetElement new_element){
	Node new_node = listCreate(new_element);
	if(!new_node){
		return MY_SET_OUT_OF_MEMORY;
	}
	 Node head = set->head;
	if(!(head && set->compareElements(head->element, new_node->element)<0)){
		Add_And_Link(&new_node, &head);
		set->head = new_node;
	}
	else {
		while(head->next && set->compareElements(head->next->element, new_node->element) < 0){
			head = head->next;
		}
		Add_And_Link(&new_node, &head->next);
	}
	set->list = NULL;
	return MY_SET_SUCCESS;

}

static Node findNodeOfElement(MySet set, MySetElement element){
	MY_SET_INNER_FOREACH(Node, itr, set){ //find function
			if(!set->compareElements(itr->element, element)){
				return itr;
			}
	}
	return NULL;
}

static inline bool true_condition(){
	return true;
}

MySet mySetCreate(copyMySetElements copyElement, freeMySetElements freeElement, compareMySetElements compareElements){
	if(!(copyElement && freeElement && compareElements)){
		return NULL;
	}
	MySet set = (MySet) malloc(sizeof(struct MySet_t));
	if(!set){
		return NULL;
	}
	set->list = NULL;
	set->head = NULL;
	set->compareElements = compareElements;
	set->copyElement = copyElement;
	set->freeElement = freeElement;
	set->set_size = 0;
	return set;
}

MySet mySetCopy(MySet set){
	if(!set){
		return NULL;
	}
	MySet set_copy = mySetFilter(set, true_condition);
	set_copy->set_size = set->set_size;
	return set_copy;
}

void mySetDestroy(MySet set){
	mySetClear(set);
	free(set);
}

int mySetGetSize(MySet set){
	return set->set_size;
}

bool mySetIsIn(MySet set, MySetElement element){	//What with the iterator ?
	if(!set || !element){
		return NULL;
	}
	bool result = false;
	Node tmp = findNodeOfElement(set, element);
		if(tmp){
			result = true;
		}
	return result;
}

MySetElement mySetGetFirst(MySet set){
	if(!set || !set->head){
		return NULL;
	}
	set->list = set->head;
	return set->list->element;
}

MySetElement mySetGetNext(MySet set){
	if(!set || !set->list){
		return NULL;
	}
	set->list = set->list->next;

	return !set->list ? NULL : set->list->element;

}

MySetElement mySetGetCurrent(MySet set){
	if(!set || !set->list){
		return NULL;
	}
	return set->list->element;
}

MySetResult mySetAdd(MySet set, MySetElement element){
	if(!set ||!element){
		return MY_SET_NULL_ARGUMENT;
	}
	if(mySetIsIn(set, element))
		return MY_SET_ITEM_ALREADY_EXISTS;
	MySetElement element_copy = set->copyElement(element); //copy NULL?
	MySetResult result = listInsertAfter(set, element_copy);
	set->set_size += !result;
	return result;
}

MySetResult mySetRemove(MySet set, MySetElement element){
	if(!set || !element){
		return MY_SET_NULL_ARGUMENT;
	}
	MySetElement to_delete = mySetExtract(set, element);
	if(!to_delete){
		return MY_SET_ITEM_DOES_NOT_EXIST;
	}
	set->freeElement(to_delete);
	return MY_SET_SUCCESS;
}

MySetElement mySetExtract(MySet set, MySetElement element){
	if(!set || !element || !mySetIsIn(set, element)){
			return NULL;
		}
	Node list = set->head;
		MySetElement to_delete = NULL;
		bool first = true;
		if (set->compareElements(element, list->element)) {
			first = false;
			while (list->next && set->compareElements(list->next->element, element)) {
				list = list->next;
			}
		}
		to_delete = Remove_And_Return(&list, first);
		if (first) {
			set->head = list;
		}
		set->set_size--;
		return to_delete;
}

MySetResult mySetClear(MySet set){
	if(!set){
		return MY_SET_NULL_ARGUMENT;
	}
	listDestroy(set);
	set->head = NULL;
	return MY_SET_SUCCESS;
}

MySet mySetFilter(MySet set, logicalCondition condition){
	if(!set || !condition){
		return NULL;
	}

	MySet new_set = mySetCreate(set->copyElement, set->freeElement, set->compareElements);
	if(!new_set){
		return NULL;
	}
	MY_SET_INNER_FOREACH(Node, itr, set){
		if(condition(itr->element)){
			if(mySetAdd(new_set, itr->element) == MY_SET_OUT_OF_MEMORY){
				mySetDestroy(new_set);
				return NULL;

			}

		}
	}
	return new_set;
}

/*
int main(){
	MySet set = mySetCreate(copyInt, freeInt, compareInt);


}
*/

/*tmp = list;
			 to_return = list->element;
			 list = list->next;
			 free(tmp);
			 return to_return; */


/*
			tmp = list;
			to_return = list->element;
			previous = list->next;
			free(tmp);
			return to_return; */

