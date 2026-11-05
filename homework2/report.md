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
  
- **輸入運算子 (`>>`)**：支援從標準輸入讀取多項式的項數與各項係數、指數，並確保內部資料結構符合降冪排序與 ADT 規範。
- **輸出運算子 (`<<`)**：支援將多項式物件直接透過 `cout` 輸出，轉換為易讀的數學表達式格式（如 $3x^2 + 2x + 1$）。
  
---
## 程式實作

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

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

## 效能分析

設多項式 $A$ 之非零項數為 $m$，多項式 $B$ 之非零項數為 $n$：

| 演算法 / 成員函式 | 時間複雜度 (Time) | 空間複雜度 (Space) | 主要效能瓶頸與運算細節 |
| :--- | :---: | :---: | :--- |
| **`Polynomial::Add`** | $\mathcal{O}(m + n)$ | $\mathcal{O}(m + n)$ | **雙指標線性合併**：同步走訪兩多項式項陣列，最壞情況比較 $m+n$ 次；遇到指數相同時進行係數相加。 |
| **`Polynomial::Mult`** | $\mathcal{O}(m^2 \cdot n)$ | $\mathcal{O}(m \cdot n)$ | **雙重迴圈與同類項合併**：產生 $m \times n$ 個相乘項，每次透過 `Add()` 進行合併，累積時間為 $\mathcal{O}(m^2 \cdot n)$。 |
| **`Polynomial::Eval`** | $\mathcal{O}(m)$ | $\mathcal{O}(1)$ | **單次線性走訪**：走訪 $m$ 個非零項，每項呼叫 `std::pow()` 計算次方並進行乘加運算。 |
| **`operator>>` (Sort)** | $\mathcal{O}(k^2)$ | $\mathcal{O}(k)$ | **降冪排序開銷**：輸入 $k$ 項後，使用氣泡排序法 (Bubble Sort) 確保多項式項陣列依指數大至小排列。 |

## 測試與驗證 

針對多項式加法、乘法、代入求值與消去項等邊界條件，設計以下測試案例矩陣：

| 測試編號 (Case ID) | 輸入 $p_1(x)$ | 輸入 $p_2(x)$ | 測試操作 (Operation) | 預期輸出結果 (Expected Result) 
| :---: | :--- | :--- | :--- | :--- 
| **1** | $3x^2 + 2x + 1$ | $x + 5$ | `p1 + p2` | `3x^2 + 3x + 6` 
| **2** | $3x^2 + 2x + 1$ | $x + 5$ | `p1 * p2` | `3x^3 + 17x^2 + 11x + 5` 
| **3** | $x^2 - 4$ | $-x^2 + 4$ | `p1 + p2` | `0` (所有同類項抵銷) 
| **4** | $2x^3 - 5$ | $0$ (空多項式) | `p1 * p2` | `0` (零多項式相乘) 
| **5** | $3x + 2x^3 + 1$ | — | `cin >> p1` (未排序輸入) | `2x^3 + 3x + 1` (自動降冪排序) 

## 結論

| 檢討項目 | 開發重點與心得總結 |
| :--- | :--- |
| **1. 記憶體管理 (Rule of Three)** | 在 C++ 中使用動態陣列指標（`Term *termArray`）時，若未妥善實作**複製建構子 (Copy Constructor)** 與**指定運算子 (`operator=`)**，容易因為淺複製導致記憶體重複釋放或指向無效記憶體。本次作業完整實作了深複製機制，確保物件在頻繁回傳與賦值時的穩定性。 |
| **2. 稀疏多項式效率評估** | 採用「非零項陣列」來儲存多項式，其最大優勢在於**空間極致節省**，特別適合處理高次方但項數極少的稀疏多項式（例如 $x^{1000} + 1$）。但在進行多項式相乘時，因需頻繁進行同類項合併與排序，時間複雜度較高，未來可評估改用鏈結串列進行優化。 |
| **3. 綜合評述** | 本作業成功實作並驗證了多項式 ADT 之加法、乘法、代入求值與 I/O 運算子多載。各項測試案例與效能分析結果顯示，程式碼在邊界條件（如零多項式、消去項、未排序輸入等）下皆能正確執行，記憶體管理安全無虞。 |

