> Autor: Miguel Ángel Moreno Castro
>
> Source: Carlos Ureña Almagro

# Tema 1
## Ejercicio 1

Escribe una función que genera una tabla de coordenadas de posición de vértices con las coordenadas de los vértices de un polígono regular de $n$ lados o aristas (es una constante del programa), con centro en el origen y radio unidad. 

Los vértices se almacenan como flotantes de doble precisión (`double`), con $2$ coordenadas por vértice (usa una tabla de tipo `vector<dvec2>` para esto). Adicionalente, en esa función escribe el código que crea el correspondiente descriptor de VAO a partir de esta tabla (el vao queda guardado como una variable global del programa). 

El valor de $n$ ($> 2$) es un parámetro entero sin signo. El VAO sería la base para visualizar el polígono (únicamente las aristas), considerando la tabla de vértices como una secuencia de vértices no indexada. Escribe dos variantes del código, de forma que la tabla se debe diseñar para representar el polígono usando distintos tipos de primitivas:
- `GL_LINE_LOOP`
- `GL_LINES`
En este problema y el siguiente se pide únicamente generar las tablas y el VAO, en ningún caso se pide visualizar nada.
### Solución

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	assert(glGetError() == GL_NO_ERROR);
	if (polygon == nullptr)
	{
		
		std::vector<glm::dvec2> vertices;
		
		for (size_t i = 0; i < num_verts; ++i)
		{
			float angle = 2.0f * M_PI * i / num_verts;
			vertices.push_back(glm::vec2(0.5f * cos(angle), 0.5f * sin(angle)));
		}
		polygon = new VertexArrayObject(1, 
			new AttribBufferObject(cauce->ind_atrib_posiciones, GL_DOUBLE, 2, num_verts, vertices));
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	polygon->draw(GL_LINE_LOOP);
}
```

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	assert(glGetError() == GL_NO_ERROR);
	if (polygon == nullptr)
	{
		std::vector<glm::dvec2> vertices;
		float angle = 2.0f * M_PI * i / num_verts;
		for (size_t i = 0; i < num_verts; ++i)
		{
			vertices.push_back(glm::vec2(0.5f * cos(i*angle), 0.5f * sin(i*angle)));
			vertices.push_back(glm::vec2(0.5f * cos((i+1)*angle), 0.5f * sin((i+1)*angle)));
		}
		polygon = new VertexArrayObject(1, 
			new AttribBufferObject(cauce->ind_atrib_posiciones, GL_DOUBLE, 2, num_verts, vertices));
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	polygon->draw(GL_LINES);
}
```

## Ejercicio 2

Escribe otra función para generar una tabla de vértices y una tabla de índices (y el correspondiente descriptor de VAO en una variable global), que permitiría visualizar el mismo polígono regular del problema anterior pero ahora como un conjunto de $n$ triángulos iguales rellenos que comparten un vértice en el centro del polígono (el origen). Usa ahora datos de simple precisión float para los vértices, con tres valores por vértice, siendo la $Z$ igual a 0 en todos ellos.

