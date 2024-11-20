#include <ctype.h>
#include <stdlib.h>

double ft_atof(const char *str) {
    double result = 0.0;
    double fraction = 0.0;
    int sign = 1;
    int i = 0;

    // Handle leading whitespace
    while (isspace(str[i]))
        i++;

    // Handle sign
    if (str[i] == '-' || str[i] == '+') {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    // Convert integer part
    while (isdigit(str[i])) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    // Convert fractional part
    if (str[i] == '.') {
        i++;
        double divisor = 10.0;
        while (isdigit(str[i])) {
            fraction += (str[i] - '0') / divisor;
            divisor *= 10.0;
            i++;
        }
    }

    return sign * (result + fraction);
}
