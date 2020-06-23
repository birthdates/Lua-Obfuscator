#include "obfuscator.h"

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "Input directory:" << std::endl;
    std::string dir{};
    try {
        std::getline(std::cin, dir);
        originalDir = dir;
        startMS = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            );
        count_files(dir);

        if (allFiles <= 0) {
            std::cout << "There are no files to obfuscate in that directory!" << std::endl;
            try_exit();
            return 0;
        }

        L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_base(L);
        luaL_dostring(L, o_54b23f86700cdd0d671bbeaab0542ce5);
        file_loop(dir);
        lua_close(L);
        std::chrono::milliseconds newMS = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            );
        std::cout << "Finished obfuscating " << allFiles << " file(s) in " << (newMS - startMS).count() << "ms." << std::endl;
        try_exit();
    }
    catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "Error opening the directory \"" + dir + "\"!" << std::endl;
        std::cout << "Trying again..." << std::endl;
        return main();
    }
}

void count_files(std::string dir) {
    fs::directory_iterator it = fs::directory_iterator(dir);
    std::string lastFolder = dir.substr(dir.find_last_of("\\") + 1, dir.size());
    for (const auto& entry : it) {
        if (fs::is_directory(entry.path())) {
            count_files(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".lua" || entry.path().filename().string().find("__resource") != std::string::npos || entry.path().filename().string().find("fxmanifest") != std::string::npos) {
            continue;
        }
        allFiles++;
    }
}

void file_loop(std::string dir) {
    fs::directory_iterator it = fs::directory_iterator(dir);
    std::string lastFolder = dir.substr(dir.find_last_of("\\") + 1, dir.size());
    for (const auto& entry : it) {
        if (fs::is_directory(entry.path())) {
            file_loop(entry.path().string());
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

void try_exit() {
    std::cout << "Press any key to exit..." << std::endl;
    getchar();
    exit(0);
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool ends_with(const std::string& mainStr, const std::string& toMatch)
{
    if (mainStr.size() >= toMatch.size() &&
        mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}
