#pragma once

#include "ports/ports_sys.h"
#include "ports/ports_dio.h"


typedef struct _dio_port_node {
    dio_port_t port;

    struct _dio_port_node* next;
    struct _dio_port_node* prev;
} dio_port_node_t;

typedef struct _ports_env {
    ports_native_t native;
    dio_port_node_t* dio_head;
    dio_port_node_t* dio_tail;
} ports_env_t;


hal_error_t hal_ports_init(hal_env_t* env, hal_native_t hal_native);

void hal_ports_quit(hal_env_t* env);

void hal_dio_close_port(hal_env_t* env, dio_port_t* port);
