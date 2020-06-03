#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <chrono>
#include <cstring>
#include <algorithm>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
    #include <lua.hpp>
}

namespace fs = std::filesystem;
lua_State* L;
std::string originalDir{};
double allFiles = 0;
double completedFiles = 0;
char* o_54b23f86700cdd0d671bbeaab0542ce5 = "\x66""u\156c\x74""i\157n\x20""o\142f\x28"")\040l\x6F""c\141l\x20""a\040=\x20""[\133]\x5D"";\040a\x20""=\040a\x20"".\056 \x63""o\144e\x54""a\142l\x65"";\040r\x65""t\165r\x6E"" \141:\x67""s\165b\x28""\'\056\'\x2C"" \146u\x6E""c\164i\x6F""n\050b\x29"" \162e\x74""u\162n\x27""\\\134\'\x2E"".\142:\x62""y\164e\x28"")\040e\x6E""d\051 \x6F""r\040a\x2E"".\047\\\x22""\'\040e\x6E""d";

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool endsWith(const std::string& mainStr, const std::string& toMatch)
{
    if (mainStr.size() >= toMatch.size() &&
        mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}

void fileLoop(std::string dir) {
    fs::directory_iterator it = fs::directory_iterator(dir);
    std::string lastFolder = dir.substr(dir.find_last_of("\\") + 1, dir.size());
    for (const auto& entry : it) {
        if (fs::is_directory(entry.path())) {
            fileLoop(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".lua" || entry.path().filename().string().find("__resource") != std::string::npos || entry.path().filename().string().find("fxmanifest") != std::string::npos) {
            continue;
        }
        std::string line;
        std::ifstream myfile(entry.path());
        if (myfile.is_open())
        {
            std::string newLine = "";
            while (getline(myfile, line))
            {
                if (line.empty()) {
                    continue;
                }
                newLine += line + "\n";
            }

            myfile.close();
            
            lua_pushstring(L, newLine.c_str());
            lua_setglobal(L, "codeTable");

            lua_getglobal(L, "obf");
            lua_pcall(L, 0, 1, 0);

            std::string obfCode = lua_tostring(L, -1);

            std::ofstream obfFile(entry.path());

            obfFile << "load(\"" << obfCode << "\")()";

            obfFile.close();
            completedFiles++; 
            std::string folderPath = entry.path().string();
            replace(folderPath, originalDir, "");
            if (originalDir._Starts_with("C:\\") || originalDir._Starts_with("C:/")) {
                folderPath.replace(0, 1, "");
            }
            std::cout << "(" << floor(completedFiles / allFiles * 100) << "%) Successfully obfusacted " << folderPath << std::endl;
        }
    }
}

void checkFiles(std::string dir) {
    fs::directory_iterator it = fs::directory_iterator(dir);
    std::string lastFolder = dir.substr(dir.find_last_of("\\") + 1, dir.size());
    for (const auto& entry : it) {
        if (fs::is_directory(entry.path())) {
            checkFiles(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".lua" || entry.path().filename().string().find("__resource") != std::string::npos || entry.path().filename().string().find("fxmanifest") != std::string::npos) {
            continue;
        }
        allFiles++;
    }
}

void tryExit() {
    std::cout << "Press any key to exit..." << std::endl;
    getchar();
    exit(0);
}

std::chrono::milliseconds startMS;

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "Input directory:" << std::endl;
    std::string dir{};
    try {
        std::getline(std::cin, dir);
        system("CLS");
        originalDir = dir;
        startMS = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            );
        checkFiles(dir);

        if (allFiles <= 0) {
            std::cout << "There are no files to obfuscate in that directory!" << std::endl;
            tryExit();
            return 0;
        }

        L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_base(L);
        luaL_dostring(L, o_54b23f86700cdd0d671bbeaab0542ce5);
        fileLoop(dir);
        lua_close(L);
        std::chrono::milliseconds newMS = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            );
        std::cout << "Finished obfuscating " << allFiles << " file(s) in " << (newMS - startMS).count() << "ms." << std::endl;
        tryExit();
    }
    catch (const std::exception& e) {
        std::cout << "ERROR: " <<  e.what() << std::endl;
        std::cout << "Error opening the directory \"" + dir + "\"!" << std::endl;
        std::cout << "Trying again..." << std::endl;
        return main();
    }
}