#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long timestamp;
    char valor[128];} Entrada;

long converter_data_para_ts(const char *dataHora) {
    struct tm tm_info = {0};
    if (sscanf(dataHora, "%4d-%2d-%2d %2d:%2d:%2d",
            &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday,
            &tm_info.tm_hour, &tm_info.tm_min, &tm_info.tm_sec) != 6) {
        return -1;
    }
    tm_info.tm_year -= 1900;
    tm_info.tm_mon -= 1;
    tm_info.tm_isdst = -1;
    return mktime(&tm_info);
}

Entrada* ler_arquivo(const char* nomeArquivo, int *quantidade) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) return NULL;

    int capacidade = 100;
    Entrada *dados = malloc(sizeof(Entrada) * capacidade);
    int total = 0;

    while (fscanf(f, "%ld %*s %127[^\n]", &dados[total].timestamp, dados[total].valor) == 2) {
        total++;
        if (total >= capacidade) {
            capacidade *= 2;
            Entrada *temp = realloc(dados, sizeof(Entrada) * capacidade);
            if (!temp) {
            free(dados);
            fclose(f);
            return NULL;
            }
            dados = temp;
        }
    }
    fclose(f);
    *quantidade = total;
    return dados;
}

int busca_binaria_proxima(Entrada *dados, int n, long alvo) {
    int esquerda = 0, direita = n - 1;
    if (n == 0) return -1;

    while (esquerda + 1 < direita) {
        int meio = (esquerda + direita) / 2;
        if (dados[meio].timestamp < alvo) {
            esquerda = meio;
        } else {
            direita = meio;
        }
    }

    if (labs(dados[direita].timestamp - alvo) < labs(dados[esquerda].timestamp - alvo)) {
        return direita;
    } else {
        return esquerda;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s SENSOR_ID 'AAAA-MM-DD HH:MM:SS'\n", argv[0]);
        return 1;
    }

    char arquivo[128];
    snprintf(arquivo, sizeof(arquivo), "sensores/%s.txt", argv[1]);

    int total_dados = 0;
    Entrada *dados = ler_arquivo(arquivo, &total_dados);
    if (!dados) {
        fprintf(stderr, "Não foi possível abrir o arquivo do sensor '%s'\n", argv[1]);
        return 2;
    }

    if (total_dados == 0) {
        printf("Nenhum dado disponivel para o sensor '%s'\n", argv[1]);
        free(dados);
        return 3;
    }

    long ts_alvo = converter_data_para_ts(argv[2]);
    if (ts_alvo == -1) {
        fprintf(stderr, "Formato de data invalido. Use 'AAAA-MM-DD HH:MM:SS'\n");
        free(dados);
        return 4;
    }

    int indice = busca_binaria_proxima(dados, total_dados, ts_alvo);
    if (indice == -1) {
        printf("Dados insuficientes para a consulta.\n");
        free(dados);
        return 5;
    }

    printf("Consulta para sensor '%s' na data %s:\n", argv[1], argv[2]);
    printf("Leitura mais proxima: %ld %s %s\n", dados[indice].timestamp, argv[1], dados[indice].valor);

    free(dados);
    return 0;
}