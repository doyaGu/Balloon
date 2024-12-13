/**
 * @file IWeakRefFlag.h
 * @brief The interface of week reference flag
 */
#ifndef BALLOON_IWEAKREFFLAG_H
#define BALLOON_IWEAKREFFLAG_H

namespace balloon {
    inline namespace v1 {
        /**
         * @interface IWeakRefFlag
         * @brief The interface of week reference flag
         */
        class IWeakRefFlag {
        public:
            // Memory management

            /**
             * \brief Increase the reference count of the object.
             * \return The new reference count.
             */
            virtual int AddRef() const = 0;

            /**
             * \brief Decrease the reference count of the object.
             * \return The new reference count.
             */
            virtual int Release() const = 0;

            // Value

            /**
             * \brief Get the value of the weak reference flag.
             * \return The value of the weak reference flag.
             */
            virtual bool Get() const = 0;

            /**
             * \brief Set the value of the weak reference flag.
             * \param value The value to set.
             */
            virtual void Set(bool value) = 0;

            // Thread management

            /**
             * \brief Lock the object for thread synchronization.
             */
            virtual void Lock() const = 0;

            /**
             * \brief Unlock the object after thread synchronization.
             */
            virtual void Unlock() const = 0;

        protected:
            virtual ~IWeakRefFlag() = default;
        };
    }
}

#endif // BALLOON_IWEAKREFFLAG_H
