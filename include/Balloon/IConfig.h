/**
 * @file IConfig.h
 * @brief The interface of config.
 */
#ifndef BALLOON_ICONFIG_H
#define BALLOON_ICONFIG_H

#include <cstdint>

namespace balloon {
    inline namespace v1 {
        class IConfigSection;
        class IConfigEntry;

        /**
         * @interface IConfig
         * @brief The utility interface of config.
         * @warning Mod should have no classes implementing this interface.
         */
        class IConfig {
        public:
            /**
             * @brief Increase the reference count of the object.
             * @return The new reference count.
             */
            virtual int AddRef() const = 0;

            /**
             * @brief Decrease the reference count of the object.
             * @return The new reference count.
             */
            virtual int Release() const = 0;

            /**
             * @brief Get the ID of the configuration.
             * @return The ID as a null-terminated string.
             */
            virtual const char *GetId() const = 0;

            /**
             * @brief Clear all entries and sections.
             */
            virtual void Clear() = 0;

            /**
             * @brief Clear the default values in the configuration.
             */
            virtual void ClearDefault() = 0;

            /**
             * @brief Get the number of entries in the configuration.
             * @return The number of entries.
             */
            virtual size_t GetNumberOfEntries() const = 0;

            /**
             * @brief Get the number of sections in the configuration.
             * @return The number of sections.
             */
            virtual size_t GetNumberOfSections() const = 0;

            /**
             * @brief Get the total number of entries, including nested sections.
             * @return The number of entries.
             */
            virtual size_t GetNumberOfEntriesRecursive() const = 0;

            /**
             * @brief Get the total number of sections, including nested sections.
             * @return The number of sections.
             */
            virtual size_t GetNumberOfSectionsRecursive() const = 0;

            /**
             * @brief Add a boolean entry to the configuration.
             * @param parent The parent section name or nullptr for the root section.
             * @param name The name of the entry.
             * @param value The boolean value.
             * @return A pointer to the created entry or nullptr on failure.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, bool value) = 0;

            /**
             * @brief Add an unsigned integer entry to the configuration.
             * @param parent The parent section name or nullptr for the root section.
             * @param name The name of the entry.
             * @param value The unsigned integer value.
             * @return A pointer to the created entry or nullptr on failure.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, uint32_t value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (int32_t).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, int32_t value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (uint64_t).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, uint64_t value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (int64_t).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, int64_t value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (float).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, float value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (double).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, double value) = 0;

            /**
             * @brief Add an entry to the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry.
             * @param value The value of the entry (const char*).
             * @return Pointer to the added entry.
             */
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, const char *value) = 0;

            /**
             * @brief Create a new section in the configuration.
             * @param parent The parent section name or nullptr for the root section.
             * @param name The name of the section.
             * @return A pointer to the created section or nullptr on failure.
             */
            virtual IConfigSection *CreateSection(const char *parent, const char *name) = 0;

            /**
             * @brief Remove an entry from the configuration.
             *
             * @param parent The parent name of the entry.
             * @param name The name of the entry to be removed.
             * @return True if the entry is successfully removed, false otherwise.
             */
            virtual bool RemoveEntry(const char *parent, const char *name) = 0;

            /**
             * @brief Remove a section from the configuration.
             *
             * @param parent The parent name of the section.
             * @param name The name of the section to be removed.
             * @return True if the section is successfully removed, false otherwise.
             */
            virtual bool RemoveSection(const char *parent, const char *name) = 0;

            /**
             * @brief Adds a default entry with a boolean value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The boolean value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, bool value) = 0;

            /**
             * @brief Adds a default entry with an unsigned integer value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The unsigned integer value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint32_t value) = 0;

            /**
             * @brief Adds a default entry with a signed integer value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The signed integer value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int32_t value) = 0;

            /**
             * @brief Adds a default entry with a 64-bit unsigned integer value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The 64-bit unsigned integer value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint64_t value) = 0;

            /**
             * @brief Adds a default entry with a 64-bit signed integer value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The 64-bit signed integer value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int64_t value) = 0;

            /**
             * @brief Adds a default entry with a floating-point value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The floating-point value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, float value) = 0;

            /**
             * @brief Adds a default entry with a double precision floating-point value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The double precision floating-point value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, double value) = 0;

            /**
             * @brief Adds a default entry with a string value to the configuration.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the entry.
             * @param value  The string value of the entry.
             * @return       A pointer to the added entry.
             */
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, const char *value) = 0;

