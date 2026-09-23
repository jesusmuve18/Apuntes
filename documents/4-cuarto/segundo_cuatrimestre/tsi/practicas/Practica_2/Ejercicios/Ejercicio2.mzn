include "globals.mzn";

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         VARIABLES         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Conjunto de estudiantes
enum ESTUDIANTE = { Alba, Bruno, Carla, Diego, Elena, Pablo};

% Conjunto de proyectos
array[1..6] of string: PROYECTO = [
  "A* cuántico",
  "Demostración matemática de que RTA* es un algoritmo absurdo",
  "Modelado de la incertidumbre a la hora de aprobar TSI sin estudiar nada",
  "Poda alfa-beta aplicada al encaje de bolillos",
  "Planificación automática jerárquica híbrida dedicada a la yuxtaposición de los mundos paralelos",
  "Brazo robótico para jugar a la pocha"
];

% Conjunto de aulas
enum AULA = { A1, A2, B1, B2, C1, C2 };

% Conjunto de horas de presentación
array[1..6] of int: HORA = [ 15, 16, 17, 18, 19, 20 ];

% Soluciones
array[ESTUDIANTE] of var 1..6: proyecto_index;
array[ESTUDIANTE] of var AULA: aula;
array[ESTUDIANTE] of var 1..6: hora_index;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       RESTRICCIONES       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Cada estudiante tiene un proyecto
constraint alldifferent(proyecto_index);

% Todo es diferente
constraint alldifferent(aula);
constraint alldifferent(hora_index);

% No pueden coincidir dos proyectos en hora y aula 
% constraint forall(e1, e2 in ESTUDIANTE where e1 < e2) (
%   aula[e1] != aula[e2] \/ hora_index[e1] != hora_index[e2]
% );

% Bruno es el primero en presentar (están ordenados por indexación)
constraint hora_index[Bruno] = min(hora_index);

% El proyecto 4 se presenta en el aula B2 y antes que el proyecto 1
constraint forall(e1, e2 in ESTUDIANTE) ((proyecto_index[e1] == 4 /\ proyecto_index[e2] == 1) -> (aula[e1] = B2 /\ hora_index[e1] < hora_index[e2]));

% El proyecto 1 se presenta inmediatamente antes que el proyecto 3, en un aula que no es ni la B2 ni la A1
constraint forall(e1, e2 in ESTUDIANTE) ((proyecto_index[e1] == 1 /\ proyecto_index[e2] == 3) -> (hora_index[e1] = hora_index[e2] - 1 /\ aula[e1] != B2 /\ aula[e1] != A1  ));

% Diego presenta exactamente una hora después que Alba
constraint hora_index[Diego] = hora_index[Alba] + 1;

% Carla presenta exactamente una hora después que Diego.
constraint hora_index[Carla] = hora_index[Diego] + 1;

% Elena presenta en el aula C1, y su presentación tiene lugar después de Carla pero antes de Pablo.
constraint aula[Elena] = C1 /\ hora_index[Elena] > hora_index[Carla] /\ hora_index[Elena] < hora_index[Pablo];

% El proyecto 2 se presenta exactamente una hora después que el proyecto 3, en un aula que no es la A2
constraint forall(e1, e2 in ESTUDIANTE) ((proyecto_index[e1] == 2 /\ proyecto_index[e2] == 3) -> (hora_index[e1] = hora_index[e2] + 1 /\ aula[e1] != A2));

% El proyecto 3 se presenta en el aula A2
constraint forall(e in ESTUDIANTE) ((proyecto_index[e] == 3) -> aula[e] = A2);

% El proyecto 5 se presenta a las 19:00, mientras que el proyecto 6 se presenta en la C2 a última hora
constraint forall(e in ESTUDIANTE, h in 1..6) ((proyecto_index[e] == 5 /\ HORA[h] == 19) -> hora_index[e] = h);
constraint forall(e in ESTUDIANTE) ((proyecto_index[e] == 6) -> (aula[e] = C2 /\ hora_index[e] = 6));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         RESOLVER         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

solve satisfy;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%          OUTPUT          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

output [
    "\(e): \(PROYECTO[fix(proyecto_index[e])]), \(aula[e]), \(HORA[fix(hora_index[e])]):00\n" | e in ESTUDIANTE
];