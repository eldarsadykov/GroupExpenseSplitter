#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#define MAX_INPUT_LENGTH 32
#define MAX_NAME_LENGTH 10
#define MAX_PEOPLE 9
#define DIVIDER_WIDTH 42

#define COLOR_RESET "\x1b[0m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED   "\x1b[31m"
#define COLOR_CYAN  "\x1b[36m"
#define COLOR_BOLD  "\x1b[1m"

typedef int Cents;

typedef struct
{
  int id_;
  Cents money_spent_;
  Cents balance_;
  char name_[MAX_NAME_LENGTH];
} Person;

typedef struct
{
  int number_of_people_;
  Person people_[MAX_PEOPLE];
  Cents total_money_spent_;
  Cents average_money_spent_;
  int iterations_;
} App;

void promptNames(App *app);

void promptNumberOfPeople(App *app);

bool enterNumberOfPeople(int *number_of_people);

void promptMoneySpent(App *app);

bool enterMoneySpent(Person people[MAX_PEOPLE], int person_index);

void printResults(App *app);

void printPersonBalance(const Person *person);

double max(double a, double b);

double min(double a, double b);

Cents doubleToCents(double number);

double centsToDouble(Cents number);

void printMoneySpent(const App *app);

void distributeRemainder(App *app);

void printDivider(char character);

int cmp_creditors(const void *a, const void *b);

int cmp_debtors(const void *a, const void *b);

void settle(Person people[], int count);

int main(void)
{
  App app = {
    .number_of_people_ = 0,
    .people_ = {(Person){0, 0, 0}},
    .total_money_spent_ = 0,
    .iterations_ = 0,
  };

  for (int i = 0; i < MAX_PEOPLE; i++)
  {
    app.people_[i].id_ = i + 1;
  }

  promptNumberOfPeople(&app);
  promptNames(&app);
  promptMoneySpent(&app);
  printResults(&app);

  return 0;
}

void promptNames(App *app)
{
  printf("\nWhat are their names? (up to %d characters)\n", MAX_NAME_LENGTH - 1);
  for (int i = 0; i < app->number_of_people_; i++)
  {
    printf("Person %d: ", i + 1);

    if (fgets(app->people_[i].name_, MAX_NAME_LENGTH, stdin) != NULL)
    {
      const size_t len = strlen(app->people_[i].name_);

      // Remove trailing newline if present
      if (len > 0 && app->people_[i].name_[len - 1] == '\n')
      {
        app->people_[i].name_[len - 1] = '\0';
      }
      else
      {
        // Input exceeded buffer, flush remaining characters
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
      }
    }
  }
}

void promptNumberOfPeople(App *app)
{
  bool no_error = true;
  do
  {
    no_error = enterNumberOfPeople(&app->number_of_people_);
  } while (!no_error);
}

bool enterNumberOfPeople(int *number_of_people)
{
  printf("Enter the number of people (up to %d): ", MAX_PEOPLE);

  char input_string[MAX_INPUT_LENGTH] = {0};
  fgets(input_string, MAX_INPUT_LENGTH, stdin);

  char *end = NULL;
  *number_of_people = (int) strtol(input_string, &end, 10);

  if (*end != '\0' && *end != '\n')
  {
    printf("Please enter a valid number.\n");
    return false;
  }

  if (*number_of_people <= 1)
  {
    printf("That is too few.\n");
    return false;
  }

  if (*number_of_people > MAX_PEOPLE)
  {
    printf("That is too many.\n");
    return false;
  }

  return true;
}

void promptMoneySpent(App *app)
{
  printf("\nHow much did each person spend?\n");

  for (int i = 0; i < app->number_of_people_; i++)
  {
    bool no_error = true;
    do
    {
      no_error = enterMoneySpent(app->people_, i);
    } while (!no_error);

    app->total_money_spent_ += app->people_[i].money_spent_;
  }
}

