CREATE KEYSPACE IF NOT EXISTS liga 
WITH REPLICATION = { 
	'class' : 'SimpleStrategy', 
	'replication_factor' : '1' 
};

USE liga;

CREATE TABLE clasificacion_ligas (
    temporada INT,
    nombre_liga TEXT,
    nombre_equipo TEXT,
    puntos INT,            
    partidos_jugados INT,  
    victorias INT,         
    empates INT,           
    derrotas INT,          
    goles_favor INT,     
    goles_contra INT,     
    diferencia_goles INT,
    
    -- Partition Key: nombre_liga (agrupamos por liga y temporada)
    -- Clustering Key: Ordenamos por Puntos > Dif Goles > Goles Favor > Nombre
    PRIMARY KEY ((nombre_liga, temporada), puntos, diferencia_goles, goles_favor, nombre_equipo)
) WITH CLUSTERING ORDER BY (
    puntos DESC, 
    diferencia_goles DESC, 
    goles_favor DESC, 
    nombre_equipo ASC
);

-- Inserción de equipos
-- 1. Barcelona (49 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Barcelona', 19, 16, 1, 2, 53, 20, 33, 49);

-- 2. Real Madrid (48 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Real Madrid', 20, 15, 3, 2, 43, 17, 26, 48);

-- 3. Villarreal (41 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Villarreal', 19, 13, 2, 4, 37, 19, 18, 41);

-- 4. Atlético Madrid (38 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Atlético Madrid', 19, 11, 5, 3, 34, 17, 17, 38);

-- 5. RCD Espanyol (34 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'RCD Espanyol', 20, 10, 4, 6, 23, 22, 1, 34);

-- 6. Real Betis (32 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Real Betis', 20, 8, 8, 4, 33, 25, 8, 32);

-- 7. Celta de Vigo (29 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Celta de Vigo', 19, 7, 8, 4, 25, 20, 5, 29);

-- 8. Athletic (24 pts) - Nota: Dif Goles -9
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Athletic', 20, 7, 3, 10, 19, 28, -9, 24);

-- 9. Girona (24 pts) - Nota: Dif Goles -14
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Girona', 20, 6, 6, 8, 20, 34, -14, 24);

-- 10. Elche C. F. (23 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Elche C. F.', 19, 5, 8, 6, 25, 24, 1, 23);

-- 11. Osasuna (22 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Osasuna', 20, 6, 4, 10, 21, 24, -3, 22);

-- 12. Rayo Vallecano (22 pts) - Peor DG que Osasuna
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Rayo Vallecano', 19, 5, 7, 7, 16, 22, -6, 22);

-- 13. Real Sociedad (21 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Real Sociedad', 19, 5, 6, 8, 24, 27, -3, 21);

-- 14. R.C.D. Mallorca (21 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'R.C.D. Mallorca', 20, 5, 6, 9, 24, 30, -6, 21);

-- 15. Getafe (21 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Getafe', 19, 6, 3, 10, 15, 25, -10, 21);

-- 16. Sevilla (20 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Sevilla', 19, 6, 2, 11, 24, 30, -6, 20);

-- 17. Alavés (19 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Alavés', 19, 5, 4, 10, 16, 24, -8, 19);

-- 18. Valencia C. F. (17 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Valencia C. F.', 19, 3, 8, 8, 18, 31, -13, 17);

-- 19. Levante (14 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Levante', 19, 3, 5, 11, 21, 32, -11, 14);

-- 20. Real Oviedo (13 pts)
INSERT INTO clasificacion_ligas (temporada, nombre_liga, nombre_equipo, partidos_jugados, victorias, empates, derrotas, goles_favor, goles_contra, diferencia_goles, puntos)
VALUES (2025, 'La Liga Española', 'Real Oviedo', 20, 2, 7, 11, 11, 31, -20, 13);
