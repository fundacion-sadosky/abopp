#ifndef _IMG_SEVER_COMMON_H_
#define _IMG_SEVER_COMMON_H_

#define SOCKET_PATH "/tmp/imgserver"

typedef enum _message_type_t {
  IMG_GS_MAKE,
  IMG_GS_FREE,
  IMG_GS_DISPLAY,
  IMG_GS_GET_BYTES,
  IMG_GS_SET_BYTES
} message_type_t;

typedef struct _msg_header_t {
  message_type_t type;
} msg_header_t;

typedef struct _img_header_t {
  unsigned short width;
  unsigned short height;
} img_header_t;

typedef struct _msg_display_data_t {
  unsigned short x;
  unsigned short y;
} msg_display_data_t;

#endif
