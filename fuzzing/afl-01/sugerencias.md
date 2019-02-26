# AFL-01: Sugerencias

Al momento de resolver este ejercicio, se recomienda tener en cuenta lo siguiente:

- Para este ejercicio particular, conviene compilar el programa  objetivo definiendo la variable de entorno AFL_DONT_OPTIMIZE=1. Por  defecto, afl-gcc compila con -O3, lo cuál puede eliminar ciertas  vulnerabilidades; en particular, el código del ejercicio es invulnerable  si se compila con optimizaciones. Definiendo esta variable le indicamos  al compilador que no optimice el código de nuestro programa.
- Si se planea utilizar AFL por períodos prolongados, y en especial si  se planea paralelizar el fuzzing, conviene utilizar un filesystem en  memoria para minimizar la actividad del disco. El cómo se detalla al  final del archivo con la solución.