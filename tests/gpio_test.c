#include "mock_framework.h"
#include "gpio_driver.h"

// Mock for hardware register access
static uint32_t mock_gpio_register = 0;

// Mock function declarations
REGISTER_MOCK(hw_write_register);
static void mock_hw_write_handler(void* args[], int argCount) {
    if (argCount > 0 && args[0]) {
        mock_gpio_register = *(uint32_t*)args[0];
    }
}

// Test setup and teardown
void setup(void) {
    mock_gpio_register = 0;
    MockFunction* hw_write_mock = register_hw_write_register_mock();
    hw_write_mock->handler = mock_hw_write_handler;
}

void teardown(void) {
    // Clean up any test resources
}

// Test cases
void test_gpio_init(void) {
    GPIO_Pin pin = {.pin = 1};
    gpio_init(&pin);
    
    ASSERT_EQUAL(GPIO_LOW, pin.state);
}

void test_gpio_set_state(void) {
    GPIO_Pin pin = {.pin = 1};
    gpio_init(&pin);
    
    gpio_set_state(&pin, GPIO_HIGH);
    ASSERT_EQUAL(GPIO_HIGH, pin.state);
    
    gpio_set_state(&pin, GPIO_LOW);
    ASSERT_EQUAL(GPIO_LOW, pin.state);
}

void test_gpio_get_state(void) {
    GPIO_Pin pin = {.pin = 1};
    gpio_init(&pin);
    
    gpio_set_state(&pin, GPIO_HIGH);
    GPIO_State state = gpio_get_state(&pin);
    ASSERT_EQUAL(GPIO_HIGH, state);
}

int main(void) {
    TestSuite gpio_suite = {
        .name = "GPIO Driver Tests",
        .setup = setup,
        .teardown = teardown
    };
    
    TestCase test_cases[] = {
        {"GPIO Initialization", test_gpio_init},
        {"GPIO Set State", test_gpio_set_state},
        {"GPIO Get State", test_gpio_get_state}
    };
    
    run_test_suite(&gpio_suite, test_cases, 
                   sizeof(test_cases) / sizeof(test_cases[0]));
    print_test_summary();
    
    return testStats.failedTests;
}