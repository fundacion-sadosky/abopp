# ABO++
STIC, Fundación Sadosky

> Por consultas, correcciones, sugerencias, o cualquier comentario relacionado a la guía, pueden mandar un mail a  abarreal@fundacionsadosky.org.ar. Agradecimientos especiales a Juan Heguiabehere y a Teresa Alberto de STIC por sus aportes para mejorar el material.

---

## Introducción

Este repositorio contiene ejercicios varios sobre explotación de vulnerabilidades de corrupción de memoria, abarcando distintas técnicas y distintos temas teóricos. Adicionalmente, cada ejercicio cuenta con su correspondiente resolución.

La idea de este material es que sea complementario a la [guía de escritura de exploits](https://fundacion-sadosky.github.io/guia-escritura-exploits/) de Teresa Alberto, basada en los [ABOs de Gerardo Richarte](https://github.com/gerasdf/InsecureProgramming). El objetivo de ABO++ es proveer material adicional que permita adquirir los conocimientos y las destrezas necesarias para encontrar y explotar vulnerabilidades de corrupción de memoria en entornos modernos, con herramientas modernas utilizadas en la industria hoy en día.

Los ejercicios están organizados en secciones:

**Parte 1**:

1. Infoleak
2. Shellcode
3. Stack
4. Mitigaciones - parte 1
5. Búsqueda de vulnerabilidades y desarrollo de primitivas
6. Ataques de corrupción de memoria - parte 1

**Parte 2**:

7. Punteros y C++

8. Heap

9. Mitigaciones - parte 2

10. Ataques de corrupción de memoria - parte 2

**Temas adicionales:**

11. Fuzzing



Las primeras tres secciones de la parte 1 pueden llevarse en paralelo a la guía de Teresa. Para las secciones que siguen se recomienda ya tener familiaridad con los niveles 1 y 2 de la mencionada guía como mínimo.

Las soluciones fueron desarrolladas en el siguiente entorno:

* Ubuntu 18.04 LTS (Desktop). Se puede verificar ejecutando `lsb_release -a`.
* GNU C Library (Ubuntu GLIBC 2.27-3ubuntu1) stable release version 2.27. Se puede verificar ejecutando `/lib/x86_64-linux-gnu/libc.so.6`.
* Compilador gcc 7.3.0, binutils 2.30 (se puede verificar con los comandos `gcc -v`, `ld -v`).

Para la segunda parte se utilizan también g++ y distintas versiones de Clang:

* g++ 7.3.0 (debería tener la misma versión que gcc; verificar con `g++ -v`).
* clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
* clang version 7.0.0-3~ubuntu0.18.04.1 (tags/RELEASE_700/final)
* clang version 8.0.1-svn358947-1~exp1~20190423034033.62 (branches/release_80)

Cambiar las versiones de las bibliotecas y los mecanismos de compilación probablemente tenga efecto en la disposición de los distintos elementos en el espacio de memoria de los programas objetivo, con lo cuál algunos números en los exploits (e.g. offsets) podrían cambiar respecto a los valores provistos en las soluciones. En cualquier caso, esto es esperable, y los mecanismos de resolución no deberían variar drásticamente.

Para los ejercicios de la primera parte, deberán ser todos compilados con `gcc` sin flags adicionales, a excepción de que se indique lo contrario. Es decir, compilar con `gcc ejercicio.c -o ejercicio`.

**Nota**: Algunas distribuciones de Linux vienen con un gcc que no habilita ciertas mitigaciones por defecto. Un comando de compilación alternativo y más explícito es el siguiente:

```bash
gcc -fstack-protector -pie ejercicio.c -o ejercicio
```

En caso de usar alguna distribución que no sea un Ubuntu reciente, se recomienda compilar con este comando listado arriba.



## Ejercicios


#### Infoleak

Ejercicios sobre filtración de datos. Muchos exploits modernos requieren la capacidad de leer el espacio de memoria del proceso que está siendo atacado. Estos ejercicios introducen algunas técnicas para lograrlo.

* **Infoleak-01** ([Ejercicio](infoleak/infoleak-01/infoleak-01.c) / [Solución](infoleak/infoleak-01/infoleak-01.md))<br/>
El objetivo de este ejercicio es lograr, sin editar el código ni el binario generado por el compilador, que el programa emita por la salida estándar el valor del campo secret_token.

* **Infoleak-02** ([Ejercicio](infoleak/infoleak-02/infoleak-02.c) / [Solución](infoleak/infoleak-02/infoleak-02.md))<br/>Nuevamente, el objetivo es lograr, sin editar el programa, que el mismo emita por salida estándar el valor del campo secret_token. Adicionalmente, en la solución de este ejercicio introducimos gdb y gef para el análisis de programas en tiempo de ejecución.

#### Shellcode

Ejercicios sobre escritura y manejo de shellcode.

* **Shellcode-01** ([Ejercicio](shellcode/shellcode-01/shellcode-01.c) / [Solución](shellcode/shellcode-01/shellcode-01.md))<br/>El objetivo de este ejercicio es lograr, sin modificar el código ni el binario, que el programa muestre "hello, world!" en la terminal y luego finalice sin error.

* **Shellcode-02** ([Ejercicio](shellcode/shellcode-01/shellcode-01.c) / [Solución](shellcode/shellcode-02/shellcode-02.md))<br/>El objetivo de este ejercicio es, tomando el mismo código de shellcode-01, lograr que el programa se conecte a un cierto puerto  TCP y provea una shell que permita ejecutar comandos. Para desarrollar la solución utilizamos msfvenom, parte de la suite de Metasploit.

#### Stack

Ejercicios para familiarizarse con la estructura del stack y con el flujo de ejecución de los procesos. Se recomienda ir leyendo la [introducción al funcionamiento del stack](https://fundacion-sadosky.github.io/guia-escritura-exploits/buffer-overflow/1-introduccion.html) en la guía de escritura de exploits de Teresa Alberto.

* **Stack-01** ([Ejercicio](stack/stack-01/stack-01.c) / [Solución](stack/stack-01/stack-01.md))<br/>El objetivo de este ejercicio es lograr, sin modificar el código fuente ni el binario compilado, que el programa en cuestión muestre un cierto mensaje en la terminal. Para eso hay que encontrar alguna forma alternativa de controlar el flujo de ejecución del programa.
* **Stack-02** ([Ejercicio](stack/stack-02/stack-02.c) / [Solución](stack/stack-02/stack-02.md))<br/>El objetivo de este ejercicio es lograr, sin modificar el código fuente ni el binario compilado, que el programa ejecute una función que normalmente no se ejecutaría.
* **Stack-03** ([Ejercicio](stack/stack-03/stack-03.c) / [Solución](stack/stack-03/stack-03.md))<br/>En enunciado de este ejercicio está en el código. La idea del ejercicio es familiarizarse con una mitigación que hoy día impide que los ataques clásicos ya ampliamente conocidos sigan funcionando. En la solución se presentan los detalles del ataque y del funcionamiento de la mitigación.

#### Mitigaciones - parte 1

Resúmenes sobre los mecanismos más relevantes y más ampliamente adoptados por los sistemas Linux modernos para prevenir ataques de corrupción de memoria. Antes de comenzar con este material se recomienda familiarizarse con el stack overflow clásico, [explicado detalladamente en la guía de escritura de exploits de Teresa Alberto](https://fundacion-sadosky.github.io/guia-escritura-exploits/buffer-overflow/2-practica.html).

* **Stack Protector** ([ver solución del ejercicio stack-03](stack/stack-03/stack-03.md))
* **[Write ^ Execute](mitigaciones/wxe/wxe.md)**
* **[Address Space Layout Randomization](mitigaciones/aslr/aslr.md)**
* **[Relocation Read Only](mitigaciones/relro/relro.md)**

Cabe mencionar que estas no son las únicas mitigaciones existentes, aunque en su conjunto logran una protección de propósito general bastante adecuada. Existen, por otro lado, mitigaciones adicionales que pueden ser adoptadas por sistemas con requisitos de seguridad más rigurosos. Dichas mitigaciones todavía no son tan ubicuas, sin embargo, ya sea por requerir configuración manual, por impactar significativamente en el consumo de recursos, por ser difíciles de implementar, por ser demasiado recientes, o por algún otro motivo. En general, más allá de las mitigaciones aquí listadas, cada sistema adoptará medidas adicionales según sus requisitos de seguridad concretos. En la segunda parte estaremos analizando algunas de estas mitigaciones en detalle.

#### Búsqueda de vulnerabilidades y desarrollo de primitivas

* **[Introducción](primitivas/intro/intro.md)**
* **Vuln-01** ([Ejercicio](primitivas/vuln-01) / [Solución](primitivas/vuln-01/vuln-01.md))<br/>El código de este ejercicio consiste en dos archivos [vuln-01.c](primitivas/vuln-01/vuln-01.c) y [file.h](primitivas/vuln-01/file.h). El objetivo es desarrollar una primitiva que permita escribir datos arbitrarios a partir de direcciones arbitrarias en el espacio de memoria del proceso objetivo. Esencialmente, queremos poder escribir una secuencia de bytes arbitraria `d` a partir de una dirección arbitraria `x`. El objetivo **no** es conseguir una shell ni ejecutar un exploit completo; esto probablemente no sea posible sin deshabilitar ASLR.

#### Técnicas de explotación y ataques de corrupción de memoria - parte 1

* **ROP-01** ([Ejercicio](xploit/rop-01/rop-01.c) / [Solución](xploit/rop-01/rop-01.md))<br/>El objetivo de este ejercicio es aprender a utilizar la técnica llamada Return Oriented Programming (ROP) para vencer a W^X y lograr la eventual ejecución de código arbitrario. Los datos necesarios para vencer a ASLR y a Stack Protector son provistos por el programa. Por una introducción detallada a ROP consultar [2]; alternativamente, en la primera parte de la solución se provee una introducción más amena.
* **ROP-02** ([Ejercicio](xploit/rop-02/rop-02.c) / [Solución](xploit/rop-02/rop-02.md))<br/>Este ejercicio es similar al anterior. Esta vez, sin embargo, el conjunto de direcciones conocidas es diferente. Será necesario realizar algunos pasos adicionales para lograr la explotación.
* **First Boss** ([Intro](xploit/first-boss/intro.md) / [Ejercicio](xploit/first-boss/server.c) / [Solución](xploit/first-boss/first-boss.md))<br/>La idea de este ejercicio es integrar todos o la mayoría de los conceptos estudiados hasta ahora. El objetivo es desarrollar desde cero un exploit totalmente funcional para lograr ejecutar código arbitrario en un servidor TCP. El binario debería ser generado sin flags que pudieran debilitar la seguridad del programa resultante (i.e. sin deshabilitar mitigaciones). Al 22/01/2019 el servidor es explotable en un Ubuntu 18.04 x64 actualizado. En la solución implementamos también un módulo de Metasploit que puede ser utilizado para explotar el servidor en forma automática. **Nota**: Está de más decir que este servidor no debería ser expuesto a redes o a usuarios que no sean de confianza. Ante la duda, se recomienda ejecutar el servidor en una máquina virtual aislada.

#### Punteros y C++

* [**C++**](punteros/cpp/cpp.md)<br/>Resumen sobre la implementación de objetos y polimorfismo a nivel assembly. Comprender estos mecanismos es relevante para entender los ataques basados en el control de punteros a tablas virtuales.
* **ptr-01** ([Ejercicio](punteros/ptr-01/ptr-01.c) / [Solución](punteros/ptr-01/ptr-01.md))<br/>La idea de este ejercicio es lanzar un ataque de ROP con eventual ejecución de shellcode, comenzando por controlar un puntero a código.
* **ptr-02** ([Ejercicio](punteros/ptr-02/ptr-02.cpp) / [Solución](punteros/ptr-02/ptr-02.md))<br/>La idea de este ejercicio es combinar lo aprendido en los dos ejercicios anteriores para llevar a cabo un ataque un poco más sofisticado. El objetivo es, nuevamente, lograr ejecutar shellcode.

#### Heap

* [**Heap**](heap/heap/heap.md)<br/>Resumen introductorio a la implementación del heap de glibc malloc. Comprender el heap es esencial para explotar vulnerabilidades más sofisticadas que van más allá de los desbordes con los que venimos trabajando.
* **heap-01** ([Ejercicio](heap/heap-01/heap-01.cpp) / [Solución](heap/heap-01/heap-01.md))<br/>La idea de este ejercicio es combinar lo que vimos en la sección sobre punteros y C++ con lo que sabemos del heap para lograr ejecutar la función `runme`.
* **heap-02** ([Intro](heap/heap-02/intro.md) / [Ejercicio](heap/heap-02) / [Solución](heap/heap-02/heap-02.md))<br/>La idea de este ejercicio es emular un escenario de explotación de intérpretes, típico, por ejemplo, en explotación de browsers. El archivo [Intro](heap/heap-02/intro.md) contiene un resumen de la funcionalidad básica del intérprete y del modo de uso. Superar este ejercicio requerirá explotar exitosamente una técnica que se conoce como **use after free** (UAF). El objetivo de este ejercicio es desarrollar un mecanismo que permitiría controlar el flujo de ejecución, aunque no es necesario lograr ejecutar código arbitrario; controlar un puntero a código o un vptr será suficiente.
* **heap-03** ([Ejercicio](heap/heap-03) / [Solución](heap/heap-03/heap-03.md))<br/>El código es muy similar al del ejercicio anterior, heap-02, aunque el objetivo ahora es lograr determinar la dirección de la función `main` para una ejecución particular. Es decir, hay que proveer al programa una entrada tal que la salida del mismo permita determinar la dirección en la cuál estuvo la función `main` durante la ejecución del proceso. El programa consiste ahora en los archivos `heap-03.cpp`, `Entity-v2.h` e `Interpreter-v2.h`.

#### Mitigaciones - parte 2

En esta sección estaremos estudiamos algunas mitigaciones que han estado tomando relevancia estos últimos años, recibiendo adopción por parte de diversos compiladores y sistemas operativos. Por motivos que estaremos analizando, sin embargo, las mitigaciones aquí listadas no han podido ser tan ampliamente adoptadas como lo han sido aquellas que estudiamos en la primera parte de la guía. En cualquier caso, la carrera por mejorar las mitigaciones existentes, y por desarrollar nuevas, todavía sigue en curso.

* [**CFI: Concepto**](mitigaciones/cfi/cfi.md)
* [**Shadow Call Stack**](mitigaciones/shadow-call-stack/shadow-call-stack.md)
* [**Sanitizadores: AddressSanitizer**](mitigaciones/asan-sm/asan-sm.md)
* [**CFI: Implementaciones**](mitigaciones/cfi/implementation.md)
* [**CPI & SafeStack**](mitigaciones/cpi/cpi.md)

Nuevamente, no podemos pretender cubrir todas las mitigaciones existentes con un resumen corto. La literatura está llena de propuestas, así como de técnicas para vencerlas. Estos resúmenes servirán, sin embargo, como un oportuno ramp-up para comenzar a investigar el estado del arte en las mitigaciones de ataques de corrupción de memoria, por lo menos por los próximos años, y los inevitables bypasses que irán surgiendo con el tiempo para vencerlas.

#### Ataques de corrupción de memoria - parte 2

* **Final Boss** ([Intro](xploit/final-boss/docs/intro.md), [Código](xploit/final-boss/), [Solución](xploit/final-boss/docs/final-boss.md))<br/>Este es el ejercicio integrador final de la guía, y podría resultar algo extenso. La idea es explotar un programa PIE escrito en C++ y protegido por W^X, ASLR, CFI y SafeStack para lograr ejecutar código arbitrario. Los detalles se encuentran en la [introducción](xploit/final-boss/docs/intro.md).
* [**¿Qué más hay?**](more.md)

#### Fuzzing

Ejercicios sobre técnicas y herramientas de fuzzing. Fuzzing es una técnica muy utilizada hoy en día para ayudar a la detección de vulnerabilidades.

* **[Introducción](fuzzing/fuzzing.md)**<br/>Material introductorio. Solo se hace mención de algunos conceptos básicos y de las herramientas más relevantes que estaremos usando para resolver los siguientes ejercicios.

* **libFuzzer-01** ([Ejercicio](fuzzing/libfuzzer-01/myfunction.c) / [Solución](fuzzing/libfuzzer-01/libfuzzer-01.md))<br/> El objetivo de este ejercicio es aprender a utilizar libFuzzer, un fuzzer integrado con el compilador Clang, fuzzeando un ejemplo sencillo.

* **libFuzzer-02** ([Ejercicio](fuzzing/libfuzzer-02/replace.c) / [Solución](fuzzing/libfuzzer-01/libfuzzer-01.md))<br/> El objetivo de este ejercicio es aprender a construir un corpus para el fuzzer, y a desarrollar funciones objetivo quizás menos triviales.

* **AFL-01** ([Ejercicio](fuzzing/afl-01/afl-01.c) / [Solución](fuzzing/afl-01/afl-01.md) / [Sugerencias](fuzzing/afl-01/sugerencias.md))<br/> El objetivo de este ejercicio es familiarizarse con AFL, un fuzzer un poco más complejo que libFuzzer, si bien más flexible en varios aspectos.

* **AFL-02** ([Ejercicio](fuzzing/afl-02/afl-02.c) / [Solución](fuzzing/afl-02/afl-02.md))<br/> El objetivo de este ejercicio es aprender a definir diccionarios para AFL. Los diccionarios le permiten al fuzzer aprender y fuzzear gramáticas complicadas como la de SQL, HTML, HTTP, etc.

* **TCP-01** ([Ejercicio](fuzzing/tcp-01/tcp-01.c) / [Solución](fuzzing/tcp-01/tcp-01.md))<br/> El objetivo de este ejercicio es lograr fuzzear un servidor TCP simple. Al día de la fecha no parecería existir un procedimiento genérico y siempre aplicable para fuzzear servidores. En la solución de este ejercicio se presenta un posible esquema que resulta ser bastante eficiente y fácil de implementar cuando es posible aplicarlo.

* **TCP-02** ([Ejercicio](fuzzing/tcp-01/tcp-01.c) / [Solución](fuzzing/tcp-02/tcp-02.md))<br/> La idea aquí es tomar el mismo código del ejercicio TCP-01 y fuzzearlo usando preeny, una solución similar en espíritu a la que sugerimos originalmente pero más práctica y más robusta en varios aspectos.

* **TCP-03** ([Ejercicio](fuzzing/tcp-03/tcp-03.c) / [Solución](fuzzing/tcp-03/tcp-03.md))<br/> La idea de este ejercicio es fuzzear un servidor TCP usando preeny. La diferencia con TCP-02 es que en este caso conviene llevar al servidor a un cierto estado concreto **antes** de enviar el payload mutado.

* **Black-Box-01** ([Ejercicio](fuzzing/black-box-01/black-box-01.c) / [Solución](fuzzing/black-box-01/black-box-01.md))<br/> La idea de este ejercicio es fuzzear un binario MIPS no instrumentado. Para ello podemos utilizar AFL y una versión instrumentada de qemu, la cuál puede ser compilada en base a parches distribuidos con el mismo fuzzer.

* **Black-Box-02** ([Ejercicio](fuzzing/tcp-01/tcp-01.c) / [Solución](fuzzing/black-box-02/black-box-02.md))<br/> El objetivo de este ejercicio es tomar el servidor fuzzeado en el ejercicio TCP-01 y compilarlo para MIPS, sin enlace estático. Luego hay que utilizar AFL y qemu para fuzzear el programa en modo caja negra. Si fuese necesario hacer algún cambio, solo es admisible parchear el binario.

* **[Fuzzing nanosvg](fuzzing/nanosvg/fuzzing-nanosvg.md)**<br/>Ejemplo de cómo fuzzear una biblioteca con AFL. Introducimos algunas funciones adicionales de AFL que no habíamos mencionado en los ejercicios anteriores, y también detallamos algunos componentes de la interfaz gráfica del fuzzer para permitir el monitoreo del rendimiento y del progreso de las pruebas.


## Recursos útiles

* Teresa Alberto (STIC). *[Guía de auto-estudio para la escritura de exploits](https://fundacion-sadosky.github.io/guia-escritura-exploits/)*.<br/>Es una guía de escritura de exploits basada en los ABOs de Gerardo Richarte. Contiene ejercicios resueltos y sus correspondientes introducciones teóricas.

* Teresa Alberto, Adrián Barreal (STIC). *[Exploits con rueditas: Introducción a la escritura de exploits](https://github.com/fundacion-sadosky/workshop-eko)*.<br/>Se trata de un workshop presentado en la ekoparty 2018. El repositorio contiene las diapositivas de la presentación, y también hay varias demos de explotación grabadas con [asciinema](https://asciinema.org/).


## Referencias

[1] Anley, Heasman, Lindner & Richarte (2007). *The Shellcoder's Handbook: Discovering and Exploiting Security Holes*.

[2] Shacham (2007). *The Geometry of Innocent Flesh on the Bone: Return-into-libc without Function Calls (on the x86)*.

**Nota**: Hay también referencias adicionales listadas en las soluciones de los ejercicios relevantes.
