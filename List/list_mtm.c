/*
 * list.c
 *
 *  Created on: 21 באפר 2016
 *      Author: batel
 */

#include "list_mtm.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_t* Node;

struct node_t {
	ListElement data;
	Node next;
};

struct List_t {
	Node head;
	Node iterator;
	int size;
	CopyListElement copy;
	FreeListElement free;
};

/********************************Static Function****************************/
/**************************************************************************/
static Node listCreateNode(ListElement data, CopyListElement copyFunction) {
	assert(data || copyFunction);
	Node node = malloc(sizeof(*node));
	if (!node) {
		return NULL;
	}
	node->data = copyFunction(data);
	if (!node->data) {
		free(node);
		return NULL;
	}
	node->next = NULL;
	return node;
}

static void listDestroyNode(Node to_destroy, FreeListElement destroyElement){
	assert(destroyElement);
	if (!to_destroy) {
		return;
	}
	destroyElement(to_destroy->data);
	free(to_destroy);
	return;
}

static Node listGetBeforeCurrent(List list) {
	assert(list->head != list->iterator);
	Node before_current = NULL;
	for (Node ptr_head = list->head; ptr_head; ptr_head = ptr_head->next) {
		if (ptr_head->next == list->iterator) {
			before_current = ptr_head;
		}
	}
	assert(before_current);
	return before_current;
}

static void swap(ListElement array[], int i, int j) {
	ListElement tmp = array[i];
	array[i] = array[j];
	array[j] = tmp;
}

static int partition(ListElement* array, int length,
		CompareListElements compareFunction) {
	int right = length - 1;
	int left = 0;
	ListElement pivot = array[0];
	while (left <= right) {
		while ( (left <= right)
				&& (compareFunction(array[right], pivot)>0)){
			right--;
		}
		while ( (left<= right)
				&& (compareFunction(array[left], pivot)<=0)){
			left++;
		}
		if (left < right) {
			swap(array, left, right);
			left++;
			right--;
		}
	}
	return right;
}

static void quickSort(ListElement array[], int n,
		CompareListElements compareFunction) {
	if (n < 2)
		return;
	int t = partition(array, n, compareFunction);
	swap(array, 0, t);
	quickSort(array, t, compareFunction);
	quickSort(array + t + 1, n - t - 1, compareFunction);
}

static void freeArray(ListElement* array, int size,
		FreeListElement destroyElement) {
	assert(destroyElement && array);
	for (int i = 0; i < size; i++) {
		destroyElement(array[i]);
	}
	free(array);
	return;
}

static ListResult listNodeToArray(List list, ListElement* array, int size) {
	assert(list != NULL && array != NULL && size >= 0);
	Node ptr_head = list->head;
	for (int i = 0; i < size; i++) {
		assert(ptr_head);
		array[i] = (list->copy)(ptr_head->data);
		if (!array[i]) {
			freeArray(array, i, list->free);
			return LIST_OUT_OF_MEMORY;
		}
		ptr_head = ptr_head->next;
	}
	return LIST_SUCCESS;
}

static int getIteratorLocation(List list) {
	assert(list);
	int size = listGetSize(list);
	int location=-1;
	Node ptr_head = list->head;
	for (int i = 0; i < size; i++) {
		if (list->iterator == ptr_head) {
			location = i;
			break;
		}
		ptr_head = ptr_head->next;
	}
	assert(location < size);
	return location;
}

/********************************List Function****************************/
/**************************************************************************/

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	if (!copyElement || !freeElement) {
		return NULL;
	}
	List new_list = malloc(sizeof(*new_list));
	if (!new_list) {
		return NULL;
	}
	new_list->head = NULL;
	new_list->iterator = NULL;
	new_list->size = 0;
	new_list->copy = copyElement;
	new_list->free = freeElement;

	return new_list;
}

List listCopy(List list) {
	if (!list) {
		return NULL;
	}
	List list_copy = listCreate(list->copy, list->free);
	if (!list_copy) {
		return NULL;
	}
	list_copy->size = list->size;
	if (list_copy->size == 0) {
		return list_copy;
	}
	list_copy->head = listCreateNode(list->head->data, list->copy);
	if (!list_copy->head) {
		listDestroy(list_copy);
		return NULL;
	}
	if (list->head == list->iterator) {
		list_copy->iterator = list_copy->head;
	}
	Node ptr_copy = list_copy->head, ptr_list = list->head;
	for (ptr_list = ptr_list->next; ptr_list; ptr_list = ptr_list->next) {
		ptr_copy->next = listCreateNode(ptr_list->data, list->copy);
		if (!ptr_copy->next) {
			listDestroy(list_copy);
			return NULL;
		}
		if (ptr_list == list->iterator) {
			list_copy->iterator = ptr_copy->next;
		}
		ptr_copy = ptr_copy->next;
	}
	return list_copy;
}

int listGetSize(List list) {
	return (!list) ? -1 : list->size;
}

