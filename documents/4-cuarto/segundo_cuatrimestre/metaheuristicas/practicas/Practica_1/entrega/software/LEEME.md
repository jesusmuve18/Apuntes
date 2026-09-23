# Software de la entrega
Todo el proyecto c++ se ha incluido en la carpeta `code`. Para compilar y ejecutarlo se podrá hacer desde la terminal en el directorio raíz con 
```bash
./softare/compilar_y_ejecutar.sh
```
Si tan solo se quiere compilar para poder ejecutarlo de forma personalizada se deberá hacer:
```bash
cd code
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
Para más información sobre la ejecución se puede consultar la memoria de la práctica.

Además, el directorio incluye 2 scripts de python descritos en la memoria:
- `graphics.py`
- `metrics.py`
