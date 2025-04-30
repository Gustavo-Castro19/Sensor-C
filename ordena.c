#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LINE 10000


typedef struct{
  time_t timestamps;
  char* id_sensor;
  double value;
}Sensor;

void ordena_timestamp(Sensor *any,int size);

void escreve_arquivo(Sensor *sensores,int size,char* sens_type[],int sens_size);


int main(void){
  Sensor Lista_Sensores[1000];
  char* sensor_name[5]={"UMID","FLUX","PRESS","TEMP","VIBR"};
  int i=0;
  FILE *fp;
  char text[MAX_LINE];
  fp=fopen("test.txt","r");
  char *token; 
  while(fgets(text,MAX_LINE,fp)!=NULL){
    token=strtok(text," ");
    Lista_Sensores[i].timestamps=(time_t)atol(token);

    token=strtok(NULL," ");
    Lista_Sensores[i].id_sensor=strdup(token);
    token=strtok(NULL,"\n");
    Lista_Sensores[i].value=atof(token);
    i++;
  }
  fclose(fp);
  ordena_timestamp(Lista_Sensores,sizeof(Lista_Sensores)/sizeof(Lista_Sensores[0]));
  escreve_arquivo(Lista_Sensores,i,sensor_name,0);

  for(int j=0;j<i;j++){
    free(Lista_Sensores[j].id_sensor);
  }

  return 0;
}
void ordena_timestamp(Sensor *any,int size){
  Sensor temp;
  for(int i=0;i<size-1;i++){
    for(int j=0;j<size-i-1;j++){
      if(any[j].timestamps>any[j+1].timestamps){
        temp=any[j];
        any[j]=any[j+1];
        any[j+1]=temp;
      }
    }
  }
}
void escreve_arquivo(Sensor *sensores,int size,char* sens_type[],int sens_size){
  if(sens_size==5){
    return;
  }
  FILE *fp;
  char filename[35];
  snprintf(filename,sizeof(filename),"%s.txt",sens_type[sens_size]);
  fp=fopen(filename,"a");
  if(fp==NULL){
    perror("error ao abrir o arquivo");
    return;
  }
  for(int cont=0;cont<size;cont++){
    if(strcmp(sensores[cont].id_sensor,sens_type[sens_size])==0){
      fprintf(fp,"%ld %s %.2lf\n",sensores[cont].timestamps,sensores[cont].id_sensor,sensores[cont].value);
    }
  }
  fclose(fp);
  sens_size++;
  escreve_arquivo(sensores,size,sens_type,sens_size);
}



