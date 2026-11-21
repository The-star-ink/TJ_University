// 感染二叉树需要的总时间（LeetCode 2385 的编号版）
//
// 关键观察：感染沿「边」扩散，每分钟走一步，所以答案就是从 start 出发
// 到所有结点的最短路的最大值——也就是以 start 为根的树的高度。
// 二叉树里父亲到孩子不是双向的，但感染可以往父亲走，于是把树看作
// 无向图（每个结点连向左右孩子和父亲），再从 start 做一次 BFS，
// 最后一个被访问到的层的层号就是答案。
//
// 时间 O(n)，空间 O(n)。n <= 1e5，用队列迭代，不会爆栈。
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, start;
  if (!(cin >> n >> start))
    return 0;

  // 邻接表：无向边
  vector<vector<int>> adj(n);
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    if (a != -1) {
      adj[i].push_back(a);
      adj[a].push_back(i);
    }
    if (b != -1) {
      adj[i].push_back(b);
      adj[b].push_back(i);
    }
  }

  vector<int> dist(n, -1);
  queue<int> q;
  dist[start] = 0;
  q.push(start);

  int ans = 0;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : adj[u]) {
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        if (dist[v] > ans)
          ans = dist[v];
        q.push(v);
      }
    }
  }

  cout << ans << '\n';
  return 0;
}
