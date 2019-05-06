# ¿Qué más hay?

Si has llegado hasta esta instancia, resolviendo todos los ejercicios y entendiendo todas las técnicas expuestas, ¡felicitaciones! Como habrás podido notar, esta sección contiene actualmente un solo ejercicio, el ejercicio final de la segunda parte ABO++. Hay, sin embargo, una buena cantidad de técnicas de explotación que no hemos llegado a cubrir. Algunas ya han sido mencionadas, si bien tangencialmente (e.g. JOP [1], COOP [2]). Hay otras que no, aunque en algunos contextos son ampliamente utilizadas (e.g. JIT-CR [3]). Hay otras que, si bien relativamente sofisticadas y en algunos casos "teóricas", han sido consideradas "catastróficas" por la industria (e.g. [Meltdown & Spectre](https://meltdownattack.com/), y todas las variantes que han evolucionado de ellas). Hay algunas que se encuentran en la vanguardia, habiendo sido presentadas muy recientemente (e.g. PIROP [4]). Podemos también suponer que hay técnicas que, si bien conocidas por algunos, no han sido públicamente expuestas todavía. 

Desgraciadamente, desarrollar ejemplos y explicaciones para cada una de estas técnicas, y otras, consumiría mucho tiempo. Con lo que hemos visto hasta aquí, sin embargo, los papers y otros artículos relevantes ya deberían resultar accesibles, y probablemente no hay mejor fuente que esa. Eventualmente, ABO++ quizás sea actualizada con nuevos ejercicios, nuevas mitigaciones y nuevas técnicas. Por el momento, esperamos que la guía en su estado actual haya servido como una introducción sólida a la materia, y que aquellos interesados en sumarse a la industria estén ahora en mejores condiciones de hacerlo.

Gracias por la lectura, cualquier comentario es bienvenido.
Saludos y suerte en el campo.


## Material adicional

[1] Bletsch, Jiang & Freeh (2011). *Jump-Oriented Programming: A New Class of Code-Reuse Attack*.

[2] Schuster, Tendyck, Liebchen, Davi, Sadeghi & Holz (2015). *Counterfeit Object-oriented Programming: On the Difficulty of Preventing Code Reuse Attacks in C++ Applications*.

[3] Snow, Monrose, Davi, Dmitrienko, Liebchen, Sadeghi (2013). *Just-In-Time Code Reuse: On the Effectiveness of Fine-Grained Address Space Layout Randomization*.

[4] E. Göktaş, B. Kollenda, P. Koppe, E. Bosman, G. Portokalidis, T. Holz, H. Bos, C. Giuffrida, Position-independent Code Reuse: On the Effectiveness of ASLR in the Absence of Information Disclosure, in: EuroS&P, 2018. 

[5] Meltdown & Spectre: https://meltdownattack.com/
