#include "libimg.h"
#include "imgcommon.h"

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

static struct sockaddr_un srv_address;

//==============================================================================
// img_init
//------------------------------------------------------------------------------

void img_init() {
  bzero(&srv_address, sizeof(srv_address));
  srv_address.sun_family = AF_UNIX;
  strncpy(srv_address.sun_path, SOCKET_PATH, sizeof(srv_address.sun_path) - 1);
}

//==============================================================================
// connect_to_daemon
//------------------------------------------------------------------------------

static int connect_to_daemon() {
  int c;

  if ((c = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    return -1;
  }
  if (connect(c, (struct sockaddr *)&srv_address, sizeof(srv_address)) == -1) {
    return -2;
  }

  return c;
}


//==============================================================================
// make_grayscale_image
//------------------------------------------------------------------------------

int make_grayscale_image(
    unsigned short width,
    unsigned short height,
    unsigned char *px_intensity_data) {

  msg_header_t msg_header;
  msg_header.type = IMG_GS_MAKE;

  img_header_t img_header;
  img_header.width = width;
  img_header.height = height;

  int sock;
  if ((sock = connect_to_daemon()) < 0) {
    return sock;
  }

  send(sock, &msg_header, sizeof(msg_header_t), MSG_NOSIGNAL);
  send(sock, &img_header, sizeof(img_header_t), MSG_NOSIGNAL);

  unsigned short px_total = width*height;
  send(sock, px_intensity_data, px_total, MSG_NOSIGNAL);

  int handle = -1;
  read(sock, &handle, sizeof(int));

  close(sock);
  return handle;
}


//==============================================================================
// free_grayscale_image
//------------------------------------------------------------------------------

int free_grayscale_image(int handle) {
  msg_header_t msg_header;
  msg_header.type = IMG_GS_FREE;

  int sock;
  if ((sock = connect_to_daemon()) < 0) {
    return sock;
  }

  send(sock, &msg_header, sizeof(msg_header_t), MSG_NOSIGNAL);
  send(sock, &handle, sizeof(int), MSG_NOSIGNAL);

  // Leemos el handle como confirmación.
  read(sock, &handle, sizeof(int));

  close(sock);
  return handle;
}


//==============================================================================
// get_grayscale_image_bytes
//------------------------------------------------------------------------------

int get_grayscale_image_bytes(
    int handle,
    unsigned char **out_buffer,
    unsigned short *out_width,
    unsigned short *out_height) {

  msg_header_t msg_header;
  msg_header.type = IMG_GS_GET_BYTES;

  int sock;
  if ((sock = connect_to_daemon()) < 0) {
    return sock;
  }

  send(sock, &msg_header, sizeof(msg_header_t), MSG_NOSIGNAL);

  // Enviamos el handle de la imagen.
  send(sock, &handle, sizeof(int), MSG_NOSIGNAL);

  // Leemos los datos de la imagen.
  img_header_t img_header;
  read(sock, &img_header, sizeof(img_header_t));

  // Devolvemos las dimensiones de la imagen.
  *out_width = img_header.width;
  *out_height = img_header.height;

  // Reservamos espacio en el heap local.
  unsigned int px_total = img_header.width*img_header.height;
  *out_buffer = (unsigned char *) malloc(px_total);

  // Leemos los datos de la imagen.
  read(sock, *out_buffer, px_total);

  close(sock);
  return 0;
}


//==============================================================================
// set_grayscale_image_bytes
//------------------------------------------------------------------------------

int set_grayscale_image_bytes(
    int handle,
    unsigned char *px_intensity_data,
    unsigned int data_length) {

  msg_header_t msg_header;
  msg_header.type = IMG_GS_SET_BYTES;

  int sock;
  if ((sock = connect_to_daemon()) < 0) {
    return sock;
  }

  send(sock, &msg_header, sizeof(msg_header_t), MSG_NOSIGNAL);

  // Enviamos el handle de la imagen.
  send(sock, &handle, sizeof(int), MSG_NOSIGNAL);

  // Enviamos los datos de la imagen.
  send(sock, px_intensity_data, data_length, MSG_NOSIGNAL);

  // Leemos el handle como confirmación.
  read(sock, &handle, sizeof(int));

  close(sock);
  return handle;
}


//==============================================================================
// display_image
//------------------------------------------------------------------------------

int display_image(int handle, unsigned short x, unsigned short y) {
  msg_header_t msg_header;
  msg_header.type = IMG_GS_DISPLAY;

  int sock;
  if ((sock = connect_to_daemon()) < 0) {
    return sock;
  }

  send(sock, &msg_header, sizeof(msg_header_t), MSG_NOSIGNAL);
  send(sock, &handle, sizeof(int), MSG_NOSIGNAL);

  // Mandamos las coordenadas a partir de las cuales mostrar la imagen.
  msg_display_data_t coords;
  coords.x = x;
  coords.y = y;
  send(sock, &coords, sizeof(msg_display_data_t), MSG_NOSIGNAL);

  // Leemos el handle como confirmación.
  read(sock, &handle, sizeof(int));

  close(sock);
  return handle;
}
