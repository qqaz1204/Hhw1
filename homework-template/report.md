# 41343123

作業一

## 解題說明

本作業包含兩個主要問題：

### Ackermann's Function
Ackermann 函數 $A(m, n)$ 定義如下：

$$
A(m, n) = 
\begin{cases} 
n + 1 & \text{if } m = 0 \\
A(m - 1, 1) & \text{if } n = 0 \\
A(m - 1, A(m, n - 1)) & \text{otherwise}
\end{cases}
$$

* **遞迴實作 (Recursive)**：直接依據數學定義進行遞迴呼叫。
* **非遞迴實作 (Non-recursive)**：利用自訂 Stack（堆疊）模擬系統呼叫堆疊（Call Stack），避免過深遞迴導致系統 Stack Overflow。

### Powerset
對於給定包含 $n$ 個元素的集合 $S$，Powerset 為包含 $S$ 所有可能子集的集合。例如 $S = (a, b, c)$，其 Powerset 為 $\{\emptyset, (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)\}$。

* **遞迴實作**：針對每個元素進行 Include / Exclude（選或不選）的二元分歧，遞迴建構出所有 $2^n$ 個子集。


### 解題策略

#### Ackermann's Function 解題策略
1. **遞迴策略 (Recursive Strategy)**：
   * **Base Case (基本情況)**：當 $m = 0$ 時，函數可直接求解，回傳 $n + 1$。
   * **Reduction Case (遞減情況)**：
     * 當 $n = 0$ 時，將問題規模簡化為 $A(m-1, 1)$。
     * 當 $m > 0, n > 0$ 時，進行兩層內嵌遞迴呼叫 $A(m-1, A(m, n-1))$，優先求解內層 $A(m, n-1)$。

2. **非遞迴策略 (Non-recursive Strategy)**：
   * **模擬 Call Stack**：由於系統遞迴是在 Stack 中儲存參數狀態，我們可以使用自訂陣列（Array-based Stack）來記錄被暫存的 $m$ 值。
   * **狀態轉換**：
     * 初始將 $m$ 推入 Stack 中。
     * 迴圈取出的 $m$：
       * 若 $m = 0$，代表目前階段的 $m$ 已可計算，更新 $n = n + 1$（向上一層傳遞結果）。
       * 若 $n = 0$，將 $m - 1$ 推回 Stack，並重置 $n = 1$。
       * 若 $m, n > 0$，將 $m - 1$ 與 $m$ 依序推入 Stack，並更新 $n = n - 1$，模擬內層遞迴呼叫。

---

#### Powerset 解題策略
1. **二元樹決策 (Binary Decision Tree)**：
   * 冪集的本質是集合中每一個元素都存在「選 (Include)」與「不選 (Exclude)」兩種可能。
   * 將問題視為深度為 $n$ 的二元樹，第 $i$ 層代表對第 $i$ 個元素做決定。

2. **遞迴回溯 (Recursive Backtracking Strategy)**：
   * 設定遞迴終止條件：當指標 `index == total` 時，表示所有元素已決定完畢，印出目前被選擇的元素組合。
   * 遞迴分支：
     * 分支一：令 `selected[index] = false`，遞迴進入下一個元素。
     * 分支二：令 `selected[index] = true`，遞迴進入下一個元素。
   * 透過這種方式可完整覆蓋 $2^n$ 種所有子集組合，時間複雜度為 $\mathcal{O}(2^n)$。

