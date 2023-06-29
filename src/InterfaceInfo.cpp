#include "InterfaceInfo.h"

#include <cstring>

using namespace balloon;

InterfaceInfo::InterfaceInfo(const char *o, const char *n, int v)
    : owner(o), name(n), version(v), interface(nullptr) {}

InterfaceInfo::InterfaceInfo(const char *o, const char *n, int v, void *i)
    : version(v), interface(i) {
    if (interface) {
        owner = strdup(o);
        name = strdup(n);
    } else {
        owner = o;
        name = n;
    }
}

InterfaceInfo::InterfaceInfo(const InterfaceInfo &rhs) {
    const char *o;
    const char *n;

    if (rhs.interface) {
        o = strdup(rhs.owner);
        n = strdup(rhs.name);
    } else {
        o = rhs.owner;
        n = rhs.name;
    }

    if (interface) {
        free(const_cast<char *>(owner));
        free(const_cast<char *>(name));
    }

    owner = o;
    name = n;
    version = rhs.version;
    interface = rhs.interface;
}

InterfaceInfo::InterfaceInfo(InterfaceInfo &&rhs) noexcept {
    if (interface) {
        free(const_cast<char *>(owner));
        free(const_cast<char *>(name));
    }

    owner = rhs.owner;
    name = rhs.name;
    version = rhs.version;
    interface = rhs.interface;

    rhs.owner = nullptr;
    rhs.name = nullptr;
    rhs.version = -1;
    rhs.interface = nullptr;
}

InterfaceInfo::~InterfaceInfo() {
    if (interface) {
        free(const_cast<char *>(owner));
        free(const_cast<char *>(name));
    }
}

InterfaceInfo &InterfaceInfo::operator=(const InterfaceInfo &rhs) {
    if (this != &rhs) {
        const char *o;
        const char *n;

        if (rhs.interface) {
            o = strdup(rhs.owner);
            n = strdup(rhs.name);
        } else {
            o = rhs.owner;
            n = rhs.name;
        }

        if (interface) {
            free(const_cast<char *>(owner));
            free(const_cast<char *>(name));
        }

        owner = o;
        name = n;
        version = rhs.version;
        interface = rhs.interface;
    }

    return *this;
}

InterfaceInfo &InterfaceInfo::operator=(InterfaceInfo &&rhs) noexcept {
    if (this != &rhs) {
        if (interface) {
            free(const_cast<char *>(owner));
            free(const_cast<char *>(name));
        }

        owner = rhs.owner;
        name = rhs.name;
        version = rhs.version;
        interface = rhs.interface;

        rhs.owner = nullptr;
        rhs.name = nullptr;
        rhs.version = -1;
        rhs.interface = nullptr;
    }
    return *this;
}

bool InterfaceInfo::operator==(const InterfaceInfo &rhs) const {
    return strcmp(owner, rhs.owner) == 0 &&
           strcmp(name, rhs.name) == 0 &&
           version == rhs.version;
}

bool InterfaceInfo::operator!=(const InterfaceInfo &rhs) const {
    return !(rhs == *this);
}

bool InterfaceInfo::operator<(const InterfaceInfo &rhs) const {
    int oc = strcmp(owner, rhs.owner);
    if (oc < 0)
        return true;
    if (oc > 0)
        return false;
    int nc = strcmp(name, rhs.name);
    if (nc < 0)
        return true;
    if (nc > 0)
        return false;
    return version < rhs.version;
}

bool InterfaceInfo::operator>(const InterfaceInfo &rhs) const {
    return rhs < *this;
}

bool InterfaceInfo::operator<=(const InterfaceInfo &rhs) const {
    return !(rhs < *this);
}

bool InterfaceInfo::operator>=(const InterfaceInfo &rhs) const {
    return !(*this < rhs);
}
