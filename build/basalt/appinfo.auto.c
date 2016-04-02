#include "pebble_process_info.h"
#include "src/resource_ids.auto.h"

const PebbleProcessInfo __pbl_app_info __attribute__ ((section (".pbl_header"))) = {
  .header = "PBLAPP",
  .struct_version = { PROCESS_INFO_CURRENT_STRUCT_VERSION_MAJOR, PROCESS_INFO_CURRENT_STRUCT_VERSION_MINOR },
  .sdk_version = { PROCESS_INFO_CURRENT_SDK_VERSION_MAJOR, PROCESS_INFO_CURRENT_SDK_VERSION_MINOR },
  .process_version = { 0, 3 },
  .load_size = 0xb6b6,
  .offset = 0xb6b6b6b6,
  .crc = 0xb6b6b6b6,
  .name = "Budget",
  .company = "deverett@gmail.com",
  .icon_resource_id = RESOURCE_ID_MENU_IMAGE,
  .sym_table_addr = 0xA7A7A7A7,
  .flags = 0,
  .num_reloc_entries = 0xdeadcafe,
  .uuid = { 0x7D, 0x27, 0x9D, 0x77, 0x9D, 0xE7, 0x4F, 0xF6, 0xB0, 0x7E, 0xA6, 0x20, 0x47, 0x44, 0xCC, 0x4C },
  .virtual_size = 0xb6b6
};
