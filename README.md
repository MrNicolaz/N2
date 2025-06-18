# Projeto de Organização e Consulta de Leituras de Sensores:

Este projeto contém três programas escritos em linguagem C, responsáveis por gerar, organizar e consultar leituras de sensores.

# Estrutura dos Programas:

1. Gerador de Dados de Teste (prog3.c)
Gera um arquivo de dados com 2000 leituras aleatórias para cada sensor.

2. Organizador de Dados por Sensor (prog1.c)
Separa os dados por sensor dentro de uma pasta chamada "sensores", e ordena os registros por timestamp de forma crescente.

3. Consulta de Leitura por Timestamp (prog2.c)
Realiza uma busca binária para encontrar a leitura mais próxima de um instante de tempo.

# Autor
Nicolas Mauricio e Silva

# Compilação:
```bash
gcc -o prog3.exe prog3.c     // Gera os dados brutos
gcc -o prog1.exe prog1.c     // Organiza os dados por sensor 
gcc -o prog2.exe prog2.c     // Consulta os dados por timestamp

# Execução sugerida:

.\prog3.exe 01:06:2025 16:06:2025 CONJ_Z:sensorA CONJ_Q:sensorB TEXTO:sensorC BINARIO:sensorD

.\prog1.exe dados_teste.txt

.\prog2.exe sensorA "2025-06-10 15:30:00