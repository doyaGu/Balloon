#ifndef BALLOON_BALLOON_H
#define BALLOON_BALLOON_H

#include <memory>
#include <list>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "ModLoader.h"
#include "ModContext.h"
#include "ModRegistry.h"
#include "Config.h"

namespace balloon {
        class Balloon final {
        public:
            enum Flag {
                BALLOON_INITED = 0x00000001,

                BALLOON_MODS_LOADED = 0x00000010,
                BALLOON_MODS_INITED = 0x00000020,
                BALLOON_MODS_CONNECTED = 0x00000040,

                BALLOON_LOGGER_INITED = 0x00000100,
            };

            static Balloon &GetInstance();

            Balloon(const Balloon &rhs) = delete;
            Balloon(Balloon &&rhs) noexcept = delete;

            ~Balloon();

            Balloon &operator=(const Balloon &rhs) = delete;
            Balloon &operator=(Balloon &&rhs) noexcept = delete;

            bool IsInited() const { return (m_Flag & BALLOON_INITED) != 0; }
            bool Init();
            void Shutdown();

            bool AreModsLoaded() const { return (m_Flag & BALLOON_MODS_LOADED) != 0; }
            bool LoadMods();
            void UnloadMods();

            bool AreModsInited() const { return (m_Flag & BALLOON_MODS_INITED) != 0; }
            bool InitMods();
            void ShutdownMods();

            bool AreModsConnected() const { return (m_Flag & BALLOON_MODS_CONNECTED) != 0; }
            bool ConnectMods();
            void DisconnectMods();

            void OnProcess();
            void OnGUI();

            ModContext &GetContext() const { return *m_Context; }

            HMODULE GetDllHandle() const { return m_DllHandle; }
            void SetDllHandle(HMODULE handle) { m_DllHandle = handle; }

            HWND GetWindowHandle() const { return m_WindowHandle; }
            void SetWindowHandle(HWND handle) { m_WindowHandle = handle; }

        private:
            Balloon();

            bool InitFileSystem();
            bool ShutdownFileSystem();

            bool IsLoggerInited() const { return (m_Flag & BALLOON_LOGGER_INITED) != 0; }
            void InitLogger();
            void ShutdownLogger();
            void CreateLogFile(ILogger *logger);

            bool LoadConfig(IConfig *config, const std::string &path);
            bool SaveConfig(IConfig *config, const std::string &path);

            bool LoadModConfig(const std::shared_ptr<ModContainer> &mod);
            bool SaveModConfig(const std::shared_ptr<ModContainer> &mod);

            void RegisterBuiltinInterfaces();
            void RegisterBuiltinFactories();

            int m_Flag = 0;
            HMODULE m_DllHandle = nullptr;
            HWND m_WindowHandle = nullptr;
            std::vector<FILE *> m_LogFiles;
            IConfig *m_Config = nullptr;

            std::shared_ptr<ModRegistry> m_Registry;
            std::shared_ptr<ModLoader> m_Loader;
            std::shared_ptr<ModContext> m_Context;

            std::vector<IMod *> m_ModsOnUpdate;
            std::vector<IMod *> m_ModsOnLateUpdate;
            std::vector<IMod *> m_ModsOnGUI;
        };
}

#endif // BALLOON_BALLOON_H
