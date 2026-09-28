#include <iostream>
#include <vector>
using namespace std;

// 最大公约数（欧几里得算法，兼容任何 C++ 标准，无需 C++17 的 std::gcd）
int myGcd(int a, int b) {
  while (b != 0) {
    int t = a % b;
    a = b;
    b = t;
  }
  return a;
}

// ========== 方法一：暴力法（每次右移一位，重复 k 次）==========
// 时间复杂度：O(n*k)，空间复杂度：O(1)
vector<int> rotateBruteForce(vector<int> nums, int k) {
  int n = (int)nums.size();
  k %= n;
  for (int step = 0; step < k; ++step) {
    int last = nums[n - 1];
    for (int i = n - 1; i >= 1; --i) {
      nums[i] = nums[i - 1];
    }
    nums[0] = last;
  }
  return nums;
}

// ========== 方法二：借助额外数组 ==========
// 时间复杂度：O(n)，空间复杂度：O(n)
vector<int> rotateExtraArray(vector<int> nums, int k) {
  int n = (int)nums.size();
  k %= n;
  vector<int> ans(n);
  for (int i = 0; i < n; ++i) {
    ans[(i + k) % n] = nums[i]; // 下标 i 的元素最终落到 (i+k)%n
  }
  return ans;
}

// ========== 方法三：三次翻转（原地算法，O(1) 额外空间）==========
// 时间复杂度：O(n)，空间复杂度：O(1)
void reverseRange(vector<int> &nums, int l, int r) {
  while (l < r) {
    swap(nums[l], nums[r]);
    ++l;
    --r;
  }
}

void rotateByReverse(vector<int> &nums, int k) {
  int n = (int)nums.size();
  k %= n;
  reverseRange(nums, 0, n - 1); // 1) 整体翻转
  reverseRange(nums, 0, k - 1); // 2) 翻转前 k 个
  reverseRange(nums, k, n - 1); // 3) 翻转后 n-k 个
}

// ========== 方法四：环状替换（juggling / GCD 法，原地算法）==========
// 时间复杂度：O(n)，空间复杂度：O(1)
void rotateByCyclic(vector<int> &nums, int k) {
  int n = (int)nums.size();
  k %= n;
  if (k == 0)
    return;
  int cycles = myGcd(n, k); // 环的个数 = gcd(n, k)
  for (int start = 0; start < cycles; ++start) {
    int cur = start;
    int prev = nums[start];
    do {
      int nxt = (cur + k) % n; // 当前位置元素的目标位置
      swap(prev, nums[nxt]);
      cur = nxt;
    } while (cur != start);
  }
}

// ========== 主程序：读入 -> 轮转（方法三）-> 输出 ==========
int main() {
  int n, k;
  cin >> n >> k;
  vector<int> nums(n);
  for (int i = 0; i < n; ++i)
    cin >> nums[i];

  rotateByReverse(nums, k); // 推荐：O(n) 时间、O(1) 空间的原地算法

  for (int i = 0; i < n; ++i) {
    if (i)
      cout << ' ';
    cout << nums[i];
  }
  cout << '\n';
  return 0;
}
