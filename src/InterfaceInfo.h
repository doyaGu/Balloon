#ifndef BALLOON_INTERFACEINFO_H
#define BALLOON_INTERFACEINFO_H

#include <cstdint>
#include <utility>

namespace balloon {
    struct InterfaceInfo {
        const char *owner;
        const char *name;
        int version;
        void *interface;

        InterfaceInfo(const char *o, const char *n, int v);
        InterfaceInfo(const char *o, const char *n, int v, void *i);

        InterfaceInfo(const InterfaceInfo &rhs);
        InterfaceInfo(InterfaceInfo &&rhs) noexcept;

        ~InterfaceInfo();

        InterfaceInfo &operator=(const InterfaceInfo &rhs);
        InterfaceInfo &operator=(InterfaceInfo &&rhs) noexcept;

        bool operator==(const InterfaceInfo &rhs) const;
        bool operator!=(const InterfaceInfo &rhs) const;

        bool operator<(const InterfaceInfo &rhs) const;
        bool operator>(const InterfaceInfo &rhs) const;
        bool operator<=(const InterfaceInfo &rhs) const;
        bool operator>=(const InterfaceInfo &rhs) const;
    };
}

namespace std {
    // BKDR hash function for InterfaceInfo
    template<> struct hash<balloon::InterfaceInfo> {
        size_t operator() (const balloon::InterfaceInfo &info) const {
            size_t hash = 0;

            if (info.owner) {
                const char *owner = info.owner;
                while (uint8_t ch = *owner++) {
                    hash = hash * 131 + ch;
                }
            }

            if (info.name) {
                const char *name = info.name;
                while (uint8_t ch = *name++) {
                    hash = hash * 131 + ch;
                }
            }

            return hash * 131 + info.version;
        }
    };
}

#endif // BALLOON_INTERFACEINFO_H
