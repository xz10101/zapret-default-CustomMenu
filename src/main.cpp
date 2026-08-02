#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <print>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <conio.h>
#include "WindowsProcess.h"

#define _PUBLIC_VERSION "0.1.1"
/* 

    :: 
    :: ПРИ КОМПИЛЯЦИИ ИСПОЛЬЗОВАТЬ ФЛАГ -std=c++23
    ::
*/

// Подключение MessageBox:W/L прямо в коде для компиляции
#pragma comment(lib, "user32.lib")

// Основной путь для работы с запретом
namespace fsCore = std::filesystem;

std::ofstream fileLog ("log_file.log");

static auto now = std::chrono::system_clock::now();
static auto local_time = std::chrono::current_zone()->to_local(now);


fsCore::path GetExeDirectory() {
    wchar_t buffer[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    return fsCore::path(buffer).parent_path();
    fileLog << "[" << local_time << "]" << " ->  " << "GetExeDirectory()" << "\n";
}


std::vector<std::string> ReturnBatService(const fsCore::path& dir) {
    std::vector<std::string> files;
    try {
        if (!fsCore::exists(dir)) {
            fileLog << "[" << local_time << "]" << " ->  " << "directory does not exist" << "\n";
            std::cerr << "Error! Directory does not exist: " << dir.string() << "\n";
            return files;
        }
        for (const auto& entry : fsCore::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".bat") {
                
                files.push_back(entry.path().string());
            }
        }
        fileLog << "[" << local_time << "]" << " ->  " << "Files is found!" << "\n";
        std::sort(files.begin(), files.end());
    } catch (const fsCore::filesystem_error& e) {
        fileLog << "[" << local_time << "]" << " ->  " << "folder not open (folder -> true)" << "\n";
        std::cerr << "Error! " << e.what() << "\n";
    }
    return files;
}


void menu() {
    fsCore::path exe_dir = GetExeDirectory();
    fsCore::path zapret_dir = exe_dir / "zapret_discord_youtube";
    fileLog << std::unitbuf;
    fileLog << "[" << local_time << "]" << " ->  " << "zapret-console is started\n";

    

    while (true) {
        std::system("cls");
        std::println("Welcome to menu ZAPRET-DISCORD-CONSOLE");
        // std::println("EXE dir:    {}", exe_dir.string());
        // std::println("Search dir: {}\n", zapret_dir.string());

        auto bat_files = ReturnBatService(zapret_dir);
        if (bat_files.empty()) {
            std::println("Error! No .bat files found.");
            fileLog << "[" << local_time << "]" << " ->  " << "not found .bat files, return\n";
            std::system("pause");
            return;
        }

        for (size_t i = 0; i < bat_files.size(); ++i) {
            std::println("[{}] = '{}'", i + 1, fsCore::path(bat_files[i]).filename().string());
        }

        std::println("\nQ | q --> exit");
        std::print("\nSelect: ");

        std::string selectBat;
        std::cin >> selectBat;
        if (selectBat == "q" || selectBat == "Q")  {
            
            fileLog << "[" << local_time << "]" << " ->  " << "close program, bye!\n";
            return;
        }

        int selectInt;
        try {
            selectInt = std::stoi(selectBat);
            fileLog << "[" << local_time << "]" << " ->  " << "selectbat var switch on int\n";
        } catch (...) {
            std::println("Error! Enter a valid number.");
            fileLog << "[" << local_time << "]" << " ->  " << "valid number, return\n";
            std::system("pause");
            continue;
        }

        if (selectInt < 1 || selectInt > static_cast<int>(bat_files.size())) {
            std::println("Error! Not found bat");
            std::system("pause");
            continue;
        }

        std::string bat_path = bat_files[selectInt - 1];
        fsCore::path bat_dir = fsCore::path(bat_path).parent_path();

        // КЛЮЧЕВАЯ СТРОКА: cmd /c "cd /d "папка" && call "батник""
        std::string cmd = "cmd /c \"cd /d \"" + bat_dir.string() + "\" && call \"" + bat_path + "\"\"";

        // std::println("\nDEBUG command: {}", cmd);
        // std::system("pause");

        if (CheckProcessName(L"winws.exe")) {
            int result_mb = MessageBoxW(NULL,
                L"zapret-discord-youtube уже запущен! Хотите закрыть его?",
                L"Предупреждение", 
                MB_YESNO | MB_ICONWARNING
            );

            if (result_mb == IDYES) {
                fileLog << "[" << local_time << "]" << " ->  " << "zapret is open, say this users\n";
                KillProcessName(L"winws.exe");
                std::system(cmd.c_str());
            }
        } else {
            fileLog << "[" << local_time << "]" << " ->  " << "started winws.exe\n";
            std::system(cmd.c_str());
        }

        std::system("pause");
    }
}

int main(int argc, char* argv[]) {
    // std::cout << "Program name : " << argv[0] << std::endl;
    // std::cout << "Prt num : " << argc - 1 << std::endl


            for (int i = 1; i < argc; ++i) {
                fileLog << "[" << local_time << "]" << " ->  " << "argc -- " << argc - 1 << "  |   argv -- " << argv[i] << "\n";
                std::string arg = argv[i];

                if (arg == "-h" || arg == "--help") {
                    fileLog << "[" << local_time << "]" << " ->  " << "argv -- " << arg << "\n";
                    std::println("-h | --help    --- Print help command\n");
                    std::println("-s | --settings * --- Open settings");
                    std::println("-Ss | --show-settings * --- Show all parametrs in JSON(prototype, son real JSON) style");
                    std::println("-Sc | --show-config --- Show all config (In all folder)");
                    std::println("-I | --information  --- Print all info");
                    std::println("* -> function testing or not released");

                }
                else if (arg == "-s" || arg == "--settings") {
                    fileLog << "[" << local_time << "]" << " ->  " << "argv -- " << arg << "\n";
                    std::println("{} -> not released", arg);
                    return 0;
                }
                else if (arg == "-Ss" || arg == "--show-settings") {
                    fileLog << "[" << local_time << "]" << " ->  " << "argv -- " << arg << "\n";
                    std::println("{} -> not released", arg);
                    return 0;
                }
                else if (arg == "-Sc" || arg == "--show-config") {
                    fileLog << "[" << local_time << "]" << " ->  " << "argv -- " << arg << "\n";
                    fsCore::path exe_dir = GetExeDirectory();
                    fsCore::path zapret_dir = exe_dir / "zapret_discord_youtube";

                    auto bat_files = ReturnBatService(zapret_dir);
                        if (bat_files.empty()) {
                            std::println("Error! No .bat files found.");
                            fileLog << "[" << local_time << "]" << " ->  " << "not found .bat files, return\n";
                            std::system("pause");
                            return 1;
                        }

                        for (size_t i = 0; i < bat_files.size(); ++i) {
                            std::println("[{}] = '{}'", i + 1, fsCore::path(bat_files[i]).filename().string());
                        }
                    
                        return 0;
                }
                else if (arg.empty()) {
                    menu();
                }
                else {
                    fileLog << "[" << local_time << "]" << " ->  " << "argv -- " << arg << " -> not found" << "\n";
                    std::println("{} not found prt", arg);
                    return 1;
                }

                    
                }
            

    fileLog << "[" << local_time << "]" << " ->  " << "Program started!!! argc -- " << argc - 1 << "  |   argv -- " << argv[0] << "\n";


    return 0;
}
