#ifndef _IMG_SERVER_TYPES_H_
#define _IMG_SERVER_TYPES_H_

/* Esta clase define un tipo genérico de imágen. La imágen genérica solo
 * tiene noción de su tamaño en píxeles; el formato y los mecanismos concretos
 * para mostrar la imágen serán definidos por el tipo concreto.
 *
 */
class Image {

private:
  unsigned short width;
  unsigned short height;

protected:
  Image(unsigned short width, unsigned short height);

public:
  unsigned short get_width();
  unsigned short get_height();

  virtual void set_data(const unsigned char *bytes) = 0;
  virtual const unsigned char *get_data() = 0;
  virtual unsigned int get_data_length() = 0;

  virtual void display(unsigned short x, unsigned short y) = 0;
  virtual ~Image();
};

/* El tipo GrayscaleImage implementa un tipo de imágen tal que el valor de
 * i-ésimo byte del buffer representa la intensidad luminosa del píxel en
 * la posición (x, y) = (i % width, i/height).
 *
 * https://en.wikipedia.org/wiki/Grayscale
 *
 * Para los fines del ejercicio, los detalles concretos sobre la representación
 * numérica no vienen al caso.
 *
 */
class GrayscaleImage : public Image {
private:
  unsigned char *buffer;

public:
  GrayscaleImage(
    unsigned short width,
    unsigned short height,
    unsigned char *data);

  virtual void set_data(const unsigned char *bytes);
  virtual const unsigned char *get_data();
  virtual unsigned int get_data_length();

  virtual void display(unsigned short x, unsigned short y);
  virtual ~GrayscaleImage();
};


#endif