            /**
             * @brief Creates a default configuration section within the specified parent section.
             *
             * @param parent The name of the parent section.
             * @param name   The name of the new section to be created.
             * @return       A pointer to the newly created configuration section.
             */
            virtual IConfigSection *CreateDefaultSection(const char *parent, const char *name) = 0;

            /**
             * @brief Retrieves the configuration entry with the specified name.
             *
             * @param name The name of the entry to retrieve.
             * @return     A pointer to the configuration entry if found, nullptr otherwise.
             */
            virtual IConfigEntry *GetEntry(const char *name) const = 0;

            /**
             * @brief Retrieves the configuration section with the specified name.
             *
             * @param name The name of the section to retrieve.
             * @return     A pointer to the configuration section if found, nullptr otherwise.
             */
            virtual IConfigSection *GetSection(const char *name) const = 0;

            /**
             * @brief Checks if the item at the specified index is a configuration entry.
             *
             * @param index The index to check.
             * @return      True if the item at the index is an entry, false otherwise.
             */
            virtual bool IsEntry(size_t index) = 0;

            /**
             * @brief Checks if the item at the specified index is a configuration section.
             *
             * @param index The index to check.
             * @return      True if the item at the index is a section, false otherwise.
             */
            virtual bool IsSection(size_t index) = 0;

            /**
             * @brief Retrieves the configuration entry at the specified index.
             *
             * @param index The index of the entry to retrieve.
             * @return      A pointer to the configuration entry if found, nullptr otherwise.
             */
            virtual IConfigEntry *GetEntry(size_t index) const = 0;

            /**
             * @brief Retrieves the configuration section at the specified index.
             *
             * @param index The index of the section to retrieve.
             * @return      A pointer to the configuration section if found, nullptr otherwise.
             */
            virtual IConfigSection *GetSection(size_t index) const = 0;

            /**
             * @brief Reads the configuration data into the provided buffer.
             *
             * @param buffer The buffer to read the configuration data into.
             * @param len    The length of the buffer.
             * @return       True if the read operation is successful, false otherwise.
             */
            virtual bool Read(char *buffer, size_t len) = 0;

            /**
             * @brief Writes the configuration data and returns it as a character buffer.
             *
             * @param len A pointer to store the length of the returned buffer.
             * @return    A character buffer containing the configuration data.
             */
            virtual char *Write(size_t *len) = 0;

            /**
             * @brief Free the memory allocated by the configuration.
             * @param ptr The memory pointer to free.
             */
            virtual void Free(void *ptr) const = 0;

