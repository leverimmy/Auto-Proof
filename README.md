# Auto Proof

Repository for the codes of Discrete Mathematics(1), Fall 2023

**目录**
- [Auto Proof](#auto-proof)
  - [作业描述](#作业描述)
  - [代码实现](#代码实现)
    - [中缀表达式转为后缀表达式](#中缀表达式转为后缀表达式)
    - [后缀表达式转为表达式树](#后缀表达式转为表达式树)
    - [模拟王浩算法](#模拟王浩算法)
      - [算法主体](#算法主体)
      - [判断是否已经化简完毕](#判断是否已经化简完毕)
      - [判断是否为重言式](#判断是否为重言式)
  - [总结和心得](#总结和心得)

## 作业描述

输入两个逻辑表达式 $P$ 和 $Q$（以中缀表达式的形式），通过模拟 [王浩算法](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=5392526)，判断 $P \to Q$ 是否为重言式。

在输入数据中，使用 `!` 表示 **非**，`&` 表示 **与**，`|` 表示 **或**，`>` 表示 **蕴含**，`=` 表示 **双蕴含**。

如果 $P \to Q$ 为重言式，则输出 `Yes`；否则输出 `No`。

## 代码实现

### 中缀表达式转为后缀表达式

我们可以用一个 **栈** 来存储运算符，用一个 `std::string` 来存储输出后缀表达式。转化规则：

1. 确立各个操作符的优先级：`(`, `)` > `!` > `&` > `|` > `>` > `=`。
2. 当扫描到左右括号，则直接入栈；若扫描到的是运算符：
    栈顶操作符的优先级大于或等于当前运算符优先级且栈不为空时，循环执行出栈操作并加入后缀表达式中，直到遇到优先级小于当前运算符的元素为止。循环执行完后再将当前运算符压栈。特别注意的是，**只有遇到右括号时，左括号才出栈。**

特别注意：不管是右括号还是弹出的左括号都不执行输出操作。

```cpp
// 已知 str 为中缀表达式
// bool cmp(char a, char b) 如果 a 的优先级 >= b，则返回 true；否则返回 false

int n = str.length();
std::stack<char> s;

for (int i = 0; i < n; i++) {
    if (str[i] == '!' || str[i] == '&' || str[i] == '|' || str[i] == '>' || str[i] == '=') {
        while (!s.empty() && cmp(s.top(), str[i])) { // 先把所有 >= 的操作符弹出
            post.push_back(s.top());
            s.pop();
        }
        s.push(str[i]);
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
```

### 后缀表达式转为表达式树

从左到右扫描后缀表达式，如果当前符号为命题变元，则压入栈中；否则一定为操作符，则对栈顶和次顶元素进行运算后压入栈中。在进行运算的同时，构建表达式树并且连接左右子树。

```cpp
// 已知 post 为后缀表达式
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
```

### 模拟王浩算法

#### 算法主体

按照教材上的 $10$ 种情况进行讨论即可。值得注意的是前件为空公式的情况，如果不进行特判，则会出错，所以我用 $P \lor \lnot P$ 替代了空公式。

```cpp
if (str.length() == 0)
    str = "P|!P";
```

#### 判断是否已经化简完毕

```cpp
bool isReduced(std::queue<Term*> qTerm) {
    bool flag = true;
    std::queue<Term*> tmp = qTerm;
    while (!tmp.empty()) {
        Term* t = tmp.front();
        tmp.pop();
        if (t->op == '!' || t->op == '&' || t->op == '|' || t->op == '>' || t->op == '=') { // 如果已经化简完毕，则所有的 op 都应该是命题变元（单个字母）
            flag = 0;
            break;
        }
    }
    return flag;
}
```

#### 判断是否为重言式

```cpp
// 已知 Judgement j
std::set<char> s;

std::queue<Term*> tmpp = j.l;
while (!tmpp.empty()) {
    Term* x = tmpp.front();
    tmpp.pop();
    s.insert(x->op); // 拿出所有 Term
}
tmpp = j.r;
while (!tmpp.empty()) {
    Term* x = tmpp.front();
    tmpp.pop();
    if (s.count(x->op) != 0) // 看看 j.r 中的 Term 是否在 j.l 中出现过
        return true;
}
return false;
```

拿出 `j.l` 中的所有 `Term`（此时应该全部为单个的命题变元），和 `j.r` 中的所有 `Term` 进行比较即可。如果存在相同，则直接返回 `true`；否则返回 `false`。

## 总结和心得

由于整体的代码超过了 $250$ 行，所以我经历了比较艰难的调试过程。在调试的过程中，我使用了一些辅助函数进行中间结果的输出，这样能方便地显示当前状态。

```cpp
void printTree(Term* u) { // 输出以 u 为根节点的表达式树
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

void print(std::queue<Term*> qTerm) { // 输出多棵表达式树
    std::queue<Term*> tmp = qTerm;
    while (!tmp.empty()) {
        Term* t = tmp.front();
        tmp.pop();
        printTree(t);
        puts("");
    }
}
```

这次研讨，不仅提升了我对 C++ 语言的熟练程度，还引导我对王浩算法以及命题逻辑的公理系统进行了更深一步的了解。感谢清华大学计算机系 [马昱春](https://www.cs.tsinghua.edu.cn/info/1131/5274.htm) 老师的指导，助教们在微信群里的答疑，以及 [Hao Wang (academic) - Wikipedia](https://en.wikipedia.org/wiki/Hao_Wang_(academic)) 和 [Towards Mechanical Mathematics](https://ieeexplore.ieee.org/document/5392526) 中的相关资料。