ListElement listGetFirst(List list) {
	if (!list || list->size == 0) {
		return NULL;
	}
	list->iterator = list->head;
	return list->head->data;
}

ListElement listGetNext(List list) {
	if (!list || !list->iterator) {
		return NULL;
	}
	list->iterator = list->iterator->next;
	return listGetCurrent(list);
}

ListElement listGetCurrent(List list) {
	return (!list || !list->iterator) ? NULL: list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	Node new_node = listCreateNode(element, list->copy);
	if (!new_node) {
		return LIST_OUT_OF_MEMORY;
	}
	if (list->head) {
		new_node->next = list->head;
	}
	list->head = new_node;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	} else if (!list->head) {
		return listInsertFirst(list, element);
	}
	Node new_node = listCreateNode(element, list->copy);
	if (!new_node) {
		return LIST_OUT_OF_MEMORY;
	}
	Node last_node=NULL;
	for (Node ptr_list = list->head; ptr_list; ptr_list = ptr_list->next){
		last_node=ptr_list;
	}
	assert(last_node);
	last_node->next = new_node;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}else if (!list->iterator) {
		return LIST_INVALID_CURRENT;
	} else if (list->iterator == list->head) {
		return listInsertFirst(list, element);
	}
	Node new_node = listCreateNode(element, list->copy);
	if (!new_node) {
		return LIST_OUT_OF_MEMORY;
	}
	Node before_current = listGetBeforeCurrent(list);
	assert(before_current);
	new_node->next = list->iterator;
	before_current->next = new_node;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	} else if (!list->iterator) {
		return LIST_INVALID_CURRENT;
	}
	Node new_node = listCreateNode(element, list->copy);
	if (!new_node) {
		return LIST_OUT_OF_MEMORY;
	}
	new_node->next = list->iterator->next;
	list->iterator->next = new_node;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	} else if (!list->iterator) {
		return LIST_INVALID_CURRENT;
	}
	Node to_delete = list->iterator;
	if (list->iterator == list->head) {
		list->head = list->head->next;
	} else {
		Node before_current = listGetBeforeCurrent(list);
		assert(before_current);
		before_current->next = list->iterator->next;
	}
	listDestroyNode(to_delete, list->free);
	list->iterator = NULL;
	(list->size)--;
	return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
	if (list == NULL || compareElement == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	int size = listGetSize(list);
	if(size==0){
		return LIST_SUCCESS;
	}
	ListElement* elements_array = malloc(size * sizeof(*elements_array));
	if(!elements_array){
		return LIST_OUT_OF_MEMORY;
	}
	ListResult result = listNodeToArray(list, elements_array, size);
	if (result != LIST_SUCCESS) {
		free(elements_array);
		return LIST_OUT_OF_MEMORY;
	}
	quickSort(elements_array, size, compareElement);
	assert(list->copy && list->free);
	List list_sort = listCreate(list->copy, list->free);
	if (!list_sort) {
		freeArray(elements_array, size, list_sort->free);
		return LIST_OUT_OF_MEMORY;
	}
	for (int i = 0; i < size; i++) {
		if (listInsertLast(list_sort, elements_array[i])
				== LIST_OUT_OF_MEMORY) {
			listDestroy(list_sort);
			freeArray(elements_array, size, list_sort->free);
			return LIST_OUT_OF_MEMORY;
		}
	}
	int location = getIteratorLocation(list);
	listClear(list);
	Node ptr_list = list_sort->head;
	if(location==-1){
		list->iterator =NULL;
	} else if(location>-1){
		for (int i = 0; i < location; i++) {
		   ptr_list = ptr_list->next;
		}
		list->iterator = ptr_list;
	}
	list->head=list_sort->head;
	list->size=list_sort->size;
	freeArray(elements_array, size, list_sort->free);
	free(list_sort);
	return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement,
		ListFilterKey key) {
	if (!list || !filterElement) {
		return NULL;
	}
	List new_list = listCopy(list);
	if (new_list == NULL) {
		return NULL;
	} else if(listGetSize(list)==0){
		return new_list;
	}
	listGetFirst(new_list);
	while (new_list->iterator) {
		if (!filterElement(new_list->iterator->data, key)) {
			Node ptr=new_list->iterator->next;
#ifndef NDEBUG
			ListResult result =
#endif
			listRemoveCurrent(new_list);
			assert(result == LIST_SUCCESS);
			new_list->iterator=ptr;
		} else {
			listGetNext(new_list);
		}
	}
	listGetFirst(new_list);
	// sets the internal iterator to the first element in new_list
	return new_list;
}

ListResult listClear(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	while (list->head) {
		Node to_destroy = list->head;
		list->head = to_destroy->next;
		listDestroyNode(to_destroy, list->free);
	}
	list->head = NULL;
	list->iterator = NULL;
	list->size = 0;
	return LIST_SUCCESS;
}

void listDestroy(List list) {
	if (!list) {
		return;
	}
	listClear(list);
	free(list);
}
