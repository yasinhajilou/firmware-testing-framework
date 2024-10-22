#ifndef MOCK_FRAMEWORK_H
#define MOCK_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TEST_CATEGORY_UNIT,
    TEST_CATEGORY_INTEGRATION,
    TEST_CATEGORY_PERFORMANCE
} TestCategory;

// Test framework structures
typedef struct {
    const char* name;
    TestCategory category;  
    void (*setup)(void);
    void (*teardown)(void);
} TestSuite;

typedef struct {
    const char* name;
    void (*testFunc)(void);
} TestCase;



typedef struct {
    const char* name;
    void* params;
} TestParams;

// Mock function tracking
#define MAX_MOCK_CALLS 100
#define MAX_ARGS_PER_CALL 10

typedef struct {
    const char* funcName;
    int callCount;
    struct {
        void* args[MAX_ARGS_PER_CALL];
        int argCount;
        void* returnValue;
    } calls[MAX_MOCK_CALLS];
    void (*handler)(void* args[], int argCount);
} MockFunction;

// Global test statistics
static struct {
    int totalTests;
    int passedTests;
    int failedTests;
} testStats = {0, 0, 0};

// Mock function registry
static MockFunction mockRegistry[50];
static int mockCount = 0;

// Mock registration and management
MockFunction* mock_register(const char* name) {
    mockRegistry[mockCount].funcName = name;
    mockRegistry[mockCount].callCount = 0;
    mockRegistry[mockCount].handler = NULL;
    return &mockRegistry[mockCount++];
}

void mock_record_call(MockFunction* mock, void* args[], int argCount) {
    int currentCall = mock->callCount;
    if (currentCall < MAX_MOCK_CALLS) {
        for (int i = 0; i < argCount && i < MAX_ARGS_PER_CALL; i++) {
            mock->calls[currentCall].args[i] = args[i];
        }
        mock->calls[currentCall].argCount = argCount;
        mock->callCount++;
    }
    
    if (mock->handler) {
        mock->handler(args, argCount);
    }
}

// Assertion macros
#define ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("❌ Assertion failed: %s:%d\n", __FILE__, __LINE__); \
            printf("Expected: %d, Got: %d\n", (expected), (actual)); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

#define ASSERT_STRING_EQUAL(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf("❌ Assertion failed: %s:%d\n", __FILE__, __LINE__); \
            printf("Expected: %s, Got: %s\n", (expected), (actual)); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

    #define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            printf("❌ Assertion failed: %s:%d\n", __FILE__, __LINE__); \
            printf("Expected NULL, Got non-NULL\n"); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            printf("❌ Assertion failed: %s:%d\n", __FILE__, __LINE__); \
            printf("Expected non-NULL, Got NULL\n"); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

#define ASSERT_EXECUTION_TIME(stmt, max_ms) \
    do { \
        clock_t start = clock(); \
        stmt; \
        clock_t end = clock(); \
        double time_ms = (double)(end - start) * 1000 / CLOCKS_PER_SEC; \
        if (time_ms > max_ms) { \
            printf("❌ Performance test failed: %s:%d\n", __FILE__, __LINE__); \
            printf("Expected execution time <= %f ms, Got %f ms\n", \
                   (double)max_ms, time_ms); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

// Add memory tracking functions
void* tracked_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    printf("Memory allocated at %s:%d - %p\n", file, line, ptr);
    return ptr;
}

void tracked_free(void* ptr, const char* file, int line) {
    printf("Memory freed at %s:%d - %p\n", file, line, ptr);
    free(ptr);
}

#define TEST_MALLOC(size) tracked_malloc(size, __FILE__, __LINE__)
#define TEST_FREE(ptr) tracked_free(ptr, __FILE__, __LINE__)

// Mock registration macro
#define REGISTER_MOCK(funcName) \
    static MockFunction* funcName##_mock = NULL; \
    MockFunction* register_##funcName##_mock(void) { \
        funcName##_mock = mock_register(#funcName); \
        return funcName##_mock; \
    }

// Mock verification
#define VERIFY_MOCK_CALLS(mock, expected) \
    do { \
        if ((mock)->callCount != (expected)) { \
            printf("❌ Mock verification failed: %s\n", (mock)->funcName); \
            printf("Expected calls: %d, Actual calls: %d\n", \
                   (expected), (mock)->callCount); \
            testStats.failedTests++; \
            return; \
        } \
    } while (0)

// Test runner functions
void run_test_suite(TestSuite* suite, TestCase* cases, int caseCount) {
    printf("\nRunning test suite: %s\n", suite->name);
    printf("----------------------------------------\n");

    for (int i = 0; i < caseCount; i++) {
        testStats.totalTests++;
        
        if (suite->setup) suite->setup();
        
        printf("Running test: %s... ", cases[i].name);
        cases[i].testFunc();
        
        if (suite->teardown) suite->teardown();

        if (testStats.failedTests < testStats.totalTests) {
            printf("✅ Passed\n");
            testStats.passedTests++;
        }
    }
}

void print_test_summary(void) {
    printf("\nTest Summary\n");
    printf("----------------------------------------\n");
    printf("Total Tests: %d\n", testStats.totalTests);
    printf("Passed: %d\n", testStats.passedTests);
    printf("Failed: %d\n", testStats.failedTests);
    printf("Success Rate: %.1f%%\n", 
           (float)testStats.passedTests / testStats.totalTests * 100);
}

#endif // MOCK_FRAMEWORK_H