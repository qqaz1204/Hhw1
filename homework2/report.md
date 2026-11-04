# 41343123 作業二：多項式 (Polynomial) ADT 與運算子多載

## 核心功能說明

本作業實作了多項式抽象資料型態及其相關運算，並透過 C++ 運算子多載實現標準的多項式輸入與輸出功能。

### 1. 多項式 ADT 介面與資料結構
- **`Term` 結構**：包含 `float coef`與 `int exp`。
- **`Polynomial` 類別**：包含動態 Term 陣列指標 `Term *termArray`、容量 `int capacity` 與非零項數 `int terms`。
- **建構與解構**：
  - `Polynomial()`：預設建構子，初始化多項式為 $p(x) = 0$。
  - `Polynomial(const Polynomial &a)`：複製建構子，實現深複製 (Deep Copy)。
  - `Polynomial& operator=(const Polynomial &a)`：指定運算子，防止淺複製造成的記憶體崩潰。
  - `~Polynomial()`：解構子，釋放動態配置記憶體。
- **運算函式**：
  - `Polynomial Add(Polynomial poly)`：多項式加法。
  - `Polynomial Mult(Polynomial poly)`：多項式乘法。
  - `float Eval(float f)`：代入 $x = f$ 之求值運算。
  - `void NewTerm(float theCoef, int theExp)`：動態新增非零項（具容量加倍機制）。
- **I/O 運算子多載**：
  - `friend istream &operator>>(istream &s, Polynomial &p)`：輸入多項式並自動進行指數降冪排序。
  - `friend ostream &operator<<(ostream &s, const Polynomial &p)`：將多項式以標準數學格式美化輸出。

---
## 程式碼 

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
```