La tabla está destinada a ser visualizada con el tipo de primitiva `GL_TRIANGLES`. Escribe dos variantes del código: 
- Usa una secuencia no indexada con $3n$ vértices
- Usa una secuencia indexada (sin vértices repetidos), con $n + 1$ vértices y $3n$ índices.
### Solución

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	assert(glGetError() == GL_NO_ERROR);
	if (polygon == nullptr)
	{
		std::vector<glm::vec2> vertices;
		float angle = 2.0f * M_PI / num_verts;
		for (size_t i = 0; i < num_verts; ++i)
		{
			vertices.push_back(glm::vec2(0.0f, 0.0f));
			vertices.push_back(glm::vec2(0.5f * cos(angle * i), 0.5f * sin(angle * i)));
			vertices.push_back(glm::vec2(0.5f * cos(angle * (i+1)), 0.5f * sin(angle * (i+1))));
		} 
		polygon = new VertexArrayObject(1, 
					new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	polygon->draw(GL_TRIANGLES);
}
```

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	if (polygon == nullptr)
	{
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec3> colors;
		std::vector<glm::uvec3> indices;
		for (size_t i = 0; i < num_verts; ++i)
		{
			float angle = 2.0f * M_PI * i / num_verts;
			vertices.push_back(glm::vec2(0.5f * cos(angle), 0.5f * sin(angle)));
		}
			vertices.push_back(glm::vec2(0.0f, 0.0f)); 
		for (size_t i = 0; i < num_verts; ++i)
		{
			indices.push_back(glm::uvec3(i, (i + 1) % num_verts, num_verts));
		}
		polygon = new VertexArrayObject(cauce->num_atribs, 
					new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
		polygon->add(new IndexBufferObject(indices));
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	polygon->draw(GL_TRIANGLES);
}
```

## Ejercicio 3

Modifica el código de los ejercicios anteriores para incluir una nueva función para visualizar las aristas y el relleno del polígono regular de $n$ lados, usando los dos descriptores de VAO que se mencionan en
- El enunciado del ejercicio 1 (`GL_LINE_LOOP`) para las aristas,
- El enunciado del ejercicio 2 (secuencia indexada) para el relleno.

El polígono se verá relleno de un color plano y las aristas con otro color (también plano), distintos ambos del color de fondo. Debes usar un VAO para las aristas y otro para el relleno. No uses una tabla de colores de vértices para este problema, en su
lugar usa la función `glVertexAttrib` para cambiar el color antes de dibujar.

Incluye todo el código en una función de visualización (nueva), esa función debe incluir tanto la creación de tablas y VAOs (en la primera llamada), como la visualización (en todas las llamadas).
### Solución

```c++
void DrawBoundedPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	assert(glGetError() == GL_NO_ERROR);
	VertexArrayObject *polygon = nullptr;
	VertexArrayObject *boundary = nullptr;
	
	std::vector<glm::vec2> vertices;
	std::vector<glm::uvec3> indices;
	
	for (size_t i = 0; i < num_verts; ++i)
	{
		float angle = 2.0f * M_PI * i / num_verts;
		vertices.push_back(glm::vec2(0.5f * cos(angle), 0.5f * sin(angle)));
		indices.push_back(glm::uvec3(i, (i + 1) % num_verts, num_verts));
	}
	boundary = new VertexArrayObject(1, new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
	polygon = new VertexArrayObject(1, new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
	polygon->add(new IndexBufferObject(indices));
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertexAttrib3f(cauce->ind_atrib_colores, 1.0, 0.0, 0.0);
	polygon->draw(GL_TRIANGLES);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(cauce->ind_atrib_colores, 0.0, 0.0, 0.0);
	boundary->draw(GL_LINE_LOOP);
}
```

## Ejercicio 4

Repite el problema anterior, pero ahora intenta usar el mismo VAO para las aristas y los triángulos rellenos. Para eso
puedes usar una única tabla de $n + 1$ posiciones de vértices, esa tabla sirve de base para el relleno, usando índices. Para las aristas, puedes usar `GL_LINE_LOOP`, pero teniendo en cuenta únicamente los $n$ vértices del polígono (sin usar el vértice en el origen).
### Solución

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	
	std::vector<glm::vec2> vertices;
	std::vector<glm::uvec3> indices;
	
	if (polygon == nullptr)
	{
		for (size_t i = 0; i < num_verts; ++i)
		{
			float angle = 2.0f * M_PI * i / num_verts;
			vertices.push_back(glm::vec2(0.5f * cos(angle), 0.5f * sin(angle)));
		}
		vertices.push_back(glm::vec2(0.0f, 0.0f));
		
		for (size_t i = 0; i < num_verts; ++i)
		{
			indices.push_back(glm::uvec3(i, (i + 1) % num_verts, num_verts));
		}
		polygon = new VertexArrayObject(cauce->num_atribs, 
					new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
		polygon->add(new IndexBufferObject(indices));
	}
	polygon->createVAO();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertexAttrib3f(cauce->ind_atrib_colores, 1.0, 0.0, 0.0);
	glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, (void *)0);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(cauce->ind_atrib_colores, 0.0, 0.0, 0.0);
	glDrawArrays(GL_LINE_LOOP, 0, num_verts);
}
```


## Ejercicio 5

Repite el problema anterior, pero ahora el relleno, en lugar de hacerse todo del mismo color plano, se hará mediante interpolación de colores (las aristas siguen estando con un único color). Para eso se debe generar una tabla de colores de vértices (`vector<vec3>`), inicializada con colores aleatorios para cada uno de los $n + 1$ vértices. Ten en cuenta que para visualizar las aristas, debes de deshabilitar antes el uso de la tabla de colores.
### Solución

```c++
void DrawPolygon(unsigned int num_verts)
{
	assert(2 < num_verts);
	
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::uvec3> indices;
	
	if (polygon == nullptr)
	{
		for (size_t i = 0; i < num_verts; ++i)
		{
			float angle = 2.0f * M_PI * i / num_verts;
			vertices.push_back(glm::vec2(0.5f * cos(angle), 0.5f * sin(angle)));
			colors.push_back(glm::vec3(0.5f * cos(angle) + 0.5f, 0.5f * sin(angle) + 0.5f, 0.5f));
		}
		vertices.push_back(glm::vec2(0.0f, 0.0f));
		colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		
		for (size_t i = 0; i < num_verts; ++i)
		{
			indices.push_back(glm::uvec3(i, (i + 1) % num_verts, num_verts));
		}
		polygon = new VertexArrayObject(cauce->num_atribs, 
					new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
		polygon->add(new AttribBufferObject(cauce->ind_atrib_colores, colors));
		polygon->add(new IndexBufferObject(indices));
	}
	
	polygon->createVAO();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnableVertexAttribArray(cauce->ind_atrib_colores);
	glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, (void *)0);
	
	glDisableVertexAttribArray(cauce->ind_atrib_colores);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(cauce->ind_atrib_colores, 0.0, 0.0, 0.0);
	glDrawArrays(GL_LINE_LOOP, 0, num_verts);
}
```

## Ejercicio 6

Repite el problema anterior, pero ahora, para guardar las posiciones y los vértices, se usará una estructura tipo AOS, es decir, se usa un array de estructuras o bloques de datos, en cada estructura o bloque se guardan 3 flotantes para la posición y 3 flotantes para el color RGB.

La estructura completa se debe alojar en un único VBO de atributos con todos los flotantes de las posiciones y colores, así que no uses la clase `DescrVAO`, sino que escribe el código OpenGL directamente.
### Solución

```c++
void DrawPolygonWithoutVAO(unsigned int num_verts)
{
	assert(2 < num_verts); 
	static GLuint vao = 0;
	static GLuint vbo_vertices = 0;
	static GLuint vbo_indices = 0;
	std::vector<std::pair<glm::vec2, glm::vec3>> vertices;
	std::vector<glm::uvec3> indices;
	if (polygon == nullptr)
	{
		for (size_t i = 0; i < num_verts; ++i)
		{
			float angle = 2.0f * M_PI * i / num_verts;
			vertices.emplace_back(
				glm::vec2(cos(angle), sin(angle)), 
				glm::vec3(cos(angle) + 0.5f, sin(angle) + 0.5f, 0.5f)
			);
		}
		vertices.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		for (size_t i = 0; i < num_verts; ++i)
		{
			indices.push_back(glm::uvec3(i, (i + 1) % num_verts, num_verts));
		}
		// Create the VAO
		glGenVertexArrays(1, &vao);
		// Bind the VAO
		glBindVertexArray(vao);
		// Create the VBO for the vertices
		glGenBuffers(1, &vbo_vertices);
		// Bind the VBO of vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
		// Copy the data to the VBO of vertices
		glBufferData(
			GL_ARRAY_BUFFER, 
			vertices.size() * sizeof(glm::vec2) + vertices.size() * sizeof(glm::vec3), 
			vertices.data(), 
			GL_STATIC_DRAW
		);
		// Specify the location and data format of the array of vertices
		glVertexAttribPointer(
			cauce->ind_atrib_posiciones, 
			2, 
			GL_FLOAT, 
			GL_FALSE, 
			sizeof(glm::vec2) + sizeof(glm::vec3), 
			(void *)0
		);
		// Enable the vertex attribute array
		glEnableVertexAttribArray(cauce->ind_atrib_posiciones);
		// Specify the location and data format of the array of colors
		glVertexAttribPointer(
			cauce->ind_atrib_colores, 
			3, 
			GL_FLOAT, 
			GL_FALSE, 
			sizeof(glm::vec2) + sizeof(glm::vec3), 
			(void *)(sizeof(glm::vec2))
		);
		// Enable the vertex attribute array
		glEnableVertexAttribArray(cauce->ind_atrib_colores);
		// Create the VBO for the indices
		glGenBuffers(1, &vbo_indices);
		// Bind the VBO of indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
		// Copy the data to the VBO of indices
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			indices.size() * sizeof(glm::uvec3), 
			indices.data(), 
			GL_STATIC_DRAW
		);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnableVertexAttribArray(cauce->ind_atrib_colores);
	glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, (void *)0);
	
	glDisableVertexAttribArray(cauce->ind_atrib_colores);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(cauce->ind_atrib_colores, 0.0, 0.0, 0.0);
	glDrawArrays(GL_LINE_LOOP, 0, num_verts);
}
```
## Ejercicio 7

Modifica el código del ejemplo `opengl3-minimo` para añadir a la clase `Cauce` un método que permita especificar la región visible. El método debe de fijar el valor del parámetro uniform con la matriz de proyección, según lo visto en las transparencias de teoria.

```c++
void Cauce::setVisibleRegion(const glm::vec3 min, const glm::vec3 max)
{
	assert(loc_mat_proyeccion != -1);
	assert(glGetError() == GL_NO_ERROR);
	glm::vec3 s = {2.0f / (max.x - min.x), 2.0f / (max.y - min.y), 2.0f / (max.z - min.z)};
	glm::vec3 c = {(min.x + max.x)/2.0f, (min.y + max.y)/2.0f, (min.z + max.z)/2.0f};
	mat_proyeccion = glm::mat4(
		s.x, 0, 0, -c.x * s.x,
		0, s.y, 0, -c.y * s.y,
		0, 0, s.z, -c.z * s.z,
		0, 0, 0, 1
	);
	glUniformMatrix4fv(loc_mat_proyeccion, 1, GL_FALSE, glm::value_ptr(mat_proyeccion));
	assert(glGetError() == GL_NO_ERROR);
}
```


## Ejercicio 8

Modifica el código del ejemplo `opengl3-minimo` para que no se introduzcan deformaciones cuando la ventana se redimensiona y el alto queda distinto del ancho. El código original del repositorio presenta los objetos deformados (escalados con distinta escala en vertical y horizontal) cuando la ventana no es cuadrada, ya que visualiza en el viewport (no cuadrado) una cara (cuadrada) del cubo de lado $2$. 

Para evitar este problema, en cada cuadro se deben de leer las variables que contienen el tamaño actual de la ventana y en función de esas variables modificar la zona visible, que ya no será siempre un cubo de lado $2$ unidades, sino que será un ortoedro que contendrá dicho cubo de lado $2$, pero tendrá unas dimensiones superiores a $2$ (en $X$ o en $Y$, no en ambas), adaptadas a las proporciones de la ventana (el ancho en $X$ dividido por el alto en $Y$ es un valor que debe coincidir en el ortoedro visible y en el viewport).
### Solución

```c++
void ShowFrame()
{
	. . .
	// Set the projection matrix
	cauce->setProjectionMatrix(glm::mat4(1.0));
	// Set the visible region of the projection matrix
	const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	if (aspect_ratio < 1.0)
	{
		cauce->setVisibleRegion({-1.0, -1.0 / aspect_ratio, -1.0}, {1.0, 1.0 / aspect_ratio, 1.0});
	}
	else
	{
		cauce->setVisibleRegion({-aspect_ratio, -1.0, -1.0}, {aspect_ratio, 1.0, 1.0});
	}
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	. . .
}
```

## Ejercicio 9

Demuestra que el producto escalar de dos vectores se puede calcular usando sus coordenadas en cualquier marco cartesiano como la suma del producto componente a componente, a partir de las propiedades que definen dicho producto escalar.
### Solución

Sea $R =[ \dot{o}, \vec{x}, \vec{y}, \vec{z}]$  un sistema de referencia ortonormal cualquiera del espacio afín $\mathbb{R}^3$, entonces el producto escalar es una aplicación bilineal 
$$\langle \cdot, \cdot \rangle \colon \mathbb{R}^3 \times \mathbb{R}^3 \to \mathbb{R}$$
Por tanto podemos considerar la matriz asociada a dicha forma bilineal
$$A = \begin{pmatrix} \langle \vec{x}, \vec{x} \rangle & \langle \vec{x}, \vec{y} \rangle & \langle \vec{x}, \vec{z} \rangle \\ \langle \vec{y}, \vec{x} \rangle & \langle \vec{y}, \vec{y} \rangle & \langle \vec{y}, \vec{z} \rangle \\ \langle \vec{z}, \vec{x} \rangle & \langle \vec{z}, \vec{y} \rangle & \langle \vec{z}, \vec{z} \rangle \end{pmatrix} = \begin{pmatrix} 1 & 0 & 0 \\ 0 & 1 & 0 \\ 0 & 0 & 1 \end{pmatrix}$$
de forma que dados $u = (u_1, u_2, u_3)$ y $v = (v_1, v_2, v_3)$ se tiene que
$$\langle u, v \rangle = u^t \cdot A \cdot v = \begin{pmatrix} u_1 & u_2 & u_3 \end{pmatrix} \cdot \begin{pmatrix} v_1 \\ v_2 \\ v_3 \end{pmatrix} = u_1 \cdot v_1 + u_2 \cdot v_2 + u_3 \cdot v_3 $$

## Ejercicio 10

Demuestra que el producto vectorial de dos vectores es perpendicular a cada uno de esos dos vectores.
### Solución

Dados $x = (x_1, x_2, x_3)$ e $y = (y_1, y_2, y_3)$ se tiene que
$$\langle x, x \times y \rangle = x_1 \cdot \begin{vmatrix} x_2 & x_3 \\ y_2 & y_3 \end{vmatrix} + x_2 \cdot \begin{vmatrix} x_3 & x_1 \\ y_3 & y_1 \end{vmatrix} + x_3 \cdot \begin{vmatrix} x_1 & x_2 \\ y_1 & y_2 \end{vmatrix} = \begin{vmatrix} x_1 & x_2 & x_3 \\ x_1 & x_2 & x_3 \\ y_1 & y_2 & y_3 \end{vmatrix} = 0$$
$$\langle y, x \times y \rangle = y_1 \cdot \begin{vmatrix} x_2 & x_3 \\ y_2 & y_3 \end{vmatrix} + y_2 \cdot \begin{vmatrix} x_3 & x_1 \\ y_3 & y_1 \end{vmatrix} + y_3 \cdot \begin{vmatrix} x_1 & x_2 \\ y_1 & y_2 \end{vmatrix} = \begin{vmatrix} y_1 & y_2 & y_3 \\ x_1 & x_2 & x_3 \\ y_1 & y_2 & y_3 \end{vmatrix} = 0$$
# Tema 2

## Ejercicio 1

Supongamos que queremos codificar una esfera de radio $1/2$ y centro en el origen de dos formas:
- Por enumeración espacial, dividiendo el cubo que engloba a la esfera en celdas, de forma que haya $k$ celdas por lado del cubo, todas ellas son cubos de $1/k$ de ancho. Cada celda ocupa un bit de memoria (si su centro está en la esfera, se guarda un $1$, en otro caso un $0$).
- Usando un modelo de fronteras (una malla indexada de triángulos), en el cual se usa una rejilla de triángulos y aristas que siguen los meridianos y paralelos, habiendo en cada meridiano y en cada paralelo un total de $k$ vértices (se guarda únicamente la tabla de vértices y la de triángulos).

Asumiendo que un `float` y un `int` ocupan $4$ bytes cada uno, contesta a estas cuestiones:
- Expresa el tamaño de ambas representaciones en bytes como una función de $k$.
- Suponiendo que $k = 16$ calcula cuantos KB de memoria ocupa cada estructura.
- Haz lo mismo asumiendo ahora que $k = 1024$ (expresa los resultados en MB)
Compara los tamaños de ambas representaciones en ambos casos ($k = 16$ y $k = 1024$).

### Enumeración Espacial

Considerando el cubo de lado unidad que engloba a la esfera de radio $1/2$, si cada lado del cubo está compuesto por $k$ celdas obtenemos un total de $k^3$ celdas contenidas en el cubo. Si cada una de dichas celdas ocupan un bit en memoria el tamaño en bytes sería de $\frac{k^3}{8} = \left( \frac{k}{2} \right)^3$ bytes. Luego:
- $k = 16$: Ocuparía un total de $512$ bytes, lo que equivale a $0.5$ KB
- $k = 1024$: Ocuparía un total de $134217728$ bytes, lo que equivalen a $128$ MB

### Modelo de Fronteras

Considerando que cada meridiano/paralelo tiene $k$ vértices, será necesario almacenar un total de $k^2$ vértices. Además ya que cada vértice está compuesto de $3$ componentes de tipo `float` tendríamos que almacenar $12 \cdot k^2$ bytes. Por otro lado, cada $4$ vértices construimos $2$ triángulos por lo que tendremos $2 \cdot k^2$ triángulos y, puesto que cada triángulo está compuesto de $3$ componentes de tipo `int` tendríamos que almacenar $24  \cdot k^2$ bytes. Luego:
- $k = 16$: Ocuparía un total de $9216$ bytes, lo que equivale a $9$ KB
- $k = 1024$: Ocuparía un total de $37748736$ bytes, lo que equivalen a $36$ MB

## Ejercicio 2

Considera una malla indexada (tabla de vértices y caras, esta última con índices de vértices) con topología de rejilla como la de la figura, en la cual hay $m$ columnas de pares de triángulos y $n$ filas (es decir, hay $n + 1$ filas de vértices y $m + 1$ columnas de vértices, con $n,\,m > 0$).

![](./resources/img133.png)

En relación a este tipo de mallas, responde a estas dos cuestiones:
- Supongamos que un `float` ocupa $4$ bytes (igual a un `int`) ¿que tamaño en memoria ocupa la malla completa, en bytes?(Tener en cuenta únicamente el tamaño de la tabla de vértices y triángulos, suponiendo que se almacenan usando los tipos `float` e `int`, respectivamente). Expresa el tamaño como una función de `m` y `n`.
- Escribe el tamaño en KB suponiendo que $m = n = 128$.
- Supongamos que $m$ y $n$ son ambos grandes (es decir, asumimos que $1/n$ y $1/m$ son prácticamente $0$). Deduce que relación hay entre el número de caras $n_C$ y el número de vértices $n_V$ en este tipo de mallas.
### Solución

Respecto a la tabla de vértices, sabemos que tenemos $n + 1$ filas de vértices con $m + 1$ vértices en cada una, y considerando que cada vértice es de tipo `vec3`, esto es, tres coordenadas de tipo `float`, dicha tabla ocupará
$$12 \cdot(n + 1)(m + 1) \; \text{ bytes }$$
Análogamente, respecto a la tabla de triángulos, sabemos que cada triángulo está formado por tres coordenadas de tipo `int` y, tenemos $2n$ triángulos en cada una de las $m$ columnas, por tanto ocupa un total de
$$12 \cdot (2 \cdot n \cdot m) \; \text{ bytes }$$
Si consideramos $m = n = 128$, la malla nos ocupará unos $579$ KB. Por otro lado, si consideramos que $m$ y $n$ son ambos muy grandes
$$\frac{n_C}{n_V} = \frac{2nm}{(n+1)(m+1)} \overset{n \to \infty}{\longrightarrow} 2 \; \Longrightarrow n_C = 2n_V$$
## Ejercicio 3

Imagina de nuevo una malla como la del ejercicio anterior, supongamos que usamos una representación como tiras de triángulos, de forma que cada fila de triángulos (con $2n$ triángulos) se almacena en una tira, habiendo un total de $m$ tiras.

La tabla de punteros a tiras tiene un entero (el número de tiras) y $m$ punteros, cada puntero suponemos que tiene $8$ bytes de tamaño. De nuevo, asume que las coordenadas son de tipo float ($4$ bytes). Responde a estas cuestiones:

- Indica que cantidad de memoria ocupa esta representación, en estos dos casos:
	- Como función de $n$ y $m$, en bytes.
	- Suponiendo $m = n = 128$, en KB.
- Para $m$ y $n$ grandes (es decir, cuando $1/n$ y $1/m$ son casi nulos), describe que relación hay entre el tamaño en memoria de la malla indexada del problema anterior y el tamaño de la malla almacenada como tiras de triángulos.
- Si suponemos que la transformación de cada vértice se hace en un tiempo constante igual a la unidad, describe que relación hay entre los tiempos de procesamiento de vértices para esta malla cuando se representa como una malla indexada y como tiras de triángulos.
### Solución

Sabemos que, en este caso, la malla es una estructura con varias tiras. Cada tira es un array con $2n + 2$ vértices ($2n$ triángulos más los $2$ vértice adicionales para cerrar el primer y último triángulo), el cuál cada uno ocupa $12$ bytes. Por tanto, la malla ocupará un total de
$$12 \cdot (2n + 2) \cdot m \; \text{ bytes }$$

Adicionalmente, la tabla de tiras almacena el número de tiras y un puntero por cada tira, dicha estructura ocupará un total de 
$$4 + 8 \cdot m \; \text{ bytes  }$$
Luego, concluimos que nos harán falta
$$12 \cdot (2n + 2) \cdot m + 4 + 8 \cdot m \; \text{ bytes }$$
Si consideramos $m = n = 128$, la estructura en su conjunto nos ocupará unos $388$ KB. Por otro lado, si consideramos que $m$ y $n$ son ambos muy grandes
$$\frac{Tamaño_{TT}}{Tamaño_{MI}} = \frac{12\cdot (2n +2) \cdot m + 4 + 8\cdot m}{12 \cdot (2 \cdot n \cdot m)} = \frac{3\cdot (2n +2) \cdot m + 1 + 2\cdot m}{6 \cdot (n \cdot m)} \overset{n \to \infty}{\longrightarrow} \frac{1}{2}$$
Finalmente, estudiamos los tiempos de procesamiento de vértices para ambas estructuras
$$\frac{Procesamiento_{TT}}{Procesamiento_{MI}} = \frac{Vertices_{TT} \cdot 1}{Vertices_{MI} \cdot 1} = \frac{(2n + 2)m}{(n+1)(m+1)} \overset{n \to \infty}{\longrightarrow} 2$$
## Ejercicio 4

Supongamos una malla cerrada, simplemente conexa (topológicamente equivalente a una esfera), cuyas caras son
triángulos y cuyas aristas son todas adyacentes a exactamente dos caras (la malla es un poliedro simplemente conexo de caras triangulares). Considera el número de vértices $n_V$, el número de aristas $n_A$ y el número de caras $n_C$ en este tipo de mallas.

Demuestra que cualquiera de esos números determina a los otros dos, en concreto, demuestra que se cumplen estas dos igualdades:
$$\begin{align}n_A & = 3 (n_V − 2) \\ n_C & = 2 (n_V − 2)\end{align}$$

Nótese que, al igual que en el problema anterior, sigue siendo cierto que el número de caras es aproximadamente el doble que el de vértices.

### Solución

Dado que toda arista es adyacente a exactamente $2$ caras y cada cara está formada por $3$ aristas, tenemos que
$$2 \cdot n_A = 3 \cdot n_C \; \Longrightarrow \; n_C = \frac{2}{3} \cdot n_A$$
Por tanto, haciendo uso de la **característica de Euler** tenemos que
$$\chi(\mathbb{S}^2) = n_V - n_A + n_C = 2 \; \Longrightarrow \; \begin{cases} n_A = n_V + n_C - 2 = n_V + \frac{2}{3} \cdot n_A - 2 \; & \Longrightarrow \; n_A = 3(n_V - 2) \\
n_C = 2 + n_A - n_V = 2 + \frac{3}{2} n_C - n_V \; & \Longrightarrow \; n_C = 2(n_V - 2)\end{cases}$$
## Ejercicio 5

En una malla indexada, queremos añadir a la estructura de datos una tabla de aristas. Será un vector `edges`, que en cada entrada tendrá una tupla de tipo `uvec2`  con los índices en la tabla de vértices de los dos vértices en los extremos de la arista. El orden en el que aparecen los vértices en una arista es indiferente, pero cada arista debe aparecer una sola vez.

Escribe el código de una función C++ para crear y calcular la tabla de aristas a partir de la tabla de triángulos. Intenta encontrar una solución con la mínima complejidad en tiempo y memoria posible.

Suponer que el número de vértices adyacentes a uno cualquiera de ellos es como mucho un valor constante $k > 0$, valor que no depende del número total de vértices, que llamamos $n$.

Considerar dos casos:
1. Los triángulos se dan con orientación no coherente, es decir, si un triángulo está formado por los vértices $i$, $j$, $k$, estos tres índices pueden aparecer en cualquier orden en la correspondiente entrada de la tabla de triángulos (puede aparece como $i$, $j$, $k$ o como $i$, $k$, $j$, o como $k$, $j$, $i$, etc.)
2. Los triángulos se dan con orientación coherente, es decir, si dos triángulos comparten una arista entre los vértices $i$ y $j$, entonces en uno de los triángulos la arista aparece como $(i, j)$ y en el otro aparece como $(j, i)$ (decimos que en el triángulo $a$, $b$, $c$ aparecen las tres aristas $(a, b)$, $(b, c)$ y $(c, a)$). Además, asumimos que la malla es cerrada, es decir, que cada arista es compartida por exactamente dos triángulos.

### Orientación no coherente

Observamos que sin tener en cuenta la eficiencia (tiempo y memoria) una posible solución puede ser tan sencilla como

```c++
vector<uvec2> extractEdges(const vector<uvec3> & triangles, const int num_vertices)
{
	vector<uvec2> edges;
	for (const auto & triangle : triangles)
	{
		edges.push_back({triangle[0], triangle[1]});
		edges.push_back({triangle[1], triangle[2]});
		edges.push_back({triangle[2], triangle[3]});
	}
}
```

Sin embargo, podemos notar que esta solución inserta dos veces toda arista adyacente en nuestra tabla. Una posible solución para ello sería construir la matriz de adyacencia de los vertices, es decir, para cada vértice $i$ llevaremos un registro de los vértices que son adyacentes a él. Así obtendríamos nuestra tabla de aristas considerando todas las parejas de vértices adyacentes

```c++
vector<uvec2> extractEdges(const vector<uvec3> & triangles, const int num_vertices)
{
	vector<uvec2> edges;
	vector<vector<int>> adjacent_vertices(num_vertices, vector<int>());
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			int a = triangle[i][j];
			int b = triangle[i][(j+1) % 3];
			if (a > b)
			{
				swap(a, b);
			}
			bool found = false;
			for (size_t k = 0; k < adjacent_vertices[a].size() && !found; ++k)
			{
				if (adjacent_vertices[a][k] == b)
				{
					found = true;
				}
			}
			if (!found)
			{
				adjacent_vertices[a].push_back(b);
			}
		}
	}
	for (size_t i = 0; i < num_vertices; ++i)
	{
		for (size_t j = 0; j < adjacent_vertices[i].size(); ++j)
		{
			edges.push_back({i, adjacent_vertices[i][j]});
		}
	}
	return edges;
}
```

### Orientación coherente

Este caso es mucho más sencillo, ya que al ser una malla cerrada podemos considerar únicamente las entradas en las que $i < j$, puesto que al haber siempre dos triángulos con la misma arista así conseguiremos añadirla una úncica vez.

```c++
vector<uvec2> extractEdges(const vector<uvec3> & triangles, const int num_vertices)
{
	vector<uvec2> edges;
	vector<vector<int>> adjacent_vertices(num_vertices, vector<int>());
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			int a = triangle[i][j];
			int b = triangle[i][(j+1) % 3];
			if (a > b)
			{
				edges.push_back({a, b});
			}
		}
	}
	return edges;
}
```

## Ejercicio 6

Escribe el pseudo-código de la función para calcular el área total de una malla indexada de triángulos, a partir de la tabla de vértices y de triángulos. Será una función que acepta un puntero a una `MallaInd` y devuelve un número real (asumir que se dispone del tipo `vec3` y de los operadores usuales de tuplas o vectores, es decir suma ($+$), resta $−$, producto escalar $\cdot$, producto vectorial $\times$, etc . ).
### Solución

```c++
double getArea(const MallaInd * mesh)
{
	double area = 0.0f;
	vector<vec3> vertices = mesh->vertices;
	for (const auto & triangle : mesh->triangles)
	{
		vec3 a = vertices[triangle[1]] - vertices[triangle[0]];
		vec3 b = vertices[triangle[2]] - vertices[triangle[0]];
		area += length(cross(a, b))/2.0f;
	}
	return area;
}
```

## Ejercicio 7

Demuestra que $\vec{u}$ y $P(\vec{u})$ son siempre perpendiculares según la definición anterior.
### Solución

Sea $R = [\hat{x}, \hat{y}, \dot{o}]$ un sistema de referencia cartesiano entonces consideramos $\vec{u} = (a, b)_{R}$ y $P(\vec{u}) = (-b, a)_{R}$ las coordenadas de ambos vectores en dicho sistema de referencia. Entonces es claro observar que
$$\vec{u} \cdot P(\vec{u}) = a \cdot (-b) + b \cdot a = 0$$
Por tanto dichos vectores son perpendiculares entre sí.

## Ejercicio 8

Describe como se podría definir una rotación hacia la derecha (en el sentido de las agujas del reloj) en lugar de a izquierdas.
### Solución

Sea $R = [\hat{x}, \hat{y}, \dot{o}]$ un sistema de referencia cartesiano entonces consideramos $\vec{u} = (a, b)_{R}$ . Así, una rotación hacia la derecha de $\vec{u}$ tendría como coordenadas
$$P(\vec{u}) = (b, -a)_{R}$$
## Ejercicio 9

Demuestra que la transformación afín $P$ (cuando se aplica a vectores, no a puntos) no depende del marco cartesiano $C$ con respecto al cual expresamos las coordenadas $(a, b)$ (en el caso de aplicarla a puntos, la rotación de $90º$ es entorno al punto origen $\dot{o}$ de $C$).
### Solución

Sea $R = [\hat{x}, \hat{y}, \dot{o}]$ un sistema de referencia cartesiano entonces considerando $\vec{u} = (a, b)_{R}$, podemos expresar la transformación afín $P$ que va del sistema de referencia $R$ en el sistema de referencia $R$ mediante la siguiente matriz
$$P(\vec{u}) = \begin{pmatrix} 0 & -1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} a \\ b \end{pmatrix} = \begin{pmatrix} -b \\ a \end{pmatrix}$$
Así, la transformación $P$ no depende del sistema de referencia cartesiano que escojamos.

## Ejercicio 10

Demuestra que el producto escalar de vectores en 2D es invariante por rotación, es decir, que para cualquier ángulo $\theta$ y vectores $\vec{a}$ y $\vec{b}$ se cumple:
$$R_{\theta}(\vec{a}) \cdot R_{\theta}(\vec{b}) = \vec{a} \cdot \vec{b}$$
### Solución

Sea $R = [\hat{x}, \hat{y}, \dot{o}]$ un sistema de referencia cartesiano, $P = \mathcal{M}(R_{\theta} \; ; \; R \rightarrow R)$ la matriz asociada a la rotación de ángulo $\theta$ y $(a_1, a_2)_{R}, \; (b_1, b_2)_{R}$, las coordenadas de los vectores $\vec{a}$ y $\vec{b}$, respectivamente, en el sistema de referencia $R$. Así

$$R_{\theta}(\vec{a}) \cdot R_{\theta}(\vec{b}) = (P \begin{pmatrix} a_1 \\ a_2 \end{pmatrix})^t \cdot P\begin{pmatrix} b_1 \\ b_2 \end{pmatrix} = \begin{pmatrix} a_1 & a_2 \end{pmatrix} P^t P \begin{pmatrix} b_1 \\ b_2 \end{pmatrix} = \begin{pmatrix} a_1 & a_2 \end{pmatrix} \begin{pmatrix} b_1 \\ b_2 \end{pmatrix} = \vec{a} \cdot \vec{b}$$
donde hemos hecho uso de que la rotación de ángulo $\theta$ es una isometría, entonces $P$ es una matriz ortogonal ($P^t = P^{-1}$).

## Ejercicio 11

Demuestra que en 2D las rotaciones no modifican la longitud de un vector, es decir, que para cualquier ángulo $\theta$ y vector $\vec{v}$, se cumple:
$$\|R_{\theta}(\vec{v})\| = \|\vec{v}\|$$
### Solución

Para demostrar que las rotaciones de ángulo $\theta$ son una isometría veamos la matriz asociada a la aplicación es ortogonal, es decir, $P^t = P^{-1}$. Es sencillo observar que

$$P P^t = \begin{pmatrix} \cos \theta & - \sin \theta \\ \sin \theta & \cos \theta \end{pmatrix} \begin{pmatrix} \cos \theta & \sin \theta \\ - \sin \theta & \cos \theta \end{pmatrix} = \begin{pmatrix} cos^2 \theta + \sin^2 \theta & 0 \\ 0 & \cos^2 \theta + \sin^2 \theta \end{pmatrix} = I $$

## Ejercicio 12

Demuestra que el producto escalar de vectores en 3D es invariante por rotaciones elementales.
### Solución

Análogo al ejercicio 10

## Ejercicio 13

Demuestra que las rotaciones elementales en 3D no modifican la longitud de un vector.
### Solución

Análogo al ejercicio 11

## Ejercicio 14

Demuestra que el producto vectorial de dos vectores rota igual que lo hacen esos dos vectores, es decir, que para cualquiera dos vectores $\vec{a}$ y $\vec{b}$ y un ángulo $\theta$, se cumple:
$$R_{\theta}(\vec{a} \times \vec{b}) = R_{\theta}(\vec{a}) \times R_{\theta}(\vec{b})$$
### Solución

Consideramos $\vec{a} = (a_1, a_2, a_3)$ y $\vec{b} = (b_1, b_2, b_3)$ de tal forma
$$R_\theta(\vec{a}) \times R_\theta(\vec{b}) = \begin{pmatrix} \cos \theta & - \sin \theta & 0 \\ \sin \theta & \cos \theta & 0 \\ 0 & 0 & 1\end{pmatrix} \begin{pmatrix} a_1 \\ a_2 \\ a_3 \end{pmatrix} \times \begin{pmatrix} \cos \theta & - \sin \theta & 0 \\ \sin \theta & \cos \theta & 0 \\ 0 & 0 & 1\end{pmatrix} \begin{pmatrix} b_1 \\ b_2 \\ b_3 \end{pmatrix} = \begin{pmatrix} a_1\cos \theta - a_2\sin \theta \\ a_1 \sin \theta + a_2 \cos \theta \\ a_3 \end{pmatrix} \times \begin{pmatrix} b_1\cos \theta - b_2\sin \theta \\ b_1 \sin \theta + b_2 \cos \theta \\ b_3 \end{pmatrix}$$
$$R_\theta(\vec{a} \times \vec{b}) = \begin{pmatrix} \cos \theta & - \sin \theta & 0 \\ \sin \theta & \cos \theta & 0 \\ 0 & 0 & 1\end{pmatrix} \begin{pmatrix} a_2 b_3 - a_3 b_2 \\ a_3 b_1 - a_1 b_3\\ a_1 b_2 - a_2 b_1 \end{pmatrix}$$
## Ejercicio 15

Escribe una función llamada gancho para dibujar con OpenGL en modo diferido la polilínea de la figura (cada segmento recto tiene longitud unidad, y el extremo inferior está en el origen).

La función debe ser neutra respecto de la matriz modelview, el color o el grosor de la línea, es decir, usará la matriz modelview, el color y grosor del estado de OpenGL en el momento de la llamada (y no los cambia).
### Solución

```c++
void DrawHook()
{
	assert(glGetError() == GL_NO_ERROR);
	if (hook == nullptr)
	{
		const std::vector<glm::vec2> vertices =
		{
		{0.0, 0.0}, {0.25, 0.0}, {0.25, 0.25}, {0.0, 0.25}, {0.0, 0.5}
		};
		hook = new VertexArrayObject(1, new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
	}
	assert(glGetError() == GL_NO_ERROR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(cauce->ind_atrib_colores, 0.0, 0.0, 1.0);
	hook->draw(GL_LINE_STRIP);
	assert(glGetError() == GL_NO_ERROR);
}
```

## Ejercicio 16

Usando (exclusivamente) la función `gancho` del problema anterior, construye otra función (`gancho_x4`) para dibujar con OpenGL, usando el cauce fijo, el polígono que aparece en la figura:
### Solución

```c++
void DrawHook4()
{
	assert(glGetError() == GL_NO_ERROR);
	cauce->resetMM();
	DrawHook();
	cauce->compMM(glm::translate(glm::vec3{0.0f, 0.5f, 0.0f}));
	cauce->compMM(glm::rotate(glm::radians(90.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
	DrawHook();
	cauce->resetMM();
	cauce->compMM(glm::translate(glm::vec3{-0.5f, 0.5f, 0.0f}));
	cauce->compMM(glm::rotate(glm::radians(180.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
	DrawHook();
	cauce->resetMM();
	cauce->compMM(glm::translate(glm::vec3{-0.5f, 0.0f, 0.0f}));
	cauce->compMM(glm::rotate(glm::radians(270.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
	DrawHook();
	assert(glGetError() == GL_NO_ERROR);
}
```

## Ejercicio 17

Escribe el pseudocódigo OpenGL otra función (`gancho_2p`) para dibujar esa misma figura, pero escalada y rotada de forma que sus extremos coincidan con dos puntos arbitrarios disintos $\dot{p}_0$ y $\dot{p}_1$ , puntos cuyas coordenadas de mundo son $p_0 = (x_0, y_0 , 1)$ y $p_1 = (x_1, y_1, 1)$ . Estas coordenadas se pasan como parámetros a dicha función (como `vec3`)

Escribe una solución:
- Acumulando matrices de rotación, traslación y escalado en la matriz modelview de OpenGL. 
- En la cual la matriz modelview se calcula directamente sin necesidad de usar funciones trigonometricas (como lo son el arcotangente, el seno, coseno, arcoseno o arcocoseno).
### Solución

En primer lugar, podemos observar que la altura de la figura estará determinada por la distancia entre $\dot{p}_0$ y $\dot{p}_1$. Además, habrá que rotarla en función del ángulo que determine el vector $\vec{p_0p_1}$ con respecto al eje $\vec{x}$.

```c++
void DrawHook4(const glm::vec2 p_0, const glm::vec2 p_1)
{
	const glm::vec2 v = p_1 - p_0;
	float angle = glm::acos(glm::dot(glm::vec2{1.0f, 0.0f}, v) / glm::length(v));
	if (v.y < 0)
	{
		angle = -angle;
	}
	cauce->resetMM();
	cauce->compMM(glm::translate(glm::vec3{p_0, 0.0f}));
	cauce->compMM(glm::rotate(angle, glm::vec3{0.0f, 0.0f, 1.0f}));
	cauce->compMM(glm::scale(glm::vec3{glm::length(v)/2.0f, glm::length(v)/2.0f, 1.0f}));
	DrawHook4();
}
```

## Ejercicio 18

Usa la función del problema anterior para construir estas dos nuevas figuras, en las cuales hay un número variable de instancias de la figura original, dispuestas en circulo.
### Solución

```c++

```

## Ejercicio 19

Dados los dos siguientes grafos de escena sencillos:

![](./resources/img97.png)

Construye los grafos tipo PHIGS equivalentes más sencillos posible (en el sentido de menos nodos posibles).

### Solución







## Ejercicio 20

Escribe una función llamada `FiguraSimple` que dibuje con OpenGL en modo diferido la figura que aparece aquí (un cuadrado de lado unidad, relleno de color, con la esquina inferior izquierda en el origen, con un triángulo inscrito, relleno del color de fondo).
### Solución

```c++
void DrawSimpleFigure()
{
	VertexArrayObject *square = nullptr;
	VertexArrayObject *triangle = nullptr;
	const std::vector<glm::vec2> vertices_square =
	{
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.5}
	};
	const std::vector<glm::uvec3> indices_square =
	{
		{0, 1, 2}, {0, 2, 3}
	};
	const std::vector<glm::vec2> vertices_triangle =
	{
		{0.1, 0.1}, {0.25, 0.1}, {0.175, 0.25}
	};
	const std::vector<glm::uvec3> indices_triangle =
	{
		{0, 1, 2}
	};
	square = new VertexArrayObject(1, 
							new AttribBufferObject(cauce->ind_atrib_posiciones, vertices_square));
	square->add(new IndexBufferObject(indices_square));
	triangle = new VertexArrayObject(1, 
							new AttribBufferObject(cauce->ind_atrib_posiciones, vertices_triangle));
	triangle->add(new IndexBufferObject(indices_triangle));
	
	assert(glGetError() == GL_NO_ERROR);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cauce->usePlainColor(true);
	cauce->setColor({0.0, 0.0, 1.0});
	square->draw(GL_TRIANGLES);
	
	cauce->setColor({1.0, 1.0, 1.0});
	triangle->draw(GL_TRIANGLES);
}
```

## Ejercicio 21

Usando exclusivamente llamadas a la función del ejercicio anterior construye otra función llamada `FiguraCompleja` que dibuja la figura de aquí. Para lograrlo puedes usar manipulación de la pila de la matriz modelview (`pushMM` y `popMM`), junto con `glm::translate` y `glm::rotate`:

![](./resources/img135.png)

### Solución

```c++
void DrawComplexFigure()
{
	cauce->resetMM();
	cauce->pushMM();
	cauce->compMM(glm::scale(glm::vec3{2.0f, -1.0f, 1.0f}));
	DrawSimpleFigure();
	cauce->popMM();
	cauce->pushMM();
	cauce->compMM(glm::scale(glm::vec3{sqrt(2.0), sqrt(2.0), 1.0f}));
	cauce->compMM(glm::rotate(static_cast<float>(3*M_PI/4.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
	DrawSimpleFigure();
	cauce->popMM();
	cauce->pushMM();
	cauce->compMM(glm::translate(glm::vec3{-1.5f, -0.5f, 0.0f}));
	DrawSimpleFigure();
	cauce->popMM();
}
```

## Ejercicio 22

Escribe el código OpenGL de una función (llamada Tronco) que dibuje la figura que aparece a aquí. El código dibujará el polígono relleno de color azul claro, y las aristas que aparecen de color azul oscuro (ten en cuenta que no todas las aristas del polígono relleno aparecen).

![](./resources/img134.png)
### Solución

```c++
void DrawTrunk()
{
	const std::vector<glm::vec2> vertices =
	{
		{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}, 
		{1.5, 2.5}, {0.5, 1.5}, {0.0, 3.0}, {-0.5, 3}, {0.0, 1.5}
	};
	const std::vector<glm::uvec3> indices =
	{
		{0, 1, 2}, {2, 3, 4}, {2, 4, 5}, {2, 5, 8}, {5, 6, 8}, {6, 7, 8}, {0, 2, 8}
	};
	const std::vector<glm::uvec2> edges =
	{
		{1, 2}, {2, 3}, {4, 5}, {5, 6}, {7, 8}, {8, 0}
	};
	if (trunk == nullptr)
	{
		trunk = new VertexArrayObject(1, 
					new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
		trunk->add(new IndexBufferObject(indices));
	}
	if (trunk_edges == nullptr)
	{
		trunk_edges = new VertexArrayObject(1, 
						new AttribBufferObject(cauce->ind_atrib_posiciones, vertices));
		trunk_edges->add(new IndexBufferObject(GL_UNSIGNED_INT, 2*edges.size(), edges.data()));
	}
	cauce->resetMM();
	cauce->compMM(glm::translate(glm::vec3{-0.5f, -0.5f, 0.0f}));
	cauce->compMM(glm::scale(glm::vec3{0.25f, 0.25f, 1.0f}));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cauce->usePlainColor(true);
	cauce->setColor({0.0, 0.0, 1.0});
	trunk->draw(GL_TRIANGLES);
	cauce->setColor({0.0, 0.0, 0.0});
	trunk_edges->draw(GL_LINES);
}
```
## Ejercicio 23

Escribe una función `Arbol` la cual, mediante múltiples llamadas a `Tronco` del ejercicio anterior, dibuje el árbol que aparece en la figura de abajo. Diseña el código usando recursividad, de forma que el número de niveles sea un parámetro modificable en dicho código

![](./resources/img136.png)
### Solución

```c++
void DrawTree(unsigned int depth)
{
	assert(glGetError() == GL_NO_ERROR);
	assert(0 < depth);
	DrawTrunk();
	if (depth > 1)
	{
		cauce->pushMM();
		cauce->compMM(glm::translate(glm::vec3{-0.5f, 3.0f, 0.0f}));
		cauce->compMM(glm::scale(glm::vec3{0.5f, 0.5f, 1.0f}));
		DrawTree(depth - 1);
		cauce->popMM();
		cauce->pushMM();
		cauce->compMM(glm::translate(glm::vec3{1.5f, 2.5f, 0.0f}));
		cauce->compMM(glm::rotate(glm::radians(-45.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
		cauce->compMM(glm::scale(glm::vec3{0.707f, 0.707f, 1.0f}));
		DrawTree(depth - 1);
		cauce->popMM();
	}
}
```
## Ejercicio 24

Supón que dispones de dos funciones para dibujar dos mallas u objetos simples: `Semiesfera` y `Cilindro`. La semiesfera (en coordenadas maestras) tiene radio unidad, centro en el origen y el eje vertical en el eje $Y$. Igualmente el cilindro tiene radio y altura unidad, el centro de la base está en el origen, y su eje es el eje $Y$.

![](./resources/img137.png)

Con estas dos primitivas queremos escribir el código que visualiza la figura `Android`, usando la plantilla de código de prácticas. Para ello:
- Diseña el grafo de escena (tipo PHIGS) correspondiente, ten en cuenta que hay objetos compuestos que se pueden instanciar más de una vez (cada brazo o pierna se puede construir con un objeto compuesto de dos semiesferas en los extremos de un cilindro).
- Escribe el código OpenGL para visualizarlo, usando una clase llamada `Android`, derivada de `NodoGrafoEscena`.
### Solución

```c++
ExtremidadAndroid::ExtremidadAndroid()
{
	NodoGrafoEscena *semiesfera_superior = new NodoGrafoEscena();
	semiesfera_superior->agregar(glm::translate(glm::vec3(0.0, 3.0, 0.0)));
	semiesfera_superior->agregar(new SemiSphere(30, 30));
	
	NodoGrafoEscena *tronco = new NodoGrafoEscena();
	tronco->agregar(glm::scale(glm::vec3(1.0, 3.0, 1.0)));
	tronco->agregar(new Cilinder(30, 30));
	
	NodoGrafoEscena *semiesfera_inferior = new NodoGrafoEscena();
	semiesfera_inferior->agregar(glm::rotate(glm::pi<float>(), glm::vec3(0.0, 0.0, 1.0)));
	semiesfera_inferior->agregar(new SemiSphere(30, 30));
	
	agregar(semiesfera_superior);
	agregar(tronco);
	agregar(semiesfera_inferior);
	ponerColor({0.0f, 1.0f, 0.0f});
}

CabezaAndroid::CabezaAndroid()
{
	NodoGrafoEscena *ojo_izquierdo = new NodoGrafoEscena();
	ojo_izquierdo->agregar(glm::translate(glm::vec3(-0.35, 0.5, 0.75)));
	ojo_izquierdo->agregar(glm::scale(glm::vec3(0.1, 0.1, 0.1)));
	ojo_izquierdo->agregar(new Sphere(30, 30));
	ojo_izquierdo->ponerColor({0.0f, 0.0f, 0.0f});
	
	NodoGrafoEscena *ojo_derecho = new NodoGrafoEscena();
	ojo_derecho->agregar(glm::translate(glm::vec3(0.35, 0.5, 0.75)));
	ojo_derecho->agregar(glm::scale(glm::vec3(0.1, 0.1, 0.1)));
	ojo_derecho->agregar(new Sphere(30, 30));
	ojo_derecho->ponerColor({0.0f, 0.0f, 0.0f});
	
	NodoGrafoEscena *cabeza = new NodoGrafoEscena();
	cabeza->agregar(new SemiSphere(30, 30));
	cabeza->ponerColor({0.0f, 1.0f, 0.0f});
	
	NodoGrafoEscena *antena_derecha = new NodoGrafoEscena();
	antena_derecha->agregar(glm::translate(glm::vec3(0.35, 0.95, 0.0)));
	antena_derecha->agregar(glm::rotate(glm::pi<float>()/-6.0f, glm::vec3(0.0, 0.0, 1.0)));
	antena_derecha->agregar(glm::scale(glm::vec3(0.05, 0.5, 0.05)));
	antena_derecha->agregar(new Cilinder(30, 30));
	antena_derecha->ponerColor({0.0f, 1.0f, 0.0f});
	
	NodoGrafoEscena *antena_izquierda = new NodoGrafoEscena();
	antena_izquierda->agregar(glm::translate(glm::vec3(-0.35, 0.95, 0.0)));
	antena_izquierda->agregar(glm::rotate(glm::pi<float>()/6.0f, glm::vec3(0.0, 0.0, 1.0)));
	antena_izquierda->agregar(glm::scale(glm::vec3(0.05, 0.5, 0.05)));
	antena_izquierda->agregar(new Cilinder(30, 30));
	antena_izquierda->ponerColor({0.0f, 1.0f, 0.0f});
	
	agregar(ojo_izquierdo);
	agregar(ojo_derecho);
	agregar(cabeza);
	agregar(antena_derecha);
	agregar(antena_izquierda);
}

Android::Android()
{
	NodoGrafoEscena *brazo_izquierdo = new NodoGrafoEscena();
	brazo_izquierdo->agregar(glm::translate(glm::vec3(-1.45, 0.5, 0.0)));
	brazo_izquierdo->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
	brazo_izquierdo->agregar(new ExtremidadAndroid());
	
	NodoGrafoEscena *brazo_derecho = new NodoGrafoEscena();
	brazo_derecho->agregar(glm::translate(glm::vec3(1.45, 0.5, 0.0)));
	brazo_derecho->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
	brazo_derecho->agregar(new ExtremidadAndroid());
	
	NodoGrafoEscena *tronco = new NodoGrafoEscena();
	tronco->agregar(glm::scale(glm::vec3(1.0, 2.0, 1.0)));
	tronco->ponerColor({0.0, 1.0, 0.0});
	tronco->agregar(new Cilinder(30, 30));
	
	NodoGrafoEscena *cabeza = new NodoGrafoEscena();
	cabeza->agregar(glm::translate(glm::vec3(0.0, 2.1, 0.0)));
	cabeza->ponerColor({0.0, 1.0, 0.0});
	cabeza->agregar(new CabezaAndroid());
	
	NodoGrafoEscena *pierna_izquierda = new NodoGrafoEscena();
	pierna_izquierda->agregar(glm::translate(glm::vec3(-0.5, -1.0, 0.0)));
	pierna_izquierda->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
	pierna_izquierda->agregar(new ExtremidadAndroid());
	
	NodoGrafoEscena *pierna_derecha = new NodoGrafoEscena();
	pierna_derecha->agregar(glm::translate(glm::vec3(0.5, -1.0, 0.0)));
	pierna_derecha->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
	pierna_derecha->agregar(new ExtremidadAndroid());
	  
	agregar(brazo_izquierdo);
	agregar(brazo_derecho);
	agregar(tronco);
	agregar(cabeza);
	agregar(pierna_izquierda);
	agregar(pierna_derecha);
}
```
## Ejercicio 25

Escribe una segunda versión del grafo de escena del ejercicio anterior, de forma que las transformaciones estén parametrizadas por dos valores reales ($\alpha$ y $\beta$) que expresan el ángulo de rotación del brazo izquierdo y derecho (respectivamente), en torno al eje que pasa por los centros de las dos semiesferas superiores de los brazos.

Asimismo, habrá otro parámetro ($\gamma$) que es el ángulo de rotación de la cabeza (completa: con los ojos y antenas) entorno al eje vertical que pasa por su centro (cuando estos ángulo valen 0, el androide está en reposo y tiene exactamente la forma de la figura del ejercicio anterior).

Escribe el código de una nueva clase (`AndroidParam`, derivada de `NodoGrafoEscena`) para visualizar el androide parametrizado de esta forma.
### Solución

```c++
Android::Android(const float alpha, const float beta, const float gamma)
{
	NodoGrafoEscena *brazo_izquierdo = new NodoGrafoEscena();
	brazo_izquierdo->agregar(glm::translate(glm::vec3(-1.45, 1.0, 0.0)));
	unsigned int id_brazo_izq = brazo_izquierdo->agregar(
		glm::rotate(alpha, glm::vec3(1.0, 0.0, 0.0))
	);
	brazo_izquierdo->agregar(glm::scale(glm::vec3(-0.35, -0.35, 0.35)));
	brazo_izquierdo->agregar(new ExtremidadAndroid());
	
	rotacion_brazo_izquierdo = brazo_izquierdo->leerPtrMatriz(id_brazo_izq);
	
	NodoGrafoEscena *brazo_derecho = new NodoGrafoEscena();
	brazo_derecho->agregar(glm::translate(glm::vec3(1.45, 1.0, 0.0)));
	unsigned int id_brazo_drch = brazo_derecho->agregar(
		glm::rotate(beta, glm::vec3(1.0, 0.0, 0.0))
	);
	brazo_derecho->agregar(glm::scale(glm::vec3(-0.35, -0.35, 0.35)));
	brazo_derecho->agregar(new ExtremidadAndroid());
	
	rotacion_brazo_derecho = brazo_derecho->leerPtrMatriz(id_brazo_drch);
	. . .
	
	NodoGrafoEscena *cabeza = new NodoGrafoEscena();
	cabeza->agregar(glm::translate(glm::vec3(0.0, 2.1, 0.0)));
	unsigned id_cabeza = cabeza->agregar(glm::rotate(gamma, glm::vec3(0.0, 1.0, 0.0)));
	cabeza->ponerColor({0.0, 1.0, 0.0});
	cabeza->agregar(new CabezaAndroid());
	
	rotacion_cabeza = cabeza->leerPtrMatriz(id_cabeza);
	. . .
}

unsigned Android::leerNumParametros() const
{
	return 3;
}

void Android::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
	assert(iParam < leerNumParametros());
	
	float min = -1.0f, max = 1.0f;
	float a = (min + max)/2, b = (max - min)/2;
	float n = 0.25;
	
	switch (iParam)
	{
	case 0:
	*rotacion_brazo_izquierdo = glm::rotate(
		static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(1.0, 0.0, 0.0)
	);
	break;
	case 1:
	*rotacion_brazo_derecho = glm::rotate(
		static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(1.0, 0.0, 0.0)
	);
	break;
	case 2:
	*rotacion_cabeza = glm::rotate(
		static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(0.0, 1.0, 0.0)
	);
	break;
	}
}
```

# Tema 3

## Ejercicio 1

Supongamos una escena que contiene una representación visible del marco de coordenadas del mundo como tres flechas (roja, verde y azul), como ocurre en las prácticas. Queremos visualizar esa escena en pantalla, de forma que:
1. El eje $Y$ aparezca vertical, hacia arriba, el eje $X$ horizontal, hacia la derecha, el eje $Z$ horizontal, hacia la izquierda (los ejes $X$ y $Z$ se visualizan con la misma longitud aparente).
2. El punto de coordenadas $(0, 0.5, 0)$ (aparece como un disco de color morado en la figura) debe aparecer en el centro del viewport
3. El observador (foco de la proyeccion) estará a 3 unidades de distancia del punto $(0, 0.5, 0)$

![](./resources/img100.png)

Escribe unos valores que podríamos usar para $a$, $u$ y $n$ de forma que se cumplan estos requisitos. En la figura se observa una vista esquemática de como quedaría la figura en un viewport cuadrado, no necesariamente a escala.
### Solución

En primer lugar, ya que nos dicen que el punto $(0, 0.5, 0)$ se encuentra en el centro del viewport deducimos que ese será el punto de atención, así
$$\dot{a} = (0, 0.5, 0)$$
Por otro lado, para calcular el vector normal al plano de visión calcularemos previamente el foco de la proyección que sabemos que se encuentra a $3$ unidades de distancia del punto $(0, 0.5, 0)$, así podemos tomar
$$\dot{o}_{ec} = \left(\sqrt{3}, \frac{1}{2} + \sqrt{3}, \sqrt{3} \right)$$
Por tanto, 
$$\vec{n} = \dot{a} - \dot{o}_{ec} = \left( -\sqrt{3}, -\sqrt{3}, -\sqrt{3} \right)$$
Finalmente, para el _view-up vector_ podemos tomar
$$\vec{u} = (0, 1, 0)$$
ya que sabemos que el eje $Y$ aparece vertical.

## Ejercicio 2

Repite el problema anterior, pero ahora para esta vista:

![](./resources/img101.png)
### Solución

De nuevo tenemos que nuestro punto de atención será
$$\dot{a} = (0, 0.5, 0)$$
Por otro lado,  el vector normal al plano de visión debe encontrarse en sentido contrario al que le otorgamos en el ejercicio anterior, de forma que el eje $X$ apareza hacia la derecha y el eje $Z$ hacia la izquierda. Así
$$\vec{n} = (\sqrt{3}, -\sqrt{3}, \sqrt{3})$$
Finalmente, para el _view-up vector_, que debe ser perpendicular al plano que forman los ejes $X$ y $Z$ (observamos que este plano está rotado $\pi/4$ en sentido antihorario) podemos tomar
$$u = (-\frac{\sqrt{2}}{2}, \frac{\sqrt{2}}{2}, 0)$$
## Ejercicio 3

Escribe el código para calcular los vectores de coordenadas $x_{ec}$, $y_{ec}$, $z_{ec}$ y $o_{ec}$ que definen el marco de vista a partir de los vectores de coordenadas $a$, $u$ y $n$ (todos estos vectores de coordenadas son de tipo `vec3`).
### Solución

```c++
mat4 lookAT(const vec3 & a, const vec3 & u, const vec3 & n)
{
	vec3 oec = a - n;
	vec3 zec = n/normalize(n);
	vec3 xec = cross(u, n)/normalize(cross(u, n));
	vec3 yec = cross(zec, xec);
	return mat4{xec, yec, zec, -oec};
}
```

## Ejercicio 4

Partiendo de los vectores de coordenadas $x_{ec}$, $y_{ec}$, $z_{ec}$ y $o_{ec}$ que se calculan en el problema anterior, escribe el código que calcula explicitamente las $16$ entradas de la matriz de vista (crea una `glm::mat4` llamada $V$ y luego asigna valor a $V(i, j)$ para cada fila $i$ y columna $j$, ambas entre $0$ y $3$).
### Solución

```c++
glm::mat4::mat4(const glm::vec3 &xec, const glm::vec3 &yec, const glm::vec3 &zec, const glm::vec3 &oec)
{
	glm::mat4 out = mat4(1.0);
	for (unsigned i = 0; i < 3; ++i)
	{
		out[i][0] = xec[i];
	}
	for (unsigned i = 0; i < 3; ++i)
	{
		out[i][1] = yec[i];
	}
	for (unsigned i = 0; i < 3; ++i)
	{
		out[i][2] = zec[i];
	out[0][3] = -glm::dot(xec, oec);
	out[1][3] = -glm::dot(yec, oec);
	out[2][3] = -glm::dot(zec, oec);
	return out;
}
```

## Ejercicio 5

Queremos visualizar una escena con mallas indexadas de la cual sabemos que tiene todos los vértices dentro de un cubo de lado $s$ unidades cuyo centro es el punto de coordenadas del mundo
$$c = (c_x, c_y, c_z)$$
Para construir la matriz de vista, se situa el observador en el punto
$$o_{ec} = (c_x , c_y , c_z + s + 2)$$
El punto de atención $a$ se hace igual a $c$ (el centro del cubo se ve en el centro de la imagen), y el vector $u$ es $(0, 1, 0)$. Se visualizará en un viewport cuadrado.

Queremos construir la matriz de proyeccion perspectiva $Q$ de forma que se cumplan estos requerimientos:
1. No se recorta ningún triángulo.
2. El tamaño aparente de los objetos (proyectados en pantalla) es el mayor posible.
3. El valor del parámetro $n$ es el mayor posible.
4. El valor del parámetro $f$ es el menor posible.
5. Los objetos no aparecen deformados.
Con estos requerimientos, indica como calcular los valores $l$, $r$, $t$, $b$, $n$ y $f$ (para obtener la matriz $Q$ de proyección), en función de $s$ y $(c_x, c_y, c_z)$.
### Solución

Con los requerimientos que nos imponen deducimos que se trata de una proyección ortográfica. Además, considerando que
$$\|o_{ec} - c\| = s + 2$$
ya que buscamos que el valor de $n$ sea el mayor posible y el de $f$ el menor posible, esto se verifica cuando 
$$- n = c_z + \frac{s}{2} \hspace{2cm} -f = c_z - \frac{s}{2}$$
Por otro lado
$$l = c_x - \frac{s}{2} \hspace{1cm} r = c_x + \frac{s}{2} \hspace{1cm} t = c_y + \frac{s}{2} \hspace{1cm} b = c_y - \frac{s}{2}$$
## Ejercicio 6

Repite el ejercicio anterior, con exactamente los mismos requerimientos y suposiciones, excepto que ahora la escena, en lugar de estar contenida en un cubo de lado $s$ unidades, está contenida en una esfera de radio $s$ unidades (con centro igualmente en $c$).
### Solución

Análogamente buscamos inscribir la esfera en un cubo de dimensiones
$$-n = c_z + s \hspace{1cm} -f = c_z - s \hspace{1cm} l = c_x - s \hspace{1cm} r = c_x + s \hspace{1cm} t = c_y + s \hspace{1cm} b = c_y - s$$
## Ejercicio 7

Repite el problema anterior, con exactamente los mismos requerimientos y suposiciones, excepto que ahora, en lugar de suponer que el viewport es cuadrado, sabemos que tiene $w$ columnas de pixels y $h$ filas de pixels, y no podemos suponer que $w = h$.
### Solución

La solución es muy similar a la del ejercicio anterior pero ahora debemos tener en cuenta el _aspect-ratio_ del viewport, para ello tomamos
$$-n = c_z + s \hspace{1cm} -f = c_z - s \hspace{1cm} l = c_x - \frac{w}{h}s \hspace{1cm} r = c_x + \frac{w}{h}s \hspace{1cm} t = c_y + \frac{h}{w}s \hspace{1cm} b = c_y - \frac{h}{w}s$$

## Ejercicio 8

Repite el ejercicio 5, con exactamente los mismos requerimientos y suposiciones, excepto que ahora se nos da un ángulo $\beta$ en grados que debe ser la apertura de campo vertical de la proyección perspectiva. Para ello, ahora tenemos libertad para situar al observador en la línea paralela al eje $Z$ que pasa por $c$, de forma que la apertura de campo vertical sea exactamente $\beta$.

Indica como calcular la coordenada $Z$ que debemos usar ahora para $o_{ec}$ (la $X$ y la $Y$ son las mismas que antes), de forma que se cumpla lo dicho, también indica como debemos de calcular ahora los valores de $l$, $r$, $t$, $b$, $n$ y $f$ (todo ello en función de $\beta$, $s$ y $c = (c_x , c_y , c_z )$).
### Solución

Sabemos que la apertura vertical de $\beta$ está contenido entre $0$ y $180º$, de modo que como tenemos que $o_{ec}$ está en la recta paralela al eje $Z$ que pasa por $c$ tenemos que
$$\tan\left(\frac{\beta}{2}\right) = \frac{\frac{s}{\sqrt{2}}}{(c_z - c_z')}$$
El resto de los parámetros es igual, pero los factores que dependan de $c_z$ hay que expresarlos en fucnión de $\beta$.
## Ejercicio 9

Suponemos que visualizamos una esfera de radio unidad centrada en el origen. Se ilumina con una fuente de luz puntual situada en el punto $p = (0, 2, 0)$. El observador está situado en $o = (2, 0, 0)$. En estas condiciones:
- Describe razonadamente en que punto de la superficie de la esfera el brillo será máximo si el material es puramente difuso ($k_d = 1$ en todos los puntos, y $k_a = k_s = 0$) ¿es ese punto visible para el observador?
- Repite el razonamiento anterior asumiendo ahora que el material es puramente pseudo-especular ($MS = (1, 1, 1)$, resto a cero) ¿es ese punto visible para el observador?
### Solución

Sabemos que en un material puramente difuso, la densidad de fotones que inciden en el entorno de $p$ es proporcional al valor de $\cos(\alpha)$ donde $\alpha$ es el ángulo que forman los vectores $n_p$ y $l_i$. Por tanto, el brillo será máximo cuando $\cos(\alpha) = 1$ lo que equivale a que $n_p \cdot l_i = 0$, esto es $n_p = l_i$, que ocurre exactamente en el punto $p' = (0, 1, 0)$, el cual no es visible para el observador.

Por otro lado, si consideramos un material puramente pseudo-especular el brillo será máximo cuando el vector $r_i$ apunte hacia el observador ($r_i = v$), esto es, cuando el ángulo $\beta = 0$ y $\cos \beta = 1$. Esto ocurre exactamente en el punto $p' = (\frac{\sqrt{2}}{2}, \frac{\sqrt{2}}{2}, 0)$, luego
$$\vec{l}_i = p - p' = (0, 2, 0) - \left(\frac{\sqrt{2}}{2}, \frac{\sqrt{2}}{2}, 0\right) = \left(-\frac{\sqrt{2}}{2}, 2 - \frac{\sqrt{2}}{2}, 0\right)$$
$$\vec{v} = o - p' = (2, 0, 0) - \left(\frac{\sqrt{2}}{2}, \frac{\sqrt{2}}{2}, 0\right) = \left(2 - \frac{\sqrt{2}}{2}, -\frac{\sqrt{2}}{2}, 0\right)$$
y dicho punto si es visble para el observador.
## Ejercicio 10

Supongamos que se desea crear una malla indexada para un cubo, de forma que deseamos aplicar un textura que incluya las caras de un dado. Para ello disponemos de una imagen de textura que tiene una relación de aspecto $4:3$. La imagen aparece aquí:

![](./resources/img102.png)

Responde a estas cuestiones:
- Describe razonadamente cuantos vértices (como mínimo) tendrá el modelo.
- Escribe la tabla de coordenadas de vértices, la tabla de coordenadas de textura, y la tabla de triángulos. Ten en cuenta que el cubo tiene lado unidad y su centro está en $(1/2, 1/2, 1/2)$. Dibuja un esquema de la textura en la cual cada vértice del modelo aparezca etiquetado con su número de vértice más sus coordenadas de textura.
### Solución

Dado que cada vértice del cubo será adyacente a tres caras necesitamos como mínimo $3 \cdot 8 = 24$ vértices. Procedemos a escribir la tabla de coordenadas de vértices para dicho cubo de lado unidad centrado en $(1/2, 1/2, 1/2)$.

```c++
std::vector<glm::vec3> vertices = 
{
	// Cara frontal
	{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
	// Cara lateral derecha
	{1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1},
	// Cara trasera
	{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
	// Cara lateral izquierda
	{0, 0, 1}, {0, 0, 0}, {0, 1, 0}, {0, 1, 1},
	// Tapa superior
	{0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0}
	// Tapa inferior
	{0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 0}
}
```

Seguimos con la tabla de triangulos

```c++
std::vector<glm::uvec3> triangles =
{
	// Cara frontal
	{0, 1, 2}, {2, 3, 0},
	// Cara lateral derecha
	{4, 5, 6}, {6, 7, 4},
	// Cara trasera
	{8, 9, 10}, {10, 11, 8},
	// Cara lateral izquierda
	{12, 13, 14}, {14, 15, 12},
	// Tapa superior
	{16, 17, 18}, {18, 19, 16},
	// Tapa inferior
	{20, 21, 22}, {22, 23, 20}
}
```

Por último definimos la tabla de coordenadas de textura

```c++
std::vector<glm::vec2> texture_coordinates =
{
	// Cara frontal
	{0, 2/3}, {1/4, 2/3}, {1/4, 1/3},
	// Cara lateral derecha
	{1/4, 2/3}, {2/4, 2/3}, {2/4, 1/3}, {1/4, 1/3},
	// Cara trasera
	{2/4, 2/3}, {3/4, 2/3}, {3/4, 1/3}, {2/4, 1/3},
	// Cara lateral izquierda
	{3/4, 2/3}, {1, 2/3}, {1, 1/3}, {3/4, 1/3},
	// Tapa superior
	{2/3, 1/3}, {3/4, 1/3}, {3/4, 0}, {2/4, 0},
	// Tapa inferior
	{2/4, 1}, {3/4, 1}, {3/4, 2/3}, {2/4, 2/3}
}
```

## Ejercicio 11

Considera de nuevo el cubo y la textura del problema anterior. Ahora supón que queremos visualizar con OpenGL el cubo usando sombreado de Gouroud o de Phong, para lo cual debemos de asignar normales a los vértices. Responde a estas cuestiones:
- Describe razonadamente si sería posible usar la misma tabla de vértices y la misma tabla de coordenadas de textura que has escrito en en el problema anterior, o es necesario usar otra tabla distinta.
- Si has respondido que no es posible usar las mismas tablas, escribe la nueva tabla de vértices, la nueva tabla de coordenadas de textura. Asimismo, escribe como sería la tabla de normales.
### Solución

Si sería posible usar la mista tabla de vértices y de coordenadas de textura ya que al haber triplicado los vértices podemos asignar las normales de forma correcta y no se verían deformadas:

```c++
std::vector<glm::vec3> nor_vert = 
{
	// Cara frontal
	{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
	// Cara lateral derecha
	{1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
	// Cara trasera
	{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
	// Cara lateral izquierda
	{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
	// Tapa superior
	{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
	// Tapa inferior
	{0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}
}
```

## Ejercicio 12

Considera un cubo de lado unidad, y con centro en $(1/2, 1/2, 1/2)$ que se quiere visualizar con una textura a partir de una única imagen (cuadrada) que se replicará en las $6$ caras de dicho cubo. Asume que no se va a usar iluminación (no es necesario calcular la tabla de normales). Escribe ahora la tabla de coordenadas de vértices y la tabla de coordenadas de textura.

### Solución

Creo que podemos hacer uso de la misma que en el ejercicio 10

# Tema 4

## Ejercicio 1

Supongamos que queremos visualizar una secuencia de frames, en los cuales la cámara va cambiando. Para ellos queremos escribir el código de una función que fija la matriz de vista en el cauce. 

La función acepta como parámetro un valor real $t$, que es el tiempo en segundos transcurrido desde el inicio de la animación. Suponemos que la animación dura $s$ segundos en total.

En ese tiempo el observador de cámara se desplaza con un movimiento uniforme desde un punto de coordenadas de mundo $O_0$ (para $t = 0$) hasta un punto destino $O_1$ (para $t = 1$). Además el punto de atención de la cámara también se desplaza desde $a_0$ hasta $a_1$. Durante toda la animación, el vector $VUP$ es $(0, 1, 0)$. Escribe el pseudo-código de la citada función.
### Solución

```c++
glm::mat4 shiftCamera(const float t, const float s)
{
	assert(t < s);
	const vec3 ot = (1 - t/s) * initial_o + t/s * final_o;
	const vec3 at = (1 - t/s) * initial_a + t/s * final_a;
	
	const new_n = ot - at;

	return lookAt(ot, at, {0.0, 1.0, 0.0});
}
```

## Ejercicio 2

Una posibilidad para hacer selección en mallas de triángulos es usar cálculo de intersecciones entre un rayo (una semirrecta que pasa por el centro de un pixel) y cada uno de los triángulos de la malla.

Diseña un algoritmo en pseudo-código para el cálculo de intersecciones entre un rayo y un triángulo:
- El rayo tiene como origen o extremo el punto cuyas coordenadas del mundo es la tupla $o$, y como vector de dirección la tupla $d$ (la suponemos normalizada).
- Las coordenadas del mundo de los vértices del triángulo son $v_0$, $v_1$ y $v_2$.
- El algoritmo debe de indicar si hay intersección o no, y, en caso de que la haya, calcular las coordenadas del mundo del punto de intersección.

Ten en cuenta que habrá intersección si y solo si se cumplen cada una de estas dos condiciones:
1. El rayo interseca con el plano que contiene al triángulo, es decir, existe $t > 0$ tal que el punto $p_t = o + td$ está en dicho plano. Equivale a decir que el vector $p_t − v_0$ es perpendicular a la normal al plano.
2. El punto $p_t$ citado arriba está dentro del triángulo. Es decir, hay dos valores reales no negativos $a$ y $b$ (con $0 \leq a + b \leq 1$) tales que el vector $p_t − v_0$ es igual a $a(v_1 − v_0) + b(v_2 − v_0$).

 > A los tres valores $a$, $b$ y $c = 1 − b − a$ se les llama coordenadas baricéntricas de $p_t$ en el triángulo, se usan en Ray-Tracing.

### Solución

En primer lugar, observamos que **no** habrá intersección con el plano que contiene al triángulo si el vector de dirección $\vec{d}$ es ortogonal al vector normal al plano $\vec{n}_\Pi$, esto es, si
$$\vec{d} \cdot \vec{n}_\Pi = 0$$
Por otro lado, sí que habrá intersección con dicho plano si
$$p_t = p + t \vec{d} \in \Pi = v_0 + L \{ (v_1 - v_0), (v_2 - v_0)\}$$
Buscamos calcular las coordenadas de $p_t$ con respecto al plano $\Pi$, es decir, buscamos $a, \, b \in \mathbb{R}$ tal que 
$$p_t - v_0 = a(v_1 - v_0) + b(v_2 - v_0)$$
Es decir
$$\begin{align} p_t^1 - v_0^1 = & a(v_1^1 - v_0^1) + b(v_2^1 - v_0^1) \\ p_t^2 - v_0^2 = & a(v_1^2 - v_0^2) + b(v_2^2 - v_0^2) \end{align}$$
Luego
$$a = \frac{p_t¹ - v_0^1 - b(v_2^1 - v_0^1)}{v_1^1 - v_0^1} \; \Longrightarrow \; b = \frac{(p_t^2 - v_0^2)(v_1^1 - v_0^1) - (v_1^2 - v_0^2)(p_t^1 - v_0^1)}{(v_2^2 - v_0^2)(v_1^1 - v_0^1) - (v_2^1 - v_0^1)(v_1^1 - v_0^1)}$$

Además, para que $p_t$ pertenezca al triángulo tendremos que poner la condición adicional de que $0 \leq a + b \leq 1$. Por tanto, $t > 0$ debe verificar
$$(p_t - v_0) \cdot \vec{n}_\Pi = 0 \; \iff \; (o + t\vec{d} - v_0) \cdot \vec{n}_\Pi = 0 \; \iff \; (o^1 + t d^1 - v_0^1, o^2 + t d^2 - v_0^2, o^3 + t d^3 - v_0^3) \cdot (n_\pi^1, n_\pi^2, n_\pi^3) = 0 \; \iff$$
$$\iff \; (o^1 + t d^1 - v_0^1)n_\pi^1 + (o^2 + td^2 - v_0^2)n_\pi^2 + (o^3 + td^3 - v_0^3)n_\pi^3 = 0 \; \iff \; t = \frac{(v_0^1 - o^1)n_\pi^1 + (v_0^2 - o^2)n_\pi^2 + (v_0^3 - o^3)n_\pi^3}{n_\pi^1 d^1 + n_\pi^2d^2 + n_\pi^3d^3}$$


```c++
bool intersect(const vec3 o, const vec3 d, const vec3 v0, const vec3 v1, const vec3 v2)
{
	const vec3 u = v1 - v0;
	const vec3 v = v2 - v0;
	const vec3 n = normalize(cross(u, v));
	if (dot(d, n) == 0)
		return false;
	
	double t = dot(v0 - o, n)/dot(d, n);
	if (t < 0)
		return false;
	
	vec3 pt = o + t*d;
	double b = . . .
	double a = . . .
	if (a < 0 or b < 0 or a + b < 0 or a + b > 1)
		return false;
	
	return true;
}
```

## Ejercicio 3

Para implementar la selección usando intersecciones es necesario calcular el rayo que tiene como origen el observador y pasa por centro del pixel donde se ha hecho click. Escribe el pseudo-código del algoritmo que calcula el rayo a partir de las coordenadas del pixel donde se ha hecho click:
- Tenemos una vista perspectiva, y conocemos los $6$ valores $l$, $r$, $t$, $b$, $n$, $f$ usados para construir la matriz de proyección.
- También conocemos el marco de coordenadas de vista, es decir, las tuplas $x_{ec}$ , $y_{ec}$ y $z_{ec}$ con los versores y la tupla $\dot{o}_{ec}$ con el punto origen (todos en coordenadas del mundo).
- El viewport tiene $w$ columnas y $f$ filas de pixels. Se ha hecho click en el pixel de coordenadas enteras $x_p$ e $y_p$.

El algoritmo debe producir como salida las tuplas $o$ y $d$ (normalizado) que definen el rayo.
### Solución

En primer lugar, calcularemos el centro del píxel en el que se ha hecho click, cuyas coordenadas son $(x_p, y_p)$. Ya que el viewport tiene $w$ columnas y $f$ filas cada pixel ocupa 
$$\Delta_x = \frac{r - l}{w} \hspace{1cm} \Delta_y = \frac{t - b}{f}$$

Por tanto las coordenadas del centro del pixel son
$$p_x = l + x_p \cdot \frac{\Delta_x}{2} \hspace{1cm} p_y = b + y_p \cdot \frac{\Delta_y}{2} \hspace{1cm} p_z = -n$$

```c++
pair<vec3, vec3> rayCasting(vec3 o, vector<vec3> axis, vector<float> projection_parameters, int x_p, int y_p)
{
	const vec3 pixel_center = {l + x_p * delta_x/2, b + y_p * delta_y/2};
	const vec3 d = pixel_center - o;
	return {o, normalize(d)};
}
```

# Tema 5

## Ejercicio 1

Supongamos que un rayo (una semirecta en 3D) tiene como origen $o$ extremo el punto cuyas coordenadas del mundo es la tupla $(o_x, o_y, o_z)$, y como vector de dirección la tupla $(d_x, d_y, d_z)$ (la suponemos normalizada).

Además sabemos que un disco de radio $r$ tiene como centro el punto de coordenadas de mundo $c = (c_x, c_y, c_z)$ y está en el plano perpendicular al vector $n = (n_x, n_y, n_z)$

Con estos datos de entrada, diseña un algoritmo para calcular si hay intersección entre el rayo y el disco.

Ten en cuenta que habrá intersección si y solo si se cumplen cada una de estas dos condiciones:
1. El rayo interseca con el plano que contiene al disco, es decir, existe $t > 0$ tal que el punto $p_t = o + td$ está en dicho plano. Equivale a decir que el vector $p_t − c$ es perpendicular a la normal al plano $n$.
2. El punto $p_t$ citado arriba está dentro del disco, es decir, su distancia a $c$ es inferior al radio.
### Solución

El rayo interseca el plano que contiene al disco si y sólo si existe $t > 0$ tal que
$$\begin{align} \langle p_t - c, n \rangle = & \langle o + td - c, n \rangle = \begin{pmatrix} o_x + td_x - c_x & o_y + td_y - c_y & o_z + td_z - c_z\end{pmatrix} \cdot \begin{pmatrix} n_x \\ n_y \\ n_z \end{pmatrix} = \\ = & (o_x + td_x - c_x)\cdot n_x + (o_y + td_y - c_y)\cdot n_y + (o_z + td_z - c_z)\cdot n_z = 0\end{align}$$
Por tanto
$$t = \frac{(c_x - o_x) \cdot n_x + (c_y - o_y) \cdot n_y + (c_z - o_z) \cdot n_z}{d_x \cdot n_x + d_y \cdot n_y + d_z \cdot n_z} = \frac{\langle c - o, n \rangle}{\langle d, n \rangle}$$
Además, el punto $p_t$ debe estar dentro del disco, por tanto $\|p_t - c\| \leq r$.

```c++
bool intersectDisc(const vec3 o, const vec3 d, const vec3 c, const float r)
{
	const float t = dot(c - o, n)/dot(d, n);
	if (t < 0)
	{
		return false; // La intersección está detras del origen del rayo
	}
	const vec3 p_t = o + t*d;
	// Si el punto de intersección se encuentra dentro del disco hay intersección con él
	return length(p_t - c) <= r ? true : false;
}
```
## Ejercicio 2

Diseña un algoritmo para calcular la primera intersección entre un rayo (con oriden en $o$ y vector $d$, normalizado) y una esfera de radio unidad y centro en el origen, si hay alguna.

Ten en cuenta que un punto cualquiera $p$ está en esfera si y solo sí el módulo de $p$ es la unidad, es decir, si y solo si $F(p) = 0$, donde $F$ es el campo escalar definido así:
$$F(p) = p \cdot p − 1$$

Describe como podría usarse ese mismo algoritmo para calcular la intersección con una esfera con centro y radio arbitrarios (este problema puede reducirse al anterior si el rayo se traslada a un espacio de coordenadas donde la esfera tiene centro en el origen y radio unidad).
### Solución

Procedemos mediante un razonamiento análogo al utilizado en el ejercicio anterior, en este caso el rayo interseca la esfera si y sólo si existe $t > 0$ tal que
$$\|p_t\| = 1 \iff \|p_t\|^2 = \langle p_t, p_t \rangle = 1 \iff \langle o + td, o + td \rangle = \begin{pmatrix} o_x + td_x & o_y + td_y & o_z + td_z\end{pmatrix} \cdot \begin{pmatrix} o_x + td_x \\ o_y + td_y \\ o_z + td_z\end{pmatrix} = 1$$
Desarrollamos el producto escalar
$$(o_x + td_x)^2 + (o_y + td_y)^2 + (o_z + td_z)^2 = 1 \; \iff \; (o_x^2 + t^2 d_x^2 + 2t o_x d_x) + (o_y^2 + t^2 d_y^2 + 2t o_y d_y) + (o_z^2 + t^2 d_z^2 + 2t o_z d_z) = 1$$
Agrupamos los términos en función de $t$
$$(d_x^2 + d_y^2 + d_z^2)t^2 + 2(o_x d_x + o_y d_y + o_z d_z) t + (o_x^2 + o_y^2 + o_z^2) = 1 \; \iff \; \|d\|^2t^2 + 2\langle o, d \rangle + \|o\|^2 - 1 = 0$$
Despejando $t$ llegamos a que
$$t = \frac{-\langle o, d \rangle \pm \sqrt{\langle o, d \rangle^2 - 4\|d\|^2 (\|o\|^2 - 1)}}{2\|d\|^2}$$
Generalizando los cálculos llegamos a

```c++
bool intersectSphere(const vec3 o, const vec3 d, const vec3 c, const float r, vec3 & p_t)
{
	const float delta = pow(dot(o - c, d), 2) - 4*pow(length(d), 2)*(pow(length(o - c), 2) - r^2);
	if (delta < 0)
	{
		return false; // No hay intersección en los reales
	}
	const float t_1 = (-dot(o - c, d) + sqrt(delta))/(2*pow(length(d), 2));
	const float t_2 = (-dot(o - c, d) - sqrt(delta))/(2*pow(length(d), 2));
	if (t_1 > 0)
	{
		p_t = o + t_1*d;
	}
	else if (t_2 > 0)
	{
		p_t = o + t_2*d;
	}
	else
	{
		return false;
	}
	return true;
}
```

## Ejercicio 3

Describe como podemos definir el campo escalar cuyos ceros son los puntos en un cilindro con altura unidad y radio unidad (sin considerar los discos que forman la base ni la tapa). Usando esa definición diseña el algoritmo para calcular la intersección rayo-cilindro.

Describe asimismo el campo escalar y el algoritmo correspondientes a un cono de altura unidad y radio de la base unidad (sin considerar el disco de la base).
### Solución

En primer lugar, podemos expresar el cilindro con altura y radio unidad mediante el siguiente conjunto
$$\{ (x, y, z) \in \mathbb{R}^2 \colon x^2 + z^2 = 1, \; 0 \leq y \leq 1 \}$$
Por tanto, habrá intersección con dicho cilindro si existe $t > 0$ tal que
$$ (o_x + td_x)^2 + (o_z + td_z)^2 = 1 \; \iff \; (d_x^2 + d_z^2)t^2 + (o_xd_x + o_zd_z)t + (o_x^2 + o_z)^2 = 1$$
Despejando $t$ llegamos a que
$$t = \frac{- \langle o.xz, d.xz \rangle \pm \sqrt{\langle o.xz, d.xz \rangle^2 - 4\|d.xz\|^2(\|o.xz\|^2 - 1)}}{2\|d.xz\|^2}$$

```c++
bool intersectCilinder(const vec3 o, const vec3 d, vec3 & p_t)
{
	const float delta = pow(dot(o.xz, d.xz), 2) - 4*dot(d.xz, d.xz)*(dot(o.xz, o.xz) - 1);
	if (delta < 0)
	{
		return false; // No hay intersección en los reales
	}
	const float t_1 = (-dot(o.xz, d.xz) + sqrt(delta))/(2*dot(d.xz, d.xz));
	const float t_2 = = (-dot(o.xz, d.xz) - sqrt(delta))/(2*dot(d.xz, d.xz));
	if (t_1 > 0)
	{
		p_t = o + t_1*d;
		return 0 <= p_t.y && p_t.y <= 1 ? true : false; 
	}
	else if (t_2 > 0)
	{
		p_t = o + t_2*d;
		return 0 <= p_t.y && p_t.y <= 1 ? true : false;
	}
	else
	{
		return false;
	}
}
```

Análogamente, podemos expresar el cono de altura y radio unidad mediante el conjunto
$$\{ (x, y, z) \in \mathbb{R}^3 \colon x^2 - y^2 + z^2 = 0, \; 0 \leq y \leq 1 \}$$
Por tanto, habrá intersección con dicho cilindro si existe $t > 0$ tal que
$$(o_x + td_x)^2 - (o_y + td_y)^2 + (o_z + td_z)^2 = 0 \; \iff \; (d_x^2 - d_y^2+ d_z^2)t^2 + (o_xd_x - o_y d_y+ o_zd_z)t + (o_x^2 - o_y^2+ o_z)^2 = 0$$
Despejando $t$ llegamos a que
$$t = \frac{- (o_xd_x - o_yd_y + o_zd_z) \pm \sqrt{(o_xd_x - o_yd_y + o_zd_z)^2 - 4(d_x^2 - d_y^2 + d_z^2)(o_x^2 - o_y^2 + o_z^2)}}{2(d_x^2 - d_y^2 + d_z^2)}$$

```c++
bool intersectCone(const vec3 o, const vec3 d, vec3 & p_t)
{
	const float delta = pow(o.x*d.x -o.y*d.y + o.z*d.z, 2) 
			- 4*(pow(d.x, 2) - pow(d.y, 2) + pow(d.z, 2)*(pow(o.x, 2) - pow(o.y, 2) + pow(o.z, 2)));
	if (delta < 0)
	{
		return false; // No hay intersección en los reales
	}
	const float t_1 = (-(o.x*d.x - o.y*d_y + o.z*d.z) + sqrt(delta))/(2*(pow(d.x, 2) - pow(d.y, 2) + pow(d.z, 2)));
	const float t_2 = = (-o.x*d.x - o.y*d_y + o.z*d.z) - sqrt(delta))/(2*(pow(d.x, 2) - pow(d.y, 2) + pow(d.z, 2)));
	if (t_1 > 0)
	{
		p_t = o + t_1*d;
		return 0 <= p_t.y && p_t.y <= 1 ? true : false; 
	}
	else if (t_2 > 0)
	{
		p_t = o + t_2*d;
		return 0 <= p_t.y && p_t.y <= 1 ? true : false;
	}
	else
	{
		return false;
	}
}
```

