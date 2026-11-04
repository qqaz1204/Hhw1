# 41343123

作業二

## 解題說明

本作業主要為實作多項式抽象資料型態 (Polynomial Abstract Data Type, ADT) 及其相關運算，並透過 C++ 運算子多載 (Operator Overloading) 實現靈活的多項式輸入與輸出功能。

### 多項式 (Polynomial) ADT
多項式 $p(x) = a_0 x^{e_0} + a_1 x^{e_1} + \dots + a_n x^{e_n}$ 可視為一組由有序對 $<e_i, a_i>$ 所組成的集合，其中 $a_i$ 為非零的浮點數係數。

本實作包含以下主要成員函式與運算子多載：
- **`Polynomial()`**：預設建構子，將多項式初始化為 $p(x) = 0$。
- **`Polynomial(const Polynomial &a)`**：複製建構子 (Copy Constructor)，實現多項式物件之深複製 (Deep Copy)。
- **`Polynomial& operator=(const Polynomial &a)`**：指定運算子 (Copy Assignment Operator)，避免動態記憶體配置帶來的 Dangling Pointer 與二次釋放崩潰問題。
- **`~Polynomial()`**：解構子，動態釋放多項式項陣列之記憶體空間。
- **`Add(Polynomial poly)`**：多項式加法，將 `*this` 與 `poly` 相加後回傳新的多項式物件。
- **`Mult(Polynomial poly)`**：多項式乘法，將 `*this` 與 `poly` 相乘後回傳新的多項式物件。
- **`Eval(float f)`**：代入求值，計算多項式在 $x = f$ 時的數值結果。
- **`operator>>`**：輸入運算子多載，支援自動排序與項數動態調整。
- **`operator<<`**：輸出運算子多載，將多項式以標準數學格式（如 $3x^2 + 2x + 1$）美化輸出。

---

### 解題策略

#### 1. 資料結構設計 (Data Structure Design)
* **`Term` 結構**：包含 `float coef`（係數）與 `int exp`（指數）。
* **`Polynomial` 類別**：包含動態 Term 陣列指標 `Term *termArray`、當前容量 `int capacity` 以及非零項數 `int terms`。當項數超過容量時，以兩倍擴充（Dynamic Resizing）方式彈性調整陣列大小。

#### 2. 多項式加法策略 (`Add`)
* **雙指標合併法 (Two-Pointer Merge Algorithm)**：使用兩個指標 `aPos` 與 `bPos` 分別走訪兩多項式的項陣列（前提為項陣列已依指數由大到小排序）：
  * **指數相同 ($e_a == e_b$)**：係數相加，若相加結果不為 0，則呼叫 `NewTerm` 新增至結果多項式，兩指標同步後移。
  * **$e_a > e_b$**：將 $a$ 的該項放入結果多項式，`aPos` 後移。
  * **$e_a < e_b$**：將 $b$ 的該項放入結果多項式，`bPos` 後移。
* 將剩餘未走訪完畢的項依序加入結果多項式中。

#### 3. 多項式乘法策略 (`Mult`)
* **雙重迴圈展開與累加**：
  * 對於多項式 $A$ 的每一項 $a_i x^{e_{a_i}}$ 與多項式 $B$ 的每一項 $b_j x^{e_{b_j}}$ 進行交叉相乘。
  * 新項係數為 $a_i \times b_j$，新項指數為 $e_{a_i} + e_{b_j}$。
  * 將每一項乘積暫存於單項多項式 `temp` 中，並透過呼叫 `c = c.Add(temp)` 自動進行同類項合併與簡化。

#### 4. 輸入與輸出多載策略 (`operator>>` & `operator<<`)
* **輸入多載 (`>>`)**：讀取使用者輸入的項數與各項 (coef, exp)，並使用泡沫排序法 (Bubble Sort) 將輸入項依指數高低（降冪）重新排序，以維持 ADT 內部結構之規範。
* **輸出多載 (`<<`)**：針對首項、常數項 ($x^0$)、一次項 ($x^1$) 以及正負號進行格式化輸出，確保符合數學撰寫習慣（如隱藏 $1x$ 中的 $1$、忽略係數為 $0$ 的項等）。

---

## 程式實作

以下為完整 C++ 程式碼實作：

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Polynomial; // Forward declaration

struct Term {
    float coef; // coefficient (係數)
    int exp;    // exponent (指數)
};

class Polynomial {
private:
    Term *termArray; // 儲存非零項之動態陣列
    int capacity;    // termArray 的容量大小
    int terms;       // 目前非零項的數量

public:
    Polynomial();                    // Constructor: p(x) = 0
    Polynomial(const Polynomial &a); // 複製建構子
    ~Polynomial();                   // 解構子