bool enterMoneySpent(Person people[MAX_PEOPLE], const int person_index)
{
  printf("%d. %10s: ", people[person_index].id_, people[person_index].name_);

  char input_string[MAX_INPUT_LENGTH] = {0};
  fgets(input_string, MAX_INPUT_LENGTH, stdin);

  char *end = NULL;
  people[person_index].money_spent_ = doubleToCents(strtod(input_string, &end));

  if (*end != '\0' && *end != '\n')
  {
    printf("Please enter a valid number.\n");
    return false;
  }

  return true;
}

void printResults(App *app)
{
  printf("\n");

  printDivider('=');

  printMoneySpent(app);

  printDivider('-');

  printf("   Total amount spent:   %17.2f\n", centsToDouble(app->total_money_spent_));

  app->average_money_spent_ = app->total_money_spent_ / app->number_of_people_;

  printf(" Average amount spent: %19.2f\n", centsToDouble(app->average_money_spent_));

  printDivider('-');

  distributeRemainder(app);

  for (int i = 0; i < app->number_of_people_; i++)
  {
    printPersonBalance(&app->people_[i]);
  }

  settle(app->people_, app->number_of_people_);

  printDivider('=');
}

void printPersonBalance(const Person *person)
{
  printf("%13s balance:  %18.2f\n", person->name_, (double) person->balance_ / 100.0);
}

double max(const double a, const double b)
{
  return a > b ? a : b;
}

double min(const double a, const double b)
{
  return a < b ? a : b;
}

Cents doubleToCents(const double number)
{
  return (Cents) llround(number * 100.0);
}

double centsToDouble(const Cents number)
{
  return (double) number / 100.0;
}

void printMoneySpent(const App *app)
{
  for (int i = 0; i < app->number_of_people_; i++)
  {
    printf("%15s spent:    %16.2f\n",
           app->people_[i].name_,
           centsToDouble(app->people_[i].money_spent_));
  }
}

void distributeRemainder(App *app)
{
  const Cents base_share = app->total_money_spent_ / app->number_of_people_;
  const Cents remainder = app->total_money_spent_ % app->number_of_people_;

  for (int i = 0; i < app->number_of_people_; i++)
  {
    const Cents fair_share = base_share + (i < remainder ? 1 : 0);
    app->people_[i].balance_ = app->people_[i].money_spent_ - fair_share;
  }
}

void printDivider(const char character)
{
  for (int i = 0; i < DIVIDER_WIDTH; i++)
  {
    putchar(character);
  }
  putchar('\n');
}

int cmp_creditors(const void *a, const void *b)
{
  return ((Person *) b)->balance_ - ((Person *) a)->balance_; // Desc order
}

int cmp_debtors(const void *a, const void *b)
{
  return ((Person *) a)->balance_ - ((Person *) b)->balance_; // More negative first
}

void settle(Person people[], const int count)
{
  printDivider('-');
  Person creditors[MAX_PEOPLE], debtors[MAX_PEOPLE];
  int ci = 0, di = 0;

  for (int i = 0; i < count; i++)
  {
    if (people[i].balance_ > 0)
      creditors[ci++] = people[i];
    else if (people[i].balance_ < 0)
      debtors[di++] = people[i];
  }

  // Sort both groups
  qsort(creditors, ci, sizeof(Person), cmp_creditors);
  qsort(debtors, di, sizeof(Person), cmp_debtors);

  int c = 0, d = 0;
  while (c < ci && d < di)
  {
    const Cents amount = creditors[c].balance_ < -debtors[d].balance_
                           ? creditors[c].balance_
                           : -debtors[d].balance_;

    printf("%-9s → %9s: %19.2f\n",
           debtors[d].name_,
           creditors[c].name_,
           centsToDouble(amount));

    creditors[c].balance_ -= amount;
    debtors[d].balance_ += amount;

    if (creditors[c].balance_ == 0)
      c++;
    if (debtors[d].balance_ == 0)
      d++;
  }
}
