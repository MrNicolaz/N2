#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

time_t montar_ts(int d, int m, int a, int hh, int mm, int ss) {
    struct tm t = {0};
    t.tm_year = a - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;return mktime(&t);
}

time_t ts_aleatorio(time_t ini, time_t fim) {
    if (fim <= ini) return ini;return ini + rand() % (fim - ini + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 5 || ((argc - 3) % 2 != 0)) {
    printf("Uso: %s dia0:mes0:ano0 dia1:mes1:ano1 [SENSOR TIPO]...\n", argv[0]);return 1;
    }

    int d0, m0, a0, d1, m1, a1;
    sscanf(argv[1], "%d:%d:%d", &d0, &m0, &a0);
    sscanf(argv[2], "%d:%d:%d", &d1, &m1, &a1);
    time_t inicio = montar_ts(d0, m0, a0, 0, 0, 0);
    time_t fim = montar_ts(d1, m1, a1, 23, 59, 59);

    if (inicio > fim) {
    printf("Intervalo de datas inválido.\n");return 2;
    }

    srand(time(NULL));

    FILE *f = fopen("teste_leituras.txt", "w");
    if (!f) {
    perror("Erro ao criar arquivo de teste");return 3;
    }

    for (int i = 3; i < argc; i += 2) {
    char *sensor = argv[i];
    char *tipo = argv[i + 1];

        for (int k = 0; k < 2000; k++) {
        time_t ts = ts_aleatorio(inicio, fim);
        fprintf(f, "%ld %s ", ts, sensor);

        if (strcmp(tipo, "CONJ_Z") == 0) {
            int v = rand() % 1000 - 500;
            fprintf(f, "%d\n", v);
            } else if (strcmp(tipo, "CONJ_Q") == 0) {
            double v = (rand() % 100000) / 100.0;
            fprintf(f, "%.2f\n", v);
            } else if (strcmp(tipo, "TEXTO") == 0) {
            int len = 1 + rand() % 10;
            for (int j = 0; j < len; j++)
            fputc('A' + rand() % 26, f);
            fputc('\n', f);
            } else if (strcmp(tipo, "BINARIO") == 0) {
            fprintf(f, rand() % 2 ? "true\n" : "false\n");
            } else {
            fprintf(f, "TIPO_DESCONHECIDO\n");
            }
        }
    }

    fclose(f);
    puts("Arquivo de teste gerado com sucesso!");
    return 0;
}

// teste
    