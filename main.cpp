#include <bits/stdc++.h>

class Term {
public:
    char op;
    Term *l, *r;
    Term(Term* _l, char _op, Term* _r) : l(_l), op(_op), r(_r) {}
};

class Judgement {
public:
    std::queue<Term*> l, r;
};

bool isReduced(std::queue<Term*> qTerm) {
    bool flag = true;
    std::queue<Term*> tmp = qTerm;
    while (!tmp.empty()) {
        Term* t = tmp.front();
        tmp.pop();
        if (t->op == '!' || t->op == '&' || t->op == '|' || t->op == '>' || t->op == '=') {
            flag = 0;
            break;
        }
    }
    return flag;
}

void printTree(Term* u) {
    if (u->l != nullptr) {
        putchar('(');
        printTree(u->l);
        putchar(')');
    }
    putchar(u->op);
    if (u->r != nullptr) {
        putchar('(');
        printTree(u->r);
        putchar(')');
    }
}

void print(std::queue<Term*> qTerm) {
    std::queue<Term*> tmp = qTerm;
    while (!tmp.empty()) {
        Term* t = tmp.front();
        tmp.pop();
        printTree(t);
        puts("");
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
        Term* tmp = j.l.front();
        if (tmp->op == '!') {
            Judgement x = j;
            x.l.pop();
            x.r.push(tmp->r);
            return check(x);
        } else if (tmp->op == '&') {
            Judgement x = j;
            x.l.pop();
            x.l.push(tmp->l);
            x.l.push(tmp->r);
            return check(x);
        } else if (tmp->op == '|') {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.l.push(tmp->l);
            y.l.push(tmp->r);
            return check(x) && check(y);
        } else if (tmp->op == '>') {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.r.push(tmp->l);
            y.l.push(tmp->r);
            return check(x) && check(y);
        } else if (tmp->op == '=') {
            Judgement x = j, y = j;
            x.l.pop(), y.l.pop();
            x.l.push(tmp->l);
            x.l.push(tmp->r);
            y.r.push(tmp->l);
            y.r.push(tmp->r);
            return check(x) && check(y);
        } else {
            Judgement x = j;
            x.l.pop();
            x.l.push(tmp);
            return check(x);
        }
    }
        
    if (!isReduced(j.r)) {
        Term* tmp = j.r.front();
        if (tmp->op == '!') {
            Judgement x = j;
            x.r.pop();
            x.l.push(tmp->r);
            return check(x);
        } else if (tmp->op == '&') {
            Judgement x = j, y = j;
            x.r.pop(), y.r.pop();
            x.r.push(tmp->l);
            y.r.push(tmp->r);
            return check(x) && check(y);
        } else if (tmp->op == '|') {
            Judgement x = j;
            x.r.pop();
            x.r.push(tmp->l);
            x.r.push(tmp->r);
            return check(x);
        } else if (tmp->op == '>') {
            Judgement x = j;
            x.r.pop();
            x.l.push(tmp->l);
            x.r.push(tmp->r);            
            return check(x);
        } else if (tmp->op == '=') {
            Judgement x = j, y = j;
            x.r.pop(), y.r.pop();
            x.l.push(tmp->l);
            x.r.push(tmp->r);
            y.l.push(tmp->l);
            y.r.push(tmp->r);            
            return check(x) && check(y);
        } else {
            Judgement x = j;
            x.r.pop();
            x.r.push(tmp);
            return check(x);
        }
    }


    std::set<char> s;

    std::queue<Term*> tmpp = j.l;
    while (!tmpp.empty()) {
        Term* x = tmpp.front();
        tmpp.pop();
        s.insert(x->op);
    }
    tmpp = j.r;
    while (!tmpp.empty()) {
        Term* x = tmpp.front();
        tmpp.pop();
        if (s.count(x->op) != 0)
            return true;
    }
    return false;
}

bool cmp(char x, char y) { // x 是否优先于 y
    if (x == '!') {
        return y == '(';
    } else if (x == '&') {
        return y == '(' || y == '!';
    } else if (x == '|') {
        return y == '(' || y == '!' || y == '&';
    } else if (x == '>') {
        return y == '(' || y == '!' || y == '&' || y == '|';
    } else if (x == '=') {
        return y == '(' || y == '!' || y == '&' || y == '|' || y == '>';
    }
    return false;
}

