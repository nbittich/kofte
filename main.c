#include "conversion.h"
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1"

const char *AVAILABLE_COMMANDS[] = {"mile_to_km",
                                    "km_to_mile",
                                    "inch_to_cm",
                                    "cm_to_inch",
                                    "foot_to_meter",
                                    "meter_to_foot",
                                    "yard_to_meter",
                                    "meter_to_yard",
                                    "fahr_to_celcius",
                                    "celcius_to_fahr",
                                    "kelvin_to_celcius",
                                    "celcius_to_kelvin",
                                    "help",
                                    "exit"};

const size_t AVAILABLE_COMMANDS_SIZE =
    (sizeof AVAILABLE_COMMANDS / sizeof AVAILABLE_COMMANDS[0]);

typedef enum {
  MILES_TO_KM,
  KM_TO_MILES,
  INCH_TO_CM,
  CM_TO_INCH,
  FOOT_TO_METER,
  METER_TO_FOOT,
  YARD_TO_METER,
  METER_TO_YARD,
  FAHR_TO_CELCIUS,
  CELCIUS_TO_FAHR,
  KELVIN_TO_CELCIUS,
  CELCIUS_TO_KELVIN,
  HELP,
  EXIT
} available_command;

typedef struct {
  available_command command;
  long double input;
} command_t;

void intHandler(int dummy) {
  printf("BYE\n");
  exit(0);
}

bool is_command(const char *input, int start, int end, const char *command) {
  int length = end - start + 1;
  int command_length = strlen(command);
  if (command_length > length) {
    return false;
  }
  return strncmp(input + start, command, length) == 0;
}

int parse_command(char *input, command_t *output, const size_t *len) {

  int start_cmd = -1, end_cmd = -1;
  int start_num = -1, end_num = -1;

  for (size_t i = 0; i < *len; i++) {
    const char curr = input[i];
    if (isspace(curr)) {
      if (start_cmd > -1 && end_cmd == -1) {
        end_cmd = i - 1;
      }

      continue;
    }
    if (isalpha(curr) || curr == '_') {
      if (end_cmd > -1) {
        printf("ERROR parsing (command) \n");
        return -1;
      } else if (start_cmd == -1) {
        start_cmd = i;
      }
    }
    if (isdigit(curr) || curr == '.' || curr == '-') {
      if (start_cmd == -1 || end_cmd == -1) {
        printf("ERROR parsing (digit)\n");
        return -1;
      }
      if (start_num == -1) {
        if (curr == '.') {
          printf("ERROR parsing (digit start)\n");
          return -1;
        }
        start_num = i;
      } else {
        end_num = i;
      }
    }
  }
  if (end_num == -1) {
    end_num = start_num;
  }

  int *found = NULL;
  int i;

  for (i = 0; i < AVAILABLE_COMMANDS_SIZE; i++) {
    if (is_command(input, start_cmd, end_cmd, AVAILABLE_COMMANDS[i])) {
      found = &i;
      break;
    }
  }

  if (found) {
    output->command = *found;
  } else {
    printf("COMMAND NOT FOUND\n");
    return -1;
  }
  char *end = input + end_num + 1;
  output->input = strtold(input + start_num, &end);
  return 0;
}

void clear_buffer(char *input, int size) {
  for (int i = 0; i < size; ++i) {
    input[i] = '\0';
  }
}

void run_command(char *input, command_t *command, const size_t *len) {
  int res = parse_command(input, command, len);

  if (res == 0) {
    switch (command->command) {
    case MILES_TO_KM:
      printf("%.2Lf miles is equal to %.2Lf km\n", command->input,
             miles_to_km(&command->input));
      break;
    case KM_TO_MILES:
      printf("%.2Lf km is equal to %.2Lf miles\n", command->input,
             km_to_miles(&command->input));
      break;
    case INCH_TO_CM:
      printf("%.2Lf inch is equal to %.2Lf cm\n", command->input,
             inch_to_cm(&command->input));
      break;
    case CM_TO_INCH:
      printf("%.2Lf cm is equal to %.2Lf inch\n", command->input,
             cm_to_inch(&command->input));
      break;
    case FOOT_TO_METER:
      printf("%.2Lf foot is equal to %.2Lf meters\n", command->input,
             foot_to_meter(&command->input));
      break;
    case METER_TO_FOOT:
      printf("%.2Lf meters is equal to %.2Lf foot\n", command->input,
             meter_to_foot(&command->input));
      break;
    case YARD_TO_METER:
      printf("%.2Lf yards is equal to %.2Lf meters\n", command->input,
             yard_to_meter(&command->input));
      break;
    case METER_TO_YARD:
      printf("%.2Lf meters is equal to %.2Lf yards\n", command->input,
             meter_to_yard(&command->input));
      break;
    case CELCIUS_TO_FAHR:
      printf("%.2Lf°C is equal to %.2Lf°F\n", command->input,
             celcius_to_fahrenheit(&command->input));
      break;
    case FAHR_TO_CELCIUS:
      printf("%.2Lf°F is equal to %.2Lf°C\n", command->input,
             fahrenheit_to_celcius(&command->input));
      break;
    case CELCIUS_TO_KELVIN:
      printf("%.2Lf°C is equal to %.2Lf°K\n", command->input,
             celcius_to_kelvin(&command->input));
      break;
    case KELVIN_TO_CELCIUS:
      printf("%.2Lf°K is equal to %.2Lf°C\n", command->input,
             kelvin_to_celcius(&command->input));
      break;
    case EXIT: {
      printf("BYE\n");
      exit(0);
    }
    case HELP: {
      int max_len = strlen(AVAILABLE_COMMANDS[0]);
      for (size_t i = 1; i < AVAILABLE_COMMANDS_SIZE; ++i) {
        int len = strlen(AVAILABLE_COMMANDS[i]);
        if (len > max_len) {
          max_len = len;
        }
      }
      for (size_t i = 0; i < AVAILABLE_COMMANDS_SIZE; ++i) {
        printf("%-*s\t", max_len, AVAILABLE_COMMANDS[i]);
        if ((i + 1) % 3 == 0) {
          putchar('\n');
        }
      }
      putchar('\n');
      break;
    }
    default:
      printf("not implemented yet \n");
    }
  }
  clear_buffer(input, *len);
}

int main() {
  signal(SIGINT, intHandler);
  printf("Conversion REPL v%s\n", VERSION);
  command_t command = {0};
  char *input = NULL;
  size_t len = 0;
  ssize_t read = 0;
  while (true) {
    printf("> ");
    read = getline(&input, &len, stdin);
    if (read == -1) {
      clearerr(stdin);
      printf("\n");
      continue;
    }
    run_command(input, &command, &len);
  }
}
