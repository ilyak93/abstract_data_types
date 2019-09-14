#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../list_mtm/list_mtm.h"

static ListElement copyString(ListElement str){
	if(!str) {
		return NULL;
	}
	char* copy = malloc ( strlen((char*)(str)) + 1 );
	if (copy == NULL) {
		return NULL;
	}
	strcpy(copy, (char*)(str));
	return copy;
}

static void freeString(ListElement str){
	free(str);
}

static int comperStrings(ListElement str1, ListElement str2) {

	return strcmp((char*)str1,(char*)str2);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
	return strlen((char*) element) > *(int*)number;
}


static bool testListCreate() {

	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);
	List list= listCreate(copyString,freeString);
	ASSERT_TEST(list!= NULL);

	listDestroy(list) ;
	return true;
}

static bool testListCopy() {
	ASSERT_TEST(listCopy(NULL) == NULL);

	List list = listCreate(copyString, freeString);
	List copy_list = listCopy(list);
	ASSERT_TEST(copy_list != NULL);
	ASSERT_TEST(listGetFirst(copy_list) == NULL);
	ASSERT_TEST(listGetSize(copy_list) == 0);
	listDestroy(copy_list);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}

	copy_list = listCopy(list);
	ASSERT_TEST(copy_list != NULL);
	ASSERT_TEST(listGetSize(copy_list) == 5);
	ASSERT_TEST(strcmp((char*)listGetFirst(copy_list), "aaa")==0);
	for (int i=1;i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(copy_list), array[i])==0);
	}
	listDestroy(copy_list);

	listDestroy(list);
	return true;
}

static bool testListGetSize() {
	ASSERT_TEST(listGetSize(NULL) == -1);
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetSize(list) == 0);
	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	for (int i=0;i <5; ++i){
		listInsertFirst(list,array[i]);
	}
	ASSERT_TEST(listGetSize(list) == 5);

	listDestroy(list);
	return true;
}

static bool testListGetFirst() {

	ASSERT_TEST(listGetFirst(NULL) == NULL);
	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	List list = listCreate(copyString,freeString);

	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}

	for (int i=0;i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetFirst(list), array[i])==0);
		listRemoveCurrent(list);
	}
	// list is empty
	ASSERT_TEST(listGetFirst(NULL) == NULL);
	ASSERT_TEST(listGetSize(list) == 0);
	listDestroy(list);



	return true;
}

static bool testListGetNext() {
	ASSERT_TEST(listGetNext(NULL) == NULL);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	List list = listCreate(copyString,freeString);

	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}

	ASSERT_TEST(strcmp((char*)listGetFirst(list), "aaa")==0);
	for (int i=1;i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i])==0);
	}

	//reached the end of the list
	ASSERT_TEST(listGetNext(NULL) == NULL);


	listGetFirst(list);
	listRemoveCurrent(list);
	//the iterator is at an invalid state
	ASSERT_TEST(listGetNext(NULL) == NULL);

	ASSERT_TEST(listGetSize(list) == 4);
	listDestroy(list);
	return true;
}

static bool testlistGetCurrent() {
	ASSERT_TEST(listGetCurrent(NULL) == NULL);
	char* array[5] =  {"aaa","bbb","ccc","ddd","eee"};
	List list = listCreate(copyString,freeString);


	//the iterator is at an invalid state
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(listInsertFirst(list, array[1]) == LIST_SUCCESS);
	//the iterator is at an invalid state
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[1]) == 0);
	ASSERT_TEST(listGetNext(list) == NULL);

	//the iterator is at an invalid state
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[1]) == 0);
	ASSERT_TEST(listInsertBeforeCurrent(list, array[0]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[1]) == 0);

	ASSERT_TEST(listInsertAfterCurrent(list, array[3]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[3]) == 0);
	ASSERT_TEST(listInsertLast(list, array[4]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[3]) == 0);

	ASSERT_TEST(listInsertBeforeCurrent(list, array[2]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[3]) == 0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[4]) == 0);
	ASSERT_TEST(listGetNext(list) == NULL);

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0]) == 0);
	listRemoveCurrent(list);
	//the iterator is at an invalid state
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[1]) == 0);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[1]) == 0);

	for(int i=2; i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
		ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[i]) == 0);
	}

	ASSERT_TEST(listInsertFirst(list, array[0]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[4]) == 0);

	ASSERT_TEST(strcmp(listGetFirst(list), array[0]) == 0);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[0]) == 0);

	for(int i=1; i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
		ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[i]) == 0);
	}

	int key = 1;
	List filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(strcmp((char*)listGetCurrent(filtered), "aaa") == 0);
	ASSERT_TEST(listGetSize(filtered) == 5);

	listDestroy(filtered);
	listDestroy(list);
	return true;
}


static bool testListInsertFirst() {


	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	ASSERT_TEST(listInsertFirst(NULL, array[0]) == LIST_NULL_ARGUMENT);
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		ASSERT_TEST(listInsertFirst(list, array[i]) == LIST_SUCCESS);
	}


	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[4]) == 0);
	for (int i=3;i >= 0; --i){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
	}

	listDestroy(list);
	return true;

}

