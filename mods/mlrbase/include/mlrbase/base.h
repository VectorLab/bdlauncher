
#include <cstdint>
#include <string>
#include <filesystem>

#include <mlr/pubexport.h>

extern "C" {
void _ZdlPv(void*);
};

struct mlrbase_t{

// RO
std::size_t bds_mem_offset;
std::filesystem::path bds_bin_path;

};

PUB_EXPORT extern struct mlrbase_t mlrbase;

// hex_in,bin_out,bin_size 
PUB_EXPORT void mlrbase_hex2bin_unsafe(const char*,char*,std::size_t);
// bin_in,hex_out,bin_size 
PUB_EXPORT void mlrbase_bin2hex_unsafe(const char*,char*,std::size_t);

// mlrbase_reg_hook



