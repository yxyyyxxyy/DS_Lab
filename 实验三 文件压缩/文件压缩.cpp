#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAXFILENAMELENGTH 1000  //文件名最大长度 
#define CLEARSCREEN system("cls"); 	//清空屏幕  
#define PAUSE system("pause");exit(0);		//暂停并退出 

typedef char *String;
typedef struct{
	unsigned char ch;		//存放的8bit数 
	unsigned long weight,parent,lchild,rchild;
}HTNode;
typedef struct{
	HTNode data[512];	//0不用 
	int root;	//根 
}HTree;
HTree T;
bool oneChar=false,emptyFile=false;		//文件只有一种字符，huffman树无法生成 
static String thr[256];	//每个字符的huffman编码 
char fileName[MAXFILENAMELENGTH];	//文件名 
unsigned long charNum[256];	//每个字符出现的次数 

FILE* openFile(String fileName,String mood="rb+"){		//打开名字为fileName的二进制文件 返回文件指针 
	FILE *fp; 
	if((fp=fopen(fileName,mood))==NULL){
		printf("打开文件失败！\n");
		PAUSE
	}
	return fp;
}

void getCharNum(FILE *&fp){		//统计文件中每个字符出现的次数 
	memset(charNum,0,sizeof(charNum));	//清空 
	fseek(fp,0L,SEEK_SET);	//指针移到文件开头
	fseek(fp,0L,SEEK_CUR);	//切换读写指针 
	unsigned char ch;	//ch 0-255 
	while(1){ 
		fread(&ch,1,1,fp);	//读一字节内容
		if(feof(fp))break;	//文件结束 
		charNum[ch]++;		//计数加1 
	}
} 

int getSmallestWeight(int &a,int &b){		//获得有两个最小权重(>0)的字符在tree中的位置 返回0表示已经结束并给root赋值 否则返回结束位置 
	int i;
	a=b=0;
	for(i=1;i<512&&T.data[i].weight;i++)	if(T.data[i].parent==0){	a=i;	break;}		//找到1个a
	for(i++;i<512&&T.data[i].weight;i++)	if(T.data[i].parent==0){	b=i;	break;}		//继续找到1个b 
	if(b==0){	//第一次true时找到a（root）但是没有b 
		T.root=a;
		return 0; 	//只能找到1个（根） 
	}
	for(i++;i<512&&T.data[i].weight;i++){
		if(T.data[i].parent==0&&T.data[i].weight<T.data[a].weight)a=i;		//parent!=0
		else if(T.data[i].parent==0&&T.data[i].weight<T.data[b].weight)b=i;
	}	//获取weight最小的两个i 
	return i; 
}

void createHTree(){		//根据现有的charNum次数生成HuffmanTree 0次不写 
	memset(T.data,0,sizeof(T.data));	//清空
	int j=1,a,b,pos,nnum=0; 
	for(int i=0;i<=255;i++){
		if(charNum[i]==0)continue;	//此字符不出现	
		nnum++;
		T.data[j].rchild=T.data[j].lchild=T.data[j].parent=0;
		T.data[j].weight=charNum[i];
		T.data[j++].ch=i;		//j+1，下一次输入在下一个位置 
	} 
	if(nnum==0)	{	emptyFile=true;	return;}		//空文件 
	else if(nnum==1){	oneChar=true;	return;}		//只有一个字符 
	while(pos=getSmallestWeight(a,b)){	//能找到两个最小 
		T.data[pos].lchild=a;
		T.data[pos].rchild=b;
		T.data[pos].parent=0;
		T.data[pos].weight=T.data[a].weight+T.data[b].weight;
		T.data[a].parent=T.data[b].parent=pos;
	}
} 

void createHuffmanCode(){	//生成每个字符的huffman编码
	if(emptyFile==true||oneChar==true)return;
	for(int i=1;!T.data[i].lchild;i++){	//遍历所有叶子 
		int j=i,e,p=0;
		char temp[300]={0}; 
		while(T.data[j].parent){		//没到根的时候 
			temp[p++]=(T.data[T.data[j].parent].rchild==j)+'0';	//右子树时为1 左子树为0 
			j=T.data[j].parent;
		}
		thr[T.data[i].ch]=(String) malloc(sizeof(char)*(p+1));
		strcpy(thr[T.data[i].ch],strrev(temp));	//原地逆置 
	}
} 

unsigned int writeCompFile(FILE *&fp1){ 		//生成压缩文件  返回文件大小 
	int l=strlen(fileName),oo;
	while(--l&&fileName[l]!='.');	//找到最后一个点
	char tt[30]={0},temp[300]={0},temp2[9]={0};
	strcpy(tt+1,fileName+l+1);		//保存后缀名
	tt[0]=strlen(tt+1);				//tt[0]保存字符串长 
	strcpy(fileName+l+1,"zip");
	FILE *fp2=openFile(fileName,"wb+"); //打开一个压缩文件
	if(emptyFile==true){fclose(fp2);	return 0;}		//空文件
	else if(oneChar==true){
		unsigned char ch;
		fread(&ch,1,1,fp1);		//是哪个字符 
		fseek(fp1,0L,SEEK_END);		//移到开头 
		fprintf(fp2,"%c%d%c",0,ftell(fp1),ch);	//存入 0 字符数 字符
		fclose(fp2);
		return 6;	//大小为6 
	} 
	fwrite(tt,tt[0]+1,1,fp2);		//存后缀名
	fwrite(charNum,sizeof(charNum),1,fp2);	//写入频数 
	fseek(fp1,0L,SEEK_SET);		//移到开头 
	fseek(fp1,0L,SEEK_CUR);		//切换读写指针
	unsigned char ch;			//存放二进制 
	while(!feof(fp1)){ 
		fread(&ch,1,1,fp1);			//读二进制
		if(feof(fp1)){
			oo=strlen(temp);
			strcat(temp,"00000000");		//不足8位补0
		}
		else strcat(temp,thr[ch]);		//连接字符串
		l=strlen(temp);
		while((l-=8)>0){	//多于8位 
			strncpy(temp2,temp,8);	//复制8位出来 
			unsigned char tm=strtol(temp2,NULL,2);//给tm 
			fwrite(&tm,1,1,fp2);	//写到压缩文件中 
			strcpy(temp,temp+8);	//删除前8位 
		}			
	}
	fprintf(fp2,"%c",8-oo);		//存入  补了多少个0 
	unsigned int ll=ftell(fp2);  
	fclose(fp2);
	return ll;
} 

