include "globals.mzn";

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Conjunto de Personas
enum PERSONA = {Marty, Doc, Biff, George, Nadie};
array [PERSONA] of string: NOMBRE_PERSONA =["Marty McFly", "Doc Brown", "Biff Tannen", "George McFly", "Nadie"];

% Conjunto de Tareas
enum TAREA = { A, B, C, D, E, F, G, H, I, J, K };
array [TAREA] of string: NOMBRE_TAREA = [
  "Montar chasis",
  "Instalar ruedas",
  "Cableado eléctrico",
  "Motor de fusión nuclear",
  "Tablero de control",
  "Instalación y configuración del condensador de fluzo (flux capacitor)",
  "Ajuste aerodinámico",
  "Instalación de puertas de ala de gaviota",
  "Panel de tiempo",
  "Implementación y validación de algoritmos de búsqueda heurística para optimización de caminos",
  "Prueba de plutonio"
];

% Datos de la tabla
%                                       A   B   C   D   E   F   G   H   I   J   K
array[TAREA] of int: duracion_Marty = [10,  1,  5, 15, 12, 13,  7,  4,  2,  4,  4];
array[TAREA] of int: duracion_Doc   = [16,  2,  4,  5,  4,  5,  6,  3,  2,  2,  3];
array[TAREA] of int: ayuda_Biff     = [ 3,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1];
array[TAREA] of int: ayuda_George   = [ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1];

array[TAREA] of set of TAREA: predecesoras = [
    {},          % A
    {A},         % B
    {B},         % C
    {A, C},      % D
    {C},         % E
    {C, D, E},   % F
    {C, D},      % G
    {E, G},      % H
    {F, H},      % I
    {G, I},      % J
    {H, I}       % K
];

% Soluciones
array[TAREA] of var PERSONA: persona_asignada_a;
array[TAREA] of var PERSONA: persona_que_apoya_en;
array[TAREA] of var 1..84: dia_inicio; % La duración máxima total (secuencial y sin apoyo) es: 16+2+5+15+12+13+7+4+2+4+4 = 84
array[TAREA] of var 1..16: duracion_original;
array[TAREA] of var 0..3: descuento_ayuda;
array[TAREA] of var 1..16: duracion_final;

% Métricas
var int: duracion_total = max(t in TAREA)(dia_inicio[t] + duracion_final[t] -1);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Definición de duración de una tarea
constraint forall(t in TAREA)(
    if persona_asignada_a[t] == Marty then duracion_original[t] = duracion_Marty[t] 
    else duracion_original[t] = duracion_Doc[t] endif
);
constraint forall(t in TAREA)(
    if persona_que_apoya_en[t] == Biff then descuento_ayuda[t] = ayuda_Biff[t] 
    elseif persona_que_apoya_en[t] == George then descuento_ayuda[t] = ayuda_George[t]
    else descuento_ayuda[t] = 0 endif
);
constraint forall(t in TAREA)(duracion_final[t] = duracion_original[t] - descuento_ayuda[t]);

% Solo pueden asignarse tareas a Marty y a Doc
constraint forall(t in TAREA)(persona_asignada_a[t] in {Marty, Doc});

% Solo pueden apoyar en las tareas Biff y George (o nadie)
constraint forall(t in TAREA)(persona_que_apoya_en[t] in {Biff, George, Nadie});

% Ninguno de los dos (Marty y Doc) puede realizar dos tareas simultáneamente, aunque sí pueden trabajar en paralelo en tareas distintas
constraint forall(t1, t2 in TAREA where t1 < t2)(
    (persona_asignada_a[t1] == persona_asignada_a[t2]) -> (
        dia_inicio[t1] + duracion_final[t1] <= dia_inicio[t2] \/ 
        dia_inicio[t2] + duracion_final[t2] <= dia_inicio[t1]
    )
);
% Las tareas se tienen que hacer en orden
constraint forall(t in TAREA, p in predecesoras[t])(dia_inicio[p] + duracion_final[p] <= dia_inicio[t]);

% CONDICIONES DE BIFF

% 1. Biff solo puede apoyar en tareas cuya duración, para el trabajador asignado, sea al menos de 3 días.
constraint forall(t in TAREA)((persona_que_apoya_en[t] == Biff) -> duracion_original[t] >= 3);

% 2. Biff puede apoyar como máximo en 4 tareas en total.
constraint count(persona_que_apoya_en, Biff) <= 4;

% 3. Biff no puede apoyar en tareas que se ejecuten simultáneamente (completado con la de George).
constraint forall(t1, t2 in TAREA where t1<t2)(
    (persona_que_apoya_en[t1] ==  persona_que_apoya_en[t2] /\ persona_que_apoya_en[t1] != Nadie) -> (
        dia_inicio[t1] + duracion_final[t1] <= dia_inicio[t2] \/ 
        dia_inicio[t2] + duracion_final[t2] <= dia_inicio[t1]
    )
);

% 4. Biff no puede ayudar en las tareas D ni F, por tratarse de tareas demasiado delicadas.
constraint persona_que_apoya_en[D] != Biff /\ persona_que_apoya_en[F] != Biff;

% 5. Biff no puede apoyar en más de 2 tareas asignadas a Marty, ni en más de 2 tareas asignadas a Doc.
constraint sum(t in TAREA)(persona_que_apoya_en[t] == Biff /\ persona_asignada_a[t] == Marty) <= 2;
constraint sum(t in TAREA)(persona_que_apoya_en[t] == Biff /\ persona_asignada_a[t] == Doc) <= 2;

% 6. Gracias al apoyo de Biff, la duración de la tarea se reduce: en 3 días si la tarea es A; en 2 días si la tarea es H; en 1 día para cualquier otra tarea elegible. Implementado ya.

% CONDICIONES DE GEORGE
% 1. George solo puede ayudar a Marty, nunca a Doc.
constraint forall(t in TAREA)((persona_asignada_a[t] == Doc) -> persona_que_apoya_en[t] != George);

% 2. Si George ayuda en una tarea, la duración de dicha tarea se reduce en 1 día. Implementado ya.

% 3. George solo puede ayudar en tareas asignadas a Marty cuya duración original para Marty sea de al menos 2 días.
constraint forall(t in TAREA)((duracion_original[t] < 2) -> persona_que_apoya_en[t] != George);

% 4. George no puede realizar dos apoyos simultáneamente. Implementado en condición 3 de Biff.

% 5. George no puede coincidir con Biff en una misma tarea. Implementado ya.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve minimize duracion_total;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% "Letra - DescripciónTarea" inicio "NombreTrabajador" con apoyo de Biff
output [
  " \(t) - \(NOMBRE_TAREA[t]) inicio dia \(dia_inicio[t]) fin dia \(dia_inicio[t] + duracion_final[t] - 1). Asignada a \(NOMBRE_PERSONA[fix(persona_asignada_a[t])]) con apoyo de \(NOMBRE_PERSONA[fix(persona_que_apoya_en[t])])\n"  | t in TAREA
] ++ [
  "\nDuración total = \(duracion_total) días"
];