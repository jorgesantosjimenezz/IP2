#include "PriorityQueue.h"
#include <sstream>
#include <algorithm>
#include <utility>
#include <cstring>

namespace pq {

// ═══════════════════════════════════════════════════════════════════
//  INNER IMPLEMENTATION CLASS (Pimpl)
//  All private fields and helper methods live here, invisible to .h
// ═══════════════════════════════════════════════════════════════════

struct PriorityQueue::Impl {
    int* data;       ///< Dynamic array storing heap elements
    int count;       ///< Current number of elements
    int capacity;    ///< Current capacity of the array

    static const int INITIAL_CAPACITY = 8;

    // ─── Construction ────────────────────────────────────────────

    Impl() : count(0), capacity(INITIAL_CAPACITY) {
        data = new int[capacity];
    }

    Impl(const Impl& other) : count(other.count), capacity(other.capacity) {
        data = new int[capacity];
        std::memcpy(data, other.data, count * sizeof(int));
    }

    ~Impl() {
        delete[] data;
    }

    Impl& operator=(const Impl& other) {
        if (this != &other) {
            delete[] data;
            count = other.count;
            capacity = other.capacity;
            data = new int[capacity];
            std::memcpy(data, other.data, count * sizeof(int));
        }
        return *this;
    }

    // ─── Heap Helpers ────────────────────────────────────────────

