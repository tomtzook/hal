#include <dio_interface.h>
#include <stdio.h>


int dio_init(dio_env_t** env) {
    printf("Initialize DIO \n");
    return 0;
}
void dio_free(dio_env_t** env) {
    printf("Free DIO \n");
}

int dio_port_init(dio_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    printf("Initialize DIO Port %d (%d) \n", port_id, port_dir);
    return 0;
}
void dio_port_free(dio_env_t* env, port_id_t port_id) {
    printf("Free DIO Port %d \n", port_id);
}

void dio_port_write(dio_env_t* env, port_id_t port_id, dio_value_t value) {
    printf("Write DIO Port %d (%d) \n", port_id, value);
}
dio_value_t dio_port_read(dio_env_t* env, port_id_t port_id) {
    printf("Read DIO Port %d \n", port_id);
    return LOW;
}