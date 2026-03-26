
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include "PriorityQueue.h"

using namespace PQ;

// ─── Dual-output stream helper ───────────────────────────────────
// Writes to both stdout and the log file simultaneously.

static std::ofstream logFile;
static int totalTests = 0;
static int passedTests = 0;

void log(const std::string& msg) {
    std::cout << msg;
    logFile << msg;
}

void logln(const std::string& msg = "") {
    log(msg + "\n");
}

void reportTest(const std::string& name, bool condition) {
    ++totalTests;
    std::string result = condition ? "PASS" : "FAIL";
    if (condition) ++passedTests;
    logln("  [" + result + "] " + name);
}

// ═══════════════════════════════════════════════════════════════════
//  TEST SUITES
// ═══════════════════════════════════════════════════════════════════

void testConstructorAndEmpty() {
    logln("--- Test Suite: Constructor & Empty ---");
    PriorityQueue q;
    reportTest("Default constructor creates empty queue", q.empty());
    reportTest("Size of empty queue is 0", q.size() == 0);
    reportTest("toString of empty queue contains 'empty'",
               q.toString().find("empty") != std::string::npos);
}

void testInsert() {
    logln("\n--- Test Suite: Insert (operator+=) ---");
    PriorityQueue q;
    q += 10;
    reportTest("Insert 10: size becomes 1", q.size() == 1);
    reportTest("Insert 10: top is 10", q.top() == 10);

    q += 5;
    reportTest("Insert 5: size becomes 2", q.size() == 2);
    reportTest("Insert 5: top is 5 (min-heap)", q.top() == 5);

    q += 20;
    q += 3;
    q += 15;
    reportTest("Insert 20, 3, 15: size becomes 5", q.size() == 5);
    reportTest("Top is 3 (smallest)", q.top() == 3);
}

void testSearch() {
    logln("\n--- Test Suite: Search (operator[]) ---");
    PriorityQueue q;
    q += 10; q += 5; q += 20;
    reportTest("Search for 10: found (>= 0)", q[10] >= 0);
    reportTest("Search for 5: found (>= 0)", q[5] >= 0);
    reportTest("Search for 99: not found (== -1)", q[99] == -1);
    reportTest("Search for 20: found (>= 0)", q[20] >= 0);
}

void testEdit() {
    logln("\n--- Test Suite: Edit (operator*=) ---");
    PriorityQueue q;
    q += 10; q += 5; q += 20;

    q *= std::make_pair(20, 1); // edit 20 -> 1
    reportTest("After edit 20->1: 20 not found", q[20] == -1);
    reportTest("After edit 20->1: 1 is found", q[1] >= 0);
    reportTest("After edit 20->1: top is 1", q.top() == 1);
    reportTest("Size unchanged after edit", q.size() == 3);
}

void testDelete() {
    logln("\n--- Test Suite: Delete (operator-=) ---");
    PriorityQueue q;
    q += 10; q += 5; q += 20; q += 3;

    q -= 5;
    reportTest("After delete 5: size is 3", q.size() == 3);
    reportTest("After delete 5: 5 not found", q[5] == -1);
    reportTest("After delete 5: top is 3", q.top() == 3);

    q -= 3;
    reportTest("After delete 3: top is 10", q.top() == 10);
}

void testClear() {
    logln("\n--- Test Suite: Clear (operator!) ---");
    PriorityQueue q;
    q += 1; q += 2; q += 3;
    reportTest("Before clear: size is 3", q.size() == 3);
    !q;
    reportTest("After !q: queue is empty", q.empty());
    reportTest("After !q: size is 0", q.size() == 0);
}

void testDeepCopy() {
    logln("\n--- Test Suite: Deep Copy ---");
    PriorityQueue q1;
    q1 += 10; q1 += 5; q1 += 20;

    // Copy constructor
    PriorityQueue q2(q1);
    reportTest("Copy constructor: sizes equal", q2.size() == q1.size());
    reportTest("Copy constructor: q1 == q2", q1 == q2);

    q2 += 100;
    reportTest("Modifying copy doesn't affect original", q1.size() == 3);
    reportTest("Copy has new element", q2.size() == 4);

    // Copy assignment
    PriorityQueue q3;
    q3 = q1;
    reportTest("Copy assignment: sizes equal", q3.size() == q1.size());
    reportTest("Copy assignment: q1 == q3", q1 == q3);

    q3 -= 10;
    reportTest("Modifying assigned copy doesn't affect original", q1[10] >= 0);
}

