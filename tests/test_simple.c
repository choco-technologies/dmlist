#include "dmlist.h"
#include <stdio.h>
#include <string.h>

#define TEST_PASS() printf("PASS\n")
#define TEST_FAIL() printf("FAIL\n")

int main(void) {
    printf("=== Simple DMLIST Test ===\n");
    
    // Test 1: Initialize
    printf("Init: ");
    dmlist_context_t* list = dmlist_init("test");
    if(list == NULL) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 2: Check if empty
    printf("Is Empty: ");
    if(!dmlist_is_empty(list)) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 3: Push back
    printf("Push Back: ");
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    if(!dmlist_push_back(list, &val1) || !dmlist_push_back(list, &val2) || !dmlist_push_back(list, &val3)) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 4: Check size
    printf("Size: ");
    if(dmlist_size(list) != 3) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 5: Front
    printf("Front: ");
    int* front_val = (int*)dmlist_front(list);
    if(front_val == NULL || *front_val != 10) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 6: Back
    printf("Back: ");
    int* back_val = (int*)dmlist_back(list);
    if(back_val == NULL || *back_val != 30) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 7: Pop front
    printf("Pop Front: ");
    int* popped = (int*)dmlist_pop_front(list);
    if(popped == NULL || *popped != 10 || dmlist_size(list) != 2) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 8: Pop back
    printf("Pop Back: ");
    popped = (int*)dmlist_pop_back(list);
    if(popped == NULL || *popped != 30 || dmlist_size(list) != 1) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 9: Clear
    printf("Clear: ");
    dmlist_clear(list);
    if(!dmlist_is_empty(list) || dmlist_size(list) != 0) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 10: Destroy
    printf("Destroy: ");
    dmlist_destroy(list);
    TEST_PASS();
    
    printf("\nAll simple tests completed!\n");
    return 0;
}
