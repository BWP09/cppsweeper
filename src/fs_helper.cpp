#include <unistd.h>
#include <limits.h>
#include <filesystem>

std::filesystem::path get_executable_dir() {
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    
    if (len != -1) {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path();
    }
    else {
        throw std::runtime_error("Failed to read /proc/self/exe");
    }
}
