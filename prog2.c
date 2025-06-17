#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long ts;
    char valor[128];
} Leitura;

time_t construir_timestamp(const char *data_hora) {
    int d, m, a, hh, mm, ss;
    if (sscanf(data_hora, "%d:%d:%d:%d:%d:%d", &d, &m, &a, &hh, &mm, &ss) != 6) {return -1; 
    }

    struct tm t = {0};
    t.tm_year = a - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;
    t.tm_isdst = -1;

    return mktime(&t);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
    printf("Uso correto: %s SENSOR_ID DD:MM:AAAA:HH:MM:SS\n", argv[0]);return 1;
    }

    char *sensor = argv[1];
    time_t alvo_ts = construir_timestamp(argv[2]);
    if (alvo_ts == -1) {
    fprintf(stderr, "Formato de data inválido. Use: DD:MM:AAAA:HH:MM:SS\n");return 2;
    }

    char arquivo[128];
    snprintf(arquivo, sizeof(arquivo), "sensores/%s.txt", sensor);

    FILE *f = fopen(arquivo, "r");
    if (!f) {
    fprintf(stderr, "Sensor '%s' não encontrado em 'sensores/'\n", sensor);return 3;
    }

    Leitura *dados = malloc(sizeof(Leitura) * 100);
    int cap = 100, total = 0;

    while (fscanf(f, "%ld %*s %127s", &dados[total].ts, dados[total].valor) == 2) {
    total++;
    if (total >= cap) {
    cap *= 2;
    dados = realloc(dados, sizeof(Leitura) * cap);
    }
}
    fclose(f);

    if (total == 0) {
    puts("Nenhum dado disponível para esse sensor.");
    free(dados);return 4;
    }

    int lo = 0, hi = total - 1;
    while (lo + 1 < hi) {
    int mid = (lo + hi) / 2;
    if (dados[mid].ts < alvo_ts)
        lo = mid;
    else
        hi = mid;
    }

    int maisProximo = (abs(dados[hi].ts - alvo_ts) < abs(dados[lo].ts - alvo_ts)) ? hi : lo;

    printf("Consulta ao sensor '%s' para %s:\n", sensor, argv[2]);
    printf("Resultado mais próximo: %ld %s %s\n", dados[maisProximo].ts, sensor, dados[maisProximo].valor);

    free(dados);return 0;
}
