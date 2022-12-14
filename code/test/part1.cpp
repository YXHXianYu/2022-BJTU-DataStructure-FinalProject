#include<bits/stdc++.h>
using namespace std;
const int N = 200000;
struct edge {
 double d;
 int u, v;
}e[N];
int n;
pair<int, int> point[N];
int cnt= 0;
int fa[N];

int find(int x) {
 if(x == fa[x]) return x;
 return fa[x] = find(fa[x]);
}
double ans = 0;
int nx, ny;
void kruscal() {
 sort(e + 1, e + 1 + cnt, [&](edge x, edge y)->bool {
  return x.d < y.d;
 });
 
 int sum = 0;
 for(int i = 1; i <= n+ 1 ; ++i) fa[i] = i;
 for(int i = 1; i <= cnt; ++i) {
  int u = e[i].u, v= e[i].v;
  int fu = find(u), fv = find(v);
  if(fu == fv) continue;
  sum++;
  fa[fu] = fv;
  ans += e[i].d;
  if(sum == n) break;
 }
}
int main() {
 cin>>n;
 for(int i = 1; i <= n; ++i) {
  cin>>point[i].first;
  cin>>point[i].second;
 }
 cnt = 0;
 for(int i = 1; i <= n; ++i) {
  for(int j = i + 1; j <= n; ++j) {
   e[++cnt].d = (double)sqrt((point[i].first - point[j].first)*(point[i].first - point[j].first) + (point[i].second - point[j].second)*(point[i].second - point[j].second));
   e[cnt].u = i;
   e[cnt].v = j;
  }
 }
 for(int i = 1; i <= n; ++i) {
  e[++cnt].d = point[i].second;
  e[cnt].u = i;
  e[cnt].v = n + 1;
 }
 kruscal();
 cout<<ans<<endl;
 for(int i = 1; i <= cnt; ++i) {
  if(e[i].v == n + 1) {
   int u = e[i].u;
   e[i].d = min(point[u].first, point[u].second);
  }
 }
 ans = 0;
 kruscal();
 cout<<ans<<endl;
 cin>>nx>>ny;
 int now = 0;
 for(int i = 1; i <= cnt; ++i) {
  if((e[i].u == nx && e[i].v == ny) || (e[i].v == nx && e[i].u == ny)) {
   ans = e[i].d;
   e[i].d = 0;
   break;
  }
 }
 e[now].d = 0;
 kruscal();
 cout<<ans<<endl;
 return 0;
}