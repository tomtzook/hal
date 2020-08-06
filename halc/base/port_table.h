#ifndef HAL_PORT_TABLE_H
#define HAL_PORT_TABLE_H

#include "hal_types.h"
#include "util/lookup_table.h"

hal_result_t table_find_port(lookup_table_t* table, port_id_t port_id, hal_handle_t* result_handle);
hal_result_t table_get_port(lookup_table_t* table, hal_handle_t hal_handle, void** result_port_struct);

hal_result_t table_insert_port(lookup_table_t* table, void* port_struct, hal_handle_t* result_handle);
hal_result_t table_remove_port(lookup_table_t* table, hal_handle_t hal_handle);

#endif //HAL_PORT_TABLE_H