static bool testListInsertLast() {

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	ASSERT_TEST(listInsertLast(NULL, array[0]) == LIST_NULL_ARGUMENT);
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		ASSERT_TEST(listInsertLast(list, array[i]) == LIST_SUCCESS);
	}


	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0]) == 0);
	for (int i=1;i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
	}

	listDestroy(list);
	return true;


	return true;
}

static bool testListInsertBeforeCurrent() {
	ASSERT_TEST(listInsertBeforeCurrent(NULL,"aaa") == LIST_NULL_ARGUMENT);

	List list = listCreate(copyString, freeString);
	//the iterator is at an invalid state
	ASSERT_TEST(listInsertBeforeCurrent(list, "aaa") == LIST_INVALID_CURRENT);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};

	listInsertFirst(list, array[4]);
	ASSERT_TEST(listInsertBeforeCurrent(list, "aaa") == LIST_INVALID_CURRENT);
	listInsertFirst(list, array[2]);
	ASSERT_TEST(listInsertBeforeCurrent(list, "aaa") == LIST_INVALID_CURRENT);

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[2])==0);
	ASSERT_TEST(listInsertBeforeCurrent(list, array[0]) == LIST_SUCCESS);

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[2])==0);
	ASSERT_TEST(listInsertBeforeCurrent(list, array[1]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[2])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[4])==0);
	ASSERT_TEST(listInsertBeforeCurrent(list, array[3]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[4])==0);


	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0]) == 0);
	for (int i=1;i <5; ++i){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
	}

	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {

	ASSERT_TEST(listInsertAfterCurrent(NULL,"aaa") == LIST_NULL_ARGUMENT);
	List list = listCreate(copyString, freeString);
	//the iterator is at an invalid state
	ASSERT_TEST(listInsertAfterCurrent(list, "aaa") == LIST_INVALID_CURRENT);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};

	listInsertFirst(list, array[3]);
	ASSERT_TEST(listInsertBeforeCurrent(list, "aaa") == LIST_INVALID_CURRENT);
	listInsertFirst(list, array[0]);
	ASSERT_TEST(listInsertBeforeCurrent(list, "aaa") == LIST_INVALID_CURRENT);

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[3])==0);
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0])==0);

	ASSERT_TEST(listInsertAfterCurrent(list, array[1]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[0])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[1])==0);

	ASSERT_TEST(listInsertAfterCurrent(list, array[2]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[1])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[2])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[3])==0);
	ASSERT_TEST(listInsertAfterCurrent(list, array[4]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[3])==0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array[4])==0);
	ASSERT_TEST(listGetNext(list) == NULL);

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array[0]) == 0);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), array[0]) == 0);

	for(int i=1; i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array[i]) == 0);
	}

	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent() {
	ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);

	List list = listCreate(copyString, freeString);

	// Iterator at invalid state
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}

	for (int i=0;i<5; i++){
		ASSERT_TEST(strcmp((char*)listGetFirst(list), array[i]) == 0);
		ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
		ASSERT_TEST(listGetCurrent(list) == NULL);
		ASSERT_TEST(listGetSize(list) == 4-i);
	}
	// Iterator at invalid state
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	listDestroy(list);
	return true;
}

static bool testListSort() {
	ASSERT_TEST(listSort(NULL,comperStrings) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(NULL, NULL) == LIST_NULL_ARGUMENT);
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listSort(list, NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(list, comperStrings) == LIST_SUCCESS);
	char* array1[5] = {"c","d","e","b","a"};

	for (int i=0;i<5;i++){
		listInsertLast(list,array1[i]);
	}
	ASSERT_TEST(strcmp((char*)listGetFirst(list), array1[0]) == 0);
	ASSERT_TEST(strcmp((char*)listGetNext(list), array1[1])==0);
	// iterator points to the second element

	ASSERT_TEST(listSort(list, comperStrings) == LIST_SUCCESS);
	ASSERT_TEST(strcmp((char*)listGetCurrent(list), "b") == 0);

	char* array2[5] = {"a","b","c","d","e"};

	ASSERT_TEST(strcmp((char*)listGetFirst(list), array2[0]) == 0);
	for (int i=1;i <5; ++i){
		ASSERT_TEST(strcmp((char*)listGetNext(list), array2[i]) == 0);
	}
	listDestroy(list);
	return true;
}

static bool testListFilter(){

	int key = 2;
	ASSERT_TEST(listFilter(NULL,NULL, &key) == NULL);
	ASSERT_TEST(listFilter(NULL,isLongerThan, &key) == NULL);
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listFilter(list,NULL, &key) == NULL);

	List filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(filtered!= NULL);
	listDestroy(filtered);

	char* array[5] = {"a","bb","ccc","ddd","eee"};
	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}

	filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(listGetSize(filtered) == 3);
	ASSERT_TEST(strcmp(listGetFirst(filtered),"ccc")==0);
	listDestroy(list);
	listDestroy(filtered);
	return true;
}

static bool testListClear() {
	ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);

	char* array[5] = {"aaa","bbb","ccc","ddd","eee"};
	for (int i=0;i <5; ++i){
		listInsertLast(list,array[i]);
	}
	ASSERT_TEST(listGetSize(list) == 5);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);

	listDestroy(list);
	return true;
}

static bool testListDestroy() {
	listRemoveCurrent(NULL);
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listInsertLast(list,"hhh") == LIST_SUCCESS);
	listDestroy(list);
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testlistGetCurrent);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListFilter);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}
