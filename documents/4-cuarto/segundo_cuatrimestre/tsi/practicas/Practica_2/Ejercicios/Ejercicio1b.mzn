%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Conjunto de ingredientes
enum INGREDIENTE = { Agua_base, Jarabe_A, Jarabe_B, Concentrado_salino, Extracto_energetico, Aroma_citrico };

% Datos de la tabla (por cada 100ml)
array[INGREDIENTE] of int: coste   = [2, 12, 10, 8, 20, 6]; % Céntimos
array[INGREDIENTE] of int: azucar  = [0, 18, 12, 0, 4, 2];  % Gramos
array[INGREDIENTE] of int: sodio   = [0, 40, 20, 180, 10, 5]; % mg
array[INGREDIENTE] of int: cafeina = [0, 0, 0, 0, 30, 0];   % mg

% Parámetro de entrada
int: volumen_total_buscado_ml = 200;
% int: volumen_total_buscado_ml = 1000; 
% int: volumen_total_buscado_ml = 5000; 
% int: volumen_total_buscado_ml = 10000; 

% Cantidad de cada ingrediente
array[INGREDIENTE] of var 0..volumen_total_buscado_ml: cantidad_ml;

% (en lugar de dividir entre 100 divido entre 10 y me queda x10)
var int: azucar_total_x10 = sum(i in INGREDIENTE)((cantidad_ml[i] div 10) * azucar[i]);
var int: sodio_total_x10 = sum(i in INGREDIENTE)( (cantidad_ml[i] div 10) * sodio[i] );
var int: cafeina_total_x10 = sum(i in INGREDIENTE)( (cantidad_ml[i] div 10) * cafeina[i] );
var int: coste_total_x10 = sum(i in INGREDIENTE)( (cantidad_ml[i] div 10) * coste[i] );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Restricción de volumen total
constraint sum(cantidad_ml) == volumen_total_buscado_ml;

% Cada cantidad de ingrediente debe expresarse en múltiplos de 10 ml
constraint forall(i in INGREDIENTE)(cantidad_ml[i] mod 10 == 0);

% Entre 70 y 85 g de azúcar por cada 1000ml
constraint azucar_total_x10 * 100 >= 70 * volumen_total_buscado_ml /\ azucar_total_x10 * 100 <= 85 * volumen_total_buscado_ml;

% Entre 400 y 475 mg de sodio por cada 1000ml
constraint sodio_total_x10 * 100 >= 400 * volumen_total_buscado_ml /\ sodio_total_x10 * 100 <= 475 * volumen_total_buscado_ml;

% Entre 60 y 90 mg de cafeína por cada 1000ml
constraint cafeina_total_x10 * 100 >= 60 * volumen_total_buscado_ml /\ cafeina_total_x10 * 100 <= 90 * volumen_total_buscado_ml;

% El aroma cítrico debe usarse obligatoriamente en entre el 3% y el 20% del volumen total
constraint cantidad_ml[Aroma_citrico]*100 >= 3 * volumen_total_buscado_ml /\ cantidad_ml[Aroma_citrico]*100 <= 20 * volumen_total_buscado_ml;

% El extracto energético no puede superar el 40% del volumen total.
constraint cantidad_ml[Extracto_energetico]*100 <= 40 * volumen_total_buscado_ml;

% Debe utilizarse exactamente uno de los dos jarabes (Jarabe A o Jarabe B)
constraint (cantidad_ml[Jarabe_A] > 0) xor (cantidad_ml[Jarabe_B] > 0);

% Si se usa Jarabe A, debe emplearse al menos en el 5% del volumen total
constraint (cantidad_ml[Jarabe_A] > 0) -> cantidad_ml[Jarabe_A]*100 >= 5 * volumen_total_buscado_ml;

% Si se usa concentrado salino, debe emplearse al menos en el 3% del volumen total
constraint (cantidad_ml[Concentrado_salino] > 0) -> cantidad_ml[Concentrado_salino]*100 >= 3 * volumen_total_buscado_ml;

% Como mucho pueden usarse 4 ingredientes distintos
constraint sum(i in INGREDIENTE)(cantidad_ml[i] > 0) <= 4;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve minimize coste_total_x10;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Dividimos entre 10 en los x10
output [
    "Cantidades usadas:\n------------------\n",
    "Agua base: \(cantidad_ml[Agua_base]) ml\n",
    "Jarabe A: \(cantidad_ml[Jarabe_A]) ml\n",
    "Jarabe B: \(cantidad_ml[Jarabe_B]) ml\n",
    "Concentrado salino: \(cantidad_ml[Concentrado_salino]) ml\n",
    "Extracto energetico: \(cantidad_ml[Extracto_energetico]) ml\n",
    "Aroma citrico: \(cantidad_ml[Aroma_citrico]) ml\n\n",
    "Coste total = \(coste_total_x10 / 10) centimos\n",
    "Azucar total = \(azucar_total_x10 / 10) g\n",
    "Sodio total = \(sodio_total_x10 / 10) mg\n",
    "Cafeina total = \(cafeina_total_x10 / 10) mg\n"
];