# Homework 1: Ackermann's Function & Powerset Algorithm Analysis

本專案實作阿克曼函數（Ackermann's Function）的遞迴與非遞迴版本，以及冪集（Powerset）的遞迴產生器，並提供詳細的演算法時間與空間複雜度分析。

---

## 📄 完整 C++ 程式碼 (Source Code)

```cpp
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace std;

// ============================================================================
// Problem 1: Ackermann Function (阿克曼函數)
// ============================================================================

/**
 * @brief 遞迴版 Ackermann 函數
 * @param m 函數的第一個非負整數參數
 * @param n 函數的第二個非負整數參數
 * @return 計算結果 long long
 */
long long ackermann_recursive(long long m, long long n) {
    // 條件 1: 若 m = 0，根據定義直接回傳 n + 1
    if (m == 0) {
        return n + 1;
    }
    // 條件 2: 若 m > 0 且 n = 0，遞迴呼叫 A(m - 1, 1)
    if (n == 0) {
        return ackermann_recursive(m - 1, 1);
    }
    // 條件 3: 若 m > 0 且 n > 0，進行嵌套遞迴呼叫 A(m - 1, A(m, n - 1))
    return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
}

// ----------------------------------------------------------------------------
// 自訂 Stack 結構（因限定標頭檔，無法使用 STL <stack>，故以靜態陣列模擬）
// ----------------------------------------------------------------------------
#define MAX_STACK_SIZE 1000000 // 設定 Stack 容量上限

long long m_stack[MAX_STACK_SIZE]; // 儲存暫存 m 值的堆疊陣列
int top = -1;                       // 堆疊頂端指標，初始為 -1 代表 Empty

/**
 * @brief 將資料推入 Stack (Push)
 * @param val 要儲存的 m 值
 */
void push(long long val) {
    if (top < MAX_STACK_SIZE - 1) {
        m_stack[++top] = val; // 指標先加 1 再放入資料
    } else {
        cerr << "Error: Stack Overflow!" << endl;
    }
}

/**
 * @brief 從 Stack 彈出資料 (Pop)
 * @return 彈出的 m 值，若 Stack 為空則回傳 -1
 */
long long pop() {
    if (top >= 0) {
        return m_stack[top--]; // 先傳回資料，指標再減 1
    }
    return -1;
}

/**
 * @brief 非遞迴版 Ackermann 函數（利用自訂 Stack 模擬系統 Call Stack）
 * @param m 函數的第一個非負整數參數
 * @param n 函數的第二個非負整數參數
 * @return 計算結果 long long
 */
long long ackermann_nonrecursive(long long m, long long n) {
    top = -1; // 初始化 Stack 指標
    
    // 初始狀態：將第一個 m 值壓入 Stack 中
    push(m);
    
    // 當 Stack 內還有尚未處理完成的 m 值時繼續運算
    while (top >= 0) {
        m = pop(); // 取出目前要處理的 m 值
        
        if (m == 0) {
            // 情況 1: m == 0，n 更新為 n + 1 (代表該層遞迴計算完成)
            n = n + 1;
        } else if (n == 0) {
            // 情況 2: n == 0，將 m - 1 推回 Stack，並把 n 重置為 1
            push(m - 1);
            n = 1;
        } else {
            // 情況 3: m > 0 且 n > 0，對應 A(m - 1, A(m, n - 1))
            // 需先推入 m - 1 (外層)，再推入 m (內層)，並將 n 更新為 n - 1
            push(m - 1);
            push(m);
            n = n - 1;
        }
    }
    
    // 當 Stack 彈空，n 即為最終求得的 Ackermann 結果
    return n;
}

// ============================================================================
// Problem 2: Powerset Recursive (遞迴生成冪集)
// ============================================================================

/**
 * @brief 遞迴印出集合 S 的 Powerset (所有可能子集)
 * @param elements[] 儲存原始集合元素的字串陣列
 * @param selected[] 布林陣列，紀錄目前每個元素是否被選取 (true/false)
 * @param index      目前正決策到第幾個元素 (從 0 開始)
 * @param total      集合元素的總個數 n
 */
void print_powerset_recursive(const string elements[], bool selected[], int index, int total) {
    // Base Case (終止條件)：已對所有元素做出「選/不選」的決定
    if (index == total) {
        cout << "{ ";
        bool first = true; // 用於處理格式化逗號輸出
        for (int i = 0; i < total; ++i) {
            if (selected[i]) { // 若該位置元素被標記為選取
                if (!first) cout << ", ";
                cout << elements[i];
                first = false;
            }
        }
        cout << " }\n";
        return; // 結束該分支遞迴
    }

    // 分支 1：不選擇 (Exclude) 當前 index 的元素
    selected[index] = false;
    print_powerset_recursive(elements, selected, index + 1, total);

    // 分支 2：選擇 (Include) 當前 index 的元素
    selected[index] = true;
    print_powerset_recursive(elements, selected, index + 1, total);
}

// ============================================================================
// 主程式 (Main Testing Function)
// ============================================================================
int main() {
    cout << "=== Homework 1 Testing ===" << endl;

    // ------------------------------------------------------------------------
    // 測試 Problem 1: Ackermann 函數
    // ------------------------------------------------------------------------
    cout << "\n[Problem 1: Ackermann Function Test]" << endl;
    
    // 測試範例 1: A(2, 3) -> 預期結果 9
    cout << "Recursive A(2, 3)     = " << ackermann_recursive(2, 3) << endl;
    cout << "Non-Recursive A(2, 3) = " << ackermann_nonrecursive(2, 3) << endl;
    
    // 測試範例 2: A(3, 2) -> 預期結果 29
    cout << "Recursive A(3, 2)     = " << ackermann_recursive(3, 2) << endl;
    cout << "Non-Recursive A(3, 2) = " << ackermann_nonrecursive(3, 2) << endl;

    // ------------------------------------------------------------------------
    // 測試 Problem 2: Powerset 遞迴
    // ------------------------------------------------------------------------
    cout << "\n[Problem 2: Powerset Recursive Test]" << endl;
    
    string S[] = {"a", "b", "c"}; // 測試用集合
    int n = 3;                    // 集合元素個數
    bool selected[3] = {false};   // 紀錄選擇狀態，初值全部設為 false
    
    cout << "Powerset of {a, b, c}:" << endl;
    print_powerset_recursive(S, selected, 0, n);

    return 0;
}
