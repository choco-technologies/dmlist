#include "dmlist.h"
#include <stdio.h>
#include <string.h>

#define TEST_PASS() printf("PASS\n")
#define TEST_FAIL() printf("FAIL\n")

// Comparison function for integers
static int compare_int(const void* a, const void* b) {
    const int* ia = (const int*)a;
    const int* ib = (const int*)b;
    return *ia - *ib;
}

int main(void) {
    printf("=== Simple DMLIST Test ===\n");
    
    // Test 1: Create
    printf("Create: ");
    dmlist_context_t* list = dmlist_create("test");
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
    if(popped == NULL) {
        TEST_FAIL();
        return 1;
    }
    if(*popped != 10 || dmlist_size(list) != 2) {
        TEST_FAIL();
        return 1;
    }
    TEST_PASS();
    
    // Test 8: Pop back
    printf("Pop Back: ");
    popped = (int*)dmlist_pop_back(list);
    if(popped == NULL) {
        TEST_FAIL();
        return 1;
    }
    if(*popped != 30 || dmlist_size(list) != 1) {
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
    
    // Test 10: Find and find_next
    printf("Find and Find Next: ");
    // Add values with duplicates
    int val4 = 10;
    int val5 = 20;
    int val6 = 10;
    int val7 = 30;
    int val8 = 10;
    dmlist_push_back(list, &val4);
    dmlist_push_back(list, &val5);
    dmlist_push_back(list, &val6);
    dmlist_push_back(list, &val7);
    dmlist_push_back(list, &val8);
    
    // Find first occurrence of 10
    int search_val = 10;
    int* found1 = (int*)dmlist_find_next(list, NULL, &search_val, compare_int);
    if(found1 == NULL || *found1 != 10 || found1 != &val4) {
        TEST_FAIL();
        return 1;
    }
    
    // Find second occurrence of 10
    int* found2 = (int*)dmlist_find_next(list, found1, &search_val, compare_int);
    if(found2 == NULL || *found2 != 10 || found2 != &val6) {
        TEST_FAIL();
        return 1;
    }
    
    // Find third occurrence of 10
    int* found3 = (int*)dmlist_find_next(list, found2, &search_val, compare_int);
    if(found3 == NULL || *found3 != 10 || found3 != &val8) {
        TEST_FAIL();
        return 1;
    }
    
    // Try to find fourth occurrence (should return NULL)
    int* found4 = (int*)dmlist_find_next(list, found3, &search_val, compare_int);
    if(found4 != NULL) {
        TEST_FAIL();
        return 1;
    }
    
    // Test with NULL last_found should behave like find
    int* found_from_start = (int*)dmlist_find_next(list, NULL, &search_val, compare_int);
    if(found_from_start == NULL || found_from_start != &val4) {
        TEST_FAIL();
        return 1;
    }
    
    // Test with invalid last_found (not in list) should return NULL
    int external_val = 99;
    int* found_invalid = (int*)dmlist_find_next(list, &external_val, &search_val, compare_int);
    if(found_invalid != NULL) {
        TEST_FAIL();
        return 1;
    }
    
    TEST_PASS();
    
    // Test 11: Destroy
    printf("Destroy: ");
    dmlist_destroy(list);
    TEST_PASS();
    
    printf("\nAll simple tests completed!\n");
    return 0;
}