void comp(FILE *&fp1){	//压缩文件fp1 
	clock_t begin=clock(),end;
	printf("请耐心等待...");
	getCharNum(fp1);	//统计文件中每个字符出现的次数
	createHTree();		//生成HuffmanTree 
	createHuffmanCode();	//生成每个字符的huffman编码 
	unsigned int l=writeCompFile(fp1); 		//生成压缩文件 获得文件大小 
	CLEARSCREEN
	end=clock(); 
	printf("压缩成功！\n压缩前  文件大小%.1fkb\n压缩后  文件大小%.1fkb\n压缩率%3.1f%%\n",ftell(fp1)/1024.0,l/1024.0,emptyFile==true?100.0:(float)l/ftell(fp1)*100.0);
	printf("用时%.1fs\n压缩速率%.1fkb/s\n",(float)(end-begin)/CLOCKS_PER_SEC,ftell(fp1)/(float)(end-begin));
	PAUSE
}

unsigned int writeDiscompFile(FILE *&fp1,char c,unsigned int end){ 	//end为结尾位置，补了c个0 
	FILE *fp2=openFile(fileName,"wb+"); //打开一个压缩文件
	unsigned char ch;
	int k=8,p=T.root;
	fread(&ch,1,1,fp1);
	if(ftell(fp1)!=end){			//因为可能读完一个字节文件就已经结束 如果结束跳过if 
		while(1){
			while(k&&T.data[p].rchild)p=((ch>>--k)&1?T.data[p].rchild:T.data[p].lchild);//找到叶子节点
			if(!k){
				fread(&ch,1,1,fp1);		
				k=8;
				if(ftell(fp1)==end)break;		//删掉字符串中结尾c个0 
			}
			else {
				fwrite(&(T.data[p].ch),1,1,fp2); 
				p=T.root;
			}
		}
	}	//处理完8位8位的字符 剩余一个不满8位的 
	do{
		while(k>c&&T.data[p].rchild)p=((ch>>--k)&1?T.data[p].rchild:T.data[p].lchild);//找到叶子节点
		fwrite(&(T.data[p].ch),1,1,fp2); 
		p=T.root;
	}while(k>c);	//写进最后几位 
	unsigned int ll=ftell(fp2); 
	fclose(fp2);
	return ll;
}

void discomp(FILE *&fp1){
	char tt[30],cc,c;
	unsigned int end;
	if((c=fgetc(fp1))==EOF){	printf("这是一个空文件!\n");	fclose(fp1); PAUSE}		//空文件 
	else if(c==0){	//单字符文件 
		FILE *fp2=openFile(fileName,"wb+"); //打开一个压缩文件
		fseek(fp1,0L,SEEK_SET);	
		fseek(fp1,0L,SEEK_CUR);		//切换指针
		fscanf(fp1,"%d%c",&end,&c);	//读数量和字符
		while(end--)fwrite(&c,1,1,fp2);		//写字符
		fclose(fp2);
		fclose(fp1);
		printf("解压成功！\n");
		PAUSE
	} 
	printf("请耐心等待...");
	clock_t begin=clock(),endd;
	fseek(fp1,-1L,SEEK_END);	//读补了几个0 
	fseek(fp1,0L,SEEK_CUR);		//切换指针
	end=ftell(fp1);
	fscanf(fp1,"%c",&c); 		//几个0 
	fseek(fp1,0L,SEEK_SET);	
	fseek(fp1,0L,SEEK_CUR);		//切换指针
	fscanf(fp1,"%c",&cc);	//后缀名位数 
	fread(tt,cc,1,fp1);		//读后缀名 
	fread(charNum,sizeof(charNum),1,fp1);	//读频数表 
	int l=strlen(fileName);
	while(--l&&fileName[l]!='.');	//找到最后一个点
	strcpy(fileName+l+1,tt);
	createHTree();		//生成HuffmanTree 
	unsigned int ll=writeDiscompFile(fp1,c,end);
	endd=clock();
	CLEARSCREEN
	printf("解压成功！\n用时%.1fs\n解压速率%.1fkb/s\n",(float)(endd-begin)/CLOCKS_PER_SEC,ll/(float)(endd-begin));
	PAUSE
}

main(){
	char n;
	printf("请拖放入文件\n");
	gets(fileName);	//读入文件名 不能超长 
	FILE *fp1=openFile(fileName);	//打开文件 
	CLEARSCREEN	//清空屏幕 
	printf("1. 压缩\n2. 解压\n请输入序号"); 
	loop:scanf("%d",&n);
	switch(n){
		case 1:CLEARSCREEN	comp(fp1);break;		//压缩
		case 2:CLEARSCREEN	discomp(fp1);break;	//解压
		default:printf("输入有误，请重新输入\n");goto loop; 
	}
	fclose(fp1);
}
