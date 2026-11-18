package com.jesusmuve.etsiitsmartcampus.core.model

/** Tipo de un [Space] tal como lo muestra la interfaz (cada uno con su texto en `strings.xml`). */
sealed interface SpaceTypePresentation {
    /** Aula o espacio docente. */
    data object Classroom : SpaceTypePresentation

    /** Entrada o acceso. */
    data object Entrance : SpaceTypePresentation

    /** Escaleras o conexión vertical peatonal. */
    data object Stairs : SpaceTypePresentation

    /** Pasillo o zona de circulación. */
    data object Hall : SpaceTypePresentation

    /** Espacio común o de uso general. */
    data object Common : SpaceTypePresentation

    /** Laboratorio de prácticas. */
    data object Laboratory : SpaceTypePresentation

    /** Despacho (tutorías, profesorado). */
    data object Office : SpaceTypePresentation

    /** Biblioteca o sala de estudio. */
    data object Library : SpaceTypePresentation

    /** Cafetería o comedor. */
    data object Cafeteria : SpaceTypePresentation

    /** Tipo desconocido: se muestra un texto genérico. */
    data object Unknown : SpaceTypePresentation
}

/** Clave técnica de catálogo para aulas. */
const val SPACE_TYPE_CLASSROOM: String = "classroom"

/** Clave técnica de catálogo para entradas/accesos. */
const val SPACE_TYPE_ENTRANCE: String = "entrance"

/** Clave técnica de catálogo para escaleras. */
const val SPACE_TYPE_STAIRS: String = "stairs"

/** Clave técnica de catálogo para pasillos/zonas de circulación. */
const val SPACE_TYPE_HALL: String = "hall"

/** Clave técnica de catálogo para espacios comunes. */
const val SPACE_TYPE_COMMON: String = "common"

const val SPACE_TYPE_LABORATORY: String = "laboratory"

const val SPACE_TYPE_OFFICE: String = "office"

const val SPACE_TYPE_LIBRARY: String = "library"

const val SPACE_TYPE_CAFETERIA: String = "cafeteria"

/**
 * Convierte el valor de [Space.type] en una [SpaceTypePresentation].
 *
 * @param rawType Valor de [Space.type] proveniente del catálogo.
 * @return Presentación conocida o [SpaceTypePresentation.Unknown].
 */
fun spaceTypePresentationOf(rawType: String): SpaceTypePresentation =
    when (rawType.trim().lowercase()) {
        SPACE_TYPE_CLASSROOM -> SpaceTypePresentation.Classroom
        SPACE_TYPE_ENTRANCE -> SpaceTypePresentation.Entrance
        SPACE_TYPE_STAIRS -> SpaceTypePresentation.Stairs
        SPACE_TYPE_HALL -> SpaceTypePresentation.Hall
        SPACE_TYPE_COMMON -> SpaceTypePresentation.Common
        SPACE_TYPE_LABORATORY -> SpaceTypePresentation.Laboratory
        SPACE_TYPE_OFFICE -> SpaceTypePresentation.Office
        SPACE_TYPE_LIBRARY -> SpaceTypePresentation.Library
        SPACE_TYPE_CAFETERIA -> SpaceTypePresentation.Cafeteria
        else -> SpaceTypePresentation.Unknown
    }
