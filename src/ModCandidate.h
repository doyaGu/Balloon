#ifndef BALLOON_MODCANDIDATE_H
#define BALLOON_MODCANDIDATE_H

#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "ModMetadata.h"

namespace balloon {
    class ModCandidate {
    public:
        ModCandidate();
        ModCandidate(std::string path, std::shared_ptr<ModMetadata> metadata);

        ModCandidate(const ModCandidate &rhs) = default;
        ModCandidate(ModCandidate &&rhs) noexcept = default;

        ~ModCandidate() = default;

        ModCandidate &operator=(const ModCandidate &rhs) = default;
        ModCandidate &operator=(ModCandidate &&rhs) noexcept = default;

        bool operator==(const ModCandidate &rhs) const;
        bool operator!=(const ModCandidate &rhs) const;

        bool operator<(const ModCandidate &rhs) const;
        bool operator>(const ModCandidate &rhs) const;
        bool operator<=(const ModCandidate &rhs) const;
        bool operator>=(const ModCandidate &rhs) const;

        bool IsValid() const;

        const char *GetId() const;
        const char *GetVersion() const;

        bool IsBuiltin() const;

        std::string GetPath() const {
            return m_Path;
        }

        void SetPath(const std::string &path) {
            m_Path = path;
        }

        std::vector<std::shared_ptr<ModDependency>> GetDependencies() const {
            return std::move(m_Metadata->GetDependencies());
        }

        std::shared_ptr<ModMetadata> GetMetadata() const {
            return m_Metadata;
        }

    private:
        std::string m_Path;
        std::shared_ptr<ModMetadata> m_Metadata;
    };

    typedef std::vector<ModCandidate> ModCandidateArray;
    typedef std::list<ModCandidate> ModCandidateList;
    typedef std::unordered_set<ModCandidate> ModCandidateSet;
    typedef std::unordered_map<std::string, ModCandidate> ModCandidateMap;
    typedef std::unordered_map<std::string, ModCandidateSet> ModCandidateSetMap;
}

namespace std {
    // BKDR hash function for ModCandidate
    template<> struct hash<balloon::ModCandidate> {
        size_t operator() (const balloon::ModCandidate &candidate) const {
            size_t hash = 0;

            if (candidate.GetId()) {
                const char *id = candidate.GetId();
                while (uint8_t ch = *id++) {
                    hash = hash * 131 + ch;
                }
            }

            if (candidate.GetVersion()) {
                const char *version = candidate.GetVersion();
                while (uint8_t ch = *version++) {
                    hash = hash * 131 + ch;
                }
            }

            return hash;
        }
    };
}

#endif // BALLOON_MODCANDIDATE_H
