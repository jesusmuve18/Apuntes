# Configuración de LVM y RAID
1. En virtualbox:
   1. En `Configuración -> Almacenamiento` de nuestra máquina virtual seleccionamos la opción "Crear disco" (con todo por defecto) y elegimos los discos `RockyLVM_1"` y el 2.
2. En la máquina virtual:
   1. Instalamos la instrucción `mdadm` con `dnf provides mdadm` y `dnf install mdadm`.
   2. Ejecutamos `mdadm --create /dev/sd0 --level 1 --raid-devices=2 /dev/sdb /dev/sdc` y nos va a preguntar `Continue creating array` y aceptamos para crear el raid.
   3. Ejecutamos `watch -n 1 cat /proc/mdstat` y podemos ver cómo se crea el raid.
   4. Si ahora ejecutamos `lsbkl` podremos ver que aparece `sd0` dentro de `sdb` y `sdc`. 
   <!--Si ahora probamos (NO PROBAR) a hacer `echo 1 > /dev/sdb` corromperíamos el raid y probablemente fallaría en siguientes operaciones.-->
   <!--5. En `tail -f /var/log` podemos programar alertas automáticas para que nos notifique -->
   5. Al ejecutar `df -h` podemos ver el tamaño disponible en disco (disk free).
   6. Al ejecutar `du -k *` podemos ver el uso del disco (disk usage).
   7. Para crear un fisical volume podemos escribir `pv` y tabular y nos saldrán todas las instrucciones disponibles. Ejecutamos `pvcreate /dev/sd0`.
   8. Al ejecutar `lvdisplay` podremos ver los discos lógicos.
3. En la maquina virtual:
   1. `vgcreate raid1 /dev/md0`. 
   2. `lvcreate -L 10G -n rvar raid1`. Hacemos `lvdisplay` para ver si se ha creado bien. Podremos ver la estructura y tendrá que aparecer bajo `sd0` el disco `raid1-rvar`. 
   3. Podremos acceder desde `/dev/raid1/rvar` o desde `/dev/mapper/raid1-rvar`.
   4. Con `mkfs` (make filesystem) crearemos un sistema de ficheros.
   5. Vamos a formatear el nuevo disco con el formato `ext4` mediante `mkfs.ext4 /dev/mapper/raid1-rvar`.
   6. Para conmutar a modo mantenimiento tenemos que ejecutar `systemctl isolate runlevel1.target`.
   7. Para hacer permanente un montaje tenemos que hacer `vi /etc/fstab` y tendremos que añadir la siguiente entrada al final:
      ```   
      /dev/raid1/var    /var     ext4     defaults    0 0
      ```
   8. Antes de reiniciar podemos hacer `mount -a` para ver si hay fallos en el documento anteriormente modificado.