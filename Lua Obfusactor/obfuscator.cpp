#include "obfuscator.h"

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "Input directory:" << ENDL;
    std::string dir{};
    try {
        std::getline(std::cin, dir);
        originalDir = dir;
        std::chrono::milliseconds startMS = get_time();
        count_files(dir);

        if (allFiles <= 0) {
            std::cout << "There are no files to obfuscate in that directory!" << ENDL;
            try_exit();
            return 0;
        }

        L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_base(L);
        luaL_dostring(L, o_54b23f86700cdd0d671bbeaab0542ce5);
        file_loop(dir);
        lua_close(L);
        std::chrono::milliseconds newMS = get_time();
        std::cout << "Finished obfuscating " << allFiles << " file(s) in " << (newMS - startMS).count() << "ms." << ENDL;
        try_exit();
    }
    catch (const std::exception& exception) {
        std::cout << "ERROR: " << exception.what() << ENDL;
        std::cout << "Error opening the directory \"" + dir + "\"!" << ENDL;
        std::cout << "Trying again..." << ENDL;
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
        if (is_invalid_file(entry.path())) {
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
        if (is_invalid_file(entry.path())) {
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

            obfFile << "local loading = load or loadstring\nloading(\"" << obfCode << "\")()";

            obfFile.close();
            completedFiles++; 
            std::string folderPath = entry.path().string();
            replace(folderPath, originalDir, "");
            if (originalDir._Starts_with("C:\\") || originalDir._Starts_with("C:/")) {
                folderPath.replace(0, 1, "");
            }
            std::cout << "(" << floor(completedFiles / allFiles * 100) << "%) Successfully obfusacted " << folderPath << ENDL;
        }
    }
}

void try_exit() {
    std::cout << "Press any key to exit..." << ENDL;
    exit(getchar());
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
    return mainStr.size() >= toMatch.size() &&
        mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0;
}

bool is_invalid_file(fs::path file) {
    std::string path = file.filename().string();
    return file.extension().string() != ".lua" || path._Starts_with("__resource") || path._Starts_with("fxmanifest");
}

std::chrono::milliseconds get_time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
        );
}