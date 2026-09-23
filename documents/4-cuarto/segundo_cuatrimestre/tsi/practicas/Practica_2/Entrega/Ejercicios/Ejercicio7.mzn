include "globals.mzn";

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Constantes
enum EQUIPO = { Celtics, Knicks, Sixers, Heat, Bucks, Lakers, Warriors, Suns, Nuggets, Mavericks};
array[EQUIPO] of string: siglas = ["BOS", "NYK", "PHI", "MIA", "MIL", "LAL", "GSW", "PHX", "DEN", "DAL"];
set of int: DIA_SEMANA = 1..7;
set of int: PARTIDO = 1..12;
enum FRANJA_HORARIA = { Tarde, Noche, Prime_Time };
array[FRANJA_HORARIA] of string: nombre_franja = ["Tarde     ", "Noche     ", "Prime Time"];
enum TIPO_EQUIPO = { Local, Visitante };

% Partidos programados
array [PARTIDO, TIPO_EQUIPO] of EQUIPO: equipos_partido = [|
     Celtics, Knicks     % 1
    | Heat, Sixers       % 2
    | Bucks, Celtics     % 3
    | Lakers, Warriors   % 4
    | Suns, Nuggets      % 5
    | Mavericks, Lakers  % 6
    | Warriors, Suns     % 7
    | Knicks, Heat       % 8
    | Sixers, Bucks      % 9
    | Nuggets, Mavericks % 10
    | Celtics, Lakers    % 11
    | Heat, Warriors     % 12
|]; 

% back-to-back
array[EQUIPO, DIA_SEMANA] of var bool: juega_dia;    % Matriz que indica si un equipo juega un día de la semana
array[EQUIPO] of var 0..1: back_to_back_equipo;      % back_to_backs de cada equipo (como máximo 1)
var int: back_to_back_totales = sum(e in EQUIPO)(back_to_back_equipo[e]);

% Solucion
array [PARTIDO] of var DIA_SEMANA: dia_partido;
array [PARTIDO] of var FRANJA_HORARIA: franja_partido;  

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Un equipo no puede jugar más de un partido el mismo día
% Función bool2int: Convierte un booleano en un entero (true=1, false=0)
constraint forall(e in EQUIPO, d in DIA_SEMANA)(
    sum(p in PARTIDO where equipos_partido[p, Local] == e \/ equipos_partido[p, Visitante] == e)
       (bool2int(dia_partido[p] == d)) <= 1
);

% En una misma franja horaria puede haber como mucho 2 partidos
constraint forall(d in DIA_SEMANA, f in FRANJA_HORARIA)(sum(p in PARTIDO)(dia_partido[p] == d /\ franja_partido[p] == f) <= 2);

% En un mismo día puede haber como mucho 4 partidos
constraint forall(d in DIA_SEMANA)(sum(p in PARTIDO)(dia_partido[p] == d) <= 4);

% El día 4 no puede haber partidos, dado que corresponde a un festivo nacional.
constraint forall(p in PARTIDO)(dia_partido[p] != 4);

% El día 7 (domingo) no puede haber partidos en Prime Time
constraint forall(p in PARTIDO)((dia_partido[p] == 7) -> franja_partido[p] != Prime_Time);

% Los equipos Lakers, Celtics y Warriors no pueden jugar en la franja de Tarde, por razones televisivas
constraint forall(p in PARTIDO)(
    (franja_partido[p] == Tarde) -> forall(te in TIPO_EQUIPO)( not (equipos_partido[p, te] in {Lakers, Celtics, Warriors}))
);

% Al menos 3 partidos de la semana deben disputarse en Prime Time
constraint count(franja_partido, Prime_Time) >= 3;

% El partido Lakers vs Warriors debe jugarse el día 2 en Prime Time
constraint dia_partido[4] = 2 /\ franja_partido[4] = Prime_Time;

% El partido Celtics vs Lakers debe jugarse el día 6 en Prime Time
constraint dia_partido[11] = 6 /\ franja_partido[11] = Prime_Time;

% El partido Bucks vs Celtics debe jugarse también en Prime Time
constraint franja_partido[3] = Prime_Time;

% El pabellón de los Knicks no está disponible el día 5 (por lo que Knicks vs Heat no puede jugarse ese día)
constraint dia_partido[8] != 5;

% El pabellón de los Suns no está disponible el día 2 (por lo que Suns vs Nuggets no puede jugarse ese día)
constraint dia_partido[5] != 2;

