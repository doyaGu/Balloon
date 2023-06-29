/**
 * @file IDataShare.h
 * @brief The interface for sharing data between mods.
 */
#ifndef BALLOON_IDATASHARE_H
#define BALLOON_IDATASHARE_H

namespace balloon {
    inline namespace v1 {
        typedef void(*DataShareCallback)(const char *key, void *data, void *userdata);

        class IDataShare {
        public:
            virtual void Request(const char *key, DataShareCallback callback, void *userdata) const = 0;

            virtual void *Get(const char *key) const = 0;
            virtual void *Set(const char *key, void *data) = 0;
            virtual void *Insert(const char *key, void *data) = 0;
            virtual void *Remove(const char *key) = 0;

            virtual void *GetUserData(size_t type = 0) const = 0;
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IDATASHARE_H
