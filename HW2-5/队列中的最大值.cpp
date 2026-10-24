// =============================================================
//  p101  队列中的最大值
//
//  操作:  enqueue v   入队（队满输出 "Queue is Full"）
//         dequeue     出队并输出队首（队空输出 "Queue is Empty"）
//         max         输出当前队列最大值（队空输出 "Queue is Empty"）
//         quit        结束，输出队列中剩余全部元素
//
//  数据结构:  普通队列(循环数组)  +  单调不增双端队列 dq
//
//  dq 的含义:  dq 从队首到队尾单调不增，dq.front() 即当前队列最大值。
//    enqueue v : 把 dq 尾部所有 < v 的元素弹掉，再压入 v。
//                这些元素比 v 小、又比 v 先入队，只要它们还在队列里
//                v 就必然也在（v 后出队），所以它们永远当不了最大值，
//                可以直接丢弃 —— 这是 O(1) 均摊的关键。
//    dequeue   : 若弹出的队首元素恰好等于 dq.front()，dq 也弹掉队首。
//    max       : 直接返回 dq.front()。
//
//  注意: 弹尾条件必须是 "< v" 而不是 "<= v"。
//        若写成 <=，重复最大值(如 5 5)时，第一个 5 出队会把 dq 队首
//        弹掉，第二个 5 仍在队列里但 dq 中已无它，max 就错了。
//
//  复杂度: 每个元素最多进出 dq 各一次 -> k 次操作总计 O(k)，
//          单次均摊 O(1)，空间 O(n)。与值域无关，无需打表。
//
//  输出:  全部使用标准库 stdio（printf / puts / putchar），
//          由 C 库自身的缓冲区负责攒批写出，不手工管理缓冲区。
//
//  编译:  g++ -O2 -std=c++11 -o p101 p101.cpp
//  运行:  ./p101 < input.txt > output.txt
// =============================================================

#include <cstdio>
#include <deque>
#include <vector>

int main() {
  int n;
  if (scanf("%d", &n) != 1)
    return 0; // n = 队列容量，可能为 0

  // 循环数组实现主队列（避免 vector::erase(begin()) 带来的 O(n) 搬移）
  const int cap = (n > 0) ? n : 1; // n == 0 时也要避免除零
  std::vector<long long> que(cap);
  int head = 0; // 队首下标
  int size = 0; // 当前元素个数

  std::deque<long long> dq; // 单调不增的候选最大值

  char op[16];
  while (scanf("%15s", op) == 1) {

    if (op[0] == 'q') { // quit
      break;
    }

    else if (op[0] == 'e' && op[2] == 'q') { // enqueue
      long long v;
      if (scanf("%lld", &v) != 1)
        break;
      if (size == n) { // 队满（n==0 时恒满）
        puts("Queue is Full");
      } else {
        que[(head + size) % cap] = v;
        ++size;
        while (!dq.empty() && dq.back() < v)
          dq.pop_back(); // 严格小于
        dq.push_back(v);
      }
    }

    else if (op[0] == 'd') { // dequeue
      if (size == 0) {
        puts("Queue is Empty");
      } else {
        long long v = que[head];
        head = (head + 1) % cap;
        --size;
        if (!dq.empty() && dq.front() == v)
          dq.pop_front();
        printf("%lld\n", v);
      }
    }

    else { // max
      if (size == 0) {
        puts("Queue is Empty");
      } else {
        printf("%lld\n", dq.front());
      }
    }
  }

  // quit: 按队首到队尾的顺序输出剩余元素，空格分隔
  for (int i = 0; i < size; ++i) {
    if (i > 0)
      putchar(' ');
    printf("%lld", que[(head + i) % cap]);
  }
  putchar('\n');
  return 0;
}