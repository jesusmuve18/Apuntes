include "globals.mzn";

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Parámetro de entrada
int: N = 3;
% int: N = 4;
% int: N = 5;
% int: N = 6;
% int: N = 7;

% Tamaño
set of int: S = 1..N;
int: paridad_N = N mod 2;

% Cuadrado Mágico
int: MAXV = 100;
int: paridad_MAXV = MAXV mod 2;

% Acotaciones del dominio (para mayor eficiencia)
int: min_dominio = 1;
int: max_dominio = ((MAXV + (1-2*paridad_N)*paridad_MAXV) div 2);

% Para entender esta acotación consideramos la biyección estrictamente creciente dada por 
%   f:  (a,b) -> (a', b')
%         x  |-> 2*x      si x es par
%            |-> 2*x - 1  si x es impar
% 
% donde a,b,a',b'\in R. Esta será la trasformación que aplicaremos para los valores del cuadrado
% con el objetivo de tener la mitad de valores (reducimos el dominio a la mitad). 
% En este caso tenemos a=min_dominio y b=max_dominio. Queremos que al aplicarle la transformación
% mantengamos el mismo número de valores (es una biyección) y que quede el rango más pequeño posible
% a considerar. Para entender cómo se han fijado estos límites veámoslo con la siguiente distinción
% de casis:
%
% MAXV par => paridad_MAXV = 0
%     - N par => paridad_N = 0
%         dominio=(1, (MAXV - paridad_MAXV)/2) = (1, (MAXV/2)) --f--> (1*2, (MAXV/2)*2) = (2, MAXV)
%     - N impar => paridad_N = 1
%         dominio=(1, (MAXV + paridad_MAXV)/2) = (1, (MAXV/2)) --f--> (1*2 - 1, (MAXV/2)*2 - 1) = (2-1, MAXV-1) = (1, MAXV-1)
%
% MAXV impar => paridad_MAXV = 1
%     - N par => paridad_N = 0
%         dominio=(1, (MAXV - paridad_MAXV)/2) = (1, ((MAXV-1)/2)) --f--> (1*2, ((MAXV-1)/2)*2) = (2, MAXV-1)
%     - N impar => paridad_N = 1
%         dominio=(1, (MAXV + paridad_MAXV)/2) = (1, ((MAXV+1)/2)) --f--> (1*2 - 1, ((MAXV+1)/2)*2 - 1) = (2-1, MAXV+1-1) = (1, MAXV)
%
% Por tanto lo único que necesitamos ahora es que se sume la paridad_MAXV cuando paridad_N == 0 y que se reste en caso contrario
%
% Para ello consideramos la siguiente gráfica:
%
%      |                     Observamos que es la gráfica de la función f(x) = -2x + 1
%    1 +                     Además, se verifica
%      |\                      f(0) = 1
%      | \                     f(1) = -1
%      |  \                  Por lo que concluímos la fórmula para la cota superior multiplicando paridad_MAXV por este valor
%    0 +---\----|----------
%      |    \   1
%      |     \
%      |      \
%   -1 +       \
%      |        \
%

% Cuadrado mágico (solución)
array[S, S] of var min_dominio..max_dominio: cuadrado_magico;

% Suma mágica
var min_dominio*N..max_dominio*N: suma;

% Suma de todos los elementos
var min_dominio*N*N..max_dominio*N*N: suma_total = suma*N;

% Número de elementos distintos
var 0..N*N: distintos;
int: min_valores_distintos = (N * N) div 2 + paridad_N;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Al menos la mitad de todos los valores de la matriz sean distintos (por ejemplo, si la matriz es 3x3 debe contener al menos 5 números diferentes)
% Función nvalue: Cuenta el número de valores distintos (únicos) presentes en un array.
%    En este caso impone: distintos = número de elementos distintos en el cuadrado mágico
constraint nvalue(distintos, [cuadrado_magico[fila, columna] | fila, columna in S]);
constraint distintos >= min_valores_distintos;

% Si N es par, todos sus valores deben ser pares
% Si N es impar, todos los valores deben ser impares. Ya implementado
% constraint forall(fila, columna in S)((cuadrado_magico[fila, columna] mod 2) == (N mod 2));

% La suma de los elementos de cada fila y de cada columna, y la suma de los elementos de la diagonal principal 
% y de la diagonal secundaria, proporcionen el mismo valor.
constraint forall(fila in S)(sum(col in S) (cuadrado_magico[fila,col]) == suma);
constraint forall(col in S) (sum(fila in S) (cuadrado_magico[fila,col]) == suma);
constraint sum(fila in S)(cuadrado_magico[fila,fila]) == suma;
constraint sum(fila in S) (cuadrado_magico[fila, N-fila+1]) == suma;

% Ruptura de simetrías
% Forzamos que la esquina superior izquierda [1,1] sea menor o igual que las otras tres esquinas (evita rotaciones y reflexiones diagonales).
constraint cuadrado_magico[1,1] <= cuadrado_magico[1,N];
constraint cuadrado_magico[1,1] <= cuadrado_magico[N,1];
constraint cuadrado_magico[1,1] <= cuadrado_magico[N,N];

% Para romper la simetría de reflexión respecto a la diagonal principal forzamos un orden entre dos elementos simétricos.
constraint cuadrado_magico[1,2] <= cuadrado_magico[2,1];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Se quiere minimizar la suma total de los elementos de la matriz (proporcional a suma)
% Se han utilizado parámetros para hacerlo un poco más eficiente (de la documentación oficial de MiniZinc)
solve :: int_search(array1d(cuadrado_magico), dom_w_deg, indomain_min) 
      minimize suma;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

output [
  "N = \(N)\n",
  "MAXV = \(MAXV)\n",
  "Minimo de valores distintos exigido = \(min_valores_distintos)\n", 
  "Número de valores distintos = \(distintos)\n",
  "Suma comun de filas/columnas/diagonales = \(suma*2 - paridad_N*N)\n",
  "Suma total minimizada = \(suma_total*2 - paridad_N*N*N)\n\n",
  "Matriz:\n"
] ++ [show_int(3, cuadrado_magico[r, c]*2 - paridad_N) ++ if c == N then "\n" endif | r, c in S ];
