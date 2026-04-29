- Significa Redundance Array of Independent/Inexpensive Disks.
- Importante que los buses que se colocan entre discos sean paralelos, porque si no el rendimiento se degrada.
# Tipos
- Solo nos interesa conocer tres niveles
	- **Nivel 0 (Striping)** -> Discos en paralelo. Si ponemos tres de 1 GB, simulamos uno de 3 GB (se reduce el coste).
		- Si se rompe un disco, se pierde la información de todos, porque un mismo archivo se reparte entre varios de ellos.
	- **Nivel 1 (Mirror)** -> Se toman n discos y el RAID te genera un disco cuyo tamaño es el mínimo de los n discos.
		- Se usa en elementos críticos del sistema.
		- Mucha robustez
	- **Nivel 5** -> Coge un grupo de discos y sacrifica el espacio de uno de ellos, almacenando en el mismo un código de redundancia cíclica. El tamaño es el mismo que el tamaño de todos los discos menos 1.
		- Si perdemos uno de los discos, la función matemática almacenada usa el contenido del resto de discos para recuperar la información perdida. Cuando esto ocurre las prestaciones bajan porque dicha función se usará cada vez que se accede al disco roto.
		- Cuando se rompe ese disco, los operarios lo cambian al momento.
# Implementaciones
- En producción (mundo real) la implementación software apenas se usa.
## Hardware 
- Controladora de RAID. Chip integrado en la placa madre o aparte que se encarga de implementar todas las funciones de RAID. Cada vez que se pide un sector este sistema establece la correspondencia entre el disco "simulado" y la estructura real del sistema RAID.
- Todo esto evita la sobrecarga de la CPU, pues se encarga de todo el proceso.
- Además, este proceso es transparente al sistema operativo.
## Software
- Implementado por un driver del Sistema Operativo.
- Consume ciclos de CPU, por lo que degrada las prestaciones del sistema.

- El directorio `var` es para contenidos variables. Toda la información que vaya a cambiar en un servidor tiene que ir en esta carpeta. Servidor HTTP -> las páginas web van a VAR.
	- Es común llevarse el directorio `var` aparte ([[LVM]]).
- Si hacemos `df -h` los dispositivos `tmpfs` se borrarán cuando se reinicie.
- Es común llevarse el directorio 
## Pasos para crear RAID
1. Añadir disco en VirtualBox. Configuración -> Almacenamiento -> añadir disco.
2. Crear con todos los parámetros por defecto (creamos 2 discos).
3. Comando que usamos para gestionar RAID por software `mdadm` (hay que instalarlo).
4. `dnf provides mdadm` -> `dnf install mdadm`.
5. `mdadm --create /dev/md0 --level 1 --raid-devices 2 /dev/sdb /dev/sdc`. Los dos ultimos dispositivos deben aparecer en la máquina al hacer `lsblk`.
6. Aceptamos el aviso que nos da.
7.  `watch -n 1 cat /proc/mdstat` (para ver que no se hace de forma instantánea).
8. Si hiciéramos `echo 1 > /dev/sda0` entonces escribimos en primer sector de ambos discos por ser RAID1.
9. Si hiciéramos `echo 1 > /dev/sdb` entonces escribimos en primer sector de un solo disco y rompe el RAID.
10. En algún sitio de `tail -f /var/log` para ver si un disco está corrompido.
