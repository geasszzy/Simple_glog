#include "log.h"

Logger::Logger(std::string& logPath) {
    logPath_ = logPath;
    if (!createDirectory(logPath_)) {
        std::cout << "Failed to create directory: " << logPath_ << std::endl;
    }
}

Logger::~Logger() {
    for (auto& pair : logFiles_) {
        if (pair.second.is_open()) {
            pair.second.close();
        }
    }
}

std::string Logger::getTimestamp(const char* format) {
    std::time_t t = std::time(nullptr);
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&t, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &t);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&t);
#endif
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), format, &bt);
    return buffer;
}

std::string Logger::getLogLevelName(const LogLevel& level) {
    switch (level) {
        case LogLevel::INFO_: return "INFO";
        case LogLevel::WARNING_: return "WARNING";
        case LogLevel::ERROR_: return "ERROR";
        case LogLevel::FATAL_: return "FATAL";
    }
    return "UNKOWN";
}

std::string Logger::getLogLevelPrefix(const LogLevel& level) {
    switch (level) {
        case LogLevel::INFO_: return "I";
        case LogLevel::WARNING_: return "W";
        case LogLevel::ERROR_: return "E";
        case LogLevel::FATAL_: return "F";
    }
    return "U";
}

std::ofstream& Logger::getLogFile(const LogLevel& level) {
    if (logFiles_.find(level) == logFiles_.end()) {
#if defined(__unix__)
        std::string fileName = logPath_ + "/" + getLogLevelName(level) + "_" + getTimestamp("%Y%m%d-%H%M%S") + "." + std::to_string(getpid()) + ".log";
#else
        std::string fileName = logPath_ + "\\" + getLogLevelName(level) + "_" + getTimestamp("%Y%m%d-%H%M%S") + "." + std::to_string(getpid()) + ".log";
#endif
        std::cout << "creating log file: " << fileName << std::endl;
        logFiles_[level].open(fileName, std::ios::app);
        if (logFiles_[level].is_open()) {
            logFiles_[level] << "Log file created at: " << getTimestamp("%Y%m%d %H:%M:%S") << std::endl;
            logFiles_[level] << "Running on machine:  " << getComputerName() << std::endl;
            logFiles_[level] << "Log line format : [IWEF] mmdd hh : mm:ss process_id file : line] msg" << std::endl;
        }
        else
        {
            std::cout << "creat fail, no such directory!!!" << std::endl;
        }
    }
    return logFiles_[level];
}