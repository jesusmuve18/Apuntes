# Instalación y configuración de un servidor básico Rocky Linux

1. Descargar la imagen de este [enlace](http://atcproyectos.ugr.es/esriie/Rocky-9.0-20220805.0-x86_64-minimal.iso)
2. En virtualbox:
   1. Creamos la máquina desde el menú Máquina -> Nueva
   2. La llamamos "Rocky base"
   3. Seleccionamos la imagen iso que acabamos de descargar
   4. Marcamos la opción "Omitir instalación desantendida"
   5. Dejamos todos los parámetros por defecto (2048 de RAM y 1 Procesador).
3. Abrimos la máquina:
   1. Le damos al enter para entrar en el menú de instalación
   2. Le damos una contraseña al usuario root
   3. Dejamos todo por defecto y le damos a instalar
   4. Esperamos a que termine de instalar
   5. Apagamos la máquina
4. En virtualbox:
   1. Abrimos la configuración de redes desde Archivo -> Herramientas -> Administrador de Red -> Redes solo-anfitrión
   2. Le damos a "Crear nueva"
   3. Dejamos el resto por defecto
   4. Lo guardamos y volvemos al menú principal
   5. En la configuración de la máquina virtual "Rocky base" entramos al menú "Configuración -> Red -> Adaptador 1" y nos aseguramos de que esté puesto en NAT y "cable conectado"
   6. Entramos a "Configuración -> Red -> Adaptador 2" y lo activamos. Le ponemos la opción conectado a en "Adaptador solo anfitrión" y seleccionamos la red que acabamos de crear.
   7. Lanzamos la máquina de nuevo.
5. Abrimos la máquina:
   1. Ejecutamos la instrucción "ip a" y nos aseguramos de que contamos con la interfaz "enp0s8"
   2. Creamos el archivo "/etc/sysconfig/network-scripts/ifcfg-enp0s8" que contendrá la siguiente información:
    ```javascript
    DEVICE=enp0s8
    BOOTPROTO=static
    ONBOOT=yes
    IPADDR=192.168.56.100 // la que aparece en el ipv4 de la red solo-anfitrión
    NETMASK=255.255.255.0
    ```
    3. Ejecutamos la instrucción "sudo systemctl restart NetworkManager" para aplicar la configuración. A veces es necesario reiniciar la máquina.
    4. Probamos a hacer ping al host con "ping 192.168.56.1" (o la dirección que proceda según la red configurada).
    5. Creamos un usuario con
   ```bash
    useradd  jmunoz #Inicial y primer apellido
    passwd jmunoz #para asignarle una contraseña
    usermod -aG wheel jmunoz #para asignarle privilegios sudo
   ```
   6. Entramos en el usuario con "su jmunoz"
   7. Editamos el archivo "~/.bashrc" y añadimos la siguiente línea al final:
   ```javascript
    PS1="[\u@jmvMV-\t \w] "
   ```
   8. Ya tendremos creado el usuario jmunoz con el prompt correspondiente (se cambiará "jmvMV-" por las siglas del usuario seguidas de MV- (Máquina Virtual -))
6. Desde el ordenador host:
   1. Abrimos una terminal y probamos a hacer ping al servidor con "ping 192.168.56.100" y comprobamos que funciona.
   2. Probamos a hacer ssh al servidor con "ssh jmunoz@192.168.56.100".

Si todo funciona correctamente ya se podrá apagar el servidor y habremos conseguido lo buscado.
