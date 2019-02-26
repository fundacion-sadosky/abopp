# Fuzzing



## Introducción

Fuzzing es una técnica frecuentemente utilizada para encontrar problemas de seguridad en software. La técnica consiste en generar entradas malformadas en forma automatizada; dichas entradas son provistas a un sistema en prueba, también de forma automática, mientras de alguna forma monitoreamos las respuestas y el estado del sistema para determinar qué efecto están teniendo las entradas malformadas, y si alguna ha causado algún potencial problema de seguridad (e.g. un error de segmentación).

Dada una pieza de software que queremos "fuzzear", podríamos, por ejemplo, proceder de la siguiente forma:

1. Identificamos la superficie de ataque que expone la pieza: es decir, identificamos todos los puntos de entrada de datos que un atacante podría utilizar para interactuar con el sistema.
2. Para todos o para un subconjunto de los puntos encontrados generamos en forma evolutiva entradas de prueba. Para generar las entradas, se pueden tomar entradas correctas y mutarlas progresivamente.
3. Alimentamos los datos generados al sistema y monitoreamos su comportamiento. Si notamos, por ejemplo, que una cierta entrada provoca un core dump, tomamos nota de la misma para posterior análisis.



Los fuzzers modernos como AFL y libFuzzer proceden en forma iterativa: comienzan fuzzeando el sistema objetivo a partir de un corpus inicial de entradas de prueba; luego, en base a datos recopilados en tiempo de ejecución, generan nuevas entradas tratando de maximizar la cobertura del código bajo análisis.

Algunos métodos posibles para generar entradas de prueba son los siguientes:

* **Mutación**: Partimos de plantillas correctas denominadas "semillas", y luego las modificamos en forma progresiva. Por ejemplo, se puede tomar un archivo existente y corromper (mutar) partes del mismo en formas pseudo-aleatorias (mediante bit-flipping, por ejemplo). El proceso de fuzzing es usualmente tan bueno como las semillas. Entradas con baja entropía/complejidad no llegarán usualmente a cubrir caminos interesantes.
* **Generación**: Un fuzzer generacional es capaz de construir los datos siendo enviados al sistema en base a un modelo de datos construido por el autor del fuzzer. Es útil, por ejemplo, cuando se conoce el protocolo y se puede escribir código para generarlo. La efectividad del fuzzer estará entonces limitada por la precisión del modelo de datos.
* **Método diferencial**: Un algoritmo de fuzzing diferencial es aquel que activamente intenta reducir el espacio de posibles entradas, en vez de intentar ser exhaustivo. Los fuzzers diferenciales se enfocan en reducción de casos de prueba y en cobertura de caminos.



Para detectar bugs mediante fuzzing, será necesario desarrollar algún tipo de sistema de monitoreo. Los bugs lógicos pueden ser muy difíciles de detectar; detectar crashes, por otro lado, suele ser más fácil. Para detectar crashes se pueden monitorear los procesos vivos (e.g. para ver cuándo el proceso en cuestión muere, cuándo pasa a estado zombie, o cuándo desaparece su PID), se pueden verificar logs, se pueden buscar core dumps, etc. aunque la mejor idea es probablemente acoplar un debugger y monitorear el estado del proceso directamente.

Al momento de seleccionar un subconjunto adecuado de puntos de entrada, es necesario considerar que el camino que una aplicación tomará ante una entrada dada puede depender, en adición de la entrada misma, del entorno del proceso, y del estado de la aplicación al momento de recibir la entrada. Por lo tanto, un ciclo de fuzzing puede comenzar llevando a la aplicación a un estado particular antes de enviar la entrada malformada.



**Fuzzers recomendados**

* [American Fuzzy Lop (AFL)](http://lcamtuf.coredump.cx/afl/)
* [libFuzzer](https://llvm.org/docs/LibFuzzer.html)

**Herramientas adicionales**

* [Preeny](https://github.com/zardus/preeny)

**Enlaces recomendados**

* AFL training: https://github.com/ThalesIgnite/afl-training
* Compendio de recursos: https://github.com/secfigo/Awesome-Fuzzing