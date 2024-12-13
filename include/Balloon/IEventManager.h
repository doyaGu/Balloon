/**
 * @file IEventManager.h
 * @brief The interface of event manager.
 */
#ifndef BALLOON_IEVENTMANAGER_H
#define BALLOON_IEVENTMANAGER_H

#include "Balloon/IEvent.h"

namespace balloon {
    inline namespace v1 {
        /**
         * @brief Interface for event listeners.
         *
         * This class represents an interface for event listeners. EventManager calls OnEvent if an event has occurred.
         */
        class IEventListener {
        public:
            /**
             * @brief Called by EventManager when an event occurs.
             *
             * @param event Pointer to the event that occurred.
             * @return An integer representing the result of the event handling.
             */
            virtual int OnEvent(const IEvent *event) = 0;
        };

        /**
         * @brief Interface for event management.
         *
         * This class represents an interface for managing events and event types.
         */
        class IEventManager {
        public:
            // Event Type

            /**
             * @brief Add a new event type with the given name.
             *
             * @param name The name of the event type.
             * @return The event type identifier associated with the added event type.
             */
            virtual EventType AddEventType(const char *name) = 0;

            /**
             * @brief Get the event type identifier associated with the given event type name.
             *
             * @param name The name of the event type.
             * @return The event type identifier.
             */
            virtual EventType GetEventType(const char *name) const = 0;

            /**
             * @brief Get the name of the event type associated with the given event type identifier.
             *
             * @param type The event type identifier.
             * @return The name of the event type as a null-terminated string.
             */
            virtual const char *GetEventTypeName(EventType type) const = 0;

            /**
             * @brief Get the total number of registered event types.
             *
             * @return The count of registered event types.
             */
            virtual size_t GetEventTypeCount() const = 0;

            /**
             * @brief Rename an existing event type with the given name.
             *
             * @param type The event type identifier to rename.
             * @param name The new name for the event type.
             * @return True if the renaming was successful, false otherwise.
             */
            virtual bool RenameEventType(EventType type, const char *name) = 0;

            /**
             * @brief Rename an existing event type with the given new name.
             *
             * @param oldName The current name of the event type.
             * @param newName The new name for the event type.
             * @return True if the renaming was successful, false otherwise.
             */
            virtual bool RenameEventType(const char *oldName, const char *newName) = 0;

            // Event

            /**
             * @brief Create a new event instance with the specified event type.
             *
             * @param type The event type identifier for the new event.
             * @return A pointer to the newly created event.
             */
            virtual IEvent *NewEvent(EventType type) = 0;

            /**
             * @brief Create a new event instance with the specified event type name.
             *
             * @param name The name of the event type for the new event.
             * @return A pointer to the newly created event.
             */
            virtual IEvent *NewEvent(const char *name) = 0;

            /**
             * @brief Send the given event to all appropriate listeners.
             *
             * @param event Pointer to the event to be sent.
             * @return True if the event was sent successfully, false otherwise.
             */
            virtual bool SendEvent(IEvent *event) = 0;

            /**
             * @brief Send an event of the specified event type to all appropriate listeners.
             *
             * @param type The event type identifier.
             * @return True if the event was sent successfully, false otherwise.
             */
            virtual bool SendEvent(EventType type) = 0;

            /**
             * @brief Send an event with the specified event type name to all appropriate listeners.
             *
             * @param name The name of the event type.
             * @return True if the event was sent successfully, false otherwise.
             */
            virtual bool SendEvent(const char *name) = 0;

            // Listener

            /**
             * @brief Add a listener for the specified event type.
             *
             * @param eventType The event type identifier to listen for.
             * @param listener Pointer to the listener object to be added.
             * @return True if the listener was added successfully, false otherwise.
             */
            virtual bool AddListener(EventType eventType, IEventListener *listener) = 0;

            /**
             * @brief Add a listener for the specified event name.
             *
             * @param eventName The name of the event to listen for.
             * @param listener Pointer to the listener object to be added.
             * @return True if the listener was added successfully, false otherwise.
             */
            virtual bool AddListener(const char *eventName, IEventListener *listener) = 0;

            /**
             * @brief Remove a listener for the specified event type.
             *
             * @param eventType The event type identifier to remove the listener from.
             * @param listener Pointer to the listener object to be removed.
             * @return True if the listener was removed successfully, false otherwise.
             */
            virtual bool RemoveListener(EventType eventType, IEventListener *listener) = 0;

            /**
             * @brief Remove a listener for the specified event name.
             *
             * @param eventName The name of the event to remove the listener from.
             * @param listener Pointer to the listener object to be removed.
             * @return True if the listener was removed successfully, false otherwise.
             */
            virtual bool RemoveListener(const char *eventName, IEventListener *listener) = 0;
        };
    }
}

#endif // BALLOON_IEVENTMANAGER_H
