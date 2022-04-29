#pragma once

#include "ports/ports_sys.h"
#include "ports/ports_dio.h"
#include "list.h"


typedef hal_error_t(*port_action_t)(hal_env_t*, list_node_t*, void*);

typedef struct _ports_interface {
    hal_error_t (*open)(hal_env_t* env, hal_port_t port, list_node_t* node, void* args);
    hal_error_t (*close)(hal_env_t* env, list_node_t* node);
} ports_interface_t;

typedef struct _ports_env {
    ports_native_t native;

    list_t allocated_ports;

    ports_interface_t* dio;
    ports_interface_t* aio;
} ports_env_t;


hal_error_t hal_ports_init(hal_env_t* env);
void hal_ports_quit(hal_env_t* env);

hal_error_t hal_ports_open(hal_env_t* env, ports_interface_t* ports, hal_port_t port, void* args, hal_handle_t* handle_out);
hal_error_t hal_ports_close(hal_env_t* env, ports_interface_t* ports, hal_handle_t handle);
hal_error_t hal_ports_action(hal_env_t* env, ports_interface_t* ports, hal_handle_t handle, port_action_t action,
                             void* args);