        protected:
            virtual ~IConfig() = default;
        };

        /**
         * @brief Enumeration of configuration callback types.
         *
         * This enumeration defines the types of configuration callback events.
         */
        enum ConfigCallbackType {
            CFG_CB_MODIFY, /**< Configuration modify event type. */
        };

        /**
         * @brief Configuration callback function pointer type.
         *
         * This typedef declares a function pointer type ConfigCallback,
         * which takes an IConfigSection pointer, an IConfigEntry pointer,
         * and a void* argument.
         */
        typedef void (*ConfigCallback)(IConfigSection *section, IConfigEntry *entry, void *arg);

        /**
         * @class IConfigSection
         * @brief The interface for configuration sections.
         *
         * A configuration section represents a logical grouping of configuration entries and subsections.
         */
        class IConfigSection {
        public:
            /**
             * @brief Get the name of the configuration section.
             *
             * @return The name of the configuration section.
             */
            virtual const char *GetName() const = 0;

            /**
             * @brief Get the parent section of the current section.
             *
             * @return Pointer to the parent section.
             */
            virtual IConfigSection *GetParent() const = 0;

            /**
             * @brief Clear all entries and subsections in the section.
             */
            virtual void Clear() = 0;

            /**
             * @brief Get the number of entries in the section.
             *
             * @return The number of entries in the section.
             */
            virtual size_t GetNumberOfEntries() const = 0;

            /**
             * @brief Get the number of subsections in the section.
             *
             * @return The number of subsections in the section.
             */
            virtual size_t GetNumberOfSections() const = 0;

            /**
             * @brief Get the total number of entries in the section and its subsections recursively.
             *
             * @return The total number of entries in the section and its subsections.
             */
            virtual size_t GetNumberOfEntriesRecursive() const = 0;

            /**
             * @brief Get the total number of subsections in the section and its subsections recursively.
             *
             * @return The total number of subsections in the section and its subsections.
             */
            virtual size_t GetNumberOfSectionsRecursive() const = 0;

            /**
             * @brief Adds a boolean entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The boolean value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, bool value) = 0;

            /**
             * @brief Adds an unsigned 32-bit integer entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The unsigned 32-bit integer value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, uint32_t value) = 0;

            /**
             * @brief Adds a signed 32-bit integer entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The signed 32-bit integer value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, int32_t value) = 0;

            /**
             * @brief Adds an unsigned 64-bit integer entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The unsigned 64-bit integer value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, uint64_t value) = 0;

            /**
             * @brief Adds a signed 64-bit integer entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The signed 64-bit integer value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, int64_t value) = 0;

            /**
             * @brief Adds a floating-point entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The floating-point value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, float value) = 0;

            /**
             * @brief Adds a double precision floating-point entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The double precision floating-point value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, double value) = 0;

            /**
             * @brief Adds a string entry to the configuration section.
             *
             * @param name The name of the entry.
             * @param value The string value of the entry.
             * @return Pointer to the added config entry.
             */
            virtual IConfigEntry *AddEntry(const char *name, const char *value) = 0;

            /**
              * @brief Creates a new subsection in the configuration section.
              *
              * @param name The name of the subsection.
              * @return Pointer to the created config section.
              */
            virtual IConfigSection *CreateSection(const char *name) = 0;

            /**
             * @brief Removes an entry from the configuration section.
             *
             * @param name The name of the entry to be removed.
             * @return True if the removal was successful, false otherwise.
             */
            virtual bool RemoveEntry(const char *name) = 0;

            /**
             * @brief Removes a subsection from the configuration section.
             *
             * @param name The name of the subsection to be removed.
             * @return True if the removal was successful, false otherwise.
             */
            virtual bool RemoveSection(const char *name) = 0;

            /**
             * @brief Retrieves a config entry by its name.
             *
             * @param name The name of the entry.
             * @return Pointer to the config entry if found, nullptr otherwise.
             */
            virtual IConfigEntry *GetEntry(const char *name) const = 0;

            /**
             * @brief Retrieves a config section by its name.
             *
             * @param name The name of the section.
             * @return Pointer to the config section if found, nullptr otherwise.
             */
            virtual IConfigSection *GetSection(const char *name) const = 0;

            /**
             * @brief Checks if an item at the given index is a config entry.
             *
             * @param index The index to check.
             * @return True if the item is a config entry, false if not or index out of range.
             */
            virtual bool IsEntry(size_t index) = 0;

            /**
             * @brief Checks if an item at the given index is a config section.
             *
             * @param index The index to check.
             * @return True if the item is a config section, false if not or index out of range.
             */
            virtual bool IsSection(size_t index) = 0;

            /**
             * @brief Retrieves a config entry by its index.
             *
             * @param index The index of the entry.
             * @return Pointer to the config entry if found, nullptr otherwise.
             */
            virtual IConfigEntry *GetEntry(size_t index) const = 0;

            /**
             * @brief Retrieves a config section by its index.
             *
             * @param index The index of the section.
             * @return Pointer to the config section if found, nullptr otherwise.
             */
            virtual IConfigSection *GetSection(size_t index) const = 0;

            /**
             * @brief Adds a callback function to be triggered for the specified event type.
             *
             * @param type The type of configuration callback event.
             * @param callback The callback function to be invoked.
             * @param arg An additional argument to be passed to the callback function.
             * @return `true` if the callback was added successfully, `false` otherwise.
             */
            virtual bool AddCallback(ConfigCallbackType type, ConfigCallback callback, void *arg) = 0;

            /**
             * @brief Clears all callbacks registered for the specified event type.
             *
             * @param type The type of configuration callback event.
             */
            virtual void ClearCallbacks(ConfigCallbackType type) = 0;
        };

        /**
         * @brief Enumeration of configuration entry types.
         *
         * This enumeration defines the available types for configuration entries.
         */
        enum EntryType {
            CFG_ENTRY_NONE, /**< No entry type. */
            CFG_ENTRY_BOOL, /**< Boolean entry type. */
            CFG_ENTRY_UINT, /**< Unsigned integer entry type. */
            CFG_ENTRY_INT, /**< Signed integer entry type. */
            CFG_ENTRY_REAL, /**< Real number entry type. */
            CFG_ENTRY_STR, /**< String entry type. */
        };

        /**
         * @brief Interface for a configuration entry.
         *
         * This interface provides methods to retrieve and set values for a configuration entry.
         */
        class IConfigEntry {
        public:
            /**
             * @brief Gets the name of the configuration entry.
             *
             * @return The name of the configuration entry.
             */
            virtual const char *GetName() const = 0;

            /**
             * @brief Gets the parent configuration section of the entry.
             *
             * @return The parent configuration section of the entry.
             */
            virtual IConfigSection *GetParent() const = 0;

            /**
             * @brief Gets the type of the configuration entry.
             *
             * @return The type of the configuration entry.
             */
            virtual EntryType GetType() const = 0;

            /**
             * @brief Gets the boolean value of the configuration entry.
             *
             * This method returns the boolean value of the configuration entry.
             *
             * @return The boolean value of the configuration entry.
             */
            virtual bool GetBool() = 0;

            /**
             * @brief Gets the unsigned integer value of the configuration entry.
             *
             * This method returns the unsigned integer value of the configuration entry.
             *
             * @return The unsigned integer value of the configuration entry.
             */
            virtual uint32_t GetUint32() = 0;

            /**
             * @brief Gets the signed integer value of the configuration entry.
             *
             * This method returns the signed integer value of the configuration entry.
             *
             * @return The signed integer value of the configuration entry.
             */
            virtual int32_t GetInt32() = 0;

            /**
             * @brief Gets the unsigned 64-bit integer value of the configuration entry.
             *
             * This method returns the unsigned 64-bit integer value of the configuration entry.
             *
             * @return The unsigned 64-bit integer value of the configuration entry.
             */
            virtual uint64_t GetUint64() = 0;

            /**
             * @brief Gets the signed 64-bit integer value of the configuration entry.
             *
             * This method returns the signed 64-bit integer value of the configuration entry.
             *
             * @return The signed 64-bit integer value of the configuration entry.
             */
            virtual int64_t GetInt64() = 0;

            /**
             * @brief Gets the floating-point value of the configuration entry.
             *
             * This method returns the floating-point value of the configuration entry.
             *
             * @return The floating-point value of the configuration entry.
             */
            virtual float GetFloat() = 0;

            /**
             * @brief Gets the double precision floating-point value of the configuration entry.
             *
             * This method returns the double precision floating-point value of the configuration entry.
             *
             * @return The double precision floating-point value of the configuration entry.
             */
            virtual double GetDouble() = 0;

            /**
             * @brief Gets the string value of the configuration entry.
             *
             * This method returns the string value of the configuration entry.
             *
             * @return The string value of the configuration entry.
             */
            virtual const char *GetString() const = 0;

            /**
             * @brief Set the value as a boolean.
             *
             * @param value The boolean value to set.
             */
            virtual void SetValue(bool value) = 0;

            /**
             * @brief Set the value as an unsigned 32-bit integer.
             *
             * @param value The unsigned 32-bit integer value to set.
             */
            virtual void SetValue(uint32_t value) = 0;

            /**
             * @brief Set the value as a signed 32-bit integer.
             *
             * @param value The signed 32-bit integer value to set.
             */
            virtual void SetValue(int32_t value) = 0;

            /**
             * @brief Set the value as an unsigned 64-bit integer.
             *
             * @param value The unsigned 64-bit integer value to set.
             */
            virtual void SetValue(uint64_t value) = 0;

            /**
             * @brief Set the value as a signed 64-bit integer.
             *
             * @param value The signed 64-bit integer value to set.
             */
            virtual void SetValue(int64_t value) = 0;

            /**
             * @brief Set the value as a single-precision floating-point number.
             *
             * @param value The single-precision floating-point value to set.
             */
            virtual void SetValue(float value) = 0;

            /**
             * @brief Set the value as a double-precision floating-point number.
             *
             * @param value The double-precision floating-point value to set.
             */
            virtual void SetValue(double value) = 0;

            /**
             * @brief Set the value as a null-terminated string.
             *
             * @param value The null-terminated string value to set.
             */
            virtual void SetValue(const char *value) = 0;

            /**
             * @brief Copies the value of another configuration entry.
             *
             * This method copies the value of another configuration entry to this entry.
             *
             * @param entry The configuration entry to copy the value from.
             */
            virtual void CopyValue(IConfigEntry *entry) = 0;
        };
    }
}

#endif // BALLOON_ICONFIG_H