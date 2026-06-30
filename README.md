# AED-ProyectoParcial
## Integrantes
- Hector Emilio Huaman Puiquin
- Alessandro Facundo Freed Monzón Gallegos
- Adrian Aaron Urbina Mendoza

## Requisitos previos
- Compilador C++20
- CMake 3.29+
- Tener instalada la librería **SFML**:
```
https://www.sfml-dev.org/download/
```
## Compilación y Ejecución del Proyecto
**Clonar** el repositorio y navegar dentro de él:
```
git clone https://github.com/Alessandromg18/AED-ProyectoFinal.git && cd AED-ProyectoFinal
```
Para **compilar** el proyecto ejecutar lo siguiente:
```
cmake -B build -S . && cmake --build build
```
Para **ejecutar** el proyecto una vez generado el binario:
``` 
./build/Proyecto_Final_AED
```
## Fuente de datos utilizada

Este proyecto utiliza los conjuntos de datos **MovieLens** desarrollados por **GroupLens Research** de la Universidad de Minnesota, con la finalidad de poder a prueba la estructura de datos creada. En particular, se emplearon las siguientes versiones:

- **MovieLens 100K**: contiene aproximadamente **100,000 calificaciones** realizadas por usuarios sobre películas.
- **MovieLens 1M**: contiene aproximadamente **1 millón de calificaciones**, proporcionando un conjunto de datos más amplio para el entrenamiento y la evaluación de los modelos de recomendación.

Los datasets fueron obtenidos desde el sitio oficial de GroupLens Research:

**MovieLens Datasets**  
https://grouplens.org/datasets/movielens/
