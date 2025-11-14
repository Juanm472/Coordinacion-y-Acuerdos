Guia de Ejecucion del Proyecto — Coordinacion y Acuerdo entre Nodos (C / Sockets TCP)

Este proyecto implementa un mecanismo basico de coordinacion y acuerdo entre nodos utilizando sockets TCP en C. La arquitectura consiste en un servidor coordinador y multiples clientes, cada uno representado por un proceso independiente que envia su propuesta (un ID numerico) y recibe la decision comun (el lider).

Requisitos:

Ubuntu o WSL con Linux

GCC instalado:
sudo apt install build-essential

Compilacion:
En la carpeta del proyecto, ejecutar:

gcc server.c -o server
gcc client.c -o client

Esto generara dos ejecutables: server y client.

Ejecucion:

Iniciar el servidor:
./server

El servidor queda escuchando en el puerto 5000 y espera la conexion de 3 clientes.

Ejecutar clientes:
En tres terminales diferentes (o pestañas), ejecutar:

./client <ID>

Ejemplo:
./client 1
./client 5
./client 3

Cada cliente:

Se conecta al servidor.

Envia su ID.

Espera la decision.

Recibe el valor del lider elegido.

Resultado Esperado:
Todos los clientes deben mostrar el mismo mensaje:

Cliente (ID=x): el lider elegido es: <LIDER>

Y en el servidor se vera:

Servidor: recibido ID ...
Servidor: el lider elegido es: ...

Descripcion breve:
El servidor actua como coordinador central:

Recibe las propuestas de los nodos.

Determina el lider mediante una regla simple (ID maximo).

Envia el resultado de vuelta a todos los clientes.

Esta ejecucion refleja un proceso basico de consenso en un sistema distribuido, implementado con comunicacion real por TCP, aun ejecutandose en una sola maquina.
