#include "list.h"
#include <stdlib.h>
#include <assert.h> // for the suggested assert(pList != NULL) check

#define IS_EMPTY -1

//pools
static List listHeads[LIST_MAX_NUM_HEADS];
static Node nodes[LIST_MAX_NUM_NODES];

//list heads stack
static int freeListIndex[LIST_MAX_NUM_HEADS];
static int listStackTop = IS_EMPTY; //IS_EMPTY = -1
static bool is_init = false;

//node stack
static int freeNodeIndex[LIST_MAX_NUM_NODES];
static int nodeStackTop = IS_EMPTY;

//helper function
static void initializeFreeStack() {
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        freeListIndex[++listStackTop] = i;
    }

    for(int i = 0; i < LIST_MAX_NUM_NODES; i++){
        freeNodeIndex[++nodeStackTop] = i;
    }

    is_init = true;
}
//helper function
static Node* Create_node(void* item){

    if (nodeStackTop == IS_EMPTY) {
        return NULL; // No available nodes
    }

    int freeIndex = freeNodeIndex[nodeStackTop--];//get a free node from top of the stack

    nodes[freeIndex].item = item;
    nodes[freeIndex].next = NULL;
    nodes[freeIndex].prev = NULL;
    nodes[freeIndex].index = freeIndex;

    return &nodes[freeIndex];
}

//helper function
static void deallocateNode(Node* node) {
    if (nodeStackTop >= LIST_MAX_NUM_NODES - 1){// safety check, if node stack full
        return;
    } 
    
    node->next = NULL;
    node->prev = NULL;
    node->item = NULL;

    freeNodeIndex[++nodeStackTop] = node->index;
}

