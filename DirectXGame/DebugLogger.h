#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

class DebugLogger {
public:
    static DebugLogger& GetInstance() {
        static DebugLogger instance;
        return instance;
    }

    void Log(const std::string& message) {
        std::ofstream file("debug_log.txt", std::ios::app);
        if (file.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);

            // 使用安全的 localtime_s 替代 localtime
            std::tm timeInfo;
            localtime_s(&timeInfo, &time_t);

            file << "[" << std::put_time(&timeInfo, "%H:%M:%S") << "] "
                << message << std::endl;
            file.close();
        }
    }

private:
    DebugLogger() {
        // 清空之前的日志文件
        std::ofstream file("debug_log.txt");
        if (file.is_open()) {
            file << "=== 调试日志开始 ===" << std::endl;
            file.close();
        }
    }

    ~DebugLogger() = default;
    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;
};

// 简化使用的宏
#define LOG(message) DebugLogger::GetInstance().Log(message)