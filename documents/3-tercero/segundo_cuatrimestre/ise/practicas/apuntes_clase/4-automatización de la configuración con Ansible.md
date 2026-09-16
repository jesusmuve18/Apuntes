# Automatización de la configuración con Ansible
Vamos a diferenciar 2 tipos:
- Nodos de control: Mandan órdenes hacia `n` servidores (es donde hay que instalar Ansible).
- Nodos manejados: Son los servidores a manejar.

## Instalación de Ansible
Se recomienda instalarlo en el host (si se tiene Linux).
```bash
sudo dnf install ansible-core.x86_64
```
Va a crear un directorio en `/etc/ansible` donde habrá tres archivos principales:
- `host`: es el inventario de Ansible (por defecto). En la práctica casi no se usa. Cada usuario deberá tener su propio archivo host en `/home/ansible/`
- `roles`: no entra en el contenido de la asignatura. Son componentes parametrizables que aceptan parámetros y aceptan hacer distintas operaciones en varios dispositivos.
- `ansible.cfg`: Guarda la configuración de Ansible. Al principio va a estar vacío pero se podrá crear con `` (lo pone en el archivo) y tiene varios parámetros (que no vamos a tocar en esta práctica):
  - `forks`: ansible permite lanzar varias hebras en paralelo. El máximo número de hebras se establece en este parámetro y por defecto está a `5`.

En el fichero `hosts` contiene todos los servidores que podemos administrar. Si una ip no está en el archivo de inventario no mandará la instrucción. Puede estar en formato `dirección.com` o como dirección ip. Se pueden establecer etiquetas (conjunto de direcciones) para hacer acciones sobre un grupo. Un mismo equipo puede estar bajo varias etiquetas. Podremos hacer acciones como crear un usuario en todos los dispositivos con una etiqueta.

Podemos establecer nombres simbólicos con entradas del tipo:
```
ansi00  ansible_host=192.168.56.101 ansible_user=david  ansible_port=22322
ansi01  ansible_host=192.168.56.111 ansible_user=admin  
ansi02  ansible_host=192.168.56.112 ansible_user=admin 
```
Para el primer ejercicio vamos a necesitar proporcionar el password (de forma abierta en el propio archivo).

## Ejecución por línea de comandos

Se pueden ejecutar comandos sueltos pero no es lo más común (se suelen ejecutar scripts). Para ello usaremos la siguiente sintáxis:
```bash
ansible ansi00 -m ping
ansible <host> -m <modulo>
```
El módulo `ansible.builtin` tiene los comandos que vienen instalados por defecto con ansible y que nos permitirán hacer las acciones más utilizadas. No son tan potentes como otros. La documentación es demasiado larga y se recomienda buscar específicamente lo que queremos hacer.

Por ejemplo, este módulo tiene `ping` pero no trabaja con ICMP (lo único que hace es sacar la versión de python). Un ejemplo de uso sería:
```bash
ansible ansi00 -m ping
```
Todos los módulos tienen parámetros (ya que son funciones). A `ping` se le puede pasar un parámetro `data` de la siguiente forma:
```bash
ansible ansi00 -m ping -a 'data="Hola mundo"'
```
Que simplemente devuelve dicho parámetro.

En el host se puede poner `all` y ejecutará el módulo en todos los hosts del inventario. De igual forma se le puede pasar una etiqueta y se le pasará a los equipos que están bajo el nombre de la etiqueta.

Un módulo muy utilizado es `shell`, con el que podremos ejecutar comandos de la shell. Un ejemplo de uso sería:
```bash
ansible managed -m shell -a 'find .'
```
No está muy recomendado abusar de los comandos de shell ya que va a depender de la versión del sistema de cada servidor. Es por ello más recomendado usar módulos de ansible siempre que se pueda (ya que están preparados para soportar esta compatibilidad) y solo en caso de que no exista ninguna otra opción (cosa rara) podremos optar por comandos de shell.

Los comandos privilegiados presentan un problema: para ejecutar dicho comando habría que ejecutarlo con `sudo` (como no es interactivo no se puede completar la contraseña). 

Es posible en `/etc/sudoers` permitir a un usuario ejecutar comandos privilegiados sin necesidad de usar la contraseña. Para ello deberemos añadir una entrada como la siguiente:
```
admin   ALL=(ALL)   NOPASSWD: ALL
```
Deberemos modificar este archivo con la función `visudo`.

Hay una opción para editarlo primero y luego convencer a `visudo` de que ha sido quien ha modificado el archivo (para recargar la configuración).

Para ejecutar un comando privilegiado lo deberemos de hacer con la opción `--become`. Por ejemplo:
```bash
ansible ansi00 -m 'firewall-cmd --list-all' --become
```

Una peculiaridad de los módulos de Ansible es que son idempotentes, es decir, cuando ejecutamos una orden siempre que se pasen los mismos argumentos se va a quedar en el mismo estado. Por ejemplo si creamos un usuario llamado `Ana` y ya existía dicho usuario, no hará nada y se quedará con el usuario que ya existía anteriormente. En caso de que no exista lo crea. Se parece al comportamiento usual al instalar paquetes (si no está instalado lo instala y en caso contrario no hace nada). Lo ideal es que al crear un script de ansible sea idempotente, es decir, que al ejecutarlo varias veces solo modifique la primera. Por ejemplo si queremos añadir una línea a un archivo tendremos que asegurarnos de que la segunda vez que se ejecute no añada la línea si ya existe.

Esto se usa para configurar todos los servidores a la vez. Por ejemplo, si se añade un nuevo servidor y se lanza un script de configuración se lanzará a todos los servidores del grupo (los antiguos y los nuevos) pero solo se quiere configurar los nuevos.

Debermos hacer un script con los comandos que querremos mandar a ansible (para facilitar la ejecución a la hora de la corrección).

Para probar el contenido del inventario podremos hacer
```bash
intro ansible-inventory -i ./hosts.yml --graph
```

Se recomienda darle nombre a todas las tareas (en las entradas de un archivo yaml).

Hay una opción para comprobar la sintaxis (pero no nos ha dicho cuál es) ( puede que sea `ansible-lint`).

Cada playbook será recomendable crearlo en archivos distintos (para la asignatura será obligatorio hacerlo de esta forma).

Para ver el resultado de un comando tendremos que guardar el resultado en una variable con `register: nombre_variable` y luego la podremos imprimir.

/etc/ssh: Cambiar a PermitRootLogin no

La forma más común de asignar llaves públicas es crear un directorio llamado `keys` donde creamos un archivo que se llama `juan.pub` que contiene la llave pública de juan (y así poder ser gestionada por el playbook)

