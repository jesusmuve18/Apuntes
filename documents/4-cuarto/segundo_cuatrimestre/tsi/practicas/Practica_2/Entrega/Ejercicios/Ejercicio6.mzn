include "globals.mzn";

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Constantes del problema
int: num_tfgs = 12;
int: num_tribunales = 3;
int: num_profesores = 10;

int: min_tfg_tribunal = 4;
int: max_tfg_tribunal = 6;

set of int: TFG = 1..num_tfgs;
set of int: TRIBUNAL = 1..num_tribunales;
set of int: PROFESOR = 1..num_profesores;

% Departamentos
enum DEPT = { DECSAI, LSI, ICAR };

% Disponibilidad
enum DIA = { Lunes, Martes, Miercoles, Jueves, Viernes };
enum TURNO = { M, T }; % Mañana y tarde

% Configuración de Departamentos (quién pertenece a dónde)
array[PROFESOR] of DEPT: dept_profe = [
    DECSAI, DECSAI, DECSAI, DECSAI,   % 1-4
    LSI, LSI, LSI,                    % 5-7
    ICAR, ICAR, ICAR                  % 8-10
];

% Solución
array[TRIBUNAL, DEPT] of var PROFESOR: miembros; % Profesores asignados a cada tribunal
array[TFG] of var TRIBUNAL: tribunal_tfg;        % Tribunal asignado a cada TFG
array[TRIBUNAL] of var DIA: dia_tribunal;        % Día asignado a cada tribunal       
array[TRIBUNAL] of var TURNO: turno_tribunal;    % Turno asignado a cada tribunal

% Disponibilidad de cada profesor (días)                     
array[PROFESOR] of set of DIA: dias_disponibles = [
    { Martes },  % Profesor 1
    { Lunes, Miercoles, Viernes },  % Profesor 2
    { Lunes, Miercoles },  % Profesor 3
    { Miercoles, Jueves },  % Profesor 4
    { Lunes, Jueves },  % Profesor 5
    { Lunes, Martes, Miercoles, Jueves, Viernes },  % Profesor 6
    { Viernes },  % Profesor 7
    { Miercoles, Jueves },  % Profesor 8
    { Jueves },  % Profesor 9
    { Lunes, Martes, Viernes },  % Profesor 10
];

% Disponibilidad de cada profesor (turnos)
array[PROFESOR] of set of TURNO: turnos_disponibles = [
    { M },  % Profesor 1
    { T },  % Profesor 2
    { M, T },  % Profesor 3
    { T },  % Profesor 4
    { M },  % Profesor 5
    { T },  % Profesor 6
    { M, T },  % Profesor 7
    { M },  % Profesor 8
    { M, T },  % Profesor 9
    { M, T },  % Profesor 10
];
                                                                                
% Dirección de TFGS 
array[PROFESOR] of set of TFG: dirige_tfg = [
    {9, 11},  % Profesor 1
    {1, 10},  % Profesor 2
    {8, 7},  % Profesor 3
    {6, 12, 4},  % Profesor 4
    {2},  % Profesor 5
    {3, 5},  % Profesor 6
    {3, 5},  % Profesor 7
    {7, 4},  % Profesor 8
    {},  % Profesor 9
    {9, 11},  % Profesor 10
];
    

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Cada profesor pertenece a su departamento
constraint forall(tr in TRIBUNAL, d in DEPT)(dept_profe[miembros[tr, d]] = d);

% Límite de participación (Profesor 10 máx 2, resto máx 1)
constraint forall(p in PROFESOR)(
    let { var int: apariciones = sum(tr in TRIBUNAL, d in DEPT)(miembros[tr, d] == p) } in
    if p == 10 then apariciones <= 2 else apariciones <= 1 endif
);

% TFGs con temática similar van al mismo tribunal
constraint tribunal_tfg[6] == tribunal_tfg[12];
constraint tribunal_tfg[1] == tribunal_tfg[10];

% un mismo profesor no puede tener dos tribunales el mismo día (aunque sea en sesiones distintas)
constraint forall(tr1, tr2 in TRIBUNAL where tr1 < tr2)(
    (dia_tribunal[tr1] == dia_tribunal[tr2]) -> 
    forall(d in DEPT)(miembros[tr1, d] != miembros[tr2, d])
);

% Cada tribunal puede evaluar entre 4 y 6 TFGs (ambos inclusive)
constraint forall(tr in TRIBUNAL)(count(tribunal_tfg, tr) >= min_tfg_tribunal /\ count(tribunal_tfg, tr) <= max_tfg_tribunal);

% un profesor solo puede participar un dia y turno en el que este disponible
constraint forall(tr in TRIBUNAL, d in DEPT)(
    let { var PROFESOR: p = miembros[tr, d] } in 
    dia_tribunal[tr] in dias_disponibles[p] /\ turno_tribunal[tr] in turnos_disponibles[p]
);

% un profesor no puede estar en el tribunal de un tfg que ha dirigido
constraint forall(t in TFG)(forall(d in DEPT)(not (t in dirige_tfg[miembros[tribunal_tfg[t], d]])));

% Evitar simetrías (muestro los tribunales por orden, único)
constraint miembros[1, DECSAI] < miembros[2, DECSAI] /\ miembros[2, DECSAI] < miembros[3, DECSAI];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve satisfy;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

output [
    "Tribunal \(tr): \(miembros[tr, DECSAI])(\"DECSAI\"), \(miembros[tr, LSI])(\"LSI\"), \(miembros[tr, ICAR])(\"ICAR\") " ++
    "(Día: \(dia_tribunal[tr]), Sesion: " ++ (if fix(turno_tribunal[tr]) == M then "Mañana" else "Tarde" endif) ++ ")\n" ++
    "TFGs a evaluar: " ++ join(", ", [show(t) | t in TFG where fix(tribunal_tfg[t]) == tr]) ++ "\n"
    | tr in TRIBUNAL
];