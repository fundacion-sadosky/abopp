# Control Flow Integrity

**Control Flow Integrity** (CFI) es una propiedad que puede tener un programa, inicialmente propuesta en el paper original [1], y garantizarla debería en teoría prevenir los ataques de corrupción de memoria. Para garantizar que un programa presenta CFI, es necesario garantizar que la ejecución del programa solo puede seguir un conjunto de caminos predeterminados. En términos prácticos, dada la dirección de origen de un salto, el conjunto de posibles destinos está predeterminado; cualquier intento de saltar a un destino inválido debe causar la finalización del programa. Los conjuntos de los distintos puntos de origen de un salto, de los puntos de destino, y de los saltos admisibles conforman un grafo llamado "grafo de control de flujo".

Un programa que presenta CFI es en teoría inmune a ataques que requieren controlar el flujo de ejecución del programa mediante la corrupción de punteros a código. Los ataques que logran ejecutar código arbitrario, ya sea logrando la eventual ejecución de un shellcode, o ejecutando largas cadenas de ROP/JOP/etc., requieren indefectiblemente que la ejecución recorra caminos que en general no fueron previstos, o que no serían deseables. Estos caminos no estarían contemplados en el grafo de control de flujo, con lo cuál la ejecución del payload sería imposible; un programa que presenta CFI consideraría un salto inválido como una condición crítica de error y finalizaría.

Los mecanismos usuales para implementar CFI se basan en la instrumentación binaria. Es decir, se insertan instrucciones en puntos clave del programa para realizar las validaciones encargadas de asegurar que los caminos de ejecución están contemplados en el grafo. Lo ideal es llevar a cabo la instrumentación en un pase del compilador, aunque no es imposible reescribir binarios ya compilados para agregar las instrucciones adicionales. Por ejemplo, supongamos que tenemos la siguiente secuencia de instrucciones en assembly:

```assembly
mov rax,QWORD PTR [rbp-0x8]
jmp rax
```

Esta secuencia carga una dirección desde el stack y realiza un salto indirecto a través de rax. A esta altura ya sabemos que en general no se puede garantizar que un puntero en el stack sea íntegro al momento de ser utilizado, con lo cuál habrá que validar explícitamente que la dirección de destinto sea una admisible para la dirección de origen. Una implementación concreta podría tomar el código de arriba y modificarlo para que lea como el siguiente:

```assembly
mov rax,QWORD PTR [rbp-0x8]

; Ejecutamos validar_salto(destino, id_origen).
; Este procedimiento se encargará de verificar que la dirección de destino
; sea un destino válido para el punto de origen identificado por id_origen.
mov rdi, rax
mov rsi, <id_origen, identificador del punto de origen generado estáticamente>
call validar_salto

; Si validar_salto no devuelve 0 abortamos la ejecución.
cmp eax, 0
jne abortar

; Si validar_salto devuelve 0 el salto es válido y la ejecución sigue.
jmp rax
```

La idea de esta implementación es generar un identificador estático por cada punto de origen e implementar un mecanismo que permita, dado un punto de origen y una dirección de destino, determinar si el salto es válido o no. Si el salto no es válido, la ejecución deberá abortar. Observaremos que, esencialmente, lo que está haciendo el código es verificar un supuesto que antes se consideraba obvio: la dirección a la que estamos saltando es un destino válido. Como recordaremos de secciones anteriores, las vulnerabilidades surgen donde hay supuestos implícitos; si queremos programas seguros, debemos verificar explícitamente todos nuestros supuestos.

Idealmente, una implementación real debe ser eficiente. Poner un call adicional por cada salto indirecto podría considerarse excesivo. Adicionalmente, no hemos hablado de la implementación concreta de `validar_salto`, ni de cómo determinar los destinos válidos dado un cierto punto de origen. Esto no es casual. Resulta que garantizar CFI en forma completa y eficiente está resultando ser una tarea difícil. A lo largo del tiempo han surgido varias implementaciones, algunas de las cuales estaremos viendo luego. Muchas de ellas resultan ser demasiado ineficientes para ser ampliamente adoptadas. Las implementaciones eficientes, por otro lado, se ven forzadas a relajar ciertos supuestos de CFI, debilitando la seguridad del esquema. Está de más decir que muchas de estas implementaciones relajadas ya han sido vulneradas; estaremos viendo más al respecto en un resumen posterior. Por el momento, sin embargo, pasaremos a estudiar otras técnicas de mitigación basadas en la idea de "shadow memory". Por ahora es importante quedarse con la idea conceptual de CFI, y con el concepto de  instrumentación binaria, el cuál tomará relevancia cuando estudiemos las nuevas mitigaciones presentadas en los próximos resúmenes.



## Referencias

[1] Abadi, Budiu, Erlingsson & Ligatti (2005). *Control-Flow Integrity*.