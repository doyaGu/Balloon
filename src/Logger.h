#ifndef BALLOON_LOGGER_H
#define BALLOON_LOGGER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Balloon/ILogger.h"
#include "Balloon/RefCount.h"

#define LOG_TRACE(...) ::balloon::Logger::GetDefault()->Trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::balloon::Logger::GetDefault()->Debug(__VA_ARGS__)
#define LOG_INFO(...)  ::balloon::Logger::GetDefault()->Info(__VA_ARGS__)
#define LOG_WARN(...)  ::balloon::Logger::GetDefault()->Warn(__VA_ARGS__)
#define LOG_ERROR(...) ::balloon::Logger::GetDefault()->Error(__VA_ARGS__)
#define LOG_FATAL(...) ::balloon::Logger::GetDefault()->Fatal(__VA_ARGS__)

namespace balloon {
    class Logger final : public ILogger {
    public:
        static Logger *Create(const std::string &id, LogLevel level = LOG_LEVEL_INFO);
        static Logger *Get(const std::string &id);
        static Logger *GetDefault();
        static bool RegisterDefaultLogger(Logger *logger);

        Logger(const Logger &rhs) = delete;
        Logger(Logger &&rhs) noexcept = delete;

        ~Logger() override;

        Logger &operator=(const Logger &rhs) = delete;
        Logger &operator=(Logger &&rhs) noexcept = delete;

        int AddRef() const override;
        int Release() const override;

        const char *GetId() const override { return m_Id.c_str(); }

        void SetLevel(LogLevel level) override {
            if (level >= LOG_LEVEL_TRACE && level <= LOG_LEVEL_OFF)
                m_Level = level;
        }

        LogLevel GetLevel() const override { return m_Level; }

        const char *GetLevelString(LogLevel level) const override;

        void SetLock(LogLockFunction func, void *userdata) override {
            m_Lock = func;
            m_Userdata = userdata;
        }

        bool AddCallback(LogCallback callback, void *userdata, LogLevel level) override;
        void ClearCallbacks() override { m_Callbacks.clear(); }

        void Log(LogLevel level, const char *format, va_list args) override;

    private:
        struct Callback {
            LogCallback callback;
            void *userdata;
            LogLevel level;

            Callback(LogCallback cb, void *data, LogLevel lvl) : callback(cb), userdata(data), level(lvl) {}

            bool operator==(const Callback &rhs) const {
                return callback == rhs.callback &&
                       userdata == rhs.userdata &&
                       level == rhs.level;
            }

            bool operator!=(const Callback &rhs) const {
                return !(rhs == *this);
            }
        };

        Logger(std::string id, LogLevel level);

        void Lock() {
            if (m_Lock) { m_Lock(true, m_Userdata); }
        }

        void Unlock() {
            if (m_Lock) { m_Lock(false, m_Userdata); }
        }

        static void InitLogInfo(LogInfo *log, void *userdata);

        mutable RefCount m_RefCount;
        std::string m_Id;
        LogLevel m_Level;

        LogLockFunction m_Lock = nullptr;
        void *m_Userdata = nullptr;

        std::vector<Callback> m_Callbacks;

        static std::unordered_map<std::string, Logger *> s_Loggers;
        static Logger *s_DefaultLogger;
    };
}

#endif // BALLOON_LOGGER_H