#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 一张牌：花色 + 牌面
struct Card {
  string suit;
  string rank;
};

// 牌面大小：A < 2 < 3 < ... < 10 < J < Q < K
int rankVal(const string &r) {
  if (r == "A")
    return 1;
  if (r == "J")
    return 11;
  if (r == "Q")
    return 12;
  if (r == "K")
    return 13;
  return stoi(r); // "2" ~ "10"
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Card> deck; // deck[0] 为牌堆顶

  for (int i = 0; i < n; ++i) {
    string op;
    cin >> op;
    if (op == "Append") {
      Card c;
      cin >> c.suit >> c.rank;
      deck.push_back(c); // 添加到牌堆底部
    } else if (op == "Pop") {
      if (deck.empty()) {
        cout << "NULL\n";
      } else {
        cout << deck.front().suit << ' ' << deck.front().rank << '\n';
        deck.erase(deck.begin()); // 弹出牌堆顶
      }
    } else if (op == "Revert") {
      reverse(deck.begin(), deck.end()); // 整个牌堆逆序
    } else if (op == "Extract") {
      string suit;
      cin >> suit;
      vector<Card> got, rest;
      for (size_t k = 0; k < deck.size(); ++k) {
        if (deck[k].suit == suit)
          got.push_back(deck[k]); // 抽走该花色
        else
          rest.push_back(deck[k]); // 其余保留原序
      }
      // 抽出的牌按牌面从小到大排序（同牌面保持原有相对顺序）
      stable_sort(got.begin(), got.end(), [](const Card &x, const Card &y) {
        return rankVal(x.rank) < rankVal(y.rank);
      });
      deck.clear();
      for (size_t k = 0; k < got.size(); ++k)
        deck.push_back(got[k]); // 放到顶部
      for (size_t k = 0; k < rest.size(); ++k)
        deck.push_back(rest[k]);
    }
  }

  // 全部指令执行完毕后输出牌堆（顶 -> 底）
  if (deck.empty()) {
    cout << "NULL\n";
  } else {
    for (size_t k = 0; k < deck.size(); ++k) {
      cout << deck[k].suit << ' ' << deck[k].rank << '\n';
    }
  }
  return 0;
}
