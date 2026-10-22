# Changes made during standalone extraction (October 2026)

Extracted from the `c-projects-portfolio` collection repo. Only the project's
real source files were carried over — Visual Studio files (`.sln`, `.vcxproj`,
Debug folders, `.ilk`) were left behind.

## Bug fix: duplicate expense IDs across runs — `storage.c`

**Problem (proven by test):** `generateUniqueId()` used a `static` counter
starting at 1000. The counter resets every time the program starts, so the
first expense added in a new run was given ID 1000 even when an expense with
ID 1000 was already stored. Test of the original code: run 1 added IDs 1000
and 1001; after restarting, run 2 added another expense and it was again
assigned ID 1000 — two different expenses sharing one ID, which makes
edit/delete-by-ID ambiguous.

**Fix:** `generateUniqueId()` now loads the stored expenses and returns one
more than the highest ID in use (starting from 1000 when the file is empty).

**Verified after fix:** run 1 → IDs 1000, 1001; run 2 after restart → ID
1002; edit, delete, view, and summary all exercised successfully.

## Not carried over: `report.c`

The original repo's `report.c` is a 0-byte placeholder. No code was invented
for it: the summary report it was presumably meant to hold is already
implemented as `showSummary()` in `expense.c`, so no functionality is
missing.

## Update — October 22, 2026

**Feature: category summary.** Added `showCategorySummary()` to
`expense.c` (menu option 6; Exit moved to 7). It groups expenses by
category and prints each category's total with its percentage share of
overall spending, and reports "No expenses recorded." when the data
file is empty. Verified: built clean with
`gcc -std=c11 -Wall -Wextra`, then in a scripted run with Food $12.50,
Transport $4.00, and Food $7.50 it printed Food: $20.00 (83.3% of
total) and Transport: $4.00 (16.7% of total); the existing summary
report still showed the correct $24.00 total.
