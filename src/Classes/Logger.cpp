#include "UtilClasses/Logger.h"

Logger::Logger() {};

void Logger::info(LoggerMessage message, std::string color)
{
    debugMessage("[INFO]" + message.m_message, color);
};

void Logger::warning(LoggerMessage message, std::string color)
{
    debugMessage("[WARNING]" + message.m_message, color);
};

void Logger::succes(LoggerMessage message, std::string color)
{
    debugMessage("[SUCCES]" + message.m_message, color);
};

void Logger::error(LoggerMessage message, std::string color)
{
    debugMessage("[ERROR]" + message.m_message, color);
};

void Logger::debugMessage(std::string message, std::string color)
{
    if (m_colorsAvailable)
    {
        print(color + message + RESET_COLOR);
    }
    else
    {
        print(message);
    }
};

void Logger::print(std::string message)
{
    std::cout << message << std::endl;
};




