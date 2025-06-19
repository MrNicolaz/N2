#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef _WIN32
#include <direct.h>
#define CRIAR_DIRETORIO(dir) _mkdir(dir)
#else
#include <unistd.h>
#define CRIAR_DIRETORIO(dir) mkdir(dir, 0755)
#endif
#define MAX_SENS 100
#define MAX_ID 20
#define MAX_LINHA 128
#define INICIAL_CAP 10

typedef struct {
    long timestamp;char id[MAX_ID];char valor[MAX_LINHA]; 
} Registro;

typedef struct {
    char id[MAX_ID];
    Registro *dados;
    int qtd;
    int capacidade; 
} SensorInfo;

void inserir_registro(SensorInfo *sensor, Registro reg) {
    if (sensor->qtd >= sensor->capacidade) {
        sensor->capacidade *= 2;
        sensor->dados = realloc(sensor->dados, sizeof(Registro) * sensor->capacidade);
        if (!sensor->dados) {
        fprintf(stderr, "Erro de memoria para sensor %s\n", sensor->id);
        exit(1);
        }
    }sensor->dados[sensor->qtd++] = reg;
}

void ordenar_insertion(Registro *v, int n) {
    for (int i = 1; i < n; i++) {
        Registro chave = v[i];
        int j = i - 1;
        while (j >= 0 && v[j].timestamp > chave.timestamp) {
        v[j + 1] = v[j];
        j--;
        }
        v[j + 1] = chave;
    }
}

int acha_sensor(SensorInfo sensores[], int total, const char *id) {
    for (int i = 0; i < total; i++) {
        if (strcmp(sensores[i].id, id) == 0)
        return i;
    }return -1;
}

void salvar_arquivo(SensorInfo *sensor) {
    char caminho[128];
    snprintf(caminho, sizeof(caminho), "sensores/%s.txt", sensor->id);
    FILE *arq = fopen(caminho, "w");
    if (!arq) {
        fprintf(stderr, "Erro ao criar arquivo do sensor %s\n", sensor->id);
        return;
    }
    for (int i = 0; i < sensor->qtd; i++) {
        fprintf(arq, "%ld %s %s\n",
            sensor->dados[i].timestamp,
            sensor->dados[i].id,
            sensor->dados[i].valor);
    }
    fclose(arq);
    printf("Dados do sensor '%s' gravados em '%s'\n", sensor->id, caminho);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s arquivo_de_entrada\n", argv[0]);
        return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Falha ao abrir arquivo");
        return 2;
    }

    if (CRIAR_DIRETORIO("sensores") != 0 && errno != EEXIST) {
        perror("Erro ao criar pasta sensores");
        fclose(entrada);
        return 3;
    }

    SensorInfo sensores[MAX_SENS];
    int total_sens = 0;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), entrada)) {
        long ts;
        char id[MAX_ID];
        char val[MAX_LINHA];

        if (sscanf(linha, "%ld %19s %127s", &ts, id, val) != 3) {
        fprintf(stderr, "Linha invalida ignorada: %s", linha);
        continue;
        }

        int pos = acha_sensor(sensores, total_sens, id);
        if (pos == -1) {
            if (total_sens >= MAX_SENS) {
            fprintf(stderr, "Limite de sensores atingido.\n");
            break;
            }
            pos = total_sens++;
            strncpy(sensores[pos].id, id, MAX_ID);
            sensores[pos].dados = malloc(sizeof(Registro) * INICIAL_CAP);
            sensores[pos].qtd = 0;
            sensores[pos].capacidade = INICIAL_CAP;
            if (!sensores[pos].dados) {
            fprintf(stderr, "Falha na alocacao.\n");
            fclose(entrada);
            return 4;
            }
        }

        Registro novo_reg;
        novo_reg.timestamp = ts;
        strncpy(novo_reg.id, id, MAX_ID);
        strncpy(novo_reg.valor, val, MAX_LINHA);

        inserir_registro(&sensores[pos], novo_reg);

        ordenar_insertion(sensores[pos].dados, sensores[pos].qtd);   // Ordenar incremental após inserção!!!
    }

    fclose(entrada);

    for (int i = 0; i < total_sens; i++) {
        salvar_arquivo(&sensores[i]);
        free(sensores[i].dados);
    }

puts("Processo concluido com exito.");
return 0;
}