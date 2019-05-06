#ifndef _LIBIMG_H_
#define _LIBIMG_H_

/* Inicializa el sistema de comunicación con el servidor.
 *
 */
void img_init();

/* Genera internamente las estructuras de datos necesarias para
 * almacenar una imágen. Devuelve un handle que identifica a la imagen;
 * dicho handle puede ser utilizado en llamadas posteriores para mostrar
 * la imagen y para liberarla.
 *
 * px_intensity_data es un puntero a un arreglo de width*height elementos
 * tal que el i-ésimo elemento del arreglo es la intensidad luminosa del
 * pixel en (x, y) = (i % width, i/height).
 *
 */
extern int make_grayscale_image(
  unsigned short width,
  unsigned short height,
  unsigned char *px_intensity_data);

/* Libera las estructuras internas asociadas a la imagen identificada
 * por handle. El entero handle debe haber sido previamente retornado por
 * make_grayscale_image.
 *
 */
extern int free_grayscale_image(int handle);

/* Muestra en pantalla la imagen identificada por handle, con la esquina
 * superior izquierda de la imágen en (x, y).
 *
 */
extern int display_image(int handle, unsigned short x, unsigned short y);

/* Obtiene los datos asociados a la imagen identificada por handle, incluyendo
 * ancho (almacenado en *out_width), alto (en *out_height), y los bytes
 * correspondientes. Al finalizar la ejecución, *out_buffer apuntará a un
 * buffer en el heap local, de longitud out_width*out_height, el cuál
 * contendrá los datos de la imagen tal como fueron retornados por imgserver.
 *
 */
extern int get_grayscale_image_bytes(
  int handle,
  unsigned char **out_buffer,
  unsigned short *out_width,
  unsigned short *out_height);

  /* Reemplaza los datos asociados a la imagen en el imgserver. El servidor
   * esperará una cantidad de bytes igual a la longitud de los datos, con
   * lo cuál hay que asegurarse de el buffer data tenga el tamaño suficiente
   * para sobrescribir todos los bytes de la imagen identificada por handle.
   *
   */
  extern int set_grayscale_image_bytes(
    int handle,
    unsigned char *px_intensity_data,
    unsigned int data_length);

#endif
