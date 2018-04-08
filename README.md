# Tarea1 SS.OO.

### Integrantes:
- José Manuel Comber
- Felipe Vandeputte

Hay una decisión importante de diseño, pero que no afecta en nada el comportamiento esperado: en vez de crear una estructura de cola, donde se vaya haciendo **pop()** a los elementos y después **append()**, se ocuparon listas ligadas. 
Así, entonces, el proceso que se ejecuta no es necesariamente el primero de la queue, y hay que ver quién vino antes. Esto es equivalente a decir que, en vez de mover la cola y que el "slot" de quien va a la CPU sea fijo, la cola es fija y se va moviendo el "slot".

Un primer paso en la comparación de las tres versiones de MLFQ fue compararlas en casos en que, o bien hay pocos procesos, o estos son de ráfagas cortas con respecto a S. Con esto, los resultados de las versiones deberían ser muy similares.
Y esto es efectivamente lo que ocurre. Tomemos un caso "de juguete": 


```
PROCESS1 1 2 4 4
PROCESS2 3 2 6 6
```

Aquí las tres alternativas (con un Q=5 y S=3 o S=13, por ejemplo) entregan el mismo resultado:

```
Procesos terminados: 2
Tiempo total: 22

PROCESS1
Turnos de CPU: 3
Bloqueos: 2
Turnaround time: 14
Response time: 0
Waiting time: 6

PROCESS2
Turnos de CPU: 5
Bloqueos: 3
Turnaround time: 18
Response time: 2
Waiting time: 6
```

Por lo tanto, hasta aquí todo bien. Ahora veamos un caso en el que se diferencian, y comparemos qué versión obtiene mejores métricas:

```
[INSERTAR OTRO CASO]
```


