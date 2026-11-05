// expense.c
#define _CRT_SECURE_NO_WARNINGS

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "expense.h"
#include "storage.h"

/* Case-insensitive substring test: returns 1 if needle appears in
   haystack ignoring case, 0 otherwise. An empty needle matches. */
static int containsIgnoreCase(const char *haystack, const char *needle) {
    size_t needleLen = strlen(needle);
    if (needleLen == 0) return 1;
    for (const char *p = haystack; *p != '\0'; p++) {
        size_t i;
        for (i = 0; i < needleLen; i++) {
            if (p[i] == '\0') return 0;
            if (tolower((unsigned char)p[i]) !=
                tolower((unsigned char)needle[i])) {
                break;
            }
        }
        if (i == needleLen) return 1;
    }
    return 0;
}

void addExpense() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    Expense e;
    e.id = generateUniqueId();
    printf("Enter category: ");
    scanf("%s", e.category);
    printf("Enter amount: ");
    scanf("%f", &e.amount);
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", e.date);
    printf("Enter description: ");
    getchar();  // flush newline
    fgets(e.description, sizeof(e.description), stdin);
    strtok(e.description, "\n");  // remove trailing newline

    expenses[count++] = e;
    saveExpenses(expenses, count);

    printf("Expense added with ID: %d\n", e.id);
}

void viewExpenses() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    printf("\n--- All Expenses ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | %s | $%.2f | %s\n  %s\n",
            expenses[i].id, expenses[i].category,
            expenses[i].amount, expenses[i].date,
            expenses[i].description);
    }
}

void editExpense() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    int id;
    printf("Enter ID to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (expenses[i].id == id) {
            printf("Editing Expense ID %d\n", id);
            printf("Enter new category: ");
            scanf("%s", expenses[i].category);
            printf("Enter new amount: ");
            scanf("%f", &expenses[i].amount);
            printf("Enter new date (YYYY-MM-DD): ");
            scanf("%s", expenses[i].date);
            printf("Enter new description: ");
            getchar();
            fgets(expenses[i].description, sizeof(expenses[i].description), stdin);
            strtok(expenses[i].description, "\n");
            saveExpenses(expenses, count);
            printf("Updated successfully.\n");
            return;
        }
    }
    printf("Expense ID not found.\n");
}

void deleteExpense() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (expenses[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                expenses[j] = expenses[j + 1];
            }
            count--;
            found = 1;
            break;
        }
    }

    if (found) {
        saveExpenses(expenses, count);
        printf("Expense deleted.\n");
    }
    else {
        printf("Expense ID not found.\n");
    }
}

void showSummary() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    float total = 0, max = 0, min = 1e9;
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
        if (expenses[i].amount > max) max = expenses[i].amount;
        if (expenses[i].amount < min) min = expenses[i].amount;
    }

    printf("Total spent: $%.2f\n", total);
    printf("Highest expense: $%.2f\n", max);
    printf("Lowest expense: $%.2f\n", min);
}

void showCategorySummary() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    if (count == 0) {
        printf("No expenses recorded.\n");
        return;
    }

    float grandTotal = 0;
    for (int i = 0; i < count; i++) {
        grandTotal += expenses[i].amount;
    }

    printf("\n--- Spending by Category ---\n");
    for (int i = 0; i < count; i++) {
        int seenBefore = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(expenses[j].category, expenses[i].category) == 0) {
                seenBefore = 1;
                break;
            }
        }
        if (seenBefore) continue;

        float categoryTotal = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(expenses[j].category, expenses[i].category) == 0) {
                categoryTotal += expenses[j].amount;
            }
        }
        float share = (grandTotal > 0) ? (categoryTotal / grandTotal) * 100 : 0;
        printf("%s: $%.2f (%.1f%% of total)\n",
            expenses[i].category, categoryTotal, share);
    }
}

void searchExpenses() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    char query[20];
    printf("Enter category to search: ");
    scanf("%19s", query);

    printf("\n--- Matching Expenses ---\n");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsIgnoreCase(expenses[i].category, query)) {
            printf("ID: %d | %s | $%.2f | %s\n  %s\n",
                expenses[i].id, expenses[i].category,
                expenses[i].amount, expenses[i].date,
                expenses[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No expenses found in category '%s'.\n", query);
    }
}

void searchExpensesByDate() {
    Expense expenses[MAX_EXPENSES];
    int count = loadExpenses(expenses);

    char query[11];
    printf("Enter date to search (YYYY-MM-DD, or YYYY-MM for a whole month): ");
    scanf("%10s", query);

    printf("\n--- Matching Expenses ---\n");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsIgnoreCase(expenses[i].date, query)) {
            printf("ID: %d | %s | $%.2f | %s\n  %s\n",
                expenses[i].id, expenses[i].category,
                expenses[i].amount, expenses[i].date,
                expenses[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No expenses found on date '%s'.\n", query);
    }
}
