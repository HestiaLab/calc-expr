#include <calcexpr/calculator.h>

int main(void) {
    double value = 0.0;
    size_t err_pos = 0;

    if (evaluate("20+22", &value, &err_pos) != CALC_OK) {
        return 1;
    }

    return value == 42.0 ? 0 : 1;
}
