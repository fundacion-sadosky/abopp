#include "imgcommon.h"
#include "imgserver-types.h"
#include "imgserver-handler.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>

#define MAX_HANDLE_COUNT 256

static int next_handle = 0;
static std::map<int, Image*> img_map;

//==============================================================================
// free_image
//------------------------------------------------------------------------------

static void free_image(int handle) {
  if (img_map.count(handle)) {
    free(img_map[handle]);
  }
}

//==============================================================================
// handle_grayscale_load
//------------------------------------------------------------------------------

static void handle_grayscale_make(int client_socket) {
  img_header_t header;
  read(client_socket, &header, sizeof(img_header_t));

  unsigned short px_total = header.width*header.height;
  unsigned char *buffer = (unsigned char *) malloc(px_total);

  // Leemos los datos de la imagen y los guardamos en el buffer temporal.
  read(client_socket, buffer, px_total);

  // Creamos una nueva imagen y le asignamos un handle.
  Image *img = new GrayscaleImage(header.width, header.height, buffer);

  // Creamos un handle para la nueva imagen, para almacenarla en el mapa.
  int new_handle = next_handle++ % MAX_HANDLE_COUNT;

  // Eventualmente, el handle dará la vuelta, con lo cuál habrá que
  // asegurarse de que la imagen asociada al handle sea liberada si
  // ya estaba en uso.
  free_image(new_handle);

  // Guardamos el puntero a la nueva imagen en el mapa.
  img_map[new_handle] = img;

  // Enviamos el handle que asignamos a la imagen.
  send(client_socket, &new_handle, sizeof(int), MSG_NOSIGNAL);
}


//==============================================================================
// handle_grayscale_free
//------------------------------------------------------------------------------

static void handle_grayscale_free(int client_socket) {
  int handle;

  // Obtenemos el handle que identifica a la imagen.
  read(client_socket, &handle, sizeof(int));

  // Liberamos la memoria asociada a la imagen identificada por handle.
  free_image(handle);

  // Confirmamos que la imagen ha sido liberada.
  send(client_socket, &handle, sizeof(int), MSG_NOSIGNAL);
}


//==============================================================================
// handle_grayscale_display
//------------------------------------------------------------------------------

static void handle_grayscale_display(int client_socket) {
  int handle;

  // Obtenemos el handle que identifica a la imagen.
  read(client_socket, &handle, sizeof(int));

  // Obtenemos las coordenadas a partir de las cuáles mostrar la imágen.
  msg_display_data_t coords;
  read(client_socket, &coords, sizeof(msg_display_data_t));

  // Mostramos la imagen en pantalla si se trata de un handle válido.
  if (img_map.count(handle)) {
    img_map[handle]->display(coords.x, coords.y);
  }

  // Confirmamos que la imagen ha sido mostrada.
  send(client_socket, &handle, sizeof(int), MSG_NOSIGNAL);
}


//==============================================================================
// handle_grayscale_get_bytes
//------------------------------------------------------------------------------

static void handle_grayscale_get_bytes(int client_socket) {
  int handle;

  // Obtenemos el handle que identifica a la imagen.
  read(client_socket, &handle, sizeof(int));

  // Obtenemos la imagen asociada al handle.
  GrayscaleImage *img = dynamic_cast<GrayscaleImage*>(img_map[handle]);

  // Enviamos los datos asociados a la imagen.
  img_header_t header;
  header.width = img->get_width();
  header.height = img->get_height();

  // Enviamos las dimensiones de la imagen.
  send(client_socket, &header, sizeof(img_header_t), MSG_NOSIGNAL);

  // Enviamos los datos de la imagen.
  send(client_socket, img->get_data(), img->get_data_length(), MSG_NOSIGNAL);
}


//==============================================================================
// handle_grayscale_set_bytes
//------------------------------------------------------------------------------

static void handle_grayscale_set_bytes(int client_socket) {
  int handle;

  // Obtenemos el handle que identifica a la imagen.
  read(client_socket, &handle, sizeof(int));

  // Obtenemos la imagen asociada al handle.
  GrayscaleImage *img = dynamic_cast<GrayscaleImage*>(img_map[handle]);

  // Leemos los nuevos datos para la imagen.
  unsigned char *bytes = (unsigned char *) malloc(img->get_data_length());
  read(client_socket, bytes, img->get_data_length());

  // Reemplazamos los bytes viejos por los nuevos y liberamos el buffer.
  img->set_data(bytes);
  free(bytes);

  // Confirmamos enviando el handle de la imagen.
  send(client_socket, &handle, sizeof(int), MSG_NOSIGNAL);
}


//==============================================================================
// handle_request
//------------------------------------------------------------------------------

void handle_request(int client_socket) {
  message_type_t type;

  // Leemos del socket para determinar el tipo de mensaje.
  read(client_socket, &type, sizeof(message_type_t));

  // Gestionamos el pedido en forma diferente dependiendo del tipo.
  switch (type) {
    case IMG_GS_MAKE:
      handle_grayscale_make(client_socket);
      break;
    case IMG_GS_FREE:
      handle_grayscale_free(client_socket);
      break;
    case IMG_GS_DISPLAY:
      handle_grayscale_display(client_socket);
      break;
    case IMG_GS_GET_BYTES:
      handle_grayscale_get_bytes(client_socket);
      break;
    case IMG_GS_SET_BYTES:
      handle_grayscale_set_bytes(client_socket);
      break;
    default:
      int response = -1;
      send(client_socket, &response, sizeof(int), MSG_NOSIGNAL);
      break;
  }
}
