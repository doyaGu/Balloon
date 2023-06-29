#include "Logger.h"

#include <utility>

using namespace balloon;

static const char *g_LevelStrings[6] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static void StdoutCallback(const ILogger *self, LogInfo *info) {
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", info->time)] = '\0';

    FILE *fp = (FILE *) info->userdata;
    fprintf(fp, "[%s] [%s/%s]: ", buf, self->GetId(), g_LevelStrings[info->level]);

    vfprintf(fp, info->format, info->ap);
    fprintf(fp, "\n");
    fflush(fp);
}

std::unordered_map<std::string, Logger *> Logger::s_Loggers;
Logger *Logger::s_DefaultLogger = nullptr;

Logger *Logger::Create(const std::string &id, LogLevel level) {
    return new Logger(id, level);
}

Logger *Logger::Get(const std::string &id) {
    Logger *logger;

    if (id.empty()) {
        logger = GetDefault();
        logger->AddRef();
        return logger;
    }

    auto it = s_Loggers.find(id);
    if (it == s_Loggers.end())
        return Create(id);

    logger = it->second;
    logger->AddRef();
    return logger;
}

Logger *Logger::GetDefault() {
    if (!s_DefaultLogger)
        RegisterDefaultLogger(Create("Balloon"));
    return s_DefaultLogger;
}

bool Logger::RegisterDefaultLogger(Logger *logger) {
    if (!logger)
        return false;
    s_DefaultLogger = logger;
    return true;
}

Logger::~Logger() {
    s_Loggers.erase(GetId());
}

int Logger::AddRef() const {
    return m_RefCount.AddRef();
}

int Logger::Release() const {
    int r = m_RefCount.Release();
    if (r == 0) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete const_cast<Logger *>(this);
    }
    return r;
}

const char *Logger::GetLevelString(LogLevel level) const {
    if (level < LOG_LEVEL_TRACE || level > LOG_LEVEL_FATAL)
        return nullptr;
    return g_LevelStrings[level];
}

bool Logger::AddCallback(LogCallback callback, void *userdata, LogLevel level) {
    if (!callback)
        return false;

    Callback cb(callback, userdata, level);
    auto it = std::find(m_Callbacks.begin(), m_Callbacks.end(), cb);
    if (it != m_Callbacks.end())
        return false;

    m_Callbacks.emplace_back(cb);
    return true;
}

void Logger::Log(LogLevel level, const char *format, va_list args) {
    LogInfo info = {args, format, nullptr, nullptr, level};

    Lock();

    if (level >= m_Level && level < LOG_LEVEL_OFF) {
        InitLogInfo(&info, stdout);
        StdoutCallback(this, &info);
    }

    for (auto it = m_Callbacks.begin(); it != m_Callbacks.end() && it->callback; it++) {
        if (level >= it->level && level < LOG_LEVEL_OFF) {
            InitLogInfo(&info, it->userdata);
            it->callback(this, &info);
        }
    }

    Unlock();
}

Logger::Logger(std::string id, LogLevel level) : m_Id(std::move(id)), m_Level(level) {
    AddRef();
    s_Loggers[m_Id] = this;
}

void Logger::InitLogInfo(LogInfo *log, void *userdata) {
    if (!log->time) {
        time_t t = time(nullptr);
        log->time = localtime(&t);
    }
    log->userdata = userdata;
}