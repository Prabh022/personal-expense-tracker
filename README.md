# Personal Expense Tracker (C)

A modular command-line expense tracker written in C. Log, view, edit, and
delete personal expenses, with a summary report and persistent storage in a
local binary data file.

## Features

- Add an expense (category, amount, date, description); each expense gets a
  unique numeric ID
- View all expenses
- Edit or delete an expense by ID
- Summary report: total spent, highest expense, lowest expense
- Persistent storage in `expenses.dat` (binary file, created automatically
  the first time you add an expense)

## Build

Requires a C compiler (developed with gcc):

```
gcc -std=c11 -Wall -Wextra main.c expense.c storage.c -o expense_tracker
```

## Run

```
./expense_tracker
```

Then use the on-screen menu:

```
1. Add Expense
2. View Expenses
3. Edit Expense
4. Delete Expense
5. Summary Report
6. Exit
```

## Sample output (trimmed from an actual run)

```
--- All Expenses ---
ID: 1001 | Transport | $4.00 | 2026-01-16
  Bus pass
ID: 1002 | Food | $9.99 | 2026-01-17
  Snacks

Total spent: $13.99
Highest expense: $9.99
Lowest expense: $4.00
```

## Project structure

```
main.c      Menu and program entry point
expense.c   Add, view, edit, delete, and summary logic
expense.h   Expense struct and function declarations
storage.c   Binary file storage (load/save) and ID generation
storage.h   Storage declarations, MAX_EXPENSES, data file name
```

## Notes

- Up to 1,000 expenses (`MAX_EXPENSES` in `storage.h`).
- This project was extracted from a C projects collection repo. The
  original repo also contained an empty `report.c` placeholder file; it
  held no code (the summary report is implemented in `expense.c`), so it
  was not carried over. See CHANGES.md for the one bug fix applied during
  extraction.
