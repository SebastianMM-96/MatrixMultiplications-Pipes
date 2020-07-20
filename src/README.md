# Solución 

Los procesos hijo deben comunicar el resultado al padre, esto quiere decir que necesitarán 2 tuberías, una para enviar/recibir tareas, y otra para
enviar/recibir resultados.

a. Proceso padre: este será el encargado de leer las matrices a multiplicar, ambas de dimensión
$N \times N$. El proceso padre no hará cálculos, solamente repartirá las tareas. Cada tarea consiste en calcular
cada uno de los $N^2$ elementos de la matriz resultante. Es decir, el padre mandará $N^2$ tareas que
consistirán en un mensaje con 2 enteros, el renglón y columna a multiplicar. Estos mensajes los
escribirá uno tras otro, no importa qué hijo lo recibirá.

b. Procesos hijo: habrá P < $N^2$ procesos hijo (trabajadores) que leerán de la tubería de tareas su
tarea a resolver (renglón y columna a multiplicar). Cuando el hijo termine su tarea debe enviar por la
tubería de resultados en un único mensaje con tres valores enteros, el renglón y columna que calculó, y
el resultado. Una vez que envíe el resultado, el hijo tomará otra tarea.
