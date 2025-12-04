#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

bool is_increasing(int levels[], int count) {
    for (int i = 0; i < count - 1; i++) {
        if (levels[i + 1] < levels[i]) {
            return false;
        }
    }
    return true;
}

bool is_decreasing(int levels[], int count) {
    for (int i = 0; i < count - 1; i++) {
        if (levels[i + 1] > levels[i]) {
            return false;
        }
    }
    return true;
}

// Function to check if a report is safe
bool is_safe(int levels[], int count) {
    for (int i = 0; i < count - 1; i++) {
        int diff = abs(levels[i + 1] - levels[i]);
        if (diff < 1 || diff > 3) {
            return false; // Difference rule violated
        }
    }
    return true;
}

// Function to check if removing one level makes the report safe
bool is_safe_with_removal(int levels[], int count) {
    for (int i = 0; i < count; i++) {
        int temp_levels[MAX_LEVELS];
        int temp_count = 0;

        // Copy all levels except the current one
        for (int j = 0; j < count; j++) {
            if (j != i) {
                temp_levels[temp_count++] = levels[j];
            }
        }

        // Check if the modified report is safe
        if (is_safe(temp_levels, temp_count) &&
            (is_increasing(temp_levels, temp_count) || is_decreasing(temp_levels, temp_count))) {
            return true;
        }
    }
    return false;
}

int main() {
    FILE *fp = fopen("input.in", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    int safe_reports = 0;

    while (fgets(line, sizeof(line), fp)) {
        int levels[MAX_LEVELS];
        int count = 0;

        // Tokenize the line into integers
        char *token = strtok(line, " \t\n");
        while (token != NULL && count < MAX_LEVELS) {
            levels[count++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }

        // Check if the report is safe or can be made safe by removing one level
        if (count > 1 && (is_safe(levels, count) &&
            (is_increasing(levels, count) || is_decreasing(levels, count))) ||
            is_safe_with_removal(levels, count)) {
            safe_reports++;
        }
    }

    fclose(fp);
    printf("Number of safe reports: %d\n", safe_reports);
    return 0;
}

