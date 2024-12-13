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

        /**
         * @brief Enumeration representing different log levels.
         */
        typedef enum LogLevel {
            LOG_LEVEL_TRACE = 0,    /**< Lowest log level for tracing detailed operations. */
            LOG_LEVEL_DEBUG = 1,    /**< Log level for debugging information. */
            LOG_LEVEL_INFO = 2,     /**< Log level for informational messages. */
            LOG_LEVEL_WARN = 3,     /**< Log level for warning messages. */
            LOG_LEVEL_ERROR = 4,    /**< Log level for error messages. */
            LOG_LEVEL_FATAL = 5,    /**< Log level for critical/fatal errors. */
            LOG_LEVEL_OFF = 6       /**< Highest log level, turns off logging completely. */
        } LogLevel;

        /**
         * @brief Structure representing log information.
         */
        typedef struct LogInfo {
            ILogger *self;      /**< Pointer to the logger itself. */
            va_list ap;         /**< Variable argument list for formatting log message. */
            const char *format; /**< Format string for log message. */
            struct tm *time;    /**< Pointer to a structure holding date and time information. */
            void *userdata;     /**< User-defined data associated with the log message. */
            LogLevel level;     /**< Log level of the message. */
        } LogInfo;

        /**
         * @brief Function pointer type for log callbacks.
         *
         * This function is responsible for handling log messages.
         *
         * @param info Pointer to the log information structure.
         */
        typedef void (*LogCallback)(LogInfo *info);

        /**
         * @brief Function pointer type for log lock functions.
         *
         * This function is responsible for locking or unlocking resources used by the logging system.
         *
         * @param lock If true, lock the resources. If false, unlock the resources.
         * @param userdata User-defined data associated with the logging system.
         */
        typedef void (*LogLockFunction)(bool lock, void *userdata);

        /**
         * @interface ILogger
         * @brief The utility interface of logger.
         * @warning Mod should have no classes implementing this interface.
         */
        class ILogger {
        public:
            /**
             * @brief Increase the reference count of the logger object.
             * @return The new reference count.
             */
            virtual int AddRef() const = 0;

            /**
             * @brief Decrease the reference count of the logger object.
             * @return The new reference count.
             */
            virtual int Release() const = 0;

            /**
             * @brief Get the ID of the logger.
             * @return The ID of the logger.
             */
            virtual const char *GetId() const = 0;

            /**
             * @brief Set the log level for the logger.
             * @param level The log level to set.
             */
            virtual void SetLevel(LogLevel level) = 0;

            /**
             * @brief Get the current log level of the logger.
             * @return The current log level of the logger.
             */
            virtual LogLevel GetLevel() const = 0;

            /**
             * @brief Get the string representation of a log level.
             * @param level The log level to get the string representation of.
             * @return The string representation of the log level.
             */
            virtual const char *GetLevelString(LogLevel level) const = 0;

            /**
             * @brief Set the lock function and userdata for thread synchronization.
             * @param func The lock function to set.
             * @param userdata The userdata to set.
             */
            virtual void SetLock(LogLockFunction func, void *userdata) = 0;

            /**
             * @brief Add a log callback function with userdata and log level.
             * @param fn The log callback function to add.
             * @param userdata The userdata to pass to the log callback function.
             * @param level The log level at which the callback should be triggered.
             * @return True if the callback was added successfully, false otherwise.
             */
            virtual bool AddCallback(LogCallback fn, void *userdata, LogLevel level) = 0;

            /**
             * @brief Clear all log callbacks.
             */
            virtual void ClearCallbacks() = 0;

            /**
             * @brief Log a message with the specified log level, format, and arguments.
             * @param level The log level of the message.
             * @param format The format string of the message.
             * @param args The variable arguments list for the format string.
             */
            virtual void Log(LogLevel level, const char *format, va_list args) = 0;

            /**
             * @brief Log a trace message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
            void Trace(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_TRACE, format, args);
                va_end(args);
            }

            /**
             * @brief Log a debug message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
            void Debug(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_DEBUG, format, args);
                va_end(args);
            }

            /**
             * @brief Log an info message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
            void Info(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_INFO, format, args);
                va_end(args);
            }

            /**
             * @brief Log a warning message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
            void Warn(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_WARN, format, args);
                va_end(args);
            }

            /**
             * @brief Log an error message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
            void Error(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Log(LOG_LEVEL_ERROR, format, args);
                va_end(args);
            }

            /**
             * @brief Log a fatal error message with the specified format and arguments.
             * @param format The format string of the message.
             * @param ... The variable arguments list for the format string.
             */
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