void testComparison() {
    logln("\n--- Test Suite: Comparison Operators ---");
    PriorityQueue a, b;
    a += 1; a += 2; a += 3;
    b += 1; b += 2; b += 3;

    reportTest("a == b (same elements)", a == b);
    reportTest("!(a != b)", !(a != b));
    reportTest("a <= b", a <= b);
    reportTest("a >= b", a >= b);
    reportTest("!(a < b)", !(a < b));
    reportTest("!(a > b)", !(a > b));

    b += 4; // b is now bigger
    reportTest("a < b (fewer elements)", a < b);
    reportTest("a <= b", a <= b);
    reportTest("b > a", b > a);
    reportTest("b >= a", b >= a);
    reportTest("a != b", a != b);
}

void testExceptions() {
    logln("\n--- Test Suite: Exception Handling ---");

    // DuplicateElementException
    bool caught = false;
    try {
        PriorityQueue q;
        q += 42;
        q += 42;
    } catch (const DuplicateElementException&) {
        caught = true;
    }
    reportTest("DuplicateElementException on duplicate insert", caught);

    // std::invalid_argument on remove
    caught = false;
    try {
        PriorityQueue q;
        q -= 99;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    reportTest("invalid_argument on remove from empty", caught);

    // std::invalid_argument on edit nonexistent
    caught = false;
    try {
        PriorityQueue q;
        q += 1;
        q *= std::make_pair(99, 50);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    reportTest("invalid_argument on edit nonexistent", caught);

    // DuplicateElementException on edit to existing value
    caught = false;
    try {
        PriorityQueue q;
        q += 1; q += 2;
        q *= std::make_pair(1, 2);
    } catch (const DuplicateElementException&) {
        caught = true;
    }
    reportTest("DuplicateElementException on edit to existing value", caught);

    // std::out_of_range on top() of empty queue
    caught = false;
    try {
        PriorityQueue q;
        q.top();
    } catch (const std::out_of_range&) {
        caught = true;
    }
    reportTest("out_of_range on top() of empty queue", caught);
}

void testToString() {
    logln("\n--- Test Suite: toString & operator<< ---");
    PriorityQueue q;
    reportTest("Empty toString contains 'empty'",
               q.toString().find("empty") != std::string::npos);

    q += 10; q += 5;
    std::string s = q.toString();
    reportTest("toString contains 'size: 2'", s.find("size: 2") != std::string::npos);
    reportTest("toString contains 'min: 5'", s.find("min: 5") != std::string::npos);

    // Test operator<<
    std::ostringstream oss;
    oss << q;
    reportTest("operator<< produces non-empty output", !oss.str().empty());
    reportTest("operator<< matches toString", oss.str() == q.toString());
}

void testLargeInsert() {
    logln("--- Test Suite: Large Insert (resize) ---");
    PriorityQueue q;
    for (int i = 50; i >= 1; --i) {
        q += i;
    }
    reportTest("50 elements inserted correctly", q.size() == 50);
    reportTest("Top is 1 after inserting 50..1", q.top() == 1);
}

// ═══════════════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════════════

int main() {
    logFile.open("log.txt");
    if (!logFile.is_open()) {
        std::cerr << "ERROR: Could not open log.txt for writing.\n";
        return 1;
    }

    try {
        logln("==========================================================");
        logln("  PriorityQueue (Binary Min-Heap) ADT — TEST RESULTS");
        logln("==========================================================");

        testConstructorAndEmpty();
        testInsert();
        testSearch();
        testEdit();
        testDelete();
        testClear();
        testDeepCopy();
        testComparison();
        testExceptions();
        testToString();
        testLargeInsert();

        logln("==========================================================");
        logln("  SUMMARY: " + std::to_string(passedTests) + "/" +
              std::to_string(totalTests) + " tests passed");
        if (passedTests == totalTests) {
            logln("  ALL TESTS PASSED!");
        } else {
            logln("  SOME TESTS FAILED!");
        }
        logln("==========================================================");

    } catch (const std::exception& e) {
        logln("UNHANDLED EXCEPTION: " + std::string(e.what()));
        logFile.close();
        return 1;
    }

    logFile.close();
    return (passedTests == totalTests) ? 0 : 1;
}
