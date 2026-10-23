#include <calcexpr/calculator.h>

int main(void) {
    double value = 0.0;
    size_t err_pos = 0;

    if (evaluate("4+5", &value, &err_pos) != CALC_OK) {
        return 1;
    }

    return value == 9.0 ? 0 : 1;
}
