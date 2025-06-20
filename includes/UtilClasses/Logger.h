#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "Defaults/config.h"

// if someone wishes to log something using the Logger class
// they either pass a MESSAGE Macro or a 
#define MESSAGE(msg) LoggerMessage(__FILE__ , __LINE__ , msg)

struct LoggerMessage {
    LoggerMessage(std::string fileName, int lineNumber, std::string message)
    {
        std::stringstream formatedMessage;

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        formatedMessage << "[" << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << "] ";

        formatedMessage << fileName << ":" << lineNumber << " " << message;
        m_message = formatedMessage.str();

    };
    std::string m_message;
};

class Logger {
public:
    
    static void info(LoggerMessage, std::string=BLUE);
    static void succes(LoggerMessage, std::string=GREEN);
    static void warning(LoggerMessage, std::string=ORANGE);
    static void error(LoggerMessage, std::string=RED);
    static void print(std::string);

    static const bool m_colorsAvailable = true;

private:
    static void debugMessage(std::string, std::string);
    Logger();
};