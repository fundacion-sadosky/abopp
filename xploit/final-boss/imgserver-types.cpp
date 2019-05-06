#include "imgserver-types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image::Image(unsigned short width, unsigned short height) {
  this->width = width;
  this->height = height;
}

Image::~Image() {
}

unsigned short Image::get_width() {
  return this->width;
}

unsigned short Image::get_height() {
  return this->height;
}

GrayscaleImage::GrayscaleImage(
      unsigned short width,
      unsigned short height,
      unsigned char *data) : Image(width, height) {

    // Para asegurar la persistencia, creamos un buffer propio.
    unsigned int data_length = width*height;
    this->buffer = new unsigned char[data_length];
    memcpy(this->buffer, data, data_length);
  }

unsigned int GrayscaleImage::get_data_length() {
  return this->get_width()*this->get_height();
}

const unsigned char *GrayscaleImage::get_data() {
  return this->buffer;
}

void GrayscaleImage::set_data(const unsigned char *bytes) {
  memcpy(this->buffer, bytes, this->get_data_length());
}

GrayscaleImage::~GrayscaleImage() {
  free(buffer);
}

/* La función display en realidad no está implementada. Podemos imaginar
 * que realiza algunas operaciones de bajo nivel para mostrar la imagen en
 * algún dispositivo, aunque para los fines del ejercicio no viene al caso.
 *
 */
#define DISPLAY_GRAYSCALE_IMAGE(x, y, width, height, buffer) //

void GrayscaleImage::display(unsigned short x, unsigned short y) {
  printf("[*] Displaying grayscale image.\n");
  DISPLAY_GRAYSCALE_IMAGE(x, y, this->width, this->height, this->buffer);
}
