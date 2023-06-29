/**
 * @file ILogger.h
 * @brief The interface of logger.
 */
#ifndef BALLOON_ILOGGER_H
#define BALLOON_ILOGGER_H

#include <cstdarg>
#include <ctime>

namespace balloon {
    inline namespace v1 {
        class ILogger;

        typedef enum LogLevel {
            LOG_LEVEL_TRACE = 0,
            LOG_LEVEL_DEBUG = 1,
            LOG_LEVEL_INFO = 2,
            LOG_LEVEL_WARN = 3,
            LOG_LEVEL_ERROR = 4,
            LOG_LEVEL_FATAL = 5,
            LOG_LEVEL_OFF = 6
        } LogLevel;

        typedef struct LogInfo {
            va_list ap;
            const char *format;
            struct tm *time;
            void *userdata;
            LogLevel level;
        } LogInfo;

        typedef void (*LogCallback)(const ILogger *logger, LogInfo *info);
        typedef void (*LogLockFunction)(bool lock, void *userdata);

        /**
         * @interface ILogger
         * @brief The utility interface of logger.
         * @warning
         * Mod should have no classes implementing this interface.
         */
        class ILogger {
        public:
            virtual int AddRef() const = 0;
            virtual int Release() const = 0;

            virtual const char *GetId() const = 0;

            virtual void SetLevel(LogLevel level) = 0;
            virtual LogLevel GetLevel() const = 0;
            virtual const char* GetLevelString(LogLevel level) const = 0;

            virtual void SetLock(LogLockFunction func, void *userdata) = 0;
            virtual bool AddCallback(LogCallback fn, void *userdata, LogLevel level) = 0;
            virtual void ClearCallbacks() = 0;

            virtual void Log(LogLevel level, const char *format, va_list args) = 0;

            void Trace(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_TRACE, format, args);
                va_end(args);
            }

            void Debug(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_DEBUG, format, args);
                va_end(args);
            }

            void Info(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_INFO, format, args);
                va_end(args);
            }

            void Warn(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_WARN, format, args);
                va_end(args);
            }

            void Error(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_ERROR, format, args);
                va_end(args);
            }

            void Fatal(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_FATAL, format, args);
                va_end(args);
            }

        protected:
            virtual ~ILogger() = default;
        };
    }
}

#endif // BALLOON_ILOGGER_H