% los partidos Bucks vs Celtics y Nuggets vs Mavericks, no pueden jugarse en los días 1 y 6, respectivamente
constraint (dia_partido[3] != 1) /\ (dia_partido[10] != 6);

% Lógica de juega_dia
constraint forall(e in EQUIPO, d in DIA_SEMANA)(
    juega_dia[e, d] = exists(p in PARTIDO)(
        dia_partido[p] == d /\ (equipos_partido[p, Local] == e \/ equipos_partido[p, Visitante] == e)
    )
);

% Se dice que un equipo tiene un back-to-back si juega en dos días consecutivos
constraint forall(e in EQUIPO)(back_to_back_equipo[e] = sum(d in 1..6)( juega_dia[e, d] /\ juega_dia[e, d+1]));

% Cada equipo puede tener como máximo un back-to-back durante la semana. Implícito con la declaración del array
% constraint forall(e in EQUIPO)(back_to_back_equipo[e] <= 1);

% Puede haber como máximo dos back-to-back en toda la semana
constraint back_to_back_totales <= 2;

% Los Celtics y los Warriors no pueden tener ningún back-to-back esta semana
constraint back_to_back_equipo[Celtics] = 0 /\ back_to_back_equipo[Warriors] = 0;

% Los días 5 y 6 deben tener al menos 3 partidos cada uno
constraint sum(p in PARTIDO)(dia_partido[p] == 5) >= 3;
constraint sum(p in PARTIDO)(dia_partido[p] == 6) >= 3;

% Si un equipo juega en Prime Time, no puede jugar al día siguiente en la franja de Tarde
% Función let - in: Define variables en el entorno local (let) para facilitar la escritura y la eficiencia (no tiene que recalcularlas) y las usa (in)
constraint forall(e in EQUIPO, d in 1..6)(
    let {
        var bool: juega_prime = exists(p in PARTIDO)(
            dia_partido[p] == d /\ franja_partido[p] == Prime_Time /\
            (equipos_partido[p, Local] == e \/ equipos_partido[p, Visitante] == e)
        ),
        var bool: juega_tarde_siguiente = exists(p in PARTIDO)(
            dia_partido[p] == d+1 /\ franja_partido[p] == Tarde /\
            (equipos_partido[p, Local] == e \/ equipos_partido[p, Visitante] == e)
        )
    } in
    juega_prime -> not juega_tarde_siguiente
);

% El número de partidos de Tarde debe ser mayor que el número de partidos de Noche
constraint count(franja_partido, Tarde) > count(franja_partido, Noche);

% El número de partidos de Noche debe ser mayor que el número de partidos de Prime Time
constraint count(franja_partido, Noche) > count(franja_partido, Prime_Time);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve satisfy;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

output [
    "CALENDARIO SEMANAL NBA (CSP)\n",
    "============================\n",
    "Back-to-backs totales = \(back_to_back_totales)\n",
    "+------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n",
    "| Sesion     | Dia 1            | Dia 2            | Dia 3            | Dia 4            | Dia 5            | Dia 6            | Dia 7            |\n",
    "+------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n"
] ++
[
    if d == 1 then "| " ++ (nombre_franja[f]) ++ " | "  else "" endif ++
    let {
        array[int] of int: partidos = [p | p in PARTIDO where fix(dia_partido[p]) == d /\ fix(franja_partido[p]) == f]
    } in
    if length(partidos) == 0 then
        "-                | "
    elseif length(partidos) == 1 then
        siglas[equipos_partido[partidos[1], Local]] ++ "-" ++ siglas[equipos_partido[partidos[1], Visitante]] ++ "          | "
    else
        siglas[equipos_partido[partidos[1], Local]] ++ "-" ++ siglas[equipos_partido[partidos[1], Visitante]] ++ " / " ++
        siglas[equipos_partido[partidos[2], Local]] ++ "-" ++ siglas[equipos_partido[partidos[2], Visitante]] ++ " | "
    endif ++
    if d == 7 then "\n+------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n" else "" endif
    | f in FRANJA_HORARIA, d in DIA_SEMANA
] ++
[
    "Leyenda:\nBOS=Celtics, NYK=Knicks, PHI=Sixers, MIA=Heat, MIL=Bucks,\nLAL=Lakers, GSW=Warriors, PHX=Suns, DEN=Nuggets, DAL=Mavericks\n"
];