## 申論及開發報告

### 1. 多項式 ADT 與記憶體安全：為什麼需要 Rule of Three？

- **淺複製（Shallow Copy）導致的崩潰痛點**
  在 C++ 類別中，若包含動態記憶體配置的指標成員（如本作業的 `Term *termArray`），當進行物件賦值（如 `p3 = p1.Add(p2)`）或作為參數傳值時，C++ 預設的複製行為僅會複製指標的位址。這會導致多個 `Polynomial` 物件指向同一塊 Heap 記憶體空間。當其中一個物件離開範疇觸發解構子並執行 `delete[] termArray` 後，其餘物件的指標將會變成 **懸空指標**；而在程式結束、其餘物件亦呼叫解構子時，更會引發 **二次釋放** 的嚴重崩潰。

- **深複製（Deep Copy）與 Rule of Three 的實踐**
  為了解決上述問題，本實作完整遵循了 C++ 的 **Rule of Three**：
  1. **解構子 (`~Polynomial()`)**：確保物件生命週期結束時正確釋放動態記憶體。
  2. **複製建構子 (`Polynomial(const Polynomial &a)`)**：在建立新物件時，重新配置獨立的 `termArray` 並複製內容。
  3. **複製指定運算子 (`operator=`)**：在物件賦值時，先釋放舊有記憶體，再配置新記憶體進行內容拷貝。
  
  透過深複製機制，每個多項式物件均擁有獨立的記憶體空間，成功消除了指標衝突與記憶體洩漏的風險，確保程式在進行複雜的 `Add` 與 `Mult` 運算時能穩定執行。

---

### 2. 多項式的架構優劣與改進思考

- **以非零項陣列表示多項式的優勢**
  本作業採用「非零項陣列」來儲存多項式。這種設計相較於傳統「以陣列索引（Index）代表指數」的稠密表示法，最大好處在於 **極致的空間利用率**。例如對於高次稀疏多項式 $p(x) = 100x^{10000} + 1$，稠密表示法需要配置大小為 10,001 的陣列（其中絕大多數為 0），而本實作的稀疏陣列只需配置 2 個 `Term` 空間即可，大幅降低了記憶體浪費。

- **時間複雜度的瓶頸與潛在優化**
  儘管稀疏多項式在空間上具備極大優勢，但其在多項式乘法的運算效率上會面臨瓶頸：
  - **目前的瓶頸**：乘法運算會產生 $m \times n$ 個交叉相乘項，本實作每次產生新項後即呼叫 `Add()` 進行合併與重排，時間複雜度高達 $\mathcal{O}(m^2 \cdot n)$。
  - **未來改進方向**：若改用 **鏈結串列** 或 **Hash Table / Map** 來實作，可利用串列指標插入的特性，直接在相乘過程中將同類項快速累加與排序，避免動態陣列重複擴充與複製的額外開銷。

---

### 3. 運算子多載的體會

- **運算子多載提升程式可讀性**
  透過對 `<<` 與 `>>` 運算子進行多載，大幅提升了類別的使用體驗。使用者可以像操作基本型別（如 `int`、`float`）一樣，直接使用 `cin >> p1` 與 `cout << p1` 進行多項式的輸入與列印，不必呼叫繁瑣的內部顯示函式。

- **輸入容錯與資料結構維護**
  在 `operator>>` 的實作中，除了讀取使用者輸入的係數與指數外，更加入了 **氣泡排序 (Bubble Sort)** 機制。這確保了無論使用者輸入的指數順序為何（如未按順序輸入 $3x + 2x^3 + 1$），系統內部都會自動將其重排為標準降冪格式（$2x^3 + 3x + 1$），嚴格維持了多項式 ADT 內部結構的一致性與規範。

























