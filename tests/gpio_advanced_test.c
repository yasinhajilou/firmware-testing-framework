#include "mock_framework.h"
#include "gpio_driver.h"
#include <time.h> 
// Test data structure for parameterized tests
typedef struct {
uint8_t pin;
GPIO_State initial_state;
GPIO_State expected_state;
} GPIOTestParams;
// Setup and teardown
static void setup(void) {
// Your existing setup code
}
static void teardown(void) {
// Your existing teardown code
}
// Parameterized test example
void test_gpio_state_parameterized(void) {
GPIOTestParams test_cases[] = {
{1, GPIO_LOW, GPIO_HIGH},
{2, GPIO_HIGH, GPIO_LOW},
{3, GPIO_LOW, GPIO_LOW}
};

for (size_t i = 0; i < sizeof(test_cases)/sizeof(test_cases[0]); i++) {
    GPIO_Pin pin = {.pin = test_cases[i].pin};
    
    gpio_init(&pin);
    gpio_set_state(&pin, test_cases[i].initial_state);
    ASSERT_EQUAL(test_cases[i].initial_state, gpio_get_state(&pin));
    
    gpio_set_state(&pin, test_cases[i].expected_state);
    ASSERT_EQUAL(test_cases[i].expected_state, gpio_get_state(&pin));
}
}
// Performance test example
void test_gpio_performance(void) {
GPIO_Pin pin = {.pin = 1};

ASSERT_EXECUTION_TIME({
    gpio_init(&pin);
    for(int i = 0; i < 1000; i++) {
        gpio_set_state(&pin, GPIO_HIGH);
        gpio_set_state(&pin, GPIO_LOW);
    }
}, 50.0); // Should complete within 50ms
}

// Memory safety test example
void test_gpio_memory_safety(void) {
GPIO_Pin* pin = TEST_MALLOC(sizeof(GPIO_Pin));
ASSERT_NOT_NULL(pin);
pin->pin = 1;
gpio_init(pin);
gpio_set_state(pin, GPIO_HIGH);
ASSERT_EQUAL(GPIO_HIGH, gpio_get_state(pin));

TEST_FREE(pin);
}
int main(void) {
TestSuite gpio_suite = {
.name = "GPIO Advanced Tests",
.category = TEST_CATEGORY_UNIT,
.setup = setup,
.teardown = teardown
};

TestCase test_cases[] = {
    {"GPIO State Parameterized", test_gpio_state_parameterized},
    {"GPIO Performance", test_gpio_performance},
    {"GPIO Memory Safety", test_gpio_memory_safety}
};

run_test_suite(&gpio_suite, test_cases, 
               sizeof(test_cases) / sizeof(test_cases[0]));
print_test_summary();

return testStats.failedTests;
}