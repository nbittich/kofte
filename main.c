#include "conversion.h"
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1"
#define MILES_TO_KM_CMD "miles_to_km"

typedef enum { MILES_TO_KM } available_command;

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
  if (strncmp(input + start, command, length) == 0) {
    return true;
  }
  return false;
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

  if (is_command(input, start_cmd, end_cmd, MILES_TO_KM_CMD)) {
    output->command = MILES_TO_KM;
    char *end = input + end_num + 1;
    output->input = strtold(input + start_num, &end);
  } else {
    printf("COMMAND NOT FOUND\n");
    return -1;
  }

  return 0;
}

void clear_buffer(char *input, int size) {
  for (int i = 0; i < size; ++i) {
    input[i] = '\0';
  }
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
    int res = parse_command(input, &command, &len);

    if (res == 0) {
      switch (command.command) {
      case MILES_TO_KM:
        printf("%.2Lf miles is equal to %.2Lf km\n", command.input,
               miles_to_km(&command.input));
        break;
      default:
        printf("not implemented yet \n");
      }
    }
    clear_buffer(input, len);
  }
}
