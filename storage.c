// storage.c

#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int loadExpenses(Expense expenses[]) {
    FILE* fp = fopen(DATA_FILE, "rb");
    if (!fp) return 0;

    int count = 0;
    while (fread(&expenses[count], sizeof(Expense), 1, fp)) {
        count++;
    }
    fclose(fp);
    return count;
}

void saveExpenses(Expense expenses[], int count) {
    FILE* fp = fopen(DATA_FILE, "wb");
    for (int i = 0; i < count; i++) {
        fwrite(&expenses[i], sizeof(Expense), 1, fp);
    }
    fclose(fp);
}

int generateUniqueId() {
    // Derive the next ID from the stored expenses so IDs stay unique
    // across program runs (a static counter restarts at 1000 every run
    // and hands out IDs that are already in the data file).
    Expense existing[MAX_EXPENSES];
    int count = loadExpenses(existing);
    int maxId = 999;
    for (int i = 0; i < count; i++) {
        if (existing[i].id > maxId) {
            maxId = existing[i].id;
        }
    }
    return maxId + 1;
}
