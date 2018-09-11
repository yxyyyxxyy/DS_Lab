#include<stdio.h> 
#include<stdlib.h>
#include<string.h>

#define MAXVEXNUM 20
#define MAXNAMELENGTH 50 

typedef enum {DG,UDG} GraphKind;

typedef struct{
	GraphKind kind;
	int vexnum;
	int arcnum;
	struct{
		int adj;
		char name[MAXNAMELENGTH];
	}arcs[MAXVEXNUM][MAXVEXNUM];
	struct{
		char name[MAXNAMELENGTH];
	}vex[MAXVEXNUM];
}Graph; 

Graph map;
int way[MAXVEXNUM][MAXVEXNUM],vexOnWay[MAXVEXNUM][MAXVEXNUM];
int min;

int getVex(char *name){	//通过名字返回顶点序号 
	int i = 0;
	while(strcmp(map.vex[i].name,name) && i < map.vexnum)i++;
	if(i > map.vexnum - 1)return -1;	//失败 
	return i;
}

void createGraph(){
	int kind,i,j,u,v;
	char vname[MAXNAMELENGTH];
	for(i = 0; i < MAXVEXNUM; i++){
		for(j = 0; j < MAXVEXNUM; j++){
			if(i != j)map.arcs[i][j].adj = INT_MAX;
			else map.arcs[i][j].adj = 0;
		}
	}
	i = 0;
	printf("请在第一行输入图的类型（0表示无向图，1表示有向图）\
			\n第二行依次输入顶点数、边数\
			\n第三行依次输入顶点的名字，没有名字的输入\"#\"，每个名字不超过%d字节，以空格隔开\
			\n第四行输入一个数0或1，0表示以数字输入之后的顶，1表示用名字输入之后的顶\
			\n接下来的几行每行依次输入每条边关联的尾、头、边权、边的名称（没有名称输入#）\n", MAXNAMELENGTH - 5);
	scanf("%d%d%d",&kind,&map.vexnum,&map.arcnum); 
	map.kind = kind ? DG : UDG;
	while(i < map.vexnum)scanf("%s",map.vex[i++].name);	//顶点名字 
	scanf("%d",&kind);	//顶的输入模式
	i = 0;
	while(i++ < map.arcnum){
		again:if(kind){	//以名字输入 
			scanf("%s",vname);
			u = getVex(vname);
			scanf("%s",vname);
			v = getVex(vname);
		} 
		else scanf("%d%d",&u,&v);
		if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
			printf("输入有误，请重新输入该行\n");
			fflush(stdin);
			goto again; 
		} 
		scanf("%d%s",&map.arcs[u][v].adj,map.arcs[u][v].name);
		if(map.kind == UDG)map.arcs[v][u] = map.arcs[u][v];	 	//结构体赋值 
	}
}

void printGraphArcs(){		//打印邻接矩阵 
	printf("邻接矩阵为\n");
	int i,j;
	for(i = 0; i < map.vexnum; i++){
		for(j = 0; j < map.vexnum; j++){
			printf("%5d",map.arcs[i][j].adj);
		}
		printf("\n");
	}
} 

void floydWay(){
	int i,j,k;
	memset(vexOnWay,-1,MAXVEXNUM * MAXVEXNUM);
	for(i = 0; i < MAXVEXNUM; i++)
		for(j = 0; j < MAXVEXNUM; j++)
			way[i][j] = map.arcs[i][j].adj;
	for(i = 0; i < MAXVEXNUM; i++){
		for(j = 0; j < MAXVEXNUM; j++){
			for(k = 0; k < MAXVEXNUM; k++){
				if(way[j][k] > way[j][i] + way[i][k]){
					if(way[j][i] == INT_MAX || way[i][k] == INT_MAX)continue;
					way[j][k] = way[j][i] + way[i][k];
					vexOnWay[j][k] = i;
				}
			}
		}
	}

}

void printWay(int u,int v){	//输出最短路径 
	int mid = vexOnWay[u][v];
	if(mid == -1){
		printf("顶%d[%s] -> 边[%s](权：%d) -> ",u,map.vex[u].name,map.arcs[u][v].name,map.arcs[u][v].adj);
		return;
	}
	printWay(u,mid);
	printWay(mid,v);
}

