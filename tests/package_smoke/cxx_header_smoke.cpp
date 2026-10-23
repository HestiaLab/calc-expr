#include <calcexpr/calculator.h>

int main() {
    double value = 0.0;
    return evaluate("1+2", &value, nullptr) == CALC_OK && value == 3.0 ? 0 : 1;
}
