/**
 * @file demo.cpp
 * @brief Demonstration program for the PriorityQueue ADT.
 *
 * This program calls every public method of PriorityQueue at least once,
 * demonstrating the full functionality of the class. Requires no user input.
 *
 * @author Jorge Santos
 * @date March 2026
 */

#include <iostream>
#include <utility>
#include "PriorityQueue.h"

using namespace PQ;

int main() {
    try {
        std::cout << "==========================================================\n";
        std::cout << "  PriorityQueue (Binary Min-Heap) ADT — DEMONSTRATION\n";
        std::cout << "==========================================================\n\n";

        // ─── 1. Default Constructor ──────────────────────────────────
        std::cout << "--- 1. Default Constructor ---\n";
        PriorityQueue q1;
        std::cout << "Created empty queue q1: " << q1 << "\n";
        std::cout << "q1.size() = " << q1.size() << "\n";
        std::cout << "q1.empty() = " << (q1.empty() ? "true" : "false") << "\n\n";

        // ─── 2. Insert (operator+=) ──────────────────────────────────
        std::cout << "--- 2. Insert (operator+=) ---\n";
        q1 += 10;
        std::cout << "After q1 += 10: " << q1 << "\n";
        q1 += 5;
        std::cout << "After q1 += 5:  " << q1 << "\n";
        q1 += 20;
        std::cout << "After q1 += 20: " << q1 << "\n";
        q1 += 3;
        std::cout << "After q1 += 3:  " << q1 << "\n";
        q1 += 15;
        std::cout << "After q1 += 15: " << q1 << "\n";
        q1 += 8;
        std::cout << "After q1 += 8:  " << q1 << "\n\n";

        // ─── 3. top() and size() ─────────────────────────────────────
        std::cout << "--- 3. top() and size() ---\n";
        std::cout << "q1.top() = " << q1.top() << " (min element)\n";
        std::cout << "q1.size() = " << q1.size() << "\n\n";

        // ─── 4. Search (operator[]) ──────────────────────────────────
        std::cout << "--- 4. Search (operator[]) ---\n";
        std::cout << "q1[5]  = " << q1[5] << " (position of 5)\n";
        std::cout << "q1[99] = " << q1[99] << " (99 not found -> -1)\n\n";

        // ─── 5. Edit (operator*=) ────────────────────────────────────
        std::cout << "--- 5. Edit (operator*=) ---\n";
        std::cout << "Before edit: " << q1 << "\n";
        q1 *= std::make_pair(20, 1); // change 20 to 1
        std::cout << "After q1 *= {20, 1} (edit 20 -> 1): " << q1 << "\n\n";

        // ─── 6. Delete (operator-=) ──────────────────────────────────
        std::cout << "--- 6. Delete (operator-=) ---\n";
        std::cout << "Before delete: " << q1 << "\n";
        q1 -= 15;
        std::cout << "After q1 -= 15: " << q1 << "\n\n";

        // ─── 7. toString() ──────────────────────────────────────────
        std::cout << "--- 7. toString() ---\n";
        std::cout << q1.toString() << "\n\n";

        // ─── 8. Copy Constructor (deep copy) ─────────────────────────
        std::cout << "--- 8. Copy Constructor (deep copy) ---\n";
        PriorityQueue q2(q1);
        std::cout << "q2 (copy of q1): " << q2 << "\n";
        q2 += 100;
        std::cout << "After q2 += 100:\n";
        std::cout << "  q1 = " << q1 << "\n";
        std::cout << "  q2 = " << q2 << " (independent copy)\n\n";

        // ─── 9. Copy Assignment (deep copy) ──────────────────────────
        std::cout << "--- 9. Copy Assignment (deep copy) ---\n";
        PriorityQueue q3;
        q3 += 999;
        std::cout << "q3 before assignment: " << q3 << "\n";
        q3 = q1;
        std::cout << "q3 after q3 = q1:     " << q3 << "\n\n";

        // ─── 10. Comparison Operators ────────────────────────────────
        std::cout << "--- 10. Comparison Operators ---\n";
        PriorityQueue a, b;
        a += 1; a += 2; a += 3;
        b += 1; b += 2; b += 3;
        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
        std::cout << "a == b: " << (a == b ? "true" : "false") << "\n";
        std::cout << "a != b: " << (a != b ? "true" : "false") << "\n";

        b += 4;
        std::cout << "\nAfter b += 4:\n";
        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
        std::cout << "a <  b: " << (a < b ? "true" : "false") << "\n";
        std::cout << "a <= b: " << (a <= b ? "true" : "false") << "\n";
        std::cout << "a >  b: " << (a > b ? "true" : "false") << "\n";
        std::cout << "a >= b: " << (a >= b ? "true" : "false") << "\n\n";

        // ─── 11. Clear (operator!) ───────────────────────────────────
        std::cout << "--- 11. Clear (operator!) ---\n";
        std::cout << "q1 before clear: " << q1 << "\n";
        !q1;
        std::cout << "q1 after !q1:    " << q1 << "\n\n";

        // ─── 12. Exception Handling ──────────────────────────────────
        std::cout << "--- 12. Exception Handling ---\n";

        // 12a. DuplicateElementException
        try {
            PriorityQueue eq;
            eq += 42;
            std::cout << "Trying to insert duplicate 42...\n";
            eq += 42; // should throw
        } catch (const DuplicateElementException& e) {
            std::cout << "  Caught DuplicateElementException: " << e.what() << "\n";
        }

        // 12b. std::invalid_argument (remove nonexistent)
        try {
            PriorityQueue eq;
            std::cout << "Trying to remove nonexistent element 77...\n";
            eq -= 77; // should throw
        } catch (const std::invalid_argument& e) {
            std::cout << "  Caught invalid_argument: " << e.what() << "\n";
        }

        // 12c. std::out_of_range (top on empty)
        try {
            PriorityQueue eq;
            std::cout << "Trying to get top() of empty queue...\n";
            eq.top(); // should throw
        } catch (const std::out_of_range& e) {
            std::cout << "  Caught out_of_range: " << e.what() << "\n";
        }

        // ─── 13. Stream operator<< ───────────────────────────────────
        std::cout << "\n--- 13. Stream operator<< ---\n";
        std::cout << "Final q1: " << q1 << "\n";
        std::cout << "Final q2: " << q2 << "\n";

        std::cout << "\n==========================================================\n";
        std::cout << "  DEMONSTRATION COMPLETE\n";
        std::cout << "==========================================================\n";

    } catch (const std::exception& e) {
        std::cerr << "UNHANDLED EXCEPTION: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