Term* read() {

    std::string str, post;
    std::cin >> str;

    int n = str.length();
    std::stack<char> s;

    for (int i = 0; i < n; i++) {
        if (str[i] == '!' || str[i] == '&' || str[i] == '|' || str[i] == '>' || str[i] == '=') {
            if (s.empty() || cmp(str[i], s.top())) {
                s.push(str[i]);
            } else {
                while (!s.empty()) {
                    post.push_back(s.top());
                    s.pop();
                }
            }
        } else if (str[i] == '(') {
            s.push(str[i]);
        } else if (str[i] == ')') {
            while (s.top() != '(') {
                post.push_back(s.top());
                s.pop();
            }
            s.pop(); // 弹出最后一个 '('
        } else {
            post.push_back(str[i]);
        }
    }
    while (!s.empty()) {
        post.push_back(s.top());
        s.pop();
    }

    int m = post.length();

    std::stack<Term*> stk;
    for (int i = 0; i < m; i++) {
        if (post[i] == '!') {
            Term *top1 = stk.top();
            stk.pop();
            stk.push(new Term(0, post[i], top1));
        } else if (post[i] == '&') {
            Term *top1 = stk.top();
            stk.pop();
            Term *top2 = stk.top();
            stk.pop();
            stk.push(new Term(top2, post[i], top1));
        } else if (post[i] == '|') {
            Term *top1 = stk.top();
            stk.pop();
            Term *top2 = stk.top();
            stk.pop();
            stk.push(new Term(top2, post[i], top1));
        } else if (post[i] == '>') {
            Term *top1 = stk.top();
            stk.pop();
            Term *top2 = stk.top();
            stk.pop();
            stk.push(new Term(top2, post[i], top1));
        } else if (post[i] == '=') {
            Term *top1 = stk.top();
            stk.pop();
            Term *top2 = stk.top();
            stk.pop();
            stk.push(new Term(top2, post[i], top1));
        } else {
            stk.push(new Term((Term*)nullptr, post[i], (Term*)nullptr));
        }
    }
    return stk.top();
}

int main() {
    
    Judgement jj;
    jj.l.push(read());
    jj.r.push(read());

    puts(check(jj) ? "Yes" : "No");

    /*// e.g 1
    Term *a = new Term((Term*)nullptr, 'Q', (Term*)nullptr);
    Term *b = new Term((Term*)nullptr, '!', a); // !Q
    Term *c = new Term((Term*)nullptr, 'P', (Term*)nullptr);
    Term *d = new Term((Term*)nullptr, 'Q', (Term*)nullptr);
    Term *e = new Term(c, '>', d); // P > Q
    Term *f = new Term(b, '&', e); // (!Q) & (P > Q)

    Term *g = new Term((Term*)nullptr, 'P', (Term*)nullptr);
    Term *h = new Term((Term*)nullptr, '!', g); // !P

    Judgement jj;
    jj.l.push(f);
    jj.r.push(h);

    puts(check(jj) ? "Yes" : "No");*/

    /*// e.g 2
    Term *a = new Term((Term*)nullptr, 'P', (Term*)nullptr);
    Term *b = new Term((Term*)nullptr, 'Q', (Term*)nullptr);
    Term *c = new Term(a, '|', b); // P | Q
    Term *d = new Term((Term*)nullptr, 'P', (Term*)nullptr);
    Term *e = new Term((Term*)nullptr, 'R', (Term*)nullptr);
    Term *f = new Term(d, '>', e); // P > R
    Term *g = new Term(c, '&', f); // (P | Q) & (P > R)
    Term *h = new Term((Term*)nullptr, 'Q', (Term*)nullptr);
    Term *i = new Term((Term*)nullptr, 'S', (Term*)nullptr);
    Term *j = new Term(h, '>', i); // Q > S
    Term *k = new Term(g, '&', j); // ((P | Q) & (P > R)) & (Q > S)

    Term *l = new Term((Term*)nullptr, 'S', (Term*)nullptr);
    Term *m = new Term((Term*)nullptr, 'R', (Term*)nullptr);
    Term *n = new Term(l, '|', m); // S | R

    Judgement jj;
    jj.l.push(k);
    jj.r.push(n);

    puts(check(jj) ? "Yes" : "No");*/
    
    return 0;
}