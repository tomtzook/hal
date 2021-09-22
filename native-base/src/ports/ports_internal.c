
#include <malloc.h>

#include "../hal_internal.h"
#include "ports_internal.h"


hal_error_t hal_ports_init(hal_env_t* env, hal_native_t hal_native) {
    env->ports_sys.dio_head = NULL;
    env->ports_sys.dio_tail = NULL;

    hal_native.ports_init(env, &env->ports_sys.native);
    return HAL_SUCCESS;
}

void hal_ports_quit(hal_env_t* env) {
    dio_port_node_t* dio_node = env->ports_sys.dio_head;
    while (NULL != dio_node) {
        dio_port_node_t* cnode = dio_node;
        dio_node = dio_node->next;

        hal_dio_close_port(env, &cnode->port);
        free(cnode);
    }

    env->ports_sys.native.free(env, &env->ports_sys.native);
}
