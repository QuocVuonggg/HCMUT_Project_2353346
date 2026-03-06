#include "bellman.h"
#include <string>
using namespace std;

void BF(int graph[][60],int size,char start_vertices,int BFValue[],int BFPrev[]) {
    int start=(start_vertices>='A'&&start_vertices<='Z')?(start_vertices-'A'):(start_vertices-'a'+26);
    if (BFValue[start]==-1) BFValue[start]=0;
    int prevValue[60];
    for (int i = 0; i < size; ++i) prevValue[i]=BFValue[i];
    for(int u = 0; u < size; ++u) {
        if(prevValue[u]==-1) continue;
        for(int v=0;v<size;++v){
            if(graph[u][v]>0){
                int tmp=prevValue[u]+graph[u][v];
                if(BFValue[v]==-1||tmp<BFValue[v]){
                    BFValue[v]=tmp;
                    BFPrev[v]=u;
                }
            }
        }
    }
}

string BF_Path(int graph[][60], int size, char start_vertices, char end_vertices) {
    int start,end;
    if(start_vertices>='A'&&start_vertices<='Z') start=start_vertices-'A';
    else start=start_vertices-'a'+26;
    if(end_vertices>='A'&&end_vertices<='Z')end=end_vertices-'A';
    else end=end_vertices-'a'+26;
    int dist[60],prev[60],hops[60];
    for (int i = 0; i < size; ++i) {
        dist[i] = -1;
        hops[i] = -1;
        prev[i] = -1;
    }
    dist[start] = 0;
    hops[start] = 0;
    for(int it=0;it<size-1;++it) {
        bool changed=false;
        for(int u=0;u<size;++u){
            if(dist[u]==-1) continue;
            for(int v=0;v<size;++v){
                if(graph[u][v]>0) {
                    int newDist=dist[u]+graph[u][v];
                    int newHops=hops[u]+1;
                    if(dist[v]==-1||newDist<dist[v]||
                        (newDist==dist[v]&&newHops<hops[v])||
                        (newDist==dist[v]&&newHops==hops[v]&&(prev[v]==-1||u<prev[v]))){
                        dist[v]=newDist;
                        hops[v]=newHops;
                        prev[v]=u;
                        changed=true;
                    }
                }
            }
        }
        if(!changed) break;
    }
    if(dist[end]==-1) return "";
    char st[60];
    int top=0;
    int cur=end;
    while(cur!=-1){
        if(cur < 26) st[top++]='A'+cur;
        else st[top++]='a'+cur-26;
        cur=prev[cur];
    }
    string res;
    for(int i=top-1;i>=0;--i){
        res+=st[i];
        if(i) res+=" ";
    }
    return res;
}
