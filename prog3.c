#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long cria_ts(int d, int m, int a, int h, int min, int s) {
    struct tm t = {0};
    t.tm_year = a - 1900; t.tm_mon = m - 1; t.tm_mday = d;
    t.tm_hour = h; t.tm_min = min; t.tm_sec = s;
    return mktime(&t);
}

long sorteia_ts(long ini, long fim) {
    if (fim <= ini) return ini;
    return ini + rand() % (fim - ini + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso correto: %s DIA0:MES0:ANO0 DIA1:MES1:ANO1 TIPO1:SENSOR1 [TIPO2:SENSOR2 ...]\n", argv[0]);
        return 1;
    }

    int d0, m0, a0, d1, m1, a1;
    if (sscanf(argv[1], "%d:%d:%d", &d0, &m0, &a0) != 3 ||
        sscanf(argv[2], "%d:%d:%d", &d1, &m1, &a1) != 3) {
        printf("Formato invalido de data. Use DIA:MES:ANO\n");
        return 2;
    }

    long inicio = cria_ts(d0, m0, a0, 0, 0, 0);
    long fim = cria_ts(d1, m1, a1, 23, 59, 59);

    if (inicio > fim) {
        printf("Intervalo inválido.\n");
        return 3;
    }

    srand((unsigned int)time(NULL));

    FILE *saida = fopen("dados_teste.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        return 4;
    }

    for (int i = 3; i < argc; i++) {
        char *arg = argv[i];
        char *tipo = strtok(arg, ":");
        char *sensor = strtok(NULL, ":");

        if (!tipo || !sensor) {
            printf("Argumento invalido: '%s'. Use formato TIPO:SENSOR\n", argv[i]);
            fclose(saida);
            return 5;
        }

        for (int k = 0; k < 2000; k++) {
            long ts = sorteia_ts(inicio, fim);
            fprintf(saida, "%ld %s ", ts, sensor);

            if (strcmp(tipo, "CONJ_Z") == 0) {
                int val = rand() % 2001 - 1000;
                fprintf(saida, "%d\n", val);
            } else if (strcmp(tipo, "CONJ_Q") == 0) {
                float val = (rand() % 500000) / 100.0f;
                fprintf(saida, "%.2f\n", val);
            } else if (strcmp(tipo, "TEXTO") == 0) {
                int len = 3 + rand() % 10;
                for (int j = 0; j < len; j++)
                    fprintf(saida, "%c", 'A' + rand() % 26);
                fputc('\n', saida);
            } else if (strcmp(tipo, "BINARIO") == 0) {
                    fprintf(saida, (rand() % 2) ? "true\n" : "false\n");
            } else {
                fprintf(saida, "TIPO_INVALIDO\n");
            }
        }
    }

    fclose(saida);
    puts("Arquivo de teste criado.");
    return 0;
}
