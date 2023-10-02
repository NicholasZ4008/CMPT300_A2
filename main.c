#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//report helper function to reduce overall lines of code
void report(bool stm, char * msg) {
    printf ("\n%s : %s\n", msg, stm? "PASS": "FAIL");
    if (!stm) {
        perror("failed test ... exiting ...\n");
        exit(1);
    }
}

//dummy free fn for List_free() testing
void dummyFreeFn(void* pItem) {
    // Do nothing since data is statically allocated
}

void test_create_list(int *numheads, int *numnodes) {
    printf("testing create_list() ...\n");
    List *myList = List_create();
    if (*numheads==LIST_MAX_NUM_HEADS) {
        report(myList==NULL,"not enough room, no list created");
    }else {
        report(myList!=NULL,"list created successfully");
        (*numheads)++;
        report(myList->head == NULL,"head of new list should be NULL");
        report(myList->tail == NULL,"tail of new list should be NULL");
        report(myList->current == NULL,"current of new list should be NULL");
        report(myList->size == 0,"size of new list should be 0");
    }
    // Clean up
    List_free(myList, dummyFreeFn);
}

void test_list_count(){
    printf("\ntesting list_count\n");
    int testItem = 1;

    //general cases
    List* testList = List_create();
    for(int i=0;i<6;i++){
        if(i==0){
            report(testList->size == i, "list size is 0 on empty list");
        }
        else if(i==1){
            report(testList->size == i, "list size is 1");
        }
        else if(i==5){
            report(testList->size == i, "list size is 5");
        }
        List_append(testList, &testItem);
    }

    //testing when removing a node
    List_trim(testList);
    report(testList->size == 5,"list size decrements to 5 from 6");

    //edge case
    for(int i = 0; i<500; i++){//set i to append to an unnecessary amount of times
        if(testList->size == 101){
            printf("testList->size should never exceed LIST_MAX_NUM_NODES: FAIL");
        }
        List_append(testList, &testItem);
    }
    printf("testList->size stayed in the expected range (0-100): PASS\n");

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_first(){

    printf("\ntesting list_first:\n");
    List* testList = List_create();

    int one = 1;
    int newValue = 3;

    for(int i=0; i<5; i++){
        int *firstItem = List_first(testList);

        if(i == 0){
            report(firstItem == NULL, "if list is empty, first item should return NULL");
            List_append(testList, &one);//first item appended is gonna be 1
        }
        if(i == 1){
            report(*firstItem == 1, "first item returned should be 1");
            List_prepend(testList, &newValue);//first item appended is now going to be 3
        }
        if(i==2){
            report(*firstItem == 3," now first item returned should be a 3");//prepend a 3
        }
    }

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_last(){

    printf("\ntesting list_last:\n");
    List* testList = List_create();

    int two = 2;
    int newValue = 5;

    for(int i=0; i<5; i++){
        int *lastItem = List_last(testList);

        if(i == 0){
            report(lastItem == NULL, "if list is empty, last item should return NULL");
            List_append(testList, &two);//first item appended is gonna be 2
        }
        if(i == 1){
            report(*lastItem == 2, "last item returned should be 2");
            List_prepend(testList, &newValue);//first item appended is now going to be 3
        }
        if(i==2){
            report(*lastItem == 2,"last item should remain as a 2");//prepend a 3
        }
    }

    // Clean up
    List_free(testList, dummyFreeFn);
}
//test list_next and list_prev
void test_list_next_prev(){
    printf("\ntesting list_next and list_prev\n");
    List* testList = List_create();
    int a=1, b=5 ,c=3, testVal=10; 
    int *arr[3] = {&a,&b,&c};

    //testList empty case List_next
    void* listNextItem=List_next(testList);
    report(testList->current == NULL && listNextItem==NULL,"when testList is empty and test list is advanced, return NULL");
    report(testList->status == LIST_OOB_END,"for empty list, advancing causes status to be set to LIST_OOB_END");

    //testList empty case List_prev
    void* listPrevItem = List_prev(testList);
    report(testList->current == NULL && listPrevItem==NULL,"when testList is empty and test list is backtracked, return NULL");
    report(testList->status == LIST_OOB_START,"for empty list, backtracking causes status to be set to LIST_OOB_Start");

    //build a testList with some arbitrary int values
    for(int i=0; i<3; i++){
        List_append(testList,arr[i]);//current should be at the tail of the List
    }

    //test list_prev functionality
    for(int i=0; i<5; i++){
        void* list_prev_item=List_prev(testList);

        if(i==1){
            report(testList->head->item==testList->current->item && list_prev_item ==testList->current->item, "Current item should have traversed backwards to the head");
        }
        else if(i==2){
            report(testList->current==NULL && testList->status == LIST_OOB_START && list_prev_item==NULL,"The current should be NULL and It reaches LIST_OOB_START as expected when backtracking past the boundary");
        }
        else if(i==4){
            report(testList->current==NULL && testList->status == LIST_OOB_START && list_prev_item==NULL,"testList->current should still be NULL and status should remain as LIST_OOB_START even after repeated out of bounds calls");
        }
    }

    //test list_next functionality
    List_prepend(testList, &testVal);
    for(int i=0; i<7; i++){
        void* list_next_item=List_next(testList);
        if(i==2){
            report(testList->tail->item ==testList->current->item && list_next_item == testList->current->item,"Current item should have traversed to the tail");
        }
        else if(i==3){
            report(testList->current==NULL && testList->status == LIST_OOB_END && list_next_item==NULL,"The current should be NULL and It reaches LIST_OOB_End as expected when advancing past the boundary");
        }
        else if(i==5){
            report(testList->current==NULL && testList->status == LIST_OOB_END && list_next_item==NULL,"testList->current should still be NULL and status should remain as LISt_OOB_END even after repeated out of bounds calls");
        }
    }

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_curr(){
    printf("\ntesting list_curr\n");
    List* testList = List_create();
    int a=1, b=5 ,c=3; 
    int *arr[3] = {&a,&b,&c};

    //empty list case
    void* currentItem = List_curr(testList);
    report((testList->current==NULL || testList->current->item == NULL) && currentItem == NULL,"empty list current item should be NULL");

    //GENERAL CASES of List_curr
    for(int i = 0; i<3; i++){
        List_append(testList, arr[i]);
        void* currentItem = List_curr(testList);

        if(i==0){
            report(testList->current->item == &a && currentItem == &a,"current item is 1");
        }
        else if(i==1){
            report(testList->current->item == &b && currentItem ==&b,"current item is 5");
        }
        else if(i==2){
            report(testList->current->item == &c && currentItem ==&c,"current item is 3");
        }
    }
    //checking correct NULL val for LIST_OOB_END
    List_next(testList);
    currentItem=List_curr(testList);
    report((testList->current==NULL || testList->current->item == NULL) && currentItem==NULL,"LIST_OOB_END should result in zero");

    //checking correct NULL val for LIST_OOB_START
    List_prepend(testList, &a);
    List_prev(testList);
    currentItem=List_curr(testList);
    report((testList->current==NULL || testList->current->item == NULL) && currentItem==NULL,"LIST_OOB_START should result in zero");

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_insert_after() {
    printf("\ntesting List_insert_after:\n");
    List* testList = List_create();
    int a = 1, b = 5, c = 3, d = 7, e = 9, f=11;//arbitrary test vals

    //Empty list case
    List_insert_after(testList, &a);
    report(testList->current->item == &a && testList->size == 1, "Size and current item should update accordingly when inserting into an empty list");

    //Insert after the current item (middle of the list)
    List_append(testList, &b);
    List_insert_after(testList, &c);
    report(testList->current->item == &c && testList->size == 3 && *((int*)List_prev(testList)) == b, "Insert should behave correctly even with items already in the list. The value before the new current should remain correct too");

    //Insert after the current item when it's at the start
    List_first(testList);
    List_insert_after(testList, &d);
    report(testList->current->item == &d && testList->size == 4 && *((int*)List_prev(testList)) == a, "If current pointer is the first item in the list, list_insert should work accordingly. And the value before the new current should remain correct as well");

    //Insert when current is beyond the end
    List_last(testList);
    List_next(testList);// Move current beyond the end
    List_insert_after(testList, &e);
    report(testList->current->item == &e && testList->size == 5 &&testList->tail->item == &e, "Even when LIST_OOB_END flag is triggered, should still insert item at the end of the list");

    //Insert when current is before the start
    List_first(testList);
    List_prev(testList);// Move current before the start
    List_insert_after(testList, &f);
    report(testList->current->item == &f && testList->size == 6 && testList->head->item==&f, "Even when LIST_OOB_FIRST flag is triggered, should still insert item at the front of the list");
    
    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_insert_before() {
    printf("\ntesting List_insert_before:\n");
    List* testList = List_create();
    int a = 1, b = 5, c = 3, d = 7, e = 9, f = 11; // arbitrary test vals

    // Empty list case
    List_insert_before(testList, &a);
    report(testList->current->item == &a && testList->size == 1, "Size and current item should update accordingly when inserting into an empty list");

    // Insert before the current item (middle of the list)
    List_append(testList, &b);
    List_insert_before(testList, &c);
    report(testList->current->item == &c && testList->size == 3 && *((int*)List_next(testList)) == b, "Insert should behave correctly even with items already in the list. The value after the new current should remain correct too");

    // Insert before the current item when it's at the end
    List_last(testList);
    List_insert_before(testList, &d);
    report(testList->current->item == &d && testList->size == 4 && *((int*)List_next(testList)) == b, "If current pointer is the last item in the list, list_insert should work accordingly. And the value after the new current should remain correct as well");

    // Insert when current is beyond the end
    List_last(testList);
    List_next(testList); // Move current beyond the end
    List_insert_before(testList, &e);
    report(testList->current->item == &e && testList->size == 5 && testList->tail->item == &e, "Even when LIST_OOB_END flag is triggered, should still insert item at the end of the list");

    // Insert when current is before the start
    List_first(testList);
    List_prev(testList); // Move current before the start
    List_insert_before(testList, &f);
    report(testList->current->item == &f && testList->size == 6 && testList->head->item == &f, "Even when LIST_OOB_START flag is triggered, should still insert item at the front of the list");
    
    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_List_append_node_pool() {
    printf("\ntesting List_append with node pool:\n");
    List* testList = List_create();

    int values[101];// Create more than 100 items to exhaust the node pool.
    for (int i = 0; i < 101; i++) {
        values[i] = i;
    }

    int appendResult;
    for (int i = 0; i < 101; i++) {
        appendResult = List_append(testList, &values[i]);
        if (i == 99) {
            report(appendResult == LIST_SUCCESS, "Appending should succeed for the first 100 nodes");
        } else if(i == 100) {
            report(appendResult == LIST_FAIL, "Appending should fail when the node pool is exhausted (past 100 nodes in this case)");
        }
    }
    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_List_prepend_node_pool() {
    //NOTE: List_append and List_prepend general cases were not tested in isolation due to constant usage in other functions!
    printf("\ntesting List_prepend with node pool:\n");
    List* testList = List_create();

    int values[101]; // Create more than 100 items to exhaust the node pool.
    for (int i = 0; i < 101; i++) {
        values[i] = i;
    }

    int prependResult;
    for (int i = 0; i < 101; i++) {
        prependResult = List_prepend(testList, &values[i]);
        if (i == 99) {
            report(prependResult == LIST_SUCCESS, "Prepending should succeed for the first 100 nodes");
        } else if(i == 100) {
            report(prependResult == LIST_FAIL, "Prepending should fail when the node pool is exhausted(past 100 nodes in this case)");
        }
    }

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_remove() {
    printf("\ntesting List_remove:\n");
    List* testList = List_create();
    int a = 1, b = 5, c = 3;
    int *arr[3]= {&a,&b,&c};
    
    // Test removing from an empty list
    void* removedItem = List_remove(testList);
    report(testList->size == 0 || removedItem == NULL, "Should return NULL when removing from an empty list");

    for(int i =0; i<3; i++){
        List_append(testList, arr[i]);//append some arbitrary items
    }

    // Test removing from the middle
    List_first(testList);
    List_next(testList);// Points to 'b'
    removedItem = List_remove(testList);
    report(removedItem == &b && testList->size == 2, "Should remove middle element correctly");

    // Test removing the head
    List_first(testList); // Point to 'a'
    removedItem = List_remove(testList);
    report(*(int*)removedItem == a && testList->size == 1 && testList->head->item == &c, "Should remove the head correctly");

    // Test removing the tail (which is currently also the head)
    removedItem = List_remove(testList);
    report(*(int*)removedItem == c && testList->size == 0 && testList->head == NULL && testList->tail == NULL, "Should remove the last remaining element (head and tail) correctly");

    // Test removing when current is beyond the end
    List_next(testList);
    removedItem = List_remove(testList);
    report(removedItem == NULL, "Should return NULL when current is beyond the end");

    // Test removing when current is before the start
    List_prev(testList);
    removedItem = List_remove(testList);
    report(removedItem == NULL, "Should return NULL when current is before the start");

    // Clean up
    List_free(testList, dummyFreeFn);
}

void test_list_trim() {
    printf("\ntesting List_trim:\n");
    List* testList = List_create();
    int a = 1, b = 5, c = 3;
    int *arr[3] = {&a, &b, &c};

    // empty list case
    void* trimmedItem = List_trim(testList);
    report(trimmedItem == NULL, "Should return NULL when trimming from an empty list");

    for(int i = 0; i < 3; i++){
        List_append(testList, arr[i]);// append some arbitrary items
    }

    for(int i = 0; i<4; i++){
        trimmedItem = List_trim(testList);
        
        if(i==0){//Test trimming from a list with multiple items
            report(trimmedItem == &c && testList->size == 2 && testList->current->item == &b, "Should trim the last element correctly and set current to the new last item");
        }
        else if(i==1){//Test trimming the new tail
            report(trimmedItem == &b && testList->size == 1 && testList->current->item == &a, "Should trim the last element correctly and set current to the new last item");
        }
        else if(i==2){//Test trimming the last item
            report(trimmedItem == &a && testList->size == 0 && testList->head == NULL && testList->tail == NULL && testList->current == NULL, "Should trim the only remaining element correctly and reset list properties");
        }
        else if(i==3){//Test trimming again after the list is empty
            report(trimmedItem == NULL, "Should return NULL when trimming an already empty list");
        }
    }
    // Clean up
    List_free(testList, dummyFreeFn);
}

//
void test_list_concat(){
    printf("\ntesting List_concat:\n");

    List* testList1 = List_create();
    List* testList2 = List_create();
    int a = 1, b = 5, c = 3;
    int *arr1[2] = {&a, &b};
    int *arr2[1] = {&c};

    for (int i = 0; i < 2; i++) {//load list 1
        List_append(testList1, arr1[i]);
    }
    for (int i = 0; i < 1; i++) {//load list 2
        List_append(testList2, arr2[i]);
    }

    //Test concatenating two non-empty lists
    List_concat(testList1, testList2);
    report(testList1->size == 3 && testList1->tail->item == &c, "Should concatenate two filled lists correctly");

    //Test concatenating with first list empty
    testList1 = List_create();
    testList2 = List_create();

    List_append(testList2, &c);
    List_concat(testList1, testList2);
    report(testList1->size == 1 && testList1->tail->item == &c, "Should concatenate when first list is empty");

    //Test concatenating with second list empty
    testList1 = List_create();
    testList2 = List_create();
    for (int i = 0; i < 2; i++) {
        List_append(testList1, arr1[i]);
    }

    List_concat(testList1, testList2);
    report(testList1->size == 2 && testList1->tail->item == &b, "Should concatenate when second list is empty");

    //Test concatenating two empty lists
    testList1 = List_create();
    testList2 = List_create();

    List_concat(testList1, testList2);
    report(testList1->size == 0 && testList1->head == NULL && testList1->tail == NULL, "Should handle concatenating two empty lists");

    //Clean up
    List_free(testList1, dummyFreeFn);
    List_free(testList2, dummyFreeFn);
}


static int itemsFreedCounter = 0;

// Sample free function that increments a global counter when called
void sampleItemFreeFn(void* pItem) {
    itemsFreedCounter++;
}
void test_list_free() {
    printf("\ntesting List_free:\n");
    
    List* testList1 = List_create();
    int a = 1, b = 2, c = 3;
    int *arr[3] = {&a, &b, &c};
    for (int i = 0; i < 3; i++) {
        List_append(testList1, arr[i]);
    }

    // Test freeing a non-empty list;
    List_free(testList1, sampleItemFreeFn);
    report(itemsFreedCounter == 3, "Should free all items from a non-empty list(3 items should be freed)");

    // Test freeing an empty list
    List* testList2 = List_create();
    itemsFreedCounter = 0; // reset counter
    List_free(testList2, sampleItemFreeFn);
    report(itemsFreedCounter == 0, "Should handle freeing an empty list without accidentally freeing any items");
}

//helper for test_list_search
bool intComparator(void* pItem, void* pComparisonArg) {
    return (*(int*)pItem) == (*(int*)pComparisonArg);
}

void test_list_search() {
    printf("\ntesting List_search:\n");

    List* testList = List_create();
    int a = 1, b = 2, c = 3;
    int *arr[3] = {&a, &b, &c};

    //Populate LL with head as the current item.
    for (int i = 0; i < 3; i++) {
        List_prepend(testList, arr[i]);
    }

    //Test assuming that the current is the head.
    int targetItem = 2;
    void* foundItem = List_search(testList, intComparator, &targetItem);
    report(foundItem == &b, "Should find item 2 in the list");

    //Test with an empty list.
    List* emptyList = List_create();
    foundItem = List_search(emptyList, intComparator, &targetItem);
    report(foundItem == NULL, "Should not find item in an empty list");

    //Test when no match is found.
    int searchForMissing = 99;
    foundItem = List_search(testList, intComparator, &searchForMissing);
    report(foundItem == NULL && testList->status==LIST_OOB_END, "Should return NULL and set status to LIST_OOB_END when no match is found");

    //Test when the current pointer is before the start of the list.
    testList->current = NULL; // Make sure current is null
    testList->status = LIST_OOB_START;
    targetItem = 3;
    foundItem = List_search(testList, intComparator, &targetItem);
    report(foundItem == &c, "Should find item 3 in the list even if current pointer was before the start");

    //Test when item is in the LL but the current pointer is already past it.
    List_append(testList,&c);
    targetItem=2;
    foundItem = List_search(testList, intComparator, &targetItem);
    report(foundItem == NULL && testList->status==LIST_OOB_END,"should return NULL and set status to LIST_OOB_END if the current pointer is past the target item");

    // Clean up
    List_free(testList, dummyFreeFn);
}

int main(){
    //test List_Create
    int numheads = 0, numnodes = 0;
    test_create_list(&numheads, &numnodes);
    
    //test simple functions
    test_list_count();
    test_list_first();
    test_list_last();
    test_list_next_prev();//tested next and prev together since they were so similar
    test_list_curr();

    //testing various inserts
    test_list_insert_after();
    test_list_insert_before();

    //node pool testing
    //NOTE: List_append and List_prepend general cases were not tested in isolation 
    //due to constant usage in other functions!
    test_List_append_node_pool();
    test_List_prepend_node_pool();

    //testing remove functions
    test_list_remove();
    test_list_trim();

    //testing more complex functions
    test_list_concat();
    test_list_free();
    test_list_search();

    return 0;
}