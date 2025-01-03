#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <ctime>
#include <cstdarg>
#include <mutex>
#include <unistd.h>
#include <map>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

inline std::string getComputerName() {
#ifdef _WIN32
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer);
    if (GetComputerName(buffer, &size)) {
        return std::string(buffer);
    }
#else
    char buffer[256];
    if (gethostname(buffer, sizeof(buffer)) == 0) {
        return std::string(buffer);
    }
#endif
    return "unknown";
}

inline std::string getFileName(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return filePath;
    }
    return filePath.substr(lastSlash + 1);
}

inline std::string getFilePath(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return filePath;
    }
    return filePath.substr(0, lastSlash);
}

inline bool createDirectory(const std::string& filePath) {
#ifdef _WIN32
    const char* filePathC = filePath.c_str();
    DWORD attributes = GetFileAttributes(filePathC);
    if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;
    }
    if (CreateDirectory(filePathC, nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
#else
    struct stat info;
    if (stat(filePath.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) {
        return true;
    }
    if (mkdir(filePath.c_str(), 0777) == 0 || errno == EEXIST) {
        return true;
    }
#endif
    return false;
}

class Logger {
public:
    enum class LogLevel { INFO_, WARNING_, ERROR_, FATAL_ };

    static Logger& getInstance(std::string logPath = "NULL") {
        if (logPath == "NULL") {
#ifdef _WIN32
            logPath = getFilePath(__FILE__) + "\\log";
#else
            logPath = getFilePath(__FILE__) + "/log";
#endif
        }
        static Logger instance(logPath);
        return instance;
    }

    void log_c(const pid_t& processId, const std::string& file, const int& line, LogLevel level, const char* format, ...) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        const std::string str(buffer);
        std::lock_guard<std::mutex> lock(logMutex_);
        std::ofstream& logFile = getLogFile(level);
        logFile << getLogLevelPrefix(level) << getTimestamp("%m%d %H:%M:%S") << " " << processId << " " << file << ":" << line << "] " << str << std::endl;
    }

    class LogStream {
    public:
        LogStream(std::ofstream& file) : logFile(file) {}

        template<typename T>
        LogStream& operator<<(const T& value) {
            logFile << value;
            return *this;
        }

        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            logFile << manip;
            return *this;
        }

    private:
        std::ofstream& logFile;
    };

    LogStream log_cpp(LogLevel level, const pid_t& processId, const std::string& file, const int& line) {
        std::lock_guard<std::mutex> lock(logMutex_);
        std::ofstream& logFile = getLogFile(level);
        logFile << getLogLevelPrefix(level) << getTimestamp("%m%d %H:%M:%S") << " " << processId << " " << file << ":" << line << "] ";
        return LogStream(logFile);
    }


private:
    Logger(std::string& logPath);

    ~Logger();

    std::string logPath_;
    std::mutex logMutex_;
    std::map<LogLevel, std::ofstream> logFiles_;

    std::string getTimestamp(const char* format);

    std::string getLogLevelName(const LogLevel& level);
    std::string getLogLevelPrefix(const LogLevel& level);

    std::ofstream& getLogFile(const LogLevel& level);
};

#define log_c(...) log_c(getpid(), getFileName(__FILE__), __LINE__, __VA_ARGS__)

#define LOG_INFO_C(...) Logger::getInstance().log_c(Logger::LogLevel::INFO_, __VA_ARGS__)
#define LOG_WARNING_C(...) Logger::getInstance().log_c(Logger::LogLevel::WARNING_, __VA_ARGS__)
#define LOG_ERROR_C(...) Logger::getInstance().log_c(Logger::LogLevel::ERROR_, __VA_ARGS__)
#define LOG_FATAL_C(...) Logger::getInstance().log_c(Logger::LogLevel::FATAL_, __VA_ARGS__)

#define log_cpp(level) Logger::getInstance().log_cpp(level, getpid(), getFileName(__FILE__), __LINE__)

#define LOG_INFO_CPP log_cpp(Logger::LogLevel::INFO_)
#define LOG_WARNING_CPP log_cpp(Logger::LogLevel::WARNING_)
#define LOG_ERROR_CPP log_cpp(Logger::LogLevel::ERROR_)
#define LOG_FATAL_CPP log_cpp(Logger::LogLevel::FATAL_)