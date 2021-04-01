/*
 * MemoryManager.h
 *
 *  Created on: Mar 30, 2021
 *      Author: rodolk
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <mutex>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace objmem_manager {

#define MAX_OBJETS_INCREMENTS 3

template <class T> class MemoryManager {
public:
    MemoryManager<T>(uint32_t numInstances) {
        if (numInstances > 0) initialize(numInstances);
        else throw std::bad_alloc();
    }

    MemoryManager<T>(uint32_t numInstances, uint32_t increment, uint32_t maxIncrement) {
        if (numInstances > 0 && increment > 0) {
            initialize(numInstances, increment, maxIncrement);
        } else throw std::bad_alloc();
    }

    virtual ~MemoryManager<T>() {
        if (usedObjects_ > 0) {
            std::cerr << "There are leaked objects in MemoryManager for " << typeid(T).name() << std::endl;
        }
    }

    void activateTimeControl() {

    }

    T *createInstance() {
        std::unique_lock<std::mutex> lock(listMutex_);

        if (headAvailable_) {
            return useFirstAvailableObject();
        } else {
            if (totIncrements_ < maxIncrement_) {
                std::cerr << "Warning: incrementing objects in MemoryManager for type " << typeid(T).name()
                        << std::endl;
                bool res = allocateMemForInstances(increment_, totIncrements_ + 1);
                if (!res) {
                    std::cerr << "Error incrementing in MemoryManager for type " << typeid(T).name()
                            << std::endl;
                    std::bad_alloc exception;
                    throw exception;
                }
                std::cerr << "Warning: increment " << (totIncrements_ + 1) << " out of " << maxIncrement_
                        << " in MemoryManager for type " << typeid(T).name() << std::endl;
                headAvailable_ = &incrementsLinkArray_[totIncrements_ + 1][0]; //TODO: check everything
                totIncrements_++;
                return useFirstAvailableObject();
            } else {
                std::cerr << "Error  - No more instances of " << typeid(T).name() << " available."
                        << std::endl;
                throw std::bad_alloc();
            }
        }
    }

    void returnInstance(T *retInstance) {
        std::unique_lock<std::mutex> lock(listMutex_);

        if (headAvailableLink_) {
            Link *instLink = headAvailableLink_;
            headAvailableLink_ = headAvailableLink_->next_;
            instLink->next_ = headAvailable_;
            headAvailable_ = instLink;
            instLink->object_ = retInstance;
            usedObjects_--;
            return ;
        } else {
            std::cerr << "Error  - No more links available for returning " << typeid(T).name() << " instance. Call 911" << std::endl;
            std::bad_alloc exception;
            throw exception;
        }
    }


private:
    struct Link {
        Link *next_{nullptr};
        T *object_;
    };

    bool initialized_{false};
    std::mutex listMutex_;
    Link *headAvailable_{nullptr};
    Link *headAvailableLink_{nullptr};
    uint32_t usedObjects_{0};
    uint32_t totObjects_{0};
    uint32_t increment_{0};
    uint32_t maxIncrement_{0};
    uint32_t totIncrements_{0};
    Link *incrementsLinkArray_[MAX_OBJETS_INCREMENTS + 1];
    T *incrementsObjectArray_[MAX_OBJETS_INCREMENTS + 1];

    void initialize(uint32_t numInstances) {
        bool res = allocateMemForInstances(numInstances, 0);

        if (res) {
            headAvailable_ = incrementsLinkArray_[0];
            initialized_ = true;
        } else {
            std::cerr << "Error initializing MemoryManager for type " << typeid(T).name() << std::endl;
        }
    }

    void initialize(uint32_t numInstances, uint32_t increment, uint32_t maxIncrement) {
        initialize(numInstances);
        increment_ = increment;
        if (maxIncrement > MAX_OBJETS_INCREMENTS) {
            std::cerr << "Warning initializing MemoryManager for type " << typeid(T).name()
                    << ": Max number of increments is " << MAX_OBJETS_INCREMENTS
                    << "but received: " << maxIncrement << std::endl;
            maxIncrement_ = MAX_OBJETS_INCREMENTS;
        } else {
            maxIncrement_ = maxIncrement;
        }
    }

    bool allocateMemForInstances(uint32_t numInstances, uint16_t index) {
        incrementsLinkArray_[index] = (Link *)std::malloc(numInstances * sizeof(Link));
        T *instances = (T *)std::malloc(numInstances * sizeof(T));
        incrementsObjectArray_[index] = instances;

        if (incrementsLinkArray_[index] == nullptr || instances == nullptr) {
            std::cerr << "Error allocating more memory in MemoryManager for type " << typeid(T).name() << " instances: " << numInstances << std::endl;
            return false;
        } else {
            uint32_t i;
            for(i = 0; i < (numInstances - 1); i++) {
                incrementsLinkArray_[index][i].next_ = &incrementsLinkArray_[index][i + 1];
                incrementsLinkArray_[index][i].object_ = &instances[i];
            }
            //Last element (numIsntances - 1)
            incrementsLinkArray_[index][i].next_ = nullptr;
            incrementsLinkArray_[index][i].object_ = &instances[i];
            totObjects_ += numInstances;

            return true;
        }
    }

    T *useFirstAvailableObject() {
        Link *instLink = headAvailable_;
        headAvailable_ = headAvailable_->next_;
        instLink->next_ = headAvailableLink_;
        headAvailableLink_ = instLink;
        usedObjects_++;

        return instLink->object_;
    }

};

} // namespace objmem_manager

#endif /* MEMORYMANAGER_H_ */
