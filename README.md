# Group Expense Splitter

A command-line application written in C that helps split expenses fairly among a group of people.

## Features

- Handles up to 9 people per group
- Calculates fair share for each person based on total expenses
- Distributes remainder cents to ensure accurate splitting
- Optimizes payment settlements to minimize the number of transactions
- Color-coded terminal output for better readability

## How It Works

The application:
1. Prompts for the number of people in the group (2-9)
2. Collects names for each person (up to 9 characters)
3. Records how much each person spent
4. Calculates the average amount and each person's balance
5. Determines the optimal payment transactions to settle all debts

The settlement algorithm minimizes the number of payments needed by matching creditors (those who are owed money) with debtors (those who owe money) in an efficient manner.

## Building

Requirements:
- CMake 4.0 or higher
- C17-compatible compiler

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./GroupExpenseSplitter
```

## Example

```
Enter the number of people (up to 9): 3

What are their names? (up to 9 characters)
Person 1: Alice
Person 2: Bob
Person 3: Charlie

How much did each person spend?
1.      Alice: 50.00
2.        Bob: 30.00
3.    Charlie: 20.00

==========================================
          Alice spent:               50.00
            Bob spent:               30.00
        Charlie spent:               20.00
------------------------------------------
   Total amount spent:              100.00
 Average amount spent:               33.33
------------------------------------------
        Alice balance:               16.66
          Bob balance:               -3.33
      Charlie balance:              -13.33
------------------------------------------
Charlie   →     Alice:               13.33
Bob       →     Alice:                3.33
==========================================
```

## Implementation Details

- Money is stored internally as cents (integers) to avoid floating-point precision issues
- Remainder cents from division are distributed fairly among the first N people
- Uses `qsort` for efficient sorting of creditors and debtors
- Names are limited to 9 characters to maintain consistent output formatting
