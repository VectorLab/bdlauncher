
#include <cstdint>
#include <string>

extern "C" {
void _ZdlPv(void*);
};

struct mlrbase_t{

// RO
std::size_t bds_mem_offset;
std::string bds_bin_path;

};

extern struct mlrbase_t mlrbase;

// hex_in,bin_out,bin_size 
void mlrbase_hex2bin_unsafe(const char*,char*,std::size_t);
// bin_in,hex_out,bin_size 
void mlrbase_bin2hex_unsafe(const char*,char*,std::size_t);

// mlrbase_reg_hook



