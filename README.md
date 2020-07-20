# Multiplicación de matrices usando Pipes

Una tubería (pipe) consiste en una cadena de procesos conectados de forma tal que la salida de cada
elemento de la cadena es la entrada del próximo. Permiten la comunicación y sincronización entre
procesos. Es común el uso de buffer de datos entre elementos consecutivos.

Para crear la tubería se emplea la función pipe(), que abre dos descriptores de fichero y almacena su
valor en los dos enteros que contiene el array de descriptores de fichero. El primer descriptor de fichero
es abierto como O_RDONLY, es decir, sólo puede ser empleado para lecturas. El segundo se abre como
O_WRONLY, limitando su uso a la escritura.

La tubería “p” se hereda al hacer el fork() que da lugar al proceso hijo, pero es necesario que el padre
haga un close() de p[0] (el lado de lectura de la tubería), y el hijo haga un close() de p[1] (el lado de
escritura de la tubería). Una vez hecho esto, los dos procesos pueden emplear la tubería para
comunicarse (siempre unidireccionalmente), haciendo write() en p[1] y read() en p[0], respectivamente.
