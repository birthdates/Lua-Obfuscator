#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <chrono>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lua.hpp>
}

#define ENDL std::endl

namespace fs = std::filesystem;
lua_State* L;
std::string originalDir{};
double allFiles = 0;
double completedFiles = 0;

//This program was originally meant to be private so ignore this obfusacted lua code
const char* o_54b23f86700cdd0d671bbeaab0542ce5 = "\x66""u\156c\x74""i\157n\x20""o\142f\x28"")\040l\x6F""c\141l\x20""a\040=\x20""[\133]\x5D"";\040a\x20""=\040a\x20"".\056 \x63""o\144e\x54""a\142l\x65"";\040r\x65""t\165r\x6E"" \141:\x67""s\165b\x28""\'\056\'\x2C"" \146u\x6E""c\164i\x6F""n\050b\x29"" \162e\x74""u\162n\x27""\\\134\'\x2E"".\142:\x62""y\164e\x28"")\040e\x6E""d\051 \x6F""r\040a\x2E"".\047\\\x22""\'\040e\x6E""d";

bool replace(std::string& str, const std::string& from, const std::string& to);
bool ends_with(const std::string& mainStr, const std::string& toMatch);
bool is_invalid_file(fs::path file);

void file_loop(std::string dir);
void count_files(std::string dir);
void try_exit();

std::chrono::milliseconds get_time();

int main();
