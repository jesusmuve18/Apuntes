extends Node

## -----------------------------------------------------------------------------
## Función que calcula las normales promedio de los vértices de una malla, 
## a partir de la tabla de posiciones de vértices y la tabla de triángulos

func calcNormales( verts : PackedVector3Array, 
				   tris  : PackedInt32Array ) -> PackedVector3Array :

	## Paso 1: comprobar precondiciones
	assert( verts.size() >= 3 , "CalcNormales: la malla debe tener al menos 3 vértices" )
	assert( tris.size() % 3 == 0 , "CalcNormales: el número de enteros en 'tris' debe ser múltiplo de 3")
	
	var nv : int = verts.size()  ## número de vértices
	var nt : int = tris.size()/3 ## número de triángulos
	
	# Paso 2: inicializa normales a cero
	var normales := PackedVector3Array([]) 
	for i in nv:
		normales.append( Vector3.ZERO )
	
	# Paso 3: sumar en cada vértice las normales de sus tris. adyacentes
	for it in nt :
		var t := Vector3i( tris[3*it+0], tris[3*it+1], tris[3*it+2] )
		var a := verts[t[0]] ; 
		var b := verts[t[1]] ; 
		var c := verts[t[2]] ; 
		var normalv := (c-a).cross(b-a).normalized()
		for iv in t :
			normales[iv] += normalv
	
	# Paso 4: normalizar normales
	for iv in nv:
		normales[iv] = normales[iv].normalized()
	
	# Hecho
	return normales

## -----------------------------------------------------------------------------
##
## Función de parametrización de un toroide o donut 
##
##    u = 1er parámetro (entre 0 y 1)
##    v = 2o parámetro (entre 0 y 1)
##    R  = radio mayor 
##    r  = radio menor

func ParamDonut( u,v, r, R : float ) -> Vector3:

		var cu := cos(2.0*PI*u); var su := sin(2.0*PI*u)
		var cv := cos(2.0*PI*v); var sv := sin(2.0*PI*v)
		return Vector3( (R+r*cv)*cu, (R+r*cv)*su, r*sv )

## -----------------------------------------------------------------------------
## 
## Genera una tabla de triángulos (indices) con topología toroidal
## los añade a 'indices', 
##
##    nu = numero de divisiones del 1er parámetro
##    nv = numero de divisiones del 2o parámetro	
##    indices = tabla donde se añaden los triángulos 

func GenTriToroidal( nu, nv : int , indices : PackedInt32Array )  :
	
	for i in nu: 
		var isig = (i + 1) % nu
		for j in nv:
			var jsig = (j + 1) % nv
			var i00 = i    * nv + j
			var i01 = i    * nv + jsig
			var i10 = isig * nv + j
			var i11 = isig * nv + jsig
			
			indices.append(i00); indices.append(i11); indices.append(i10)
			indices.append(i00); indices.append(i01); indices.append(i11)
					
## -----------------------------------------------------------------------------
## 
## Función que genera un toroide (Donut), con 'nu x nv' vértices 
## (como una malla indexada con vértices compartidos entre triángulos adyacentes)
##
##    vertices : tabla de vértices (añade)
##    indices : tabla de índices (añade)
##    nu = numero de divisiones del 1er parámetro
##    nv = numero de divisiones del 2o parámetro	
##    R  = radio mayor 
##    r  = radio menor

func generarDonut( vertices: PackedVector3Array, 
 					indices: PackedInt32Array, 
					nu: int = 128, nv: int = 32,   
					R: float = 1.2, r: float = 0.4):			
	
	## Genera vértices con la geometría de un donut
	for i in nu: 
		for j in nv:
			vertices.append( ParamDonut( float(i)/nu, float(j)/nv, r, R )) 

	## Genera los triángulos con topología toroidal (la de un donut)	
	GenTriToroidal( nu, nv, indices )
