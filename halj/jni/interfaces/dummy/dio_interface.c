#include <dio_interface.h>
#include <stdio.h>


int dio_init() {
    printf("Initialize DIO \n");
    return 0;
}
void dio_free() {
    printf("Free DIO \n");
}

int dio_port_init(port_id_t port_id, port_dir_t port_dir) {
    printf("Initialize DIO Port %d (%d) \n", port_id, port_dir);
    return 0;
}
void dio_port_free(port_id_t port_id) {
    printf("Free DIO Port %d \n", port_id);
}

void dio_port_write(port_id_t port_id, dio_value_t value) {
    printf("Write DIO Port %d (%d) \n", port_id, value);
}
dio_value_t dio_port_read(port_id_t port_id) {
    printf("Read DIO Port %d \n", port_id);
    return LOW;
}