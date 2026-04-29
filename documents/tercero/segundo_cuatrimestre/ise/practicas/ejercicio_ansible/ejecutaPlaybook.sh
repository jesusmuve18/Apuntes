#!/bin/bash

# En primer lugar se debe de garantizar el acceso con el usuario root utilizando contraseña
# Para ello se deberá cambiar en el archivo '/etc/ssh/sshd_config' la opción 'PermitRootLogin' a 'yes'
# Una vez hecho esto se podrá continuar con la ejecución haciendo 'sudo systemctl restart sshd'

# instalo en local sshpass (sudo apt-get install sshpass)

# Apartado 1
# ansible-playbook -i hosts.yaml ./playbooks/individual/1-crear_admin.yaml

# Apartado 2
# ansible-playbook -i hosts.yaml ./playbooks/individual/2-llave_publica_admin.yaml

# Apartado 3
# ansible-playbook -i hosts.yaml ./playbooks/individual/3-crear_wheel.yaml

# Apartado 4
# ansible-playbook -i hosts.yaml ./playbooks/individual/4-añadir_usuarios.yaml

# Apartado 5
# ansible-playbook -i hosts.yaml ./playbooks/individual/5-deshabilitar_acceso_contraseña_root.yaml

# Última parte
# ansible-playbook -i hosts2.yaml ./playbooks/6-httpd_ejercicio_4-1-1.yaml
# ansible-playbook -i hosts2.yaml ./playbooks/6-nginx_ejercicio_4-1-1.yaml

# Archivos para entregar
ansible-playbook -i hosts.yaml ./playbooks/Configurar_usuarios.yaml
ansible-playbook -i hosts2.yaml ./playbooks/Configurar_servidores_web.yaml