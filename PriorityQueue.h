#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <string>
#include <stdexcept>
#include <iostream>

namespace pq {

/**
 * @class DuplicateElementException
 * @brief Custom exception thrown when attempting to insert a duplicate
 *        element into the PriorityQueue (duplicates are not allowed).
 *
 * Derives from std::logic_error as it represents a logical precondition
 * violation rather than a runtime/system error.
 */
class DuplicateElementException : public std::logic_error {
public:
    explicit DuplicateElementException(const std::string& msg)
        : std::logic_error(msg) {}
};

/**
 * @class PriorityQueue
 * @brief A min-heap based priority queue that stores unique integers.
 *
 * The internal implementation is completely hidden via the Pimpl idiom.
 * Only a pointer to the implementation class (Impl) is stored as a
 * private member. The actual data (dynamic array, size, capacity) and
 * helper methods (siftUp, siftDown, etc.) reside in the .cpp file.
 *
 * CRUD Operations (as operators):
 *  - operator+=(int)  : Insert a new element (Create)
 *  - operator[](int)  : Search by value, returns position or -1 (Read)
 *  - operator*=(pair)  : Edit an existing element's value (Update)
 *  - operator-=(int)  : Remove an element by value (Delete)
 *
 * Additional operators:
 *  - operator!        : Clear all elements
 *  - ==, !=, <, <=, >, >= : Compare two PriorityQueues
 *  - operator<<       : Stream output via toString()
 */
class PriorityQueue {
public:
    // ─── Constructors & Destructor ───────────────────────────────────

    /** @brief Default constructor. Creates an empty priority queue. */
    PriorityQueue();

    /**
     * @brief Copy constructor. Performs a deep copy.
     * @param other The PriorityQueue to copy from.
     */
    PriorityQueue(const PriorityQueue& other);

    /**
     * @brief Copy assignment operator. Performs a deep copy.
     * @param other The PriorityQueue to assign from.
     * @return Reference to this object.
     */
    PriorityQueue& operator=(const PriorityQueue& other);

    /** @brief Destructor. Frees dynamically allocated memory. */
    ~PriorityQueue();

    // ─── CRUD Operators ──────────────────────────────────────────────

    /**
     * @brief Insert (Create): Adds a new element to the queue.
     * @param value The integer value to insert.
     * @return Reference to this object.
     * @throws DuplicateElementException if value already exists.
     */
    PriorityQueue& operator+=(int value);

    /**
     * @brief Delete: Removes an element by value from the queue.
     * @param value The integer value to remove.
     * @return Reference to this object.
     * @throws std::invalid_argument if value is not found.
     */
    PriorityQueue& operator-=(int value);

    /**
     * @brief Edit (Update): Changes an existing value to a new value.
     * @param values A pair {oldValue, newValue}.
     * @return Reference to this object.
     * @throws std::invalid_argument if oldValue is not found.
     * @throws DuplicateElementException if newValue already exists.
     */
    PriorityQueue& operator*=(const std::pair<int, int>& values);

    /**
     * @brief Search (Read): Finds the position of a value in the heap.
     * @param value The integer value to search for.
     * @return The index (0-based) of the element, or -1 if not found.
     */
    int operator[](int value) const;

    // ─── Utility Operators ───────────────────────────────────────────

    /**
     * @brief Clear: Removes all elements from the queue.
     * @return A new empty PriorityQueue.
     */
    PriorityQueue operator!() const;

    // ─── Comparison Operators ────────────────────────────────────────

    /** @brief Equality: compares size, then sorted content. */
    bool operator==(const PriorityQueue& other) const;
    bool operator!=(const PriorityQueue& other) const;
    bool operator<(const PriorityQueue& other) const;
    bool operator<=(const PriorityQueue& other) const;
    bool operator>(const PriorityQueue& other) const;
    bool operator>=(const PriorityQueue& other) const;

    // ─── Information ─────────────────────────────────────────────────

    /**
     * @brief Returns aggregate information about the priority queue.
     * @return A string with size, capacity, min, max, and elements.
     */
    std::string toString() const;

    /**
     * @brief Returns the number of elements in the queue.
     * @return The current size.
     */
    int size() const;

    /**
     * @brief Checks if the queue is empty.
     * @return true if the queue has no elements.
     */
    bool empty() const;

    /**
     * @brief Returns the minimum element (root of the min-heap).
     * @return The smallest element.
     * @throws std::out_of_range if the queue is empty.
     */
    int top() const;

    // ─── Friend Functions ────────────────────────────────────────────

    /**
     * @brief Stream insertion operator for printing.
     * @param os The output stream.
     * @param pq The PriorityQueue to print.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& pq);

private:
    /** @brief Forward declaration of the implementation class (Pimpl). */
    struct Impl;

    /** @brief Pointer to the implementation (only private field). */
    Impl* pImpl;
};

} // namespace pq

#endif // PRIORITYQUEUE_H
