%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Conjunto de Objetos
enum OBJETO = { GPS, Camara, Ordenador, Bateria_extra, Sensor_termico, Botiquin, Radio, Panel_solar };

% Datos de la tabla
array[OBJETO] of int: peso = [ 3, 2, 4, 2, 5, 3, 4, 6 ];         % Peso maximo = 29
array[OBJETO] of int: coste = [ 8, 6, 10, 4, 12, 5, 7, 9 ];      % Coste maximo = 61
array[OBJETO] of int: utilidad = [ 10, 9, 14, 7, 16, 8, 11, 13]; % Utilidad maxima = 88

% Qué objetos cogeremos
array[OBJETO] of var bool: coger;

% Totales
var int: peso_total = sum(o in OBJETO)(coger[o] * peso[o]);
var int: coste_total = sum(o in OBJETO)(coger[o] * coste[o]);
var int: utilidad_total = sum(o in OBJETO)(coger[o] * utilidad[o]);

% Funcion a minimizar (Utilidad (max) -> Peso (min) -> Coste (min))
% Como los máximos son < 100 => La idea es xxyyzz donde xx: utilidad, yy: 99-peso, zz: 99-coste
var int: metrica = utilidad_total*10000 + (99-peso_total)*100 + (99-coste_total);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Peso máximo: la suma de pesos no puede superar 15
constraint peso_total <= 15;

% Dependencia técnica: si se selecciona el "Ordenador", también debe seleccionarse la "Batería extra"
constraint coger[Ordenador] -> coger[Bateria_extra];

% Compatibilidad energética: si se selecciona el "Sensor térmico", también debe seleccionarse el "Panel solar"
constraint coger[Sensor_termico] -> coger[Panel_solar];

% Seguridad mínima: debe seleccionarse obligatoriamente el "Botiquín"
constraint coger[Botiquin];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve maximize metrica;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

output["Objetos seleccionados:\n"] ++
[
  " - \(o) (peso=\(peso[o]), coste=\(coste[o]), utilidad=\(utilidad[o]))\n"  | o in OBJETO where fix(coger[o])
] ++ [
  "\nUtilidad total = \(utilidad_total)\n",
  "Peso total = \(peso_total)\n",
  "Coste total = \(coste_total)"
];