    Polynomial& operator=(const Polynomial &a); // 指定運算子 (深複製)

    Polynomial Add(Polynomial poly);  // 多項式加法
    Polynomial Mult(Polynomial poly); // 多項式乘法
    float Eval(float f);              // 代入求值

    void NewTerm(float theCoef, int theExp); // 新增非零項

    friend istream &operator>>(istream &s, Polynomial &p);
    friend ostream &operator<<(ostream &s, const Polynomial &p);
};

// ============================================================================
// 建構與解構子實作
// ============================================================================

Polynomial::Polynomial() {
    terms = 0;
    capacity = 10;
    termArray = new Term[capacity];
}

Polynomial::Polynomial(const Polynomial &a) {
    terms = a.terms;
    capacity = a.capacity;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = a.termArray[i];
    }
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

// 指定運算子 (防止淺複製造成的 Heap 崩潰)
Polynomial& Polynomial::operator=(const Polynomial &a) {
    if (this == &a) return *this;
    delete[] termArray;
    
    terms = a.terms;
    capacity = a.capacity;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = a.termArray[i];
    }
    return *this;
}

// ============================================================================
// 成員函式實作
// ============================================================================

void Polynomial::NewTerm(float theCoef, int theExp) {
    if (theCoef == 0.0f) return;
    if (terms == capacity) {
        capacity *= 2;
        Term *temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i];
        }
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    terms++;
}

Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < poly.terms)) {
        if (termArray[aPos].exp == poly.termArray[bPos].exp) {
            float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
            if (sum != 0) {
                c.NewTerm(sum, termArray[aPos].exp);
            }
            aPos++;
            bPos++;
        } else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
            c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
            bPos++;
        } else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++) {
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    }
    for (; bPos < poly.terms; bPos++) {
        c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
    }
    return c;
}

Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial c;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < poly.terms; j++) {
            float newCoef = termArray[i].coef * poly.termArray[j].coef;
            int newExp = termArray[i].exp + poly.termArray[j].exp;
            temp.NewTerm(newCoef, newExp);
        }
        c = c.Add(temp);
    }
    return c;
}

float Polynomial::Eval(float f) {
    float result = 0.0f;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

// ============================================================================
// I/O 運算子多載實作
// ============================================================================

istream &operator>>(istream &s, Polynomial &p) {
    delete[] p.termArray;
    cout << "輸入多項式項數 (terms): ";
    s >> p.terms;
    p.capacity = max(p.terms, 10);
    p.termArray = new Term[p.capacity];

    cout << "請依序輸入每一項的「係數(coef)」與「指數(exp)」:\n";
    for (int i = 0; i < p.terms; i++) {
        cout << "第 " << i + 1 << " 項 (coef exp): ";
        s >> p.termArray[i].coef >> p.termArray[i].exp;
    }

    // 依指數降冪排序
    for (int i = 0; i < p.terms - 1; i++) {
        for (int j = 0; j < p.terms - i - 1; j++) {
            if (p.termArray[j].exp < p.termArray[j + 1].exp) {
                Term temp = p.termArray[j];
                p.termArray[j] = p.termArray[j + 1];
                p.termArray[j + 1] = temp;
            }
        }
    }
    return s;
}

ostream &operator<<(ostream &s, const Polynomial &p) {
    if (p.terms == 0) {
        s << "0";
        return s;
    }
    for (int i = 0; i < p.terms; i++) {
        if (i > 0 && p.termArray[i].coef > 0) {
            s << " + ";
        } else if (p.termArray[i].coef < 0) {
            s << " - ";
        } else if (i > 0) {
            s << " + ";
        }

        float c = abs(p.termArray[i].coef);
        int e = p.termArray[i].exp;

        if (e == 0) {
            s << c;
        } else if (e == 1) {
            if (c != 1.0f) s << c;
            s << "x";
        } else {
            if (c != 1.0f) s << c;
            s << "x^" << e;
        }
    }
    return s;
}

// ============================================================================
// 主程式 (Main Testing Function)
// ============================================================================

int main() {
    Polynomial p1, p2;
    cout << "=== Homework 2 Testing ===" << endl;

    cout << "\n=== 測試 p1 ===" << endl;
    cin >> p1;
    cout << "p1(x) = " << p1 << endl;

    cout << "\n=== 測試 p2 ===" << endl;
    cin >> p2;
    cout << "p2(x) = " << p2 << endl;

    cout << "\n=== 運算結果 ===" << endl;
    cout << "p1 + p2 = " << p1.Add(p2) << endl;
    cout << "p1 * p2 = " << p1.Mult(p2) << endl;
    return 0;
}

