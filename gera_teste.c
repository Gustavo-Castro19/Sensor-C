#include <stdio.h>
#include <stdlib.h>
#include <time.h>


time_t capturar_timestamp_valido();

char *gerar_nome();

double gerar_num();

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano);

int main(void){
  FILE *fp;
  struct tm *timenow;
  time_t clock;

  srand(time(NULL));

  fp = fopen("test.txt", "w");
  if(fp==NULL){
    perror("error ao abrir o arquivo");
    return -1;
  }
  clock=capturar_timestamp_valido();
  timenow=localtime(&clock);
  for(int i=0;i<1000;i++){
    time_t randTimestamp=gerar_timestamp_aleatorio(timenow->tm_mday,timenow->tm_mon+1,timenow->tm_year+1900);
    fprintf(fp,"%ld %s %.2lf\n",randTimestamp,gerar_nome(),gerar_num());
  }
    #ifdef wantToKnow
    printf("%d\\%d\\%d",timenow->tm_mday,timenow->tm_mon+1,timenow->tm_year+1900);
    #endif

  fclose(fp);
  return 0;
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

char *gerar_nome(){
  char *nomes[5]={"TEMP","PRESS","UMID","VIBR","FLUX"};
  int idx=rand()%5;
  return nomes[idx];
}

double gerar_num(){
  return ((double)rand())/ RAND_MAX*(100-1)+1;
} 

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
  struct tm t;
  time_t timestamp_inicial, timestamp_final;

  t.tm_year = ano - 1900;
  t.tm_mon = mes - 1;
  t.tm_mday = dia;
  t.tm_hour = 0;
  t.tm_min = 0;
  t.tm_sec = 0;
  t.tm_isdst = -1;

  timestamp_inicial = mktime(&t);
  if (timestamp_inicial == -1) {
    printf("Data inválida.\n");
    return -1;
  }

  t.tm_hour = 23;
  t.tm_min = 59;
  t.tm_sec = 59;

  timestamp_final = mktime(&t);
  if (timestamp_final == -1) {
    printf("Data inválida.\n");
    return -1;
  }

  time_t timestamp_aleatorio = timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);

  return timestamp_aleatorio;
}
