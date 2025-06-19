#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

time_t criarTimestamp(int d, int m, int a, int h, int min, int s) {
    struct tm t = {0};
    t.tm_year = a - 1900; t.tm_mon = m - 1; t.tm_mday = d;
    t.tm_hour = h; t.tm_min = min; t.tm_sec = s;
    return mktime(&t);
}

time_t randomTS(time_t ini, time_t fim) {
    if (fim <= ini) return ini;
    return ini + rand() % (fim - ini + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s DIA0:MES0:ANO0 DIA1:MES1:ANO1 TIPO:SENSOR ...\n", argv[0]);
        return 1;
    }

    int d0, m0, a0, d1, m1, a1;
    if (sscanf(argv[1], "%d:%d:%d", &d0, &m0, &a0) != 3 || sscanf(argv[2], "%d:%d:%d", &d1, &m1, &a1) != 3) {
        printf("Formato de data invalido. Use DIA:MES:ANO\n");
        return 2;
    }

    time_t inicio = criarTimestamp(d0, m0, a0, 0, 0, 0);
    time_t fim = criarTimestamp(d1, m1, a1, 23, 59, 59);

    if (inicio > fim) {
        printf("Intervaloinvalido.\n");
        return 3;
    }

    srand(time(NULL));
    FILE *saida = fopen("dados_teste.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saida");
        return 4;
    }

    for (int i = 3; i < argc; i++) {
        char *separador = strchr(argv[i], ':');
        if (!separador) {
        printf("Invalido argumento '%s'. Use formato TIPO:SENSOR\n", argv[i]);
        fclose(saida);
        return 5;
        }

        *separador = '\0';
        char *tipo = argv[i];
        char *sensor = separador + 1;

        for (int k = 0; k < 2000; k++) {
            time_t ts = randomTS(inicio, fim);
            fprintf(saida, "%ld %s ", ts, sensor);

            if (strcmp(tipo, "CONJ_Z") == 0) { fprintf(saida, "%d\n", rand() % 1000);
            } else if (strcmp(tipo, "CONJ_Q") == 0) { fprintf(saida, "%.2f\n", (rand() % 10000) / 100.0);
            } else if (strcmp(tipo, "TEXTO") == 0) {
                int len = 1 + rand() % 10;
                for (int l = 0; l < len; l++) fputc('A' + rand() % 26, saida);
                fputc('\n', saida);
            } else if (strcmp(tipo, "BINARIO") == 0) { fprintf(saida, rand() % 2 ? "true\n" : "false\n");
            } else { fprintf(saida, "TIPO_INVALIDO\n");
            }
        }
    }

    fclose(saida);
    puts("Arquivo de teste criado.");
    return 0;
}
