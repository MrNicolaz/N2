# Projeto de Organização e Consulta de Leituras de Sensores

Este projeto contém três programas em C voltados para a manipulação, consulta e geração de dados simulados de sensores.

# Estrutura dos Programas

### 1. `organizar` (`prog1.c`)
Organiza os dados dos sensores em arquivos separados por sensor.

# Uso: 
```bash

Compilar

gcc -o prog3.exe prog3.c     // Gera os dados brutos
gcc -o prog1.exe prog1.c     // Organiza os dados por sensor 
gcc -o prog2.exe prog2.c     // Consulta os dados por timestamp

Rodar

Exemplo:
.\prog3.exe 01:06:2025 16:06:2025 temp CONJ_Z pressao CONJ_Q status TEXTO ativo BINARIO 

.\prog1.exe teste_leituras.txt

.\prog2.exe temp 15:06:2024:14:30:00