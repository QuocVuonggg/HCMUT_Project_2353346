#include "tsm.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

string Traveling(int graph[60][60], int n, char start){
    const int INF = 1000000000;
    const long long BIG = 1000000000LL;

    // char -> index
    int s;
    if (start >= 'A' && start <= 'Z') s = start - 'A';
    else s = start - 'a' + 26;
    if (s < 0 || s >= n) return "";

    // chuẩn hoá: 0/i==j -> INF
    static int w[60][60];
    for (int i=0;i<n;++i)
        for (int j=0;j<n;++j)
            w[i][j] = (i==j) ? INF : ((graph[i][j]==0)?INF:graph[i][j]);

    struct Util{
        static long long cost(const vector<int>& t, int n, int ww[60][60]){
            if ((int)t.size()!=n) return (long long)1e15;
            long long c=0;
            for (int i=1;i<n;++i){ int a=t[i-1], b=t[i]; if (ww[a][b]>=1000000000) return (long long)1e15; c+=ww[a][b]; }
            if (ww[t[n-1]][t[0]]>=1000000000) return (long long)1e15;
            c+=ww[t[n-1]][t[0]]; return c;
        }
        static char idx2char(int x){ return (x<26)?char('A'+x):char('a'+(x-26)); }
    };

    vector<vector<int> > cand;

    // 1) Hungarian -> 1-factor (permutation), rồi nối subtour
    {
        static long long a[61][61], u[61], v[61], minv[61];
        static int p[61], way[61];
        for (int i=1;i<=n;++i) for (int j=1;j<=n;++j) a[i][j]=(w[i-1][j-1]>=INF)?BIG:(long long)w[i-1][j-1];
        for (int i=0;i<=n;++i){u[i]=0;v[i]=0;p[i]=0;way[i]=0;}
        for (int i=1;i<=n;++i){
            p[0]=i; int j0=0; static char used[61];
            for (int j=0;j<=n;++j){minv[j]=BIG; way[j]=0; used[j]=0;}
            do{
                used[j0]=1; int i0=p[j0], j1=0; long long delta=BIG;
                for (int j=1;j<=n;++j) if(!used[j]){
                    long long cur=a[i0][j]-u[i0]-v[j];
                    if (cur<minv[j]){minv[j]=cur; way[j]=j0;}
                    if (minv[j]<delta){delta=minv[j]; j1=j;}
                }
                for (int j=0;j<=n;++j){ if (used[j]){u[p[j]]+=delta; v[j]-=delta;} else minv[j]-=delta; }
                j0=j1;
            }while(p[j0]!=0);
            do{ int j1=way[j0]; p[j0]=p[j1]; j0=j1; }while(j0);
        }
        vector<int> to(n,-1); bool feas=true;
        for (int j=1;j<=n;++j){ int i=p[j]; if (a[i][j]>=BIG){feas=false;break;} to[i-1]=j-1; }
        if (feas){
            // nối subtour bằng patching (swap 2 cung giữa 2 vòng)
            while (true){
                vector<int> cid(n,-1); vector< vector<int> > cyc;
                for (int i=0;i<n;++i){
                    if (cid[i]!=-1) continue;
                    int cur=i; vector<int> c;
                    while (cid[cur]==-1){ cid[cur]=(int)cyc.size(); c.push_back(cur); cur=to[cur]; }
                    cyc.push_back(c);
                }
                if ((int)cyc.size()==1) break;
                bool merged=false;
                for (int c1=0;c1<(int)cyc.size() && !merged;++c1)
                    for (int c2=c1+1;c2<(int)cyc.size() && !merged;++c2){
                        long long bestD= (long long)4e18; int bi=-1,bj=-1;
                        for (int ii=0; ii<(int)cyc[c1].size(); ++ii){
                            int i=cyc[c1][ii], ai=to[i];
                            for (int jj=0; jj<(int)cyc[c2].size(); ++jj){
                                int j=cyc[c2][jj], aj=to[j];
                                if (w[i][aj]>=INF || w[j][ai]>=INF) continue;
                                long long d=(long long)w[i][aj]+w[j][ai] - (long long)w[i][ai]-w[j][aj];
                                if (d<bestD){bestD=d; bi=i; bj=j;}
                            }
                        }
                        if (bi!=-1){ int ai=to[bi], aj=to[bj]; to[bi]=aj; to[bj]=ai; merged=true; }
                    }
                if (!merged) break;
            }
            vector<int> tour(n,-1); int cur=s;
            for (int k=0;k<n;++k){ tour[k]=cur; cur=to[cur]; if (cur==-1) break; }
            bool ok=true; for(int i=0;i<n;++i) if (tour[i]==-1) ok=false;
            if (ok && Util::cost(tour,n,w)<(long long)1e15) cand.push_back(tour);
        }
    }

    // 2) Greedy có hướng (chọn cạnh nhẹ, khống chế in/out, tránh chu trình sớm)
    {
        struct Edge{int u,v,w;};
        vector<Edge> es; es.reserve(n*(n-1));
        for (int i=0;i<n;++i) for (int j=0;j<n;++j) if (i!=j && w[i][j]<INF){ Edge e; e.u=i; e.v=j; e.w=w[i][j]; es.push_back(e); }
        struct Cmp{ static bool cmp(const Edge& a, const Edge& b){ return a.w<b.w; } };
        sort(es.begin(), es.end(), Cmp::cmp);
        vector<int> out(n,-1), in(n,-1); int added=0;
        for (int idx=0; idx<(int)es.size() && added<n; ++idx){
            int u=es[idx].u, v=es[idx].v;
            if (out[u]!=-1 || in[v]!=-1) continue;
            // tránh tạo chu trình sớm
            int x=v; bool cycle=false; for (int step=0; step<n; ++step){ if (x==-1) break; if (x==u){cycle=true;break;} x=out[x]; }
            if (cycle && added<n-1) continue;
            out[u]=v; in[v]=u; ++added;
        }
        bool ok=true; for (int i=0;i<n;++i) if (out[i]==-1){ ok=false; break; }
        if (ok){
            vector<int> tour(n,-1); int cur=s;
            for (int k=0;k<n;++k){ tour[k]=cur; cur=out[cur]; }
            if (Util::cost(tour,n,w)<(long long)1e15) cand.push_back(tour);
        }
    }

    if (cand.empty()) return "";

    // lấy tour tốt nhất
    int bestId=0; long long bestC=Util::cost(cand[0],n,w);
    for (int i=1;i<(int)cand.size();++i){ long long c=Util::cost(cand[i],n,w); if (c<bestC){bestC=c; bestId=i;} }
    vector<int> tour=cand[bestId];

    // --- Local search an toàn cho ATSP: Or-opt (k=1,2,3) + Relocate(1)
    int cap = n*n*10;
    bool improved = true;
    while (improved && cap>0){
        improved=false;
        for (int klen=1; klen<=3 && cap>0; ++klen){
            for (int i=1; i+klen<=n && cap>0; ++i){
                int pre = tour[i-1];
                int blkL = tour[i];
                int blkR = tour[i+klen-1];
                int suf = (i+klen<n)? tour[i+klen] : tour[0];
                if (w[pre][suf] >= INF) { --cap; continue; }
                long long rem = (long long)w[pre][blkL] + w[blkR][suf] - w[pre][suf];
                for (int j=1; j<n && cap>0; ++j){
                    if (j>=i && j<=i+klen) { --cap; continue; }
                    int a = tour[j-1];
                    int b = (j<n)? tour[j] : tour[0];
                    if (w[a][blkL] >= INF || w[blkR][b] >= INF){ --cap; continue; }
                    long long add = (long long)w[a][blkL] + w[blkR][b] - w[a][b];
                    if (add < rem){
                        vector<int> nt = tour;
                        nt.erase(nt.begin()+i, nt.begin()+i+klen);
                        if (j > i) j -= klen;
                        nt.insert(nt.begin()+j, tour.begin()+i, tour.begin()+i+klen);
                        if (Util::cost(nt,n,w) < Util::cost(tour,n,w)){
                            tour.swap(nt); improved=true; break;
                        }
                    }
                    --cap;
                }
                if (improved) break;
            }
            if (improved) break;
        }
    }

    // đảm bảo bắt đầu từ start
    if (tour[0]!=s){
        int pos=0; for (int i=0;i<n;++i) if (tour[i]==s){pos=i;break;}
        rotate(tour.begin(), tour.begin()+pos, tour.end());
    }

    // build string "S ... S"
    string ans;
    ans.push_back(Util::idx2char(tour[0]));
    for (int i=1;i<n;++i){ ans.push_back(' '); ans.push_back(Util::idx2char(tour[i])); }
    ans.push_back(' '); ans.push_back(Util::idx2char(tour[0]));
    return ans;
}
