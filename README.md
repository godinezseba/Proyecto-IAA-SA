# Proyecto-IAA-TS
Tabu Search para Traveling Tournament Problem

## Ejecucion
make TYPE=NL N=4 SEED=0 DEBUG=0 ITERATIONS=10 LENLIST=N/2 WEIGHT=5000

## Flags
- TYPE: define que serie de archivos usar, opciones: NL (default), galaxy, Super.
- N: define que archivo usar para la serie, default 4.
- SEED: default es 0.
- DEBUG: opción para printear los movimientos seleccionados por cada iteración, default es 0.
- ITERATIONS: numero de iteraciones que realiza TabuSearch, default 10.
- LENLIST: largo de la lista tabu, default N/2 (mitad de los elementos)
- WEIGHT: Peso, valor utilizado por la función objetivo, default 5000