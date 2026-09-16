# Práctica alternativa Metaheurísticas

Para este trabajo se ha elegido la metaheurística _Sine Cosine Algorithm_. Acerca de ella se tendrán que hacer los siguientes ejercicios:

1. Descripción:
   -  Realizar un resumen de hasta 3 páginas acerca de esta metaheurística. Debe ser completo, explicar en qué se basa, en qué consiste y por qué funciona.
   -   Realizar una presentación de transparencias. Entre otras cosas se deberán analizar componentes que provocan exploración vs componentes que provocan explotación. Analizar este equilibrio.

2. Implementación:
   - Usaremos benchmark "CEC 2027 Special Session and Competition on Single Objective Bound Constrained Real-Parameter Numerical Optimization"
   - 30 funciones, distinto nivel de complejidad
   - Distintos valores de dimensión: 10, 30, 50 (no 100)
   - Criterio de parada: 10000*dimension.
   - Rango de todas las funciones: [-100, 100]
   - Se ejecuta cada una X veces para calcular la media (usamos T=10, por reducir tiempos, el estándar son 50).
   - Se mide el error con respecto al óptimo (fun*100): 100 para f1, 200 f2,...
   - Se calcula el error cada cierto % de evaluaciones: 1%, 2%, ..., 10%, 20%, ...
   - Código actualizado en https://github.com/dmolina/cec2017real/
   - Disponible en c++ con wrapper para Python
   - Se puede compilar como librería o dentro del proyecto
   - Ficheros:
     - cec17_test_func.c: fichero original del benchmark.
     - cec17.c: con las funciones del API simplificado
     - input_data: debe estar en el directorio donde se ejecute el bi8nario
     - extract.py: script en Python para agrupar los resultados
  
   Las tareas a realizar en esta parte son:
   2. Trabajar con la competición CEC2017 y utilizar el software presentado por Daniel Molina para el análisis de resultados https://tacolab.org/
   3. Diseñar una propuesta de mejora con una hibridación Ej. Algoritmo híbrido memético (MHe + Local Search, u otra hibridación) (Hasta 1 pto.), y hacer el estudio experimental comparándolo con el sofware https://tacolab.org/.
   4. Plantear mejoras sobre el comportamiento/diseño de la MHe (Hasta 1.5 ptos. con estudio experimental)


Cuestiones
- ¿Cuáles son los criterios que se van a utilizar a la hora de calificar la práctica alternativa?
   Se tienen 4 bloques y en cada uno se evaluará el trabajo realizado, que se ajuste a lo solicitado y la calidad del estudio experimental y su análisis. En el bloque 1 se analiza la calidad de la descripción, en los bloques 2 y 3 el estudio experimental y análisis, y en el bloque 4 se tienen en cuenta originalidad y la calidad de la propuesta. Si la MHe es original, se tendrá en cuenta la creatividad asociada.
- ¿Hay que implementar nuestra metaheurística inventada, o es suficiente con una explicación mediante pseudocódigo?
   Hay que implementarla y analizarla experimentalmente con la
   competición CEC2017, así como el modelo memético (punto 3) y las
   extensiones (punto 4) de la propuesta. 
- ¿Hay que superar alguna metaheurística tomada como referencia?
   No necesariamente, la naturaleza de la MHe condicionará la calidad del resultado.
- ¿Podrían subir algún guión especificando qué hay que entregar exactamente?
   Este documento sirve de guión, junto en el fichero PDF de Daniel Molina que explica cómo hacer la experimentación. Es necesario entregar un documento que describa con detalle el estudio realizado y entregar el software asociado.