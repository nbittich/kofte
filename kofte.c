#include "conversion.h"
#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1.0.1"
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
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
                                    "ounce_to_gram",
                                    "gram_to_ounce",
                                    "pound_to_kg",
                                    "kg_to_pound",
                                    "help",
                                    "exit"};
const char *AVAILABLE_COMMANDS_ALT[] = {
    "mtk", "ktm", "itc", "cti", "ftm", "mtf", "ytm", "mty", "ftc",
    "ctf", "ktc", "ctk", "otg", "gto", "ptk", "ktp", "h",   "e"};

#define AVAILABLE_COMMANDS_SIZE ARRAY_SIZE(AVAILABLE_COMMANDS)

#define AVAILABLE_COMMANDS_ALT_SIZE ARRAY_SIZE(AVAILABLE_COMMANDS_ALT)

static_assert(AVAILABLE_COMMANDS_ALT_SIZE - AVAILABLE_COMMANDS_SIZE == 0,
              "ALT COMMAND SIZE != COMMAND SIZE");

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
  OUNCE_TO_GRAM,
  GRAM_TO_OUNCE,
  POUND_TO_KG,
  KG_TO_POUND,
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

int find_command(char *input, int start_cmd, int end_cmd) {
  for (int i = 0; i < AVAILABLE_COMMANDS_SIZE; i++) {
    if (is_command(input, start_cmd, end_cmd, AVAILABLE_COMMANDS[i])) {
      return i;
    }
  }
  for (int i = 0; i < AVAILABLE_COMMANDS_ALT_SIZE; i++) {
    if (is_command(input, start_cmd, end_cmd, AVAILABLE_COMMANDS_ALT[i])) {
      return i;
    }
  }
  return -1;
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

  int found = find_command(input, start_cmd, end_cmd);

  if (found >= 0) {
    output->command = found;
    char *end = input + end_num + 1;
    output->input = strtold(input + start_num, &end);
    return 0;
  }
  printf("COMMAND NOT FOUND\n");
  return -1;
}

void clear_buffer(char *input, int size) {
  for (int i = 0; i < size; ++i) {
    input[i] = '\0';
  }
}

char *plurarlize(long double count, char *singular, char *plural) {
  if (count > 1) {
    return plural;
  }
  return singular;
}

void run_command(command_t *command) {

  switch (command->command) {
  case MILES_TO_KM: {
    long double mtk = miles_to_km(&command->input);
    printf("%.2Lf %s is equal to %.2Lf km\n", command->input,
           plurarlize(command->input, "mile", "miles"), mtk);
    break;
  }
  case KM_TO_MILES: {
    long double ktm = km_to_miles(&command->input);
    printf("%.2Lf km is equal to %.2Lf %s\n", command->input, ktm,
           plurarlize(ktm, "mile", "miles"));
    break;
  }
  case INCH_TO_CM: {
    long double itc = inch_to_cm(&command->input);
    printf("%.2Lf %s is equal to %.2Lf cm\n", command->input,
           plurarlize(command->input, "inch", "inches"), itc);
    break;
  }
  case CM_TO_INCH: {
    long double cti = cm_to_inch(&command->input);
    printf("%.2Lf cm is equal to %.2Lf %s\n", command->input, cti,
           plurarlize(cti, "inch", "inches"));
    break;
  }
  case FOOT_TO_METER:
    printf("%.2Lf ft is equal to %.2Lf meters\n", command->input,
           foot_to_meter(&command->input));
    break;
  case METER_TO_FOOT: {
    long double mtf = meter_to_foot(&command->input);
    printf("%.2Lf %s is equal to %.2Lf ft\n", command->input,
           plurarlize(command->input, "meter", "meters"), mtf);
    break;
  }
  case YARD_TO_METER: {
    long double ytm = yard_to_meter(&command->input);
    printf("%.2Lf %s is equal to %.2Lf %s\n", command->input,
           plurarlize(command->input, "yard", "yards"), ytm,
           plurarlize(ytm, "meter", "meters"));
    break;
  }
  case METER_TO_YARD: {
    long double mty = meter_to_yard(&command->input);
    printf("%.2Lf %s is equal to %.2Lf %s\n", command->input,
           plurarlize(command->input, "meter", "meters"), mty,
           plurarlize(mty, "yard", "yards"));

    break;
  }
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
  case POUND_TO_KG: {
    long double ptk = pound_to_kg(&command->input);
    printf("%.2Lf %s is equal to %.2Lf kg\n", command->input,
           plurarlize(command->input, "pound", "pounds"), ptk);

    break;
  }
  case KG_TO_POUND: {
    long double ktp = kg_to_pound(&command->input);
    printf("%.2Lf kg is equal to %.2Lf %s\n", command->input, ktp,
           plurarlize(ktp, "pound", "pounds"));
    break;
  }
  case OUNCE_TO_GRAM: {
    long double otg = ounce_to_gram(&command->input);
    printf("%.2Lf %s is equal to %.2Lf %s\n", command->input,
           plurarlize(command->input, "ounce", "ounces"), otg,
           plurarlize(otg, "gram", "grams"));

    break;
  }
  case GRAM_TO_OUNCE: {
    long double gto = gram_to_ounce(&command->input);
    printf("%.2Lf %s is equal to %.2Lf %s\n", command->input,
           plurarlize(command->input, "gram", "grams"), gto,
           plurarlize(gto, "ounce", "ounces"));

    break;
  }
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
      const char *c = AVAILABLE_COMMANDS[i];
      if (strcmp(c, "exit") == 0 || strcmp(c, "help") == 0) {
        printf("%-*s \t", (int)(max_len + strlen(" <number>")), c);
      } else {
        printf("%-*s <number> \t", max_len, AVAILABLE_COMMANDS[i]);
      }
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

int main(int argc, char **argv) {
  command_t command = {0};
  if (argc > 1) {
    char *command_str = argv[1];
    int len = strlen(command_str);
    int found = find_command(argv[1], 0, len);
    if (found >= 0) {
      command.command = found;
      if (argc > 2) {
        char *input_str = argv[2];
        int len = strlen(input_str);
        char *end = input_str + len + 1;
        command.input = strtold(input_str, &end);
      }
      run_command(&command);
      return 0;
    } else {
      printf("COMMAND not found \n");
      return -1;
    }
  }

  signal(SIGINT, intHandler);
  printf("Köfte v%s\n", VERSION);
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
      run_command(&command);
      clear_buffer(input, len);
    }
  }
}
