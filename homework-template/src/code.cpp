#include <iostream>
#include <string>

using namespace std;

// Problem 1: Recursive Ackermann function
unsigned long long ackermannRecursive(unsigned long long m,
                                      unsigned long long n) {
    if (m == 0) {
        return n + 1;
    }

    if (n == 0) {
        return ackermannRecursive(m - 1, 1);
    }

    return ackermannRecursive(
        m - 1,
        ackermannRecursive(m, n - 1)
    );
}

// Problem 1: Nonrecursive Ackermann function.
// A stack is used to simulate the recursive calls.
unsigned long long ackermannNonRecursive(unsigned long long m,
                                         unsigned long long n) {
    unsigned long long stack[10000];
    int top = -1;

    stack[++top] = m;

    while (top >= 0) {
        m = stack[top--];

        if (m == 0) {
            ++n;
        } else if (n == 0) {
            n = 1;
            stack[++top] = m - 1;
        } else {
            // Simulate:
            // A(m, n) = A(m - 1, A(m, n - 1))
            // Push m - 1 first so that A(m, n - 1) is processed first.
            stack[++top] = m - 1;
            stack[++top] = m;
            --n;
        }
    }

    return n;
}

// Problem 2: Recursive powerset.
// Each element has two choices:
// 1. Do not include it.
// 2. Include it.
void powersetRecursive(const string elements[],
                       int n,
                       int index,
                       string current) {
    if (index == n) {
        cout << "{";
        if (current.empty()) {
            cout << "}";
        } else {
            for (int i = 0; i < static_cast<int>(current.size()); ++i) {
                if (i > 0) {
                    cout << ", ";
                }
                cout << current[i];
            }
            cout << "}";
        }
        cout << '\n';
        return;
    }

    // Case 1: Do not include the current element.
    powersetRecursive(elements, n, index + 1, current);

    // Case 2: Include the current element.
    powersetRecursive(elements, n, index + 1,
                      current + elements[index]);
}

int main() {
    cout << "===== Problem 1: Ackermann Function =====\n";

    cout << "Recursive A(3, 4) = "
         << ackermannRecursive(3, 4) << '\n';

    cout << "Nonrecursive A(3, 4) = "
         << ackermannNonRecursive(3, 4) << '\n';

    cout << "\n===== Problem 2: Powerset =====\n";

    string elements[] = {"a", "b", "c"};
    int n = 3;

    cout << "S = {a, b, c}\n";
    cout << "Powerset(S):\n";
    powersetRecursive(elements, n, 0, "");

    return 0;
}
