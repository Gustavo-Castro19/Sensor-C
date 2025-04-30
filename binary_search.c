#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TIMES 10000

time_t *get_times(const char *lista_sensor[], int idx, int *out_size);
time_t binary_search(time_t target, time_t *times, int size);
time_t capturar_timestamp_valido();
void show_line(time_t target, const char **lista_sensor, int inp_option);

int main() {
  const char *lista_sensor[5] = {"TEMP", "PRESS", "UMID", "VIBR", "FLUX"};
  time_t *times = NULL;
  int size = 0;
  time_t timenow;

  puts("Qual tipo de sensor você quer consultar?");
  for (int i = 0; i < 5; i++) {
    printf("%d - %s\n", i + 1, lista_sensor[i]);
  }

  puts("Digite o número:");
  int option = 0;
  scanf("%d", &option);
  while (getchar() != '\n');   
  option -= 1;    
  if (option < 0 || option >= 5) {
    printf("Opção inválida.\n");
    return 1;
  }

  timenow = capturar_timestamp_valido();
  times = get_times(lista_sensor, option, &size);

  if (times == NULL) {
    printf("Erro ao ler timestamps.\n");
    return 1;
  }

  time_t target = binary_search(timenow, times, size);
  #ifdef DEBUG
  printf("%ld\n",timenow);
  printf("%ld\n",target);
  #endif
  show_line(target, lista_sensor, option);

  free(times);
  return 0;
}

time_t *get_times(const char **lista_sensor, int idx, int *out_size) {
  char filename[20];
  snprintf(filename, sizeof(filename), "%s.txt", lista_sensor[idx]);
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Não foi possível abrir o arquivo");
    return NULL;
  }
  time_t *timestamps = malloc(sizeof(time_t) * MAX_TIMES);
  if (timestamps==NULL) {
    fclose(fp);
    perror("Memória insuficiente");
    return NULL;
  }

  int count = 0;
  char line[256];
  while (fgets(line, sizeof(line), fp)) {
    char *token = strtok(line, " ");
    if (token) {
      timestamps[count++] = (time_t)atol(token);
      if (count >= MAX_TIMES) break;
    }
  }
  fclose(fp);
  *out_size = count;
  return timestamps;
}

time_t binary_search(time_t target, time_t *times, int size) {
  int low = 0, high = size - 1;
  int pivot;

  while (low <= high) {
    pivot = (low + high) / 2;
    if (times[pivot] == target) {
      return times[pivot];
    } else if (times[pivot] < target) {
      low = pivot + 1;
    } else {
      high = pivot - 1;
    }
  }

  if (low >= size) return times[high];
  if (high < 0) return times[low];

  if (labs(times[low] - target) < labs(times[high] - target)) {
    return times[low];
  } else {
    return times[high];
  }
}

time_t capturar_timestamp_valido() {
  int dia, mes, ano, hora, min, seg;
  struct tm t;

  while (1) {
    printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
    if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
      while (getchar() != '\n');
      printf("Entrada inválida. Tente novamente.\n");
      continue;
    }

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == -1) {
      printf("Data inválida. Tente novamente.\n");
    } else {
      return timestamp;
    }
  }
}

void show_line(time_t target, const char *lista_sensor[], int inp_option) {
  char filename[20];
  snprintf(filename, sizeof(filename), "%s.txt", lista_sensor[inp_option]);
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Erro ao abrir arquivo");
    return;
  }
  char line[256];
  char temp_line[256];
  printf("A leitura mais próxima da sua consulta no sensor %s foi:\n", lista_sensor[inp_option]);
  while (fgets(line, sizeof(line), fp)) {
    strncpy(temp_line,line,256);
    char *token = strtok(temp_line, " ");
    if ((time_t)atol(token) == target) {
      printf("%s", line);
      fclose(fp);
      return;
    }
  }

  printf("Leitura não encontrada.\n");
  fclose(fp);
}
