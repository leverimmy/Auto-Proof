#include <bits/stdc++.h>

class Term {
public:
    char l, r;
    int op;
    Term(char _l, int _op, char _r) : l(_l), r(_r), op(_op) {}
};

class Judgement {
public:
    std::queue<Term> l, r;
};

bool isReduced(std::queue<Term> qTerm) {
    bool flag = true;
    std::queue<Term> tmp = qTerm;
    while (!tmp.empty()) {
        Term t = tmp.front();
        tmp.pop();
        if (t.op != 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}

void print(std::queue<Term> qTerm) {
    std::queue<Term> tmp = qTerm;
    while (!tmp.empty()) {
        Term t = tmp.front();
        tmp.pop();
        printf("%c %d %c\n", t.l, t.op, t.r);
    }
}

bool check(Judgement j) {
    puts("---Start---");
    puts("L:");
    print(j.l);
    puts("R:");
    print(j.r);
    puts("---End---");
    if (!isReduced(j.l)) {
        Term tmp = j.l.front();
        if (tmp.op == 0) {
            Judgement x = j;
            x.l.pop();
            x.l.push(tmp);
            return check(x);
        } else if (tmp.op == 1) {
            Judgement x = j;
            x.l.pop();
            x.r.push(Term(0, 0, tmp.r));
            return check(x);
        } else if (tmp.op == 2) {
            Judgement x = j;
            x.l.pop();
            x.l.push(Term(0, 0, tmp.l));
            x.l.push(Term(0, 0, tmp.r));
            return check(x);
        } else if (tmp.op == 3) {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.l.push(Term(0, 0, tmp.l));
            y.l.push(Term(0, 0, tmp.r));
            return check(x) && check(y);
        } else if (tmp.op == 4) {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.r.push(Term(0, 0, tmp.l));
            y.l.push(Term(0, 0, tmp.r));
            return check(x) && check(y);
        } else if (tmp.op == 5) {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.l.push(Term(0, 0, tmp.l));
            x.l.push(Term(0, 0, tmp.r));
            y.r.push(Term(0, 0, tmp.l));
            y.r.push(Term(0, 0, tmp.r));
            return check(x) && check(y);
        }
    }
        
    if (!isReduced(j.r)) {
        Term tmp = j.r.front();
        if (tmp.op == 0) {
            Judgement x = j;
            x.r.pop();
            x.r.push(tmp);
            return check(x);
        } else if (tmp.op == 1) {
            Judgement x = j;
            x.r.pop();
            x.l.push(Term(0, 0, tmp.r));
            return check(x);
        } else if (tmp.op == 2) {
            Judgement x = j, y = j;
            x.r.pop(), y.r.pop();
            x.r.push(Term(0, 0, tmp.l));
            y.r.push(Term(0, 0, tmp.r));
            return check(x) && check(y);
        } else if (tmp.op == 3) {
            Judgement x = j;
            x.r.pop();
            x.r.push(Term(0, 0, tmp.l));
            x.r.push(Term(0, 0, tmp.r));
            return check(x);
        } else if (tmp.op == 4) {
            Judgement x = j;
            x.r.pop();
            x.l.push(Term(0, 0, tmp.l));
            x.r.push(Term(0, 0, tmp.r));
            return check(x);
        } else if (tmp.op == 5) {
            Judgement x = j, y = j;
            x.r.pop(), y.r.pop();
            x.l.push(Term(0, 0, tmp.l));
            x.r.push(Term(0, 0, tmp.r));
            y.l.push(Term(0, 0, tmp.l));
            y.r.push(Term(0, 0, tmp.r));
            return check(x) && check(y);
        }
    }


    std::set<char> s;

    std::queue<Term> tmpp = j.l;
    while (!tmpp.empty()) {
        Term x = tmpp.front();
        tmpp.pop();
        s.insert(x.r);
    }
    tmpp = j.r;
    while (!tmpp.empty()) {
        Term x = tmpp.front();
        tmpp.pop();
        if (s.count(x.r) != 0)
            return true;
    }
    return false;
}

int main() {
    /*Term a = Term(0, 1, 'Q');
    Term b = Term('P', 4, 'Q');
    Term c = Term(0, 1, 'P');
    Judgement jj;
    jj.l.push(a), jj.l.push(b);
    jj.r.push(c);
    puts(check(jj) ? "Yes" : "No");*/

    Term a = Term('P', 3, 'Q');
    Term b = Term('P', 4, 'R');
    Term c = Term('Q', 4, 'S');
    Term d = Term('S', 3, 'R');
    Judgement jj;
    jj.l.push(a), jj.l.push(b), jj.l.push(c);
    jj.r.push(d);
    puts(check(jj) ? "Yes" : "No");
    return 0;
}