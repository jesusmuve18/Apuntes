# Vibe Coding NO, Ingeniería Aumentada Sí

Impartida por Jose Carlos Calvo Tudela (*Nazaríes Intelligenia*) el 20-5-2026.

## Introducción

Hay muchos problemas que no son difíciles a nivel de ingeniería pero sí son muy útiles para el día a día. Esto provoca que muchos particulares estén generando código que no crearía empresas por temas de escala o especifidad. 

Hoy en día Excel mueve más dinero que SAP, Salesforce y Oracle juntos. Cualquier empresa por grande que sea usa hojas de cálculo (no necesita ingenieros para ello). 

Un programa hecho con Vibe Coding funciona y arregla el problema y puede ser distribuido a otras empresas. Pero no es lo mismo que empresas vuelquen su información en esa herramienta que no está preparado para ello y que puede poner en riesgo la infraestructura y la información.

En cuanto se ponen en juego información como monederos virtuales, emails y más datos personales deja de ser recomendable esta práctica. 

Consideremos el símil de la construcción. Podríamos permitir a un 'inexperto' hacer una barbacoa o incluso si es muy bueno una casa de una planta. Sin embargo, nunca le confiaríamos la construcción de un edificio.

## Qué construir con *vibe coding* y qué no

### Sin problemas
- Script personal para mi uso propio
- Automatización de tareas internas mías
- Prototipo desechale para validar una idea
- App para empresa familiar
- Avisar a la cafetería del desayuno
- Sustituir un excel compartido por algo más cómodo

Si se da alguna de las siguientes condiciones no se debería de confiar en el *vibe coding* (zona verde):
- Se va a publicar en cloud accesible desde internet
- Va a gestionar datos personales, sensibles o regulados
- Va a gestionar dinero, pagos o transacciones
- Va a afectar a personales que no están en la sala donde se construye
- Va a durar más de 6 meses.

### Ingeniería aumentada

- Saas multienant accesible desde internet.
- Datos médicos, financieros o de menores. 
- Sistemas con vidas en juego o sector regulado.
- Plataformas con miles o millones de usuarios.
- Software que va a vivir más de 5 años.
- Cualquier cosa con cumplimiento legal exigible.


## Seguridad

Atacar ya es prácticamente gratis. Estas herramientas de IA vienen a potenciar ambos bandos de igual manera. Alguien potenciado con IA tiene más escala tanto vertical como horizontalmente y cuenta con una ventaja muy grande con respecto a alguien que no cuente con esta ventaja.

Gente que a día de hoy no hacía *pentesting* y no tenía formación ya puede hacerlo. Una persona que ya contaba con esta formación y cuenta con herramientas de IA se ve muy potenciado.

Las empresas ya tienen sistemas de IA para atacar por fuerza bruta y para defenderse.

Por una parte somos conscientes de que las personas que pueden atacar tienen mucha más capacidad (en ambas escalas) y por otra los equipos cuentan con mucha más seguridad potenciada por la propia IA gracias a su reducido (o nulo) coste. 

Si potenciamos ambas partes para tener ese equilibrio, ese mismo software provoca que haya 20 veces más código 'de mierda' (tanto de ataque como de defensa). 

La IA es a la vez el escudo más poderoso y la espada más afilada que hemos tenido nunca y se está repartiendo a partes iguales entre los dos bandos. Nos vamos a encontrar un montón de empresas hackeadas, con problemas en administración, etc. Esto es porque lanzan sistemas con poca ingeniería pero funcionales, que son fácilmente atacados por sistemas automatizados, potenciados por IA.

Por tanto es importante no tirar *prompts*, sino usar una **metodología** antes del código.

## Metodología

Podemos definir primero todo el funcional, estilos, bibliotecas, especificaciones técnicas, de software, de despliegue... de forma documentada. Es decir, generar todo el contexto (unas tres semanas de trabajo).

Después podemos tener agentes especializados en comprobar que se cumplen los requerimientos del contexto y que vaya revisando poco a poco. Un agente para buenas prácticas (que ignore la funcionalidad), un agente que compruebe el funcionamiento (pero ignore el resto), etc.

Lo ideal ahora es extraer del contexto piezas documentales que tengan todo lo necesario para cada requerimiento funcional. De esta forma acortamos el contexto y le damos solo la información específica. Generamos la pieza de conocimiento necesaria para hacer esa pieza de forma que no falte ni sobre información. 

Ya tenemos las herramientas separadas y el *check de avance* (los agentes especializados). Además se necesita ir guardando una base de conocimiento de forma que permita trabajar en equipo con contextos comunes, que permita volver atrás, etc.

El paquete final queda muy bien documentado, muy testeado y profesional. Está preparado para aumentar las funcionalidades, etc. En este caso este sistema se hizo para *elcorteingles*.

En los próximos años, lo que va a marcar la diferencia no es saber programar, es tener criterio. En unos meses no va a ser necesario revisar tanto (ya que el código que va a dar es de calidad) pero será necesario guiar adecuadamente su generación, dar instrucciones precisas y tener una metodología clara.

Al igual que confiamos en las librerías externas podremos confiar en el código generado por la IA, pero necesitaremos el criterio necesario para ver qué código es bueno y qué decisiones son las correctas. Tenemos que saber qué aproximaciones resuelven qué problemas. 

Por ello necesitaremos leer más libros y menos tutoriales. Estudiar más teoría y picar menos código. Quizás ya no haya que evaluar lo que el estudiante construye. Quizá haya que evaluar las preguntas que hace antes de construir. Necesitaremos saber hacer las preguntas oportunas para saber elegir el camino correcto a la hora de resolver cierto problema. Históricamente se daban los requerimientos y se pedía que se implementase. Ahora nuestro trabajo va a ser sacar los requerimientos tanto funcionales como no funcionales. Esto será lo que se empiece a valorar, las buenas decisiones con criterio.

Antes, una decisión que no era buena del todo permitía modificarse sin afectar mucho (ya que se programaba más lento). Sin embargo ahora se desarrolla tan rápido que un error de 3 días atrás afectará a todo el sistema y necesitará una refactorización completa. 

Vienen años de software mal hecho y se demandará el que sepa arreglarlo. 


## Recomendaciones

Los recién salidos de la carrera tienen muy asequible el decir "no tengo el criterio o la experiencia pero sí puedo tener más criterio que un niño de 17 años". Investigar en Claude soberana, Amazon cursos, etc. Investigar en las áreas nuevas, librerías y estándares nuevos de la industria. 
