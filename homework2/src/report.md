# 41343123 

## 解題說明

本作業主要依據課程規格，實作多項式抽象資料型態及其相關運算，並透過 C++ 運算子多載實現多項式的輸入與輸出功能。

## 解題策略

多項式可表示為數學式：
$$p(x) = a_0x^{e_0} + a_1x^{e_1} + \dots + a_nx^{e_n}$$
其本質為一組有序對 $\langle e_i, a_i \rangle$ 的集合，其中 $a_i$ 為非零的浮點數係數，$e_i$ 為非負整數指數。

類別必須具備以下核心介面。
- **`Polynomial()`**：建構預設多項式 $p(x) = 0$。
- **`Polynomial Add(Polynomial poly)`**：將當前多項式 `*this` 與傳入的 `poly` 進行相加，並回傳相加後的新多項式物件。
- **`Polynomial Mult(Polynomial poly)`**：將當前多項式 `*this` 與傳入的 `poly` 進行相乘，並回傳相乘後的新多項式物件。
- **`float Eval(float f)`**：將數值 $f$ 代入當前多項式 `*this` 中進行求值，並回傳計算結果。
- 
- **輸入運算子 (`>>`)**：支援從標準輸入讀取多項式的項數與各項係數、指數，並確保內部資料結構符合降冪排序與 ADT 規範。
- **輸出運算子 (`<<`)**：支援將多項式物件直接透過 `cout` 輸出，轉換為易讀的數學表達式格式（如 $3x^2 + 2x + 1$）。
  
---
## 程式實作

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;
// Forward declaration

struct Term {
    float coef; 
    int exp;    
};

class Polynomial {
private:
    Term *termArray; 
    int capacity;   
    int terms;       

public:
    Polynomial();                   
    Polynomial(const Polynomial &a); 
    ~Polynomial();                   

    Polynomial& operator=(const Polynomial &a); 

    Polynomial Add(Polynomial poly);  
    Polynomial Mult(Polynomial poly); 
    float Eval(float f);              

    void NewTerm(float theCoef, int theExp); 

    friend istream &operator>>(istream &s, Polynomial &p);
    friend ostream &operator<<(ostream &s, const Polynomial &p);
};


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

istream &operator>>(istream &s, Polynomial &p) {
    delete[] p.termArray;
    cout << "輸入多項式項數: ";
    s >> p.terms;
    p.capacity = max(p.terms, 10);
    p.termArray = new Term[p.capacity];

    cout << "請依序輸入每一項的「係數」與「指數」:\n";
    for (int i = 0; i < p.terms; i++) {
        cout << "第 " << i + 1 << " 項 (coef exp): ";
        s >> p.termArray[i].coef >> p.termArray[i].exp;
    }

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


int main() {
    Polynomial p1, p2;
    cout << "===Testing ===" << endl;

    cout << "\n===p1 ===" << endl;
    cin >> p1;
    cout << "p1(x) = " << p1 << endl;

    cout << "\n===p2 ===" << endl;
    cin >> p2;
    cout << "p2(x) = " << p2 << endl;

    cout << "\n=== 運算結果 ===" << endl;
    cout << "p1 + p2 = " << p1.Add(p2) << endl;
    cout << "p1 * p2 = " << p1.Mult(p2) << endl;
    return 0;
}
```

##　效能分析

設多項式 $A$ 之非零項數為 $m$，多項式 $B$ 之非零項數為 $n$：

| 演算法 / 成員函式 | 時間複雜度 (Time) | 空間複雜度 (Space) | 主要效能瓶頸與運算細節 |
| :--- | :---: | :---: | :--- |
| **`Polynomial::Add`** | $\mathcal{O}(m + n)$ | $\mathcal{O}(m + n)$ | **雙指標線性合併**：同步走訪兩多項式項陣列，最壞情況比較 $m+n$ 次；遇到指數相同時進行係數相加。 |
| **`Polynomial::Mult`** | $\mathcal{O}(m^2 \cdot n)$ | $\mathcal{O}(m \cdot n)$ | **雙重迴圈與同類項合併**：產生 $m \times n$ 個相乘項，每次透過 `Add()` 進行合併，累積時間為 $\mathcal{O}(m^2 \cdot n)$。 |
| **`Polynomial::Eval`** | $\mathcal{O}(m)$ | $\mathcal{O}(1)$ | **單次線性走訪**：走訪 $m$ 個非零項，每項呼叫 `std::pow()` 計算次方並進行乘加運算。 |
| **`operator>>` (Sort)** | $\mathcal{O}(k^2)$ | $\mathcal{O}(k)$ | **降冪排序開銷**：輸入 $k$ 項後，使用氣泡排序法 (Bubble Sort) 確保多項式項陣列依指數大至小排列。 |

##　測試與驗證 

針對多項式加法、乘法、代入求值與消去項等邊界條件，設計以下測試案例矩陣：

| 測試編號 (Case ID) | 輸入 $p_1(x)$ | 輸入 $p_2(x)$ | 測試操作 (Operation) | 預期輸出結果 (Expected Result) | 驗證狀態 (Status) |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC 2-1** | $3x^2 + 2x + 1$ | $x + 5$ | `p1 + p2` | `3x^2 + 3x + 6` | **PASS** |
| **TC 2-2** | $3x^2 + 2x + 1$ | $x + 5$ | `p1 * p2` | `3x^3 + 17x^2 + 11x + 5` | **PASS** |
| **TC 2-3** | $3x^2 + 2x + 1$ | — | `p1.Eval(2.0)` | `17` | **PASS** |
| **TC 2-4** | — | $x + 5$ | `p2.Eval(2.0)` | `7` | **PASS** |
| **TC 2-5** | $x^2 - 4$ | $-x^2 + 4$ | `p1 + p2` | `0` (所有同類項抵銷) | **PASS** |
| **TC 2-6** | $2x^3 - 5$ | $0$ (空多項式) | `p1 * p2` | `0` (零多項式相乘) | **PASS** |
| **TC 2-7** | $3x + 2x^3 + 1$ | — | `cin >> p1` (未排序輸入) | `2x^3 + 3x + 1` (自動降冪排序) | **PASS** |

## 結論與開發心得 

| 檢討項目 | 開發重點與心得總結 |
| :--- | :--- |
| **1. 記憶體管理 (Rule of Three)** | 在 C++ 中使用動態陣列指標（`Term *termArray`）時，若未妥善實作**複製建構子 (Copy Constructor)** 與**指定運算子 (`operator=`)**，容易因為淺複製導致記憶體重複釋放或指向無效記憶體。本次作業完整實作了深複製機制，確保物件在頻繁回傳與賦值時的穩定性。 |
| **2. 稀疏多項式效率評估** | 採用「非零項陣列」來儲存多項式，其最大優勢在於**空間極致節省**，特別適合處理高次方但項數極少的稀疏多項式（例如 $x^{1000} + 1$）。但在進行多項式相乘時，因需頻繁進行同類項合併與排序，時間複雜度較高，未來可評估改用鏈結串列進行優化。 |
| **3. 綜合評述** | 本作業成功實作並驗證了多項式 ADT 之加法、乘法、代入求值與 I/O 運算子多載。各項測試案例與效能分析結果顯示，程式碼在邊界條件（如零多項式、消去項、未排序輸入等）下皆能正確執行，記憶體管理安全無虞。 |



