    void resize() {
        capacity *= 2;
        int* newData = new int[capacity];
        std::memcpy(newData, data, count * sizeof(int));
        delete[] data;
        data = newData;
    }

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (data[index] < data[parent]) {
                std::swap(data[index], data[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void siftDown(int index) {
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < count && data[left] < data[smallest]) {
                smallest = left;
            }
            if (right < count && data[right] < data[smallest]) {
                smallest = right;
            }
            if (smallest != index) {
                std::swap(data[index], data[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

    int findIndex(int value) const {
        for (int i = 0; i < count; ++i) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    bool contains(int value) const {
        return findIndex(value) != -1;
    }

    void insert(int value) {
        if (contains(value)) {
            throw DuplicateElementException(
                "Element " + std::to_string(value) + " already exists in the PriorityQueue");
        }
        if (count >= capacity) {
            resize();
        }
        data[count] = value;
        siftUp(count);
        ++count;
    }

    void remove(int value) {
        int index = findIndex(value);
        if (index == -1) {
            throw std::invalid_argument(
                "Element " + std::to_string(value) + " not found in the PriorityQueue");
        }
        data[index] = data[count - 1];
        --count;
        if (index < count) {
            siftUp(index);
            siftDown(index);
        }
    }

    void edit(int oldValue, int newValue) {
        int index = findIndex(oldValue);
        if (index == -1) {
            throw std::invalid_argument(
                "Element " + std::to_string(oldValue) + " not found in the PriorityQueue");
        }
        if (oldValue != newValue && contains(newValue)) {
            throw DuplicateElementException(
                "Element " + std::to_string(newValue) + " already exists in the PriorityQueue");
        }
        data[index] = newValue;
        siftUp(index);
        siftDown(index);
    }

    void clear() {
        count = 0;
    }

    /**
     * @brief Returns a sorted copy of the elements for comparison.
     */
    int* sortedCopy() const {
        int* sorted = new int[count];
        std::memcpy(sorted, data, count * sizeof(int));
        std::sort(sorted, sorted + count);
        return sorted;
    }
};

// ═══════════════════════════════════════════════════════════════════
//  CONSTRUCTORS & DESTRUCTOR
// ═══════════════════════════════════════════════════════════════════

PriorityQueue::PriorityQueue() : pImpl(new Impl()) {}

PriorityQueue::PriorityQueue(const PriorityQueue& other)
    : pImpl(new Impl(*other.pImpl)) {}

PriorityQueue& PriorityQueue::operator=(const PriorityQueue& other) {
    if (this != &other) {
        delete pImpl;
        pImpl = new Impl(*other.pImpl);
    }
    return *this;
}

PriorityQueue::~PriorityQueue() {
    delete pImpl;
}

// ═══════════════════════════════════════════════════════════════════
//  CRUD OPERATORS
// ═══════════════════════════════════════════════════════════════════

PriorityQueue& PriorityQueue::operator+=(int value) {
    pImpl->insert(value);
    return *this;
}

PriorityQueue& PriorityQueue::operator-=(int value) {
    pImpl->remove(value);
    return *this;
}

PriorityQueue& PriorityQueue::operator*=(const std::pair<int, int>& values) {
    pImpl->edit(values.first, values.second);
    return *this;
}

int PriorityQueue::operator[](int value) const {
    return pImpl->findIndex(value);
}

// ═══════════════════════════════════════════════════════════════════
//  UTILITY OPERATORS
// ═══════════════════════════════════════════════════════════════════

PriorityQueue PriorityQueue::operator!() const {
    return PriorityQueue(); // returns a new empty queue
}

// ═══════════════════════════════════════════════════════════════════
//  COMPARISON OPERATORS
//  Compare first by size, then lexicographically by sorted content.
// ═══════════════════════════════════════════════════════════════════

bool PriorityQueue::operator==(const PriorityQueue& other) const {
    if (pImpl->count != other.pImpl->count) return false;
    int* a = pImpl->sortedCopy();
    int* b = other.pImpl->sortedCopy();
    bool equal = true;
    for (int i = 0; i < pImpl->count; ++i) {
        if (a[i] != b[i]) { equal = false; break; }
    }
    delete[] a;
    delete[] b;
    return equal;
}

bool PriorityQueue::operator!=(const PriorityQueue& other) const {
    return !(*this == other);
}

bool PriorityQueue::operator<(const PriorityQueue& other) const {
    if (pImpl->count != other.pImpl->count)
        return pImpl->count < other.pImpl->count;
    int* a = pImpl->sortedCopy();
    int* b = other.pImpl->sortedCopy();
    bool result = false;
    for (int i = 0; i < pImpl->count; ++i) {
        if (a[i] != b[i]) {
            result = a[i] < b[i];
            delete[] a;
            delete[] b;
            return result;
        }
    }
    delete[] a;
    delete[] b;
    return false; // equal
}

bool PriorityQueue::operator<=(const PriorityQueue& other) const {
    return !(other < *this);
}

bool PriorityQueue::operator>(const PriorityQueue& other) const {
    return other < *this;
}

bool PriorityQueue::operator>=(const PriorityQueue& other) const {
    return !(*this < other);
}

// ═══════════════════════════════════════════════════════════════════
//  INFORMATION METHODS
// ═══════════════════════════════════════════════════════════════════

std::string PriorityQueue::toString() const {
    std::ostringstream oss;
    oss << "PriorityQueue {";
    oss << " size: " << pImpl->count;
    oss << ", capacity: " << pImpl->capacity;

    if (pImpl->count > 0) {
        // Find min (root) and max
        int minVal = pImpl->data[0];
        int maxVal = pImpl->data[0];
        for (int i = 1; i < pImpl->count; ++i) {
            if (pImpl->data[i] < minVal) minVal = pImpl->data[i];
            if (pImpl->data[i] > maxVal) maxVal = pImpl->data[i];
        }
        oss << ", min: " << minVal;
        oss << ", max: " << maxVal;
        oss << ", elements: [";
        for (int i = 0; i < pImpl->count; ++i) {
            if (i > 0) oss << ", ";
            oss << pImpl->data[i];
        }
        oss << "]";
    } else {
        oss << ", empty";
    }

    oss << " }";
    return oss.str();
}

int PriorityQueue::size() const {
    return pImpl->count;
}

bool PriorityQueue::empty() const {
    return pImpl->count == 0;
}

int PriorityQueue::top() const {
    if (pImpl->count == 0) {
        throw std::out_of_range("Cannot get top element: PriorityQueue is empty");
    }
    return pImpl->data[0];
}

// ═══════════════════════════════════════════════════════════════════
//  FRIEND FUNCTIONS
// ═══════════════════════════════════════════════════════════════════

std::ostream& operator<<(std::ostream& os, const PriorityQueue& pq) {
    os << pq.toString();
    return os;
}

}
