#include "codexion.h"


int ft_atoi(char *target) {
    unsigned int num = 0;
    int sign = 1;

    while (*target == '\t' || *target == ' ')
        target++;
    while (*target == '-' || *target == '+') {
        if (*target == '-') {
            sign *= -1;
        }
        target++;
    }

    while (*target <= '9' && *target >= '0') {
        num = (num * 10) + (*target - '0');
        target++;
    }
    return (int)(num * sign);
}

void _ft_putstr(char *str) {
    while (*str) {
        write(1, str, 1);
        str++;
    }
}

