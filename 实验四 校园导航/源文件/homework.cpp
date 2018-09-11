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

int getVex(char *name){	//ͨ�����ַ��ض������ 
	int i = 0;
	while(strcmp(map.vex[i].name,name) && i < map.vexnum)i++;
	if(i > map.vexnum - 1)return -1;	//ʧ�� 
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
	printf("���ڵ�һ������ͼ�����ͣ�0��ʾ����ͼ��1��ʾ����ͼ��\
			\n�ڶ����������붥����������\
			\n�������������붥������֣�û�����ֵ�����\"#\"��ÿ�����ֲ�����%d�ֽڣ��Կո����\
			\n����������һ����0��1��0��ʾ����������֮��Ķ���1��ʾ����������֮��Ķ�\
			\n�������ļ���ÿ����������ÿ���߹�����β��ͷ����Ȩ���ߵ����ƣ�û����������#��\n", MAXNAMELENGTH - 5);
	scanf("%d%d%d",&kind,&map.vexnum,&map.arcnum); 
	map.kind = kind ? DG : UDG;
	while(i < map.vexnum)scanf("%s",map.vex[i++].name);	//�������� 
	scanf("%d",&kind);	//��������ģʽ
	i = 0;
	while(i++ < map.arcnum){
		again:if(kind){	//���������� 
			scanf("%s",vname);
			u = getVex(vname);
			scanf("%s",vname);
			v = getVex(vname);
		} 
		else scanf("%d%d",&u,&v);
		if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
			printf("���������������������\n");
			fflush(stdin);
			goto again; 
		} 
		scanf("%d%s",&map.arcs[u][v].adj,map.arcs[u][v].name);
		if(map.kind == UDG)map.arcs[v][u] = map.arcs[u][v];	 	//�ṹ�帳ֵ 
	}
}

void printGraphArcs(){		//��ӡ�ڽӾ��� 
	printf("�ڽӾ���Ϊ\n");
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

void printWay(int u,int v){	//������·�� 
	int mid = vexOnWay[u][v];
	if(mid == -1){
		printf("��%d[%s] -> ��[%s](Ȩ��%d) -> ",u,map.vex[u].name,map.arcs[u][v].name,map.arcs[u][v].adj);
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
			memcpy(best,wv,MAXVEXNUM);	//����������õ�·�� 
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

void findTheWay(){ 		//�ҹ�ָ�����·�� 
	printf("���ڵ�һ������һ������0��1��0��ʾ����������֮��Ķ���1��ʾ����������֮��Ķ�\
			\n�ڶ��зֱ�������ĸ������� ���ĸ���ȥ ;����������\
			\n�����зֱ�����;���Ķ��㣬�뱣֤�������\n");
	int n,u,v,wv[MAXVEXNUM],best[MAXVEXNUM],i=0,m;
	char vname[MAXNAMELENGTH];
	scanf("%d",&n);
	again:if(n){	//���������� 
		scanf("%s",vname);
		u = getVex(vname);
		scanf("%s",vname);
		v = getVex(vname);
	} 
	else scanf("%d%d",&u,&v);
	if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
		printf("���������������������\n");
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
	if(min == INT_MAX)printf("������֮��û��·��\n");
	else{
		printWay(u,best[0]);
		for(i = 0;i < m - 1;i++)printWay(best[i],best[i+1]);
		printWay(best[m-1],v);
		printf("��%d[%s]\t��ȨֵΪ%d\n",v,map.vex[v].name,min);
	}
	system("pause");
}

void findWay(){
	system("cls");
	printf("���ڵ�һ������һ������0��1��2\
			\n0��ʾ����������֮��Ķ���1��ʾ����������֮��Ķ���2��ʾ������������·��\
			\n�ڶ���������ĸ������� ���ĸ���ȥ\n");
	int n,u,v;
	char vname[MAXNAMELENGTH];
	scanf("%d",&n);
	if(n == 2){
		findTheWay();
		return; 
	} 
	again:if(n){	//���������� 
		scanf("%s",vname);
		u = getVex(vname);
		scanf("%s",vname);
		v = getVex(vname);
	} 
	else scanf("%d%d",&u,&v);
	if(u < 0 || u > map.vexnum - 1 || v < 0 || v > map.vexnum - 1 || u == v){
		printf("���������������������\n");
		fflush(stdin);
		goto again; 
	} 
	if(way[u][v] < INT_MAX){
		printWay(u,v);
		printf("��%d[%s]\t��ȨֵΪ%d\n",v,map.vex[v].name,way[u][v]);
	}
	else printf("������֮��û��·��\n");
	system("pause");
}
/*�������� ��������� 
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
/*����������� 
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
	//printGraphArcs();			//��ӡ�ڽӾ��� ������ 
}
