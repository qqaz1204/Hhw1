```cpp
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Polynomial; 

struct Term {
    float coef; 
    int exp;    
};

class Polynomial {
private:
    Term* termArray; 
    int capacity;    
    int terms;       

public:
    Polynomial();                   
    Polynomial(const Polynomial& a); 
    ~Polynomial();                   

    Polynomial& operator=(const Polynomial& a);
    Polynomial Add(Polynomial poly);
    Polynomial Mult(Polynomial poly);
    float Eval(float f);

    void NewTerm(float theCoef, int theExp);

    friend istream& operator>>(istream& s, Polynomial& p);
    friend ostream& operator<<(ostream& s, const Polynomial& p);
};

Polynomial::Polynomial() {
    terms = 0;
    capacity = 10;
    termArray = new Term[capacity];
}

Polynomial::Polynomial(const Polynomial& a) {
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

// 指定運算子實作 (深複製)
Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this == &a) {
        return *this;
    }
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
        Term* temp = new Term[capacity];
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
        }
        else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
            c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
            bPos++;
        }
        else {
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

istream& operator>>(istream& s, Polynomial& p) {
    delete[] p.termArray;
    cout << "輸入多項式項數: ";
    s >> p.terms;
    p.capacity = max(p.terms, 10);
    p.termArray = new Term[p.capacity];

    cout << "依序輸入每一項的「係數」與「指數」:\n";
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

ostream& operator<<(ostream& s, const Polynomial& p) {
    if (p.terms == 0) {
        s << "0";
        return s;
    }
    for (int i = 0; i < p.terms; i++) {
        if (i > 0 && p.termArray[i].coef > 0) {
            s << " + ";
        }
        else if (p.termArray[i].coef < 0) {
            s << " - ";
        }
        else if (i > 0) {
            s << " + ";
        }

        float c = abs(p.termArray[i].coef);
        int e = p.termArray[i].exp;

        if (e == 0) {
            s << c;
        }
        else if (e == 1) {
            if (c != 1.0f) s << c;
            s << "x";
        }
        else {
            if (c != 1.0f) s << c;
            s << "x^" << e;
        }
    }
    return s;
}

int main() {
    Polynomial p1, p2;
    cout << "=== p1 ===" << endl;
    cin >> p1;
    cout << "p1(x) = " << p1 << endl << endl;

    cout << "=== p2 ===" << endl;
    cin >> p2;
    cout << "p2(x) = " << p2 << endl << endl;

    cout << "p1 + p2 = " << p1.Add(p2) << endl;
    cout << "p1 * p2 = " << p1.Mult(p2) << endl;
    cout << "p1(2.0) = " << p1.Eval(2.0f) << endl;
    cout << "p2(2.0) = " << p2.Eval(2.0f) << endl;
    return 0;
}
```
