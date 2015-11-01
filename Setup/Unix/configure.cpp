
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

class Path
{
public:
    static std::string combine(const std::string &a, const std::string &b)
    {
        if (a.empty() || a.back() == '/')
            return a + b;
        else
            return a + "/" + b;
    }
    
    static std::string extension(const std::string &filename)
    {
        auto pos = filename.find_last_of('.');
        if (pos == std::string::npos)
            return std::string();
        else
            return filename.substr(pos);
    }
};

class Directory
{
public:
    static std::vector<std::string> files(const std::string &path)
    {
        std::vector<std::string> items;
        
        DIR *dir = opendir(path.c_str());
        if (dir)
        {
            while (true)
            {
                dirent *ent = readdir(dir);
                if (ent == 0)
                    break;
                
                if (ent->d_type != DT_DIR)
                {
                    items.push_back(ent->d_name);
                }
            }
            closedir(dir);
        }
        
        std::sort(items.begin(), items.end());
        return items;
    }
    
    static std::vector<std::string> folders(const std::string &path)
    {
        std::vector<std::string> subfolders;
        
        DIR *dir = opendir(path.c_str());
        if (dir)
        {
            while (true)
            {
                dirent *ent = readdir(dir);
                if (ent == 0)
                    break;
                
                if (ent->d_type == DT_DIR)
                {
                    std::string name = ent->d_name;
                    if (name != "." && name != "..")
                        subfolders.push_back(name);
                }
            }
            closedir(dir);
        }
        
        std::sort(subfolders.begin(), subfolders.end());
        return subfolders;
    }
};

class Solution
{
public:
    static bool check_ignore_list(const std::string &name)
    {
        for (const char *search : {
            "Win32",
            "win32",
            "D3D",
            "d3d",
            "OSX",
            "osx",
            "Cocoa",
            "cocoa",
            "Android",
            "android",
            "WGL",
            "wgl",
            "IOS",
            "iOS",
            "ios",
            "AGL",
            "agl"
        })
        {
            if (name.find(search) != std::string::npos)
                return true;
        }
    
        return false;
    }

    static void output_kdevelop_project()
    {
        std::ofstream output("UICore.kdev4");
        output << "[Project]" << std::endl;
        output << "Name=UICore" << std::endl;
        output << "Manager=KDevCMakeManager" << std::endl;
        output << "VersionControl=" << std::endl;
    }
    
    static void output_cmakelists()
    {
        std::ofstream output("CMakeLists.txt");
        output << "# Warning! This is an auto-generated file. Please edit Setup/Unix/configure.cpp instead." << std::endl;
        output << "cmake_minimum_required(VERSION 2.8)" << std::endl;
        output << "project(uicore)" << std::endl;
        output << "SET( CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=gnu++11\" )" << std::endl;
        output << "SET( CMAKE_EXE_LINKER_FLAGS \"${CMAKE_EXE_LINKER_FLAGS} -std=gnu++11\" )" << std::endl;
        output << "INCLUDE(FindPkgConfig)" << std::endl;
        output << "INCLUDE(CheckIncludeFiles)" << std::endl;
        output << "pkg_check_modules(FREETYPE2 REQUIRED freetype2)" << std::endl;
        output << "CHECK_INCLUDE_FILES(linux/joystick.h HAVE_LINUX_JOYSTICK_H)" << std::endl;
        output << "if (HAVE_LINUX_JOYSTICK_H)" << std::endl;
        output << "SET( CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -DHAVE_LINUX_JOYSTICK_H\" )" << std::endl;
        output << "endif (HAVE_LINUX_JOYSTICK_H)" << std::endl;
        output << "include_directories(BEFORE Sources/Include Sources ${FREETYPE2_INCLUDE_DIRS})" << std::endl;
        output << "link_directories(${FREETYPE2_LIBRARY_DIRS})" << std::endl;
        output << "add_library(uicore" << std::endl;
        output_target_files(output, "Sources/UICore");
        output << ")" << std::endl;
        output << "target_link_libraries(uicore ${FREETYPE2_LIBRARIES})" << std::endl;
        output << "install(TARGETS uicore ARCHIVE DESTINATION lib)" << std::endl;
    }
    
    static void output_target_files(std::ofstream &output, const std::string &path)
    {
        for (auto file : Directory::files(path))
        {
            if (!check_ignore_list(file) && Path::extension(file) == ".cpp")
            {
                output << "    " << Path::combine(path, file) << std::endl;
            }
        }
        
        for (auto folder : Directory::folders(path))
        {
            if (!check_ignore_list(folder))
            {
                output_target_files(output, Path::combine(path, folder));
            }
        }
    }
    
    static void generate()
    {
        output_kdevelop_project();
        output_cmakelists();
    }
};

int main(int argc, const char * argv[])
{
    std::cout << std::endl;
    std::cout << "Generating cmake project files.." << std::endl;

    Solution::generate();

    std::cout << "Finished generating files." << std::endl;
    std::cout << std::endl;
    std::cout << "Please use cmake start building or open the project in KDevelop." << std::endl;
    std::cout << std::endl;
    
    return 0;
}