//helper function
static void deallocateList(List* list){
    if(listStackTop>= LIST_MAX_NUM_HEADS - 1){ //safety check, if list stack is full
        return;
    } 

    list->head=NULL;
    list->tail = NULL;
    list->current = NULL;
    list->size = 0;

    freeListIndex[++listStackTop] = list->index;
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create() {
    if (!is_init) {
        initializeFreeStack();
    }
    
    if (listStackTop == IS_EMPTY) {
        return NULL; // No available lists
    }
    
    // Get a free index from the top of the stack
    int freeIndex = freeListIndex[listStackTop--];

    listHeads[freeIndex].head = NULL;
    listHeads[freeIndex].tail = NULL;
    listHeads[freeIndex].current = NULL;
    listHeads[freeIndex].size = 0;
    listHeads[freeIndex].index = freeIndex;
    listHeads[freeIndex].status = LIST_OK;

    return &listHeads[freeIndex];
}

// Returns the number of items in pList.
int List_count(List* pList) {
    return pList->size;
}
// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
    if(pList->size == 0){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->head;

    return pList->current->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList) {
    if(pList->size == 0){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->tail;

    return pList->current->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList) {
    if (pList->current == NULL || pList->current->next == NULL) {
       pList->current = NULL;//beyond list is NULL
       pList->status = LIST_OOB_END;
       return NULL;
    }

    pList->current = pList->current->next;
    return pList->current->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {
    if (pList->current == NULL || pList->current->prev == NULL) {
       pList->current = NULL;//beyond list is NULL
       pList->status = LIST_OOB_START;
       return NULL;
    }
    
    pList->current = pList->current->prev;
    return pList->current->item;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
    if(pList->current == NULL||pList->current->item==NULL){
        return NULL;
    }

    return pList->current->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem) {

    Node* new_Node = Create_node(pItem);

    if (new_Node==NULL) {//if node pool is already full
        return LIST_FAIL;
    }
    if(pList->size == 0){//If list is empty
        pList->current = new_Node;
        pList->head = new_Node;
        pList->tail = new_Node;

    }else if(pList->current!=NULL) {//if current exists 
        new_Node->prev = pList->current;
        new_Node->next = pList->current->next;

        if(pList->current->next!=NULL){//check for next element after current is not NULL
            pList->current->next->prev = new_Node;
        }else{
           pList->tail = new_Node; 
        }

        pList->current->next = new_Node;
        pList->current = new_Node;
    }else if(pList->status==LIST_OOB_START){//if out of bounds start 
        pList->head->prev = new_Node;
        new_Node->next = pList->head;
        pList->head = new_Node;
        pList->current = pList->head;
    }else if(pList->status == LIST_OOB_END){//if out of bounds end
        pList->tail->next = new_Node;
        new_Node->prev = pList->tail;
        pList->tail = new_Node;
        pList->current = pList->tail;
    }

    pList->size++;
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem) {
    Node* new_Node = Create_node(pItem);

    if (new_Node==NULL) {//if node pool is already full
        return LIST_FAIL;
    }

    if(pList->size == 0){//If list is empty
        pList->current = new_Node;
        pList->head = new_Node;
        pList->tail = new_Node;

    }else if(pList->current!=NULL) {//if current exists 
        new_Node->next = pList->current;
        new_Node->prev = pList->current->prev; 

        if(pList->current->prev!=NULL){
            pList->current->prev->next = new_Node;
        }else{
            pList->head = new_Node;  
        }

        pList->current->prev = new_Node;
        pList->current = new_Node;
    }else if(pList->status==LIST_OOB_START){//if out of bounds start 
        pList->head->prev = new_Node;
        new_Node->next = pList->head;
        pList->head = new_Node;
        pList->current = pList->head;

    }else if(pList->status == LIST_OOB_END){//if out of bounds end
        pList->tail->next = new_Node;
        new_Node->prev = pList->tail;
        pList->tail = new_Node;
        pList->current = pList->tail;
    }

    pList->size++;
    return LIST_SUCCESS;
}
// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem) {
    Node* new_Node = Create_node(pItem);

    if (new_Node==NULL) {//if node pool is already full
        return LIST_FAIL;
    }

    if(pList->size == 0){//if the list is empty
        pList->head = new_Node;
        pList->tail = new_Node; 
    }else{//assumes that items exist in the list
        pList->tail->next = new_Node;
        new_Node->prev = pList->tail;
        pList->tail = new_Node;//now pList->tail is set as the new_Node for the list
    }
     
    pList->current = new_Node;
    pList->size++;
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem) {
    Node* new_Node = Create_node(pItem);

    if (new_Node==NULL) {//if node pool is already full
        return LIST_FAIL;
    }

    if(pList->size == 0){//if the list is empty
        pList->head = new_Node;
        pList->tail = new_Node; 
    }else{//assumes that items exist in the list
        pList->head->prev = new_Node;
        new_Node->next = pList->head;
        pList->head = new_Node;//now pList->head is set as the new_Node for the list
    }
     
    pList->current = new_Node;
    pList->size++;
    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList) {

    // Handle edge cases
    if (pList->size == 0 || pList->current == NULL || 
        pList->status == LIST_OOB_START || pList->status == LIST_OOB_END) {
        return NULL;
    }

    Node* toBeRemoved = pList->current;
    void* tempCurrentItem = toBeRemoved->item;

    // Adjust node links for middle case
    if (toBeRemoved->prev) {
        toBeRemoved->prev->next = toBeRemoved->next;
    }
    if (toBeRemoved->next) {
        toBeRemoved->next->prev = toBeRemoved->prev;
    }

    // Handle edge cases
    if (toBeRemoved == pList->head) {//head case
        pList->head = toBeRemoved->next;
        if (pList->head) {
            pList->head->prev = NULL;
        } else {
            pList->status = LIST_OOB_START;
        }
    } 

    if (toBeRemoved == pList->tail) {//tail case
        pList->tail = toBeRemoved->prev;
        if (pList->tail) {
            pList->tail->next = NULL;
        } else {
            pList->status = LIST_OOB_END;
        }
    }

    //Set the current to the next node
    pList->current = toBeRemoved->next;

    deallocateNode(toBeRemoved);
    pList->size--;

    return tempCurrentItem;
}


// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList) {

    // Check for empty list first
    if (pList->size == 0 || pList->current == NULL || 
        pList->status == LIST_OOB_START || pList->status == LIST_OOB_END) {
        return NULL;
    }

    void* removedItem = pList->tail->item;
    Node* removedNode = pList->tail;

    //If there's only one node in the list
    if(pList->size == 1) {
        pList->tail = NULL;
        pList->head = NULL;
        pList->current = NULL;
    } else {//rewire the prev node to properly make it the new tail and current
        Node* prevNode = pList->tail->prev;
        prevNode->next = NULL;
        pList->tail = prevNode;
        pList->current = prevNode;
    }

    deallocateNode(removedNode);

    pList->size--;
    return removedItem;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2) {

    //if either list does not exist, terminate
    if (pList1 == NULL || pList2 == NULL) {
        return;
    }

    //if both lists are empty
    if (pList1->size == 0 && pList2->size == 0) {
        deallocateList(pList2); // We can free pList2 as it is empty
        return;
    }

    //Only pList1 is empty
    if (pList1->size == 0) {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->size = pList2->size;

        pList1->current = NULL; // Reset the current of pList1 since its structure has changed
        pList1->status = LIST_OOB_START;
        deallocateList(pList2); // pList2's structure is now part of pList1, so just free its head
        return;
    }

    //Only pList2 is empty
    if (pList2->size == 0) {
        deallocateList(pList2); // We can free pList2 as it is empty
        return;
    }

    //Both lists are populated
    pList1->tail->next = pList2->head;
    pList2->head->prev = pList1->tail;
    pList1->tail = pList2->tail;

    pList1->size += pList2->size;
    deallocateList(pList2);
}


// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn) {

    //edge cases
    if (pList == NULL || pItemFreeFn == NULL) {
        return;
    }

    Node* currentNode = pList->head;//currentNode starts at the head
    Node* nextNode;

    while (currentNode != NULL) {// cycle through all the nodes beginning from the head.
        nextNode = currentNode->next;
        (*pItemFreeFn)(currentNode->item);//free fn call
        deallocateNode(currentNode);// Return the node to the node pool
        currentNode = nextNode;
    }

    deallocateList(pList);// Return the list head to the pool
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. 
// If no match is found, the current pointer is left beyond the end of the list and a NULL 
// pointer is returned.
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {
    
    //edge cases
    if (pList == NULL || pComparator == NULL) {
        return NULL;
    }

    // If the current pointer is before the start of pList, start from the head
    if (pList->status == LIST_OOB_START && pList->current == NULL) {
        pList->current = pList->head;
    }

    // Start searching from the current node
    Node* currentNode = pList->current;
    while (currentNode != NULL) {
        // Check if the current node's item matches
        if (pComparator(currentNode->item, pComparisonArg) == 1) {
            pList->current = currentNode;
            return currentNode->item;
        }
        currentNode = currentNode->next;
    }

    //If this is reached, NO MATCH and set things for PList
    pList->status = LIST_OOB_END;
    pList->current = NULL;
    return NULL;
}
