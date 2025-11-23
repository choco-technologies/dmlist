# dmlist - DMOD Linked List Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A doubly-linked list implementation designed specifically for the **DMOD (Dynamic Modules)** framework. dmlist provides a simple and efficient linked list data structure with module-aware memory management for embedded systems.

## Table of Contents

- [Overview](#overview)
- [What is DMOD?](#what-is-dmod)
- [What is dmlist?](#what-is-dmlist)
- [Architecture](#architecture)
- [Features](#features)
- [Building](#building)
- [Testing](#testing)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Overview

dmlist is a doubly-linked list library that integrates seamlessly with the DMOD dynamic module system. It provides module-aware memory management through DMOD's allocation API, making it ideal for use in embedded systems where memory tracking and module lifecycle management are critical.

## What is DMOD?

**DMOD (Dynamic Modules)** is a library that enables dynamic loading and unloading of modules in embedded systems at runtime. It allows you to:

- **Dynamically load modules**: Load functionality from `.dmf` files without recompiling
- **Manage dependencies**: Automatically handle module dependencies
- **Inter-module communication**: Modules can communicate via a common API
- **Resource management**: Efficiently manage system resources
- **Safe updates**: Update individual modules without affecting the entire system

DMOD provides a modular architecture that makes embedded systems more flexible, maintainable, and easier to extend. For more information, visit the [DMOD repository](https://github.com/choco-technologies/dmod).

## What is dmlist?

**dmlist** is a doubly-linked list implementation specifically designed to work with DMOD. It provides:

- **Module-aware allocation**: All memory allocations are tracked by module name through DMOD's memory API
- **Doubly-linked structure**: Supports efficient forward and backward traversal
- **Generic data storage**: Stores void pointers, allowing any data type to be stored
- **Rich API**: Supports common operations like push, pop, insert, remove, find, and iteration
- **Automatic cleanup**: When used with DMOD's memory management, lists can be cleaned up automatically when modules are unloaded
- **Zero external dependencies**: Only requires the DMOD framework

## Architecture

### List Structure

dmlist uses a doubly-linked list structure with head and tail pointers for efficient operations:

```
┌─────────────────────────────────────────────────┐
│            dmlist Context                       │
├─────────────────────────────────────────────────┤
│  - head: dmlist_node_t*  ───────┐              │
│  - tail: dmlist_node_t*  ───┐   │              │
│  - size: size_t              │   │              │
│  - module_name: char[]       │   │              │
└──────────────────────────────┼───┼──────────────┘
                               │   │
                   ┌───────────┘   └───────────┐
                   ▼                            ▼
          ┌────────────┐         ┌────────────┐
          │   Node 1   │◄───────►│   Node 2   │◄───►...
          ├────────────┤         ├────────────┤
          │ data: void*│         │ data: void*│
          │ next: *    │────────►│ next: *    │
          │ prev: *    │◄────────│ prev: *    │
          └────────────┘         └────────────┘
```

Each node contains:
- **data**: Pointer to the user data (void* for generic storage)
- **next**: Pointer to the next node in the list
- **prev**: Pointer to the previous node in the list

### Memory Management

All memory allocations in dmlist go through DMOD's memory API (`Dmod_Malloc` and `Dmod_Free`). This allows:

1. **Module tracking**: Every allocation is associated with the module that created the list
2. **Automatic cleanup**: When a module is unloaded, all its list allocations can be freed automatically
3. **Memory profiling**: Track memory usage per module
4. **Consistent allocation**: Use the same allocator as the rest of the DMOD ecosystem

## Features

- ✅ **Doubly-linked list**: Efficient forward and backward traversal
- ✅ **Module-aware allocations**: All allocations tracked by DMOD module system
- ✅ **Generic storage**: Stores void pointers for any data type
- ✅ **Push/Pop operations**: Add/remove from front or back in O(1) time
- ✅ **Insert/Remove at position**: Support for arbitrary position operations
- ✅ **Find operation**: Search for elements with custom comparison function
- ✅ **Iteration**: Foreach with callback function support
- ✅ **Size tracking**: Constant time size queries
- ✅ **Clear operation**: Remove all elements efficiently
- ✅ **Comprehensive logging**: Integration with DMOD logging system
- ✅ **Zero external dependencies**: Only requires DMOD framework

## Building

### Prerequisites

- **CMake**: Version 3.10 or higher
- **C Compiler**: GCC or compatible
- **Make**: For Makefile-based builds (optional)

### Using CMake

```bash
# Clone the repository
git clone https://github.com/choco-technologies/dmlist.git
cd dmlist

# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
make

# Run tests
ctest --verbose
```

### Build Options

You can customize the build with these CMake options:

```bash
# Enable tests
cmake -DDMLIST_BUILD_TESTS=ON ..

# Enable code coverage
cmake -DENABLE_COVERAGE=ON ..

# Change DMOD mode
cmake -DDMOD_MODE=DMOD_EMBEDDED ..
```

### Using Makefile

dmlist also supports traditional Makefile builds:

```bash
# Build the library
make

# The library will be created as libdmlist.a
```

The Makefile is automatically generated by CMake and includes necessary DMOD paths.

## Testing

dmlist includes basic test suites:

### Test Suites

1. **test_simple**: Basic smoke tests
   - Initialization
   - Push/Pop operations
   - Size queries
   - Clear operation
   - Destruction

### Running Tests

```bash
# Run all tests
cd build
ctest

# Run with verbose output
ctest --verbose

# Run specific test
./tests/test_simple
```

### Example Test Output

```
=== Simple DMLIST Test ===
Init: PASS
Is Empty: PASS
Push Back: PASS
Size: PASS
Front: PASS
Back: PASS
Pop Front: PASS
Pop Back: PASS
Clear: PASS
Destroy: PASS

All simple tests completed!
```

## Usage

### Basic Usage

```c
#include "dmlist.h"
#include <stdio.h>

int main(void) {
    // 1. Initialize a list
    dmlist_context_t* list = dmlist_init("main");
    if (list == NULL) {
        return -1;
    }
    
    // 2. Add elements
    int value1 = 10;
    int value2 = 20;
    dmlist_push_back(list, &value1);
    dmlist_push_back(list, &value2);
    
    // 3. Access elements
    int* front = (int*)dmlist_front(list);
    printf("Front: %d\n", *front);  // Output: Front: 10
    
    // 4. Get list size
    printf("Size: %zu\n", dmlist_size(list));  // Output: Size: 2
    
    // 5. Remove elements
    int* popped = (int*)dmlist_pop_front(list);
    printf("Popped: %d\n", *popped);  // Output: Popped: 10
    
    // 6. Clean up
    dmlist_destroy(list);
    
    return 0;
}
```

### Working with Strings

```c
#include "dmlist.h"
#include <stdio.h>
#include <string.h>

void string_list_example(void) {
    dmlist_context_t* list = dmlist_init("string_module");
    
    // Add strings to the list
    dmlist_push_back(list, "Hello");
    dmlist_push_back(list, "World");
    dmlist_push_back(list, "!");
    
    // Print all strings
    for (size_t i = 0; i < dmlist_size(list); i++) {
        const char* str = (const char*)dmlist_get(list, i);
        printf("%s ", str);
    }
    printf("\n");  // Output: Hello World !
    
    dmlist_destroy(list);
}
```

### Finding Elements

```c
#include "dmlist.h"
#include <string.h>

// Comparison function for strings
int compare_strings(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

// Comparison function for integers
int compare_ints(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ia - ib;
}

void find_example(void) {
    dmlist_context_t* list = dmlist_init("search_module");
    
    // Add some values
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        dmlist_push_back(list, &values[i]);
    }
    
    // Find a specific value
    int search_val = 30;
    int* found = (int*)dmlist_find(list, &search_val, compare_ints);
    if (found != NULL) {
        printf("Found: %d\n", *found);  // Output: Found: 30
    }
    
    dmlist_destroy(list);
}
```

### Iteration with Foreach

```c
#include "dmlist.h"
#include <stdio.h>

// Iterator callback function
bool print_int(void* data, void* user_data) {
    int* value = (int*)data;
    printf("%d ", *value);
    return true;  // Continue iteration
}

// Iterator with sum accumulator
bool sum_accumulator(void* data, void* user_data) {
    int* value = (int*)data;
    int* sum = (int*)user_data;
    *sum += *value;
    return true;
}

void iteration_example(void) {
    dmlist_context_t* list = dmlist_init("iter_module");
    
    // Add values
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        dmlist_push_back(list, &values[i]);
    }
    
    // Print all values
    printf("Values: ");
    dmlist_foreach(list, print_int, NULL);
    printf("\n");  // Output: Values: 1 2 3 4 5
    
    // Calculate sum
    int sum = 0;
    dmlist_foreach(list, sum_accumulator, &sum);
    printf("Sum: %d\n", sum);  // Output: Sum: 15
    
    dmlist_destroy(list);
}
```

### Inserting at Specific Position

```c
#include "dmlist.h"
#include <stdio.h>

void insert_example(void) {
    dmlist_context_t* list = dmlist_init("insert_module");
    
    int values[] = {1, 2, 4, 5};
    
    // Build initial list: 1, 2, 4, 5
    for (int i = 0; i < 4; i++) {
        dmlist_push_back(list, &values[i]);
    }
    
    // Insert 3 at position 2 (between 2 and 4)
    int val = 3;
    dmlist_insert(list, 2, &val);
    
    // Now list is: 1, 2, 3, 4, 5
    printf("List after insert: ");
    for (size_t i = 0; i < dmlist_size(list); i++) {
        int* v = (int*)dmlist_get(list, i);
        printf("%d ", *v);
    }
    printf("\n");  // Output: List after insert: 1 2 3 4 5
    
    dmlist_destroy(list);
}
```

### Removing Elements

```c
#include "dmlist.h"
#include <string.h>

int compare_strings(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

void remove_example(void) {
    dmlist_context_t* list = dmlist_init("remove_module");
    
    // Add elements
    dmlist_push_back(list, "apple");
    dmlist_push_back(list, "banana");
    dmlist_push_back(list, "cherry");
    dmlist_push_back(list, "date");
    
    // Remove "banana"
    const char* to_remove = "banana";
    bool removed = dmlist_remove(list, to_remove, compare_strings);
    printf("Removed: %s\n", removed ? "yes" : "no");  // Output: Removed: yes
    
    // Remove element at position 1
    const char* removed_val = (const char*)dmlist_remove_at(list, 1);
    printf("Removed at pos 1: %s\n", removed_val);  // Output: Removed at pos 1: cherry
    
    dmlist_destroy(list);
}
```

### Working with Structures

```c
#include "dmlist.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char name[50];
    int age;
    float score;
} Student;

int compare_students_by_age(const void* a, const void* b) {
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    return sa->age - sb->age;
}

bool print_student(void* data, void* user_data) {
    Student* s = (Student*)data;
    printf("  %s (age: %d, score: %.1f)\n", s->name, s->age, s->score);
    return true;
}

void struct_example(void) {
    dmlist_context_t* list = dmlist_init("student_module");
    
    // Create students
    Student s1 = {"Alice", 20, 85.5};
    Student s2 = {"Bob", 22, 90.0};
    Student s3 = {"Charlie", 19, 88.5};
    
    // Add to list
    dmlist_push_back(list, &s1);
    dmlist_push_back(list, &s2);
    dmlist_push_back(list, &s3);
    
    // Print all students
    printf("All students:\n");
    dmlist_foreach(list, print_student, NULL);
    
    // Find a specific student
    Student search = {"", 22, 0};
    Student* found = (Student*)dmlist_find(list, &search, compare_students_by_age);
    if (found != NULL) {
        printf("\nFound student aged 22: %s\n", found->name);
    }
    
    dmlist_destroy(list);
}
```

## API Reference

### Initialization and Cleanup

#### `dmlist_init`

```c
dmlist_context_t* dmlist_init(const char* module_name);
```

Initialize a linked list context.

- **Parameters:**
  - `module_name`: Name of the module using the list (for memory tracking)
- **Returns:** Pointer to the list context, or `NULL` if initialization fails
- **Note:** All memory allocations for this list will be tracked under `module_name`

#### `dmlist_destroy`

```c
void dmlist_destroy(dmlist_context_t* ctx);
```

Destroy a linked list and free all its nodes.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Note:** This frees all node structures, but does NOT free the data pointers stored in the nodes

### Size and Status

#### `dmlist_size`

```c
size_t dmlist_size(dmlist_context_t* ctx);
```

Get the number of elements in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** Number of elements in the list
- **Time Complexity:** O(1)

#### `dmlist_is_empty`

```c
bool dmlist_is_empty(dmlist_context_t* ctx);
```

Check if the list is empty.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** `true` if the list is empty, `false` otherwise
- **Time Complexity:** O(1)

### Adding Elements

#### `dmlist_push_front`

```c
bool dmlist_push_front(dmlist_context_t* ctx, void* data);
```

Add an element to the front of the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `data`: Pointer to the data to add
- **Returns:** `true` if successful, `false` otherwise
- **Time Complexity:** O(1)

#### `dmlist_push_back`

```c
bool dmlist_push_back(dmlist_context_t* ctx, void* data);
```

Add an element to the back of the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `data`: Pointer to the data to add
- **Returns:** `true` if successful, `false` otherwise
- **Time Complexity:** O(1)

#### `dmlist_insert`

```c
bool dmlist_insert(dmlist_context_t* ctx, size_t position, void* data);
```

Insert an element at a specific position in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `position`: Position to insert at (0 = front)
  - `data`: Pointer to the data to insert
- **Returns:** `true` if successful, `false` otherwise
- **Time Complexity:** O(n)
- **Note:** If position >= size, the element is added to the back

### Removing Elements

#### `dmlist_pop_front`

```c
void* dmlist_pop_front(dmlist_context_t* ctx);
```

Remove and return the element at the front of the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** Pointer to the data, or `NULL` if the list is empty
- **Time Complexity:** O(1)

#### `dmlist_pop_back`

```c
void* dmlist_pop_back(dmlist_context_t* ctx);
```

Remove and return the element at the back of the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** Pointer to the data, or `NULL` if the list is empty
- **Time Complexity:** O(1)

#### `dmlist_remove_at`

```c
void* dmlist_remove_at(dmlist_context_t* ctx, size_t position);
```

Remove an element at a specific position in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `position`: Position to remove (0 = front)
- **Returns:** Pointer to the removed data, or `NULL` if position is out of bounds
- **Time Complexity:** O(n)

#### `dmlist_remove`

```c
bool dmlist_remove(dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func);
```

Remove an element from the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `data`: Pointer to the data to remove
  - `compare_func`: Comparison function to use
- **Returns:** `true` if the element was found and removed, `false` otherwise
- **Time Complexity:** O(n)

#### `dmlist_clear`

```c
void dmlist_clear(dmlist_context_t* ctx);
```

Remove all elements from the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Time Complexity:** O(n)
- **Note:** This frees all node structures, but does NOT free the data pointers

### Accessing Elements

#### `dmlist_front`

```c
void* dmlist_front(dmlist_context_t* ctx);
```

Get the element at the front of the list without removing it.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** Pointer to the data, or `NULL` if the list is empty
- **Time Complexity:** O(1)

#### `dmlist_back`

```c
void* dmlist_back(dmlist_context_t* ctx);
```

Get the element at the back of the list without removing it.

- **Parameters:**
  - `ctx`: Pointer to the list context
- **Returns:** Pointer to the data, or `NULL` if the list is empty
- **Time Complexity:** O(1)

#### `dmlist_get`

```c
void* dmlist_get(dmlist_context_t* ctx, size_t position);
```

Get an element at a specific position in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `position`: Position to get (0 = front)
- **Returns:** Pointer to the data, or `NULL` if position is out of bounds
- **Time Complexity:** O(n)

### Searching

#### `dmlist_find`

```c
void* dmlist_find(dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func);
```

Find an element in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `data`: Pointer to the data to find
  - `compare_func`: Comparison function to use (returns 0 if equal)
- **Returns:** Pointer to the found data, or `NULL` if not found
- **Time Complexity:** O(n)

### Iteration

#### `dmlist_foreach`

```c
void dmlist_foreach(dmlist_context_t* ctx, dmlist_iterator_func_t iterator_func, void* user_data);
```

Iterate over all elements in the list.

- **Parameters:**
  - `ctx`: Pointer to the list context
  - `iterator_func`: Callback function to call for each element (returns `true` to continue)
  - `user_data`: User-provided data passed to the iterator function
- **Time Complexity:** O(n)

### Callback Types

#### `dmlist_iterator_func_t`

```c
typedef bool (*dmlist_iterator_func_t)(void* data, void* user_data);
```

Callback function type for iterating over list elements.

- **Parameters:**
  - `data`: Pointer to the data stored in the node
  - `user_data`: User-provided data
- **Returns:** `true` to continue iteration, `false` to stop

#### `dmlist_compare_func_t`

```c
typedef int (*dmlist_compare_func_t)(const void* data1, const void* data2);
```

Callback function type for comparing list elements.

- **Parameters:**
  - `data1`: Pointer to the first data element
  - `data2`: Pointer to the second data element
- **Returns:** `0` if equal, `<0` if data1 < data2, `>0` if data1 > data2

## Examples

### Example 1: Task Queue

```c
#include "dmlist.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    int priority;
    char description[100];
} Task;

int compare_priority(const void* a, const void* b) {
    const Task* ta = (const Task*)a;
    const Task* tb = (const Task*)b;
    return ta->priority - tb->priority;
}

void task_queue_example(void) {
    dmlist_context_t* queue = dmlist_init("task_queue");
    
    // Add tasks
    Task t1 = {1, "Low priority task"};
    Task t2 = {5, "High priority task"};
    Task t3 = {3, "Medium priority task"};
    
    dmlist_push_back(queue, &t1);
    dmlist_push_back(queue, &t2);
    dmlist_push_back(queue, &t3);
    
    // Process tasks
    while (!dmlist_is_empty(queue)) {
        Task* task = (Task*)dmlist_pop_front(queue);
        printf("Processing: %s (priority: %d)\n", task->description, task->priority);
    }
    
    dmlist_destroy(queue);
}
```

### Example 2: History Buffer

```c
#include "dmlist.h"
#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 10

typedef struct {
    dmlist_context_t* list;
    size_t max_size;
} History;

History* history_create(void) {
    History* hist = (History*)Dmod_Malloc(sizeof(History), "history");
    hist->list = dmlist_init("history");
    hist->max_size = MAX_HISTORY;
    return hist;
}

void history_add(History* hist, const char* command) {
    // If at max size, remove oldest
    if (dmlist_size(hist->list) >= hist->max_size) {
        dmlist_pop_front(hist->list);
    }
    
    // Add new command
    dmlist_push_back(hist->list, (void*)command);
}

bool print_history_item(void* data, void* user_data) {
    int* index = (int*)user_data;
    const char* cmd = (const char*)data;
    printf("%d: %s\n", (*index)++, cmd);
    return true;
}

void history_print(History* hist) {
    int index = 0;
    dmlist_foreach(hist->list, print_history_item, &index);
}

void history_destroy(History* hist) {
    dmlist_destroy(hist->list);
    Dmod_Free(hist);
}
```

## Contributing

Contributions are welcome! Please feel free to submit issues, fork the repository, and create pull requests.

### Development Setup

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR_USERNAME/dmlist.git`
3. Create a feature branch: `git checkout -b feature/my-new-feature`
4. Make your changes and add tests
5. Run tests: `cd build && ctest`
6. Commit your changes: `git commit -am 'Add some feature'`
7. Push to the branch: `git push origin feature/my-new-feature`
8. Submit a pull request

### Code Style

- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Update documentation for API changes

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Choco-Technologies

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Acknowledgments

- [DMOD (Dynamic Modules)](https://github.com/choco-technologies/dmod) - The dynamic module loading framework
- Choco-Technologies team for creating and maintaining this project

## Related Projects

- [DMOD](https://github.com/choco-technologies/dmod) - Dynamic Module Loading Framework
- [dmheap](https://github.com/choco-technologies/dmheap) - DMOD Heap Memory Manager
- [dmlog](https://github.com/choco-technologies/dmlog) - DMOD Logging Library
- [dmvfs](https://github.com/choco-technologies/dmvfs) - DMOD Virtual File System

---

**For more information and support, please visit the [dmlist repository](https://github.com/choco-technologies/dmlist) or contact the Choco-Technologies team.**