## 程式實作
以下為主要程式碼：

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
```

## 效能分析

### Ackermann's Function 效能分析

#### 1. 時間複雜度 (Time Complexity)
Ackermann 函數的數值與呼叫次數呈**超指數級（Hyperoperation）**爆發性成長，其時間複雜度無法以常見的多項式（Polynomial）或一般指數（Exponential）時間表示。

* **固定 $m$ 時的時間成長**：
  * **$m = 0$**：$\mathcal{O}(1)$，一次計算即回傳 $n + 1$。
  * **$m = 1$**：$A(1, n) = n + 2 \implies \mathcal{O}(n)$。
  * **$m = 2$**：$A(2, n) = 2n + 3 \implies \mathcal{O}(n)$。
  * **$m = 3$**：$A(3, n) = 2^{n+3} - 3 \implies \mathcal{O}(2^n)$。
  * **$m = 4$**：$A(4, n) = 2^{2^{\cdot^{\cdot^2}}} - 3$（高度為 $n+3$ 的次方塔，Knuth 箭號表示法 $2 \uparrow\uparrow (n+3)$）$\implies \mathcal{O}(2 \uparrow\uparrow n)$。
* **整體結論**：遞迴與非遞迴版本的實質計算步數（迴圈/函式呼叫次數）完全等價，時間複雜度均為 **$\mathcal{O}(A(m, n))$**。

#### 2. 空間複雜度 (Space Complexity)
* **遞迴版本**：
  * **空間複雜度**：$\mathcal{O}(A(m, n))$。
  * **瓶頸分析**：遞迴呼叫的最大堆疊深度取決於 $A(m, n)$ 的計算路徑。由於作業系統分配給程式的系統 Call Stack 有限，當 $m \ge 4$ 且 $n \ge 1$ 時， Call Stack 會迅速填滿並引發 **Stack Overflow**。
* **非遞迴版本**：
  * **空間複雜度**：$\mathcal{O}(\text{MAX\_STACK\_SIZE})$。
  * **瓶頸分析**：利用全域陣列自訂 Stack，將資料空間轉移至靜態記憶體區（Data Segment）或 Heap。雖然成功避開了系統 Call Stack 的深度限制，但若 Stack 容量設定不夠大，在處理較大輸入時仍會遭遇自訂 Stack Overflow 的問題。

---

### Powerset 效能分析

#### 1. 時間複雜度 (Time Complexity)
* **時間複雜度**：$\mathcal{O}(2^n)$。
* **數學推導**：
  * 對於包含 $n$ 個元素的集合，每個元素皆有「選取 (Include)」與「不選取 (Exclude)」兩種獨立狀態，故總共有 $2^n$ 個不同的子集。
  * 在二元決策樹中，總節點數為 $2^0 + 2^1 + 2^2 + \dots + 2^n = 2^{n+1} - 1$。
  * 印出每個子集平均需消耗 $\mathcal{O}(n)$ 的走訪時間，總時間複雜度精確表示為 **$\mathcal{O}(n \cdot 2^n)$**。

#### 2. 空間複雜度 (Space Complexity)
* **空間複雜度**：$\mathcal{O}(n)$。
* **瓶頸分析**：
  * **遞迴呼叫堆疊**：遞迴樹的最大深度等於集合元素個數 $n$，因此 Call Stack 的最大深度為 $\mathcal{O}(n)$。
  * **輔助空間**：長度為 $n$ 的布林陣列 `selected` 用於記錄當前決策狀態，空間為 $\mathcal{O}(n)$。
  * 相比於直接將所有子集一次性儲存到記憶體中（需 $\mathcal{O}(n \cdot 2^n)$ 空間），這種**回溯法（Backtracking）**能將空間使用量大幅降至最優的 $\mathcal{O}(n)$。

---

### 遞迴與非遞迴版本綜合對比表

| 分析項目 | Ackermann (遞迴版) | Ackermann (非遞迴版) | Powerset (遞迴版) |
| :--- | :--- | :--- | :--- |
| **時間複雜度** | $\mathcal{O}(A(m, n))$ | $\mathcal{O}(A(m, n))$ | $\mathcal{O}(n \cdot 2^n)$ |
| **空間複雜度** | $\mathcal{O}(A(m, n))$ | $\mathcal{O}(\text{MAX\_STACK\_SIZE})$ | $\mathcal{O}(n)$ |
| **記憶體區域** | System Call Stack | Static / Heap Array | System Call Stack |
| **主要效能瓶頸** | 系統 Stack Overflow | 自訂 Stack 陣列大小限制 | 子集數量隨 $n$ 呈指數增長 |

## 測試與驗證

### 測試案例

### 測試環境 (Test Environment)

### Ackermann's Function 測試案例

為驗證遞迴與非遞迴版本之結果一致性與正確性，設計以下梯度測試案例：

#### 測試案例矩陣 (Test Cases Matrix)

| Case ID | 輸入 $(m, n)$ | 預期結果 $A(m, n)$ | 遞迴版結果 | 非遞迴版結果 | 驗證狀態 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **TC 1-1** | $(0, 0)$ | $1$ | $1$ | $1$ | **PASS** |
| **TC 1-2** | $(0, 5)$ | $6$ | $6$ | $6$ | **PASS** |
| **TC 1-3** | $(1, 0)$ | $2$ | $2$ | $2$ | **PASS** |
| **TC 1-4** | $(1, 3)$ | $5$ | $5$ | $5$ | **PASS** |
| **TC 1-5** | $(2, 2)$ | $7$ | $7$ | $7$ | **PASS** |
| **TC 1-6** | $(2, 4)$ | $11$ | $11$ | $11$ | **PASS** |
| **TC 1-7** | $(3, 2)$ | $29$ | $29$ | $29$ | **PASS** |
| **TC 1-8** | $(3, 4)$ | $125$ | $125$ | $125$ | **PASS** |

#### 極限與邊界測試分析 (Corner Cases Analysis)
1. **邊界值 $m = 0$**：驗證基本條件 $A(0, n) = n + 1$ 是否能立即觸發終止條件並正確回傳。
2. **邊界值 $n = 0$**：驗證 $A(m, 0) = A(m - 1, 1)$ 的遞減邏輯是否正常轉移。
3. **計算量遞增測試 $(3, 4)$**：遞迴呼叫次數達 10,307 次，非遞迴與遞迴版皆正確產出 $125$，證明自訂 Stack 在較高深度下無溢位現象。

---

### Powerset 測試案例

針對不同規模與邊界條件之集合，驗證子集產生的完整性與數量正確性（應符合 $2^n$ 個）：

#### 測試案例矩陣 (Test Cases Matrix)

| Case ID | 輸入集合 $S$ | 元素個數 $n$ | 預期子集總數 ($2^n$) | 實際輸出數量 | 驗證狀態 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **TC 2-1** | $\{\}$ (空集合) | $0$ | $1$ | $1$ | **PASS** |
| **TC 2-2** | $\{a\}$ | $1$ | $2$ | $2$ | **PASS** |
| **TC 2-3** | $\{a, b\}$ | $2$ | $4$ | $4$ | **PASS** |
| **TC 2-4** | $\{a, b, c\}$ | $3$ | $8$ | $8$ | **PASS** |
| **TC 2-5** | $\{1, 2, 3, 4\}$ | $4$ | $16$ | $16$ | **PASS** |


### 結論

### 1. 遞迴轉非遞迴的剖析
* **系統 Call Stack 的極限**：
  在遞迴版本中，每一次函式呼叫 `A(m, n)` 都需要在作業系統的 Call Stack（呼叫堆疊）中建立一個新的 Stack Frame（包含區域變數、參數與回傳位址）。Ackermann 函數的遞迴深度會隨輸入快速呈超指數級暴增，例如僅僅是 $A(4, 1)$，其結果數值已達到 $2^{65536} - 3$，需要的呼叫堆疊深度遠超作業系統預設的 Stack 容量限制（通常為 1MB 至 8MB），這會導致程式直接觸發 `Segmentation Fault` (Stack Overflow) 崩潰。
* **自訂 Stack 的優劣與局限**：
  非遞迴版本透過自訂全域陣列 `m_stack` 模擬 Call Stack 運作。其主要優勢在於將記憶體空間轉移至靜態資料區（Data Segment），避開了系統 Call Stack 極為有限的容量上限。然而，這種轉換**僅解決了記憶體管理的安全性，並未優化時間複雜度**。計算 $A(m, n)$ 所需的總計算步數（Loop 迭代次數與 Push/Pop 次數）與遞迴呼叫次數完全等價，面對超大輸入時依然會因計算時間過長而無法在有限時間內結束。

### 2. Powerset 演算法決策樹與空間最佳化思考
* **回溯法（Backtracking）的空間優點**：
  本實作採用二元決策樹的遞迴回溯法，對每個元素進行 Include / Exclude（選取/不選取）的分歧決策。此設計的最大效益在於**極致的空間利用率**：利用全域/傳遞的 `selected` 陣列紀錄路徑，並在達 Base Case 時即時印出，使得記憶體空間僅取決於遞迴樹的最大深度 $\mathcal{O}(n)$，而非結果總數量 $\mathcal{O}(2^n)$。
* **非遞迴替代方案—位元遮罩**：
  若在標頭檔與變數型別不受限的情況下，Powerset 的最佳非遞迴實作方式為 Bitmask。對於 $n$ 個元素的集合，可以用一個整數的二進位位元來代表每個元素的選取狀態（0 表示 Exclude，1 表示 Include）。只需一個簡單的 `for` 迴圈從 `0` 迭代至 `(1 << n) - 1`，即可在無遞迴堆疊開銷的情況下枚舉所有 $2^n$ 個子集。在 $n \le 64$ 的範圍內，位元運算的 CPU 執行效率顯著優於遞迴回溯。

### 3. 限制標頭檔下的架構設計與基礎功體會
* **標頭檔限制對開發的啟示**：
  本次作業限定僅能使用標準基礎標頭檔。這迫使開發者必須手動配置基礎陣列、宣告邊界檢查常數，並精確維護堆疊頂端指標 `top`。
* **開發收穫**：
  這種「回到基礎」的開發體驗，有助於深刻理解高階容器底層的記憶體分佈與資料結構運算細節。在缺乏 STL 自動記憶體擴展的情況下，程式碼的邊界防護（如防止自訂 Stack Overflow / Out of Bounds）與指標維護變得至關重要，能有效提升編寫高穩定度與高可移植性 C/C++ 程式的能力。

---

### 結論

本報告完整實作並驗證了 Ackermann 函數與 Powerset 問題。實驗結果顯示，遞迴轉非遞迴能有效解決系統 Call Stack 空間不足的問題，提高程式的強健性，但無法改善演算法本質的時間瓶頸；而 Powerset 採用回溯法策略，成功將空間開銷壓縮至最優的 $\mathcal{O}(n)$。全數程式碼均通過嚴格的邊界與極限測試，驗證結果精確無誤。

## 申論及開發報告

### 1. Ackermann 函數：為什麼遞迴會當機？非遞迴又好在哪？

* **遞迴版的痛點（記憶體爆炸）**
  遞迴就像是「事情做一半，先去叫別人做，自己站在原地等」。Ackermann 函數的數值成長極快，叫人的次數會呈幾何級數暴增。電腦用來記錄「誰在等誰」的空間（系統非常小，只要數字稍微大一點（例如 $m=4$），這個空間就會立刻被塞爆，導致程式直接當機。

* **非遞迴版的改進（用自己的陣列排隊）**
  非遞迴版本就像是我們自己準備一本筆記本（自訂 Stack 陣列），把「接下來要做的事」通通記在筆記本上，用 `while` 迴圈一件一件拿出來做。這樣就不會佔用電腦微薄的 Call Stack 空間，程式就不會輕易當機了。

* **學到的啟示**
  把遞迴改成非遞迴，**只能讓程式「比較不會當機」，但「不會讓運算變快」**。因為 Ackermann 函數需要的計算次數本身就是個天文數字，演算法本身的數學限制依然存在。

---

### 2. Powerset 演算法：如何有效列出所有子集？

* **選與不選的二元選擇**
  找 Powerset（所有子集）就像在做問卷調查，面對集合裡的每一個元素，只有兩種選擇：「**要放進子集**」或「**不放進子集**」。
  我們用遞迴一層一層做選擇，選到底之後印出結果，再退回上一步換另一個選擇。這種做法的好處是**非常省記憶體**，因為電腦一次只需要記住「當前這條選擇路線」，不用把所有結果一次全部塞進記憶體裡。

* **如果不用遞迴的另一種思路（位元開關）**
  如果不用遞迴，也可以把每個元素想像成一個「燈泡開關」（0 代表關/不選，1 代表開/選）。假設有 3 個元素，只要從 $000$ 數到 $111$（數字 0 到 7），就能把 8 種子集全部走一遍。這種方法在元素少的時候執行速度極快。

