#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

#define MAX_SENSORS 100
#define MAX_ID_LEN 20
#define MAX_LINE 128

typedef struct {
    long ts;
    char id[MAX_ID_LEN];
    char valor[MAX_LINE];
} Leitura;

typedef struct {
    char id[MAX_ID_LEN];
    Leitura *leituras;
    int total;
    int capacidade;
} SensorDados;

int main(int argc, char *argv[]) {
    if (argc < 2) {
    printf("Uso: %s arquivo_entrada\n", argv[0]);return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
    perror("Erro ao abrir o arquivo");return 2;
    }

    if (MKDIR("sensores") != 0 && errno != EEXIST) {
    perror("Erro ao criar diretório sensores");
    fclose(entrada);return 3;
    }

    SensorDados sensores[MAX_SENSORS];
    int totalSensores = 0;
    char linha[MAX_LINE];

    while (fgets(linha, sizeof(linha), entrada)) {
    long ts;
    char id[MAX_ID_LEN];
    char valor[MAX_LINE];

    if (sscanf(linha, "%ld %19s %127s", &ts, id, valor) != 3) {
        continue; 
        }

        int idxSensor = -1;
        for (int i = 0; i < totalSensores; i++) {
        if (strcmp(sensores[i].id, id) == 0) {
        idxSensor = i;break;
        }
    }

    if (idxSensor == -1) {
        if (totalSensores >= MAX_SENSORS) {
        fprintf(stderr, "Número máximo de sensores excedido.\n");break;
        }
        idxSensor = totalSensores++;
        strncpy(sensores[idxSensor].id, id, MAX_ID_LEN);
        sensores[idxSensor].leituras = malloc(sizeof(Leitura) * 10);
        sensores[idxSensor].total = 0;
        sensores[idxSensor].capacidade = 10;
        }

        if (sensores[idxSensor].total >= sensores[idxSensor].capacidade) {
        sensores[idxSensor].capacidade *= 2;
        sensores[idxSensor].leituras = realloc(sensores[idxSensor].leituras,
        sizeof(Leitura) * sensores[idxSensor].capacidade);
        }

        Leitura *nova = &sensores[idxSensor].leituras[sensores[idxSensor].total++];
        nova->ts = ts;
        strcpy(nova->id, id);
        strcpy(nova->valor, valor);
    }

    fclose(entrada);

    for (int s = 0; s < totalSensores; s++) {
        for (int i = 0; i < sensores[s].total - 1; i++) {
        for (int j = 0; j < sensores[s].total - 1 - i; j++) {
        if (sensores[s].leituras[j].ts > sensores[s].leituras[j + 1].ts) {
        Leitura tmp = sensores[s].leituras[j];
        sensores[s].leituras[j] = sensores[s].leituras[j + 1];
        sensores[s].leituras[j + 1] = tmp;
            }
        }
    }

        char nomeArquivo[128];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "sensores/%s.txt", sensores[s].id);
        FILE *saida = fopen(nomeArquivo, "w");
        if (!saida) {
        fprintf(stderr, "Falha ao salvar o sensor %s\n", sensores[s].id);continue;
        }

        for (int i = 0; i < sensores[s].total; i++) {
        fprintf(saida, "%ld %s %s\n",
        sensores[s].leituras[i].ts,
        sensores[s].leituras[i].id,
        sensores[s].leituras[i].valor);
        }

        fclose(saida);
        printf("Sensor '%s' exportado para '%s'\n", sensores[s].id, nomeArquivo);
    }

    for (int s = 0; s < totalSensores; s++) {
        free(sensores[s].leituras);
    }

    puts("Processamento finalizado com sucesso.");return 0;
}