void allOrder(int *wv,int u,int v,int m,int n,int *best){
	if(n == m){
		if(way[ u ][ wv[0] ] == INT_MAX || way[ wv[m-1] ][ v ] == INT_MAX)return; 
		int thisway = 0;
		for(int i = 0; i < m-1; i++){
			if(way[ wv[i] ][ wv[i+1] ] == INT_MAX)return;
			else thisway += way[ wv[i] ][ wv[i+1] ];
		}
		thisway += way[ u ][ wv[0] ] + way[ wv[m-1] ][ v ];
		if(min > thisway) {
			min = thisway;
			memcpy(best,wv,MAXVEXNUM);	//保存下来最好的路径 
		}
	}
	else{
		int temp;
		for(int i = n; i < m; i++){
			temp = wv[n];
			wv[n] = wv[i];
			wv[i] = temp;
			allOrder(wv,u,v,m,n+1,best);
			temp = wv[n];
			wv[n] = wv[i];
			wv[i] = temp;
		}
	}
}

void findTheWay(){ 		//找过指定点的路径 
	printf("请在第一行输入一个数字0或1，0表示以数字输入之后的顶，1表示用名字输入之后的顶\
			\n第二行分别输入从哪个顶出发 到哪个顶去 途经几个顶点\
			\n第三行分别输入途经的顶点，请保证输入合理\n");
	int n,u,v,wv[MAXVEXNUM],best[MAXVEXNUM],i=0,m;
	char vname[MAXNAMELENGTH];
	scanf("%d",&n);
	again:if(n){	//以名字输入 
		scanf("%s",vname);
		u = getVex(vname);
		scanf("%s",vname);
		v = getVex(vname);
	} 
	else scanf("%d%d",&u,&v);
	if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
		printf("输入有误，请重新输入该行\n");
		fflush(stdin);
		goto again; 
	} 
	scanf("%d",&m);
	while(i < m){
		if(n){
			scanf("%s",vname);
			wv[i++] = getVex(vname);
		}
		else scanf("%d",wv + i++);
	}
	min = INT_MAX;
	allOrder(wv,u,v,m,0,best);
	if(min == INT_MAX)printf("该两顶之间没有路。\n");
	else{
		printWay(u,best[0]);
		for(i = 0;i < m - 1;i++)printWay(best[i],best[i+1]);
		printWay(best[m-1],v);
		printf("顶%d[%s]\t总权值为%d\n",v,map.vex[v].name,min);
	}
	system("pause");
}

void findWay(){
	system("cls");
	printf("请在第一行输入一个数字0或1或2\
			\n0表示以数字输入之后的顶，1表示用名字输入之后的顶，2表示求过多个点的最短路径\
			\n第二行输入从哪个顶出发 到哪个顶去\n");
	int n,u,v;
	char vname[MAXNAMELENGTH];
	scanf("%d",&n);
	if(n == 2){
		findTheWay();
		return; 
	} 
	again:if(n){	//以名字输入 
		scanf("%s",vname);
		u = getVex(vname);
		scanf("%s",vname);
		v = getVex(vname);
	} 
	else scanf("%d%d",&u,&v);
	if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
		printf("输入有误，请重新输入该行\n");
		fflush(stdin);
		goto again; 
	} 
	if(way[u][v] < INT_MAX){
		printWay(u,v);
		printf("顶%d[%s]\t总权值为%d\n",v,map.vex[v].name,way[u][v]);
	}
	else printf("该两顶之间没有路。\n");
	system("pause");
}
/*测试数据 过多个顶点 
0
5 4
# # # # # 
0
0 1 12 a
1 2 3 b
2 3 4 c
3 4 5 d
2
0
0 4 3
1 2 3
*/
/*输入错误的情况 
0
3 3
a b c
0
1 4 3 ab
*/
main(){
	createGraph();
	floydWay();
	while(1)findWay();
	//printGraphArcs();			//打印邻接矩阵 测试用 
}
