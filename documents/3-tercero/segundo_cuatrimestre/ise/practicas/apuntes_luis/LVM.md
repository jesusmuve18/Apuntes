- Cuando un volumen crece mucho, puede ser conveniente aislarlo del resto del disco.
- Podríamos llevarnos var a una partición, pero esto retrasaría el problema si se llenara.
- LVM se crea para poder hacer crecer los volúmenes de forma dinámica.
- Por ejemplo, tengo una partición de 20 gb en un disco, y si crece a 40 , podríamos usar varios dispositivos de almacenamiento para dicho fin.
- Creamos un volumen lógico para el directorio `/var`, de tal forma que si un volumen ocupa x GB, puede tomar el almacenamiento de cualquiera de los discos sobre los que se crea el volumen lógico.
- La ventaja es que se puede hacer crecer dinámicamente sin mayores consecuencia (si no hay espacio, pincho un disco nuevo, y si lo hay, se puede hacer sin problema).
- En los volumen group no se suelen mezclar tecnologías de almacenamiento distintas.
## Usar [[RAID]] para crear LVM y llevar la partición /var
1. `pv <Tab> <Tab>` para ver lo que podemos hacer con los volúmenes.
2. `pvcreate /dev/md0`.
3. No lo vamos a meter en rl (ver con `pv`)
4. Creamos Volume Group `vgcreate raid1`.
5. Creamos logical volume `lvcreate -L 10G -n  rvar raid1`.
6. Tenemos sdb/sdc -> md0 -> raid1-rvar en el árbol de visualizacion de `lsblk`
7. Ese dispositivo estará en /dev/raid1/rvar o en /dev/mapper/raid1-rvar.
8. Debemos formatear y montar la particion /dev/raid1/rvar.
9. Para ver formatos de ficheros que permiet, hacemos `ls /lib/modules 5.14..../kernel/fs`.
Los sistemas transaccionales de archivos marcan los cambios como provisionales pero hasta que no se confirme la información no se escribe. Si se cae la red, entonces no se habrá corrompido el disco, y los cambios no se verán si no fueron confirmados.
10. `mkfs.ext4 /dev/mapper/raid1-rvar`.
11. `mount /`
12. `cp -a /var/* /mnt` no funciona. En var se escribe continuamente, para poder copiar hay que parar la ejecución del sistema para asegurar que no se escribe en var.
13. Conmutar a modo 1 el sistema operativo, que expulsa a todos los usuarios del sistema y sus procesos excepto el administrador. El nivel 3 es el nivel multiusuario con red y el nivel 5, que es el 3 + GUI. El modo 0 es el modo reset. Esto es personalizado de cada distribución de Linux.
14. Ahora conmutamos a modo 1.
15. `systemctl isolate rulelevel1.target`
16. `systemctl status` y comprobamos el state para ver el modo en que estamos.
17. `cp /var/* /mnt`.
18. Hacer copia de seguridad de /var `mv /var /var_old`
19. `unmount /mnt/`.
20. `mkdir /var`
21. `mount /dev/mapper/raid1-rvar /var`
22. `df --h` debe aparecer `/dev/mapper/raid1-rvar` mounted on `/var`.
23. Editamos `/etc/fstab` nombre de dispositivo, punto de montaje, sistema de ficheros, opciones, y los dos números son la prioridad de montaje de los dispositivos (si tienen misma prioridad se monta por el orden de aparición en el fichero).
24. Ponemos `/dev/raid1rvar /var ext4 defaults 0 0`. Hay que ponerlo después de que 
25. `mount -a ` Para montar todos los dispositivos.
26. Reiniciar el sistema.
27. 