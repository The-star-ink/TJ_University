// 队列的应用：0/1 矩阵区域计数（按 p44.cpp 参考程序的判定规则）
//
// 参考程序的判定规则（Solution::dfs 中 bo |= dir == 4）：
//   一个格子的四个方向都在矩阵内（即不在第 0 行/最后一行，也不在第 0
//   列/最后一列），
//   称它“严格在内部”。一个四邻域连通块只要含有至少一个这样的格子，就计为一个区域；
//   若整个连通块全部落在最外圈边框上，则不计。
//
// 注意：这与“连通块只要触到边缘就不算”不同。后者会把从边框一直延伸到内部的
// 大连通块也排除掉，与参考程序的结果不一致。
#include <cstdio>
#include <queue>
#include <vector>
using namespace std;

// 读一个整数：跳过所有非数字字符（空格、回车、BOM 等），到 EOF 返回 false
static bool nextInt(int &out) {
  int c = getchar();
  while (c != EOF && (c < '0' || c > '9') && c != '-')
    c = getchar();
  if (c == EOF)
    return false;
  int sign = 1;
  if (c == '-') {
    sign = -1;
    c = getchar();
  }
  int v = 0;
  while (c >= '0' && c <= '9') {
    v = v * 10 + (c - '0');
    c = getchar();
  }
  out = v * sign;
  return true;
}

int main() {
  int n = 0, m = 0;

  if (!nextInt(n) || !nextInt(m)) { // 没有任何输入
    printf("0\n");
    return 0;
  }
  if (n <= 0 || m <= 0) { // 空矩阵
    printf("0\n");
    return 0;
  }
  if ((long long)n * m > 10000000LL) { // 防御异常数据
    printf("0\n");
    return 0;
  }

  const int N = n, M = m;
  const int total = N * M;

  // 逐字符只挑 '0' 和 '1'：兼容 "0 0 0 1 1" 和 "00011" 两种写法、CRLF、空行
  vector<unsigned char> a(total, 0);
  int filled = 0, ch;
  while (filled < total && (ch = getchar()) != EOF) {
    if (ch == '0' || ch == '1')
      a[filled++] = (unsigned char)(ch - '0');
  }

  vector<unsigned char> vis(total, 0);
  queue<int> q; // 队列：BFS 的标准容器
  const int dr[4] = {-1, 1, 0, 0};
  const int dc[4] = {0, 0, -1, 1};
  int regions = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int s = i * M + j;
      if (!a[s] || vis[s])
        continue;

      bool hasInner = false; // 该连通块是否含有“严格在内部”的格子
      vis[s] = 1;
      q.push(s);

      while (!q.empty()) {
        int cur = q.front();
        q.pop();
        int r = cur / M, c = cur % M;

        // 四邻都在界内 <=> 不在最外圈，与参考程序的 dir == 4 等价
        if (r > 0 && r < N - 1 && c > 0 && c < M - 1)
          hasInner = true;

        for (int k = 0; k < 4; k++) {
          int nr = r + dr[k], nc = c + dc[k];
          if (nr < 0 || nr >= N || nc < 0 || nc >= M)
            continue;
          int nxt = nr * M + nc;
          if (a[nxt] && !vis[nxt]) {
            vis[nxt] = 1;
            q.push(nxt);
          }
        }
      }
      if (hasInner)
        regions++; // 含内部格子的连通块才计为区域
    }
  }

  printf("%d\n", regions);
  return 0;
}