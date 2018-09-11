#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAXFILENAMELENGTH 1000  //�ļ�����󳤶� 
#define CLEARSCREEN system("cls"); 	//�����Ļ  
#define PAUSE system("pause");exit(0);		//��ͣ���˳� 

typedef char *String;
typedef struct{
	unsigned char ch;		//��ŵ�8bit�� 
	unsigned long weight,parent,lchild,rchild;
}HTNode;
typedef struct{
	HTNode data[512];	//0���� 
	int root;	//�� 
}HTree;
HTree T;
bool oneChar=false,emptyFile=false;		//�ļ�ֻ��һ���ַ���huffman���޷����� 
static String thr[256];	//ÿ���ַ���huffman���� 
char fileName[MAXFILENAMELENGTH];	//�ļ��� 
unsigned long charNum[256];	//ÿ���ַ����ֵĴ��� 

FILE* openFile(String fileName,String mood="rb+"){		//������ΪfileName�Ķ������ļ� �����ļ�ָ�� 
	FILE *fp; 
	if((fp=fopen(fileName,mood))==NULL){
		printf("���ļ�ʧ�ܣ�\n");
		PAUSE
	}
	return fp;
}

void getCharNum(FILE *&fp){		//ͳ���ļ���ÿ���ַ����ֵĴ��� 
	memset(charNum,0,sizeof(charNum));	//��� 
	fseek(fp,0L,SEEK_SET);	//ָ���Ƶ��ļ���ͷ
	fseek(fp,0L,SEEK_CUR);	//�л���дָ�� 
	unsigned char ch;	//ch 0-255 
	while(1){ 
		fread(&ch,1,1,fp);	//��һ�ֽ�����
		if(feof(fp))break;	//�ļ����� 
		charNum[ch]++;		//������1 
	}
} 

int getSmallestWeight(int &a,int &b){		//�����������СȨ��(>0)���ַ���tree�е�λ�� ����0��ʾ�Ѿ���������root��ֵ ���򷵻ؽ���λ�� 
	int i;
	a=b=0;
	for(i=1;i<512&&T.data[i].weight;i++)	if(T.data[i].parent==0){	a=i;	break;}		//�ҵ�1��a
	for(i++;i<512&&T.data[i].weight;i++)	if(T.data[i].parent==0){	b=i;	break;}		//�����ҵ�1��b 
	if(b==0){	//��һ��trueʱ�ҵ�a��root������û��b 
		T.root=a;
		return 0; 	//ֻ���ҵ�1�������� 
	}
	for(i++;i<512&&T.data[i].weight;i++){
		if(T.data[i].parent==0&&T.data[i].weight<T.data[a].weight)a=i;		//parent!=0
		else if(T.data[i].parent==0&&T.data[i].weight<T.data[b].weight)b=i;
	}	//��ȡweight��С������i 
	return i; 
}

void createHTree(){		//�������е�charNum��������HuffmanTree 0�β�д 
	memset(T.data,0,sizeof(T.data));	//���
	int j=1,a,b,pos,nnum=0; 
	for(int i=0;i<=255;i++){
		if(charNum[i]==0)continue;	//���ַ�������	
		nnum++;
		T.data[j].rchild=T.data[j].lchild=T.data[j].parent=0;
		T.data[j].weight=charNum[i];
		T.data[j++].ch=i;		//j+1����һ����������һ��λ�� 
	} 
	if(nnum==0)	{	emptyFile=true;	return;}		//���ļ� 
	else if(nnum==1){	oneChar=true;	return;}		//ֻ��һ���ַ� 
	while(pos=getSmallestWeight(a,b)){	//���ҵ�������С 
		T.data[pos].lchild=a;
		T.data[pos].rchild=b;
		T.data[pos].parent=0;
		T.data[pos].weight=T.data[a].weight+T.data[b].weight;
		T.data[a].parent=T.data[b].parent=pos;
	}
} 

void createHuffmanCode(){	//����ÿ���ַ���huffman����
	if(emptyFile==true||oneChar==true)return;
	for(int i=1;!T.data[i].lchild;i++){	//��������Ҷ�� 
		int j=i,e,p=0;
		char temp[300]={0}; 
		while(T.data[j].parent){		//û������ʱ�� 
			temp[p++]=(T.data[T.data[j].parent].rchild==j)+'0';	//������ʱΪ1 ������Ϊ0 
			j=T.data[j].parent;
		}
		thr[T.data[i].ch]=(String) malloc(sizeof(char)*(p+1));
		strcpy(thr[T.data[i].ch],strrev(temp));	//ԭ������ 
	}
} 

unsigned int writeCompFile(FILE *&fp1){ 		//����ѹ���ļ�  �����ļ���С 
	int l=strlen(fileName),oo;
	while(--l&&fileName[l]!='.');	//�ҵ����һ����
	char tt[30]={0},temp[300]={0},temp2[9]={0};
	strcpy(tt+1,fileName+l+1);		//�����׺��
	tt[0]=strlen(tt+1);				//tt[0]�����ַ����� 
	strcpy(fileName+l+1,"zip");
	FILE *fp2=openFile(fileName,"wb+"); //��һ��ѹ���ļ�
	if(emptyFile==true){fclose(fp2);	return 0;}		//���ļ�
	else if(oneChar==true){
		unsigned char ch;
		fread(&ch,1,1,fp1);		//���ĸ��ַ� 
		fseek(fp1,0L,SEEK_END);		//�Ƶ���ͷ 
		fprintf(fp2,"%c%d%c",0,ftell(fp1),ch);	//���� 0 �ַ��� �ַ�
		fclose(fp2);
		return 6;	//��СΪ6 
	} 
	fwrite(tt,tt[0]+1,1,fp2);		//���׺��
	fwrite(charNum,sizeof(charNum),1,fp2);	//д��Ƶ�� 
	fseek(fp1,0L,SEEK_SET);		//�Ƶ���ͷ 
	fseek(fp1,0L,SEEK_CUR);		//�л���дָ��
	unsigned char ch;			//��Ŷ����� 
	while(!feof(fp1)){ 
		fread(&ch,1,1,fp1);			//��������
		if(feof(fp1)){
			oo=strlen(temp);
			strcat(temp,"00000000");		//����8λ��0
		}
		else strcat(temp,thr[ch]);		//�����ַ���
		l=strlen(temp);
		while((l-=8)>0){	//����8λ 
			strncpy(temp2,temp,8);	//����8λ���� 
			unsigned char tm=strtol(temp2,NULL,2);//��tm 
			fwrite(&tm,1,1,fp2);	//д��ѹ���ļ��� 
			strcpy(temp,temp+8);	//ɾ��ǰ8λ 
		}			
	}
	fprintf(fp2,"%c",8-oo);		//����  ���˶��ٸ�0 
	unsigned int ll=ftell(fp2);  
	fclose(fp2);
	return ll;
} 

void comp(FILE *&fp1){	//ѹ���ļ�fp1 
	clock_t begin=clock(),end;
	printf("�����ĵȴ�...");
	getCharNum(fp1);	//ͳ���ļ���ÿ���ַ����ֵĴ���
	createHTree();		//����HuffmanTree 
	createHuffmanCode();	//����ÿ���ַ���huffman���� 
	unsigned int l=writeCompFile(fp1); 		//����ѹ���ļ� ����ļ���С 
	CLEARSCREEN
	end=clock(); 
	printf("ѹ���ɹ���\nѹ��ǰ  �ļ���С%.1fkb\nѹ����  �ļ���С%.1fkb\nѹ����%3.1f%%\n",ftell(fp1)/1024.0,l/1024.0,emptyFile==true?100.0:(float)l/ftell(fp1)*100.0);
	printf("��ʱ%.1fs\nѹ������%.1fkb/s\n",(float)(end-begin)/CLOCKS_PER_SEC,ftell(fp1)/(float)(end-begin));
	PAUSE
}

unsigned int writeDiscompFile(FILE *&fp1,char c,unsigned int end){ 	//endΪ��βλ�ã�����c��0 
	FILE *fp2=openFile(fileName,"wb+"); //��һ��ѹ���ļ�
	unsigned char ch;
	int k=8,p=T.root;
	fread(&ch,1,1,fp1);
	if(ftell(fp1)!=end){			//��Ϊ���ܶ���һ���ֽ��ļ����Ѿ����� �����������if 
		while(1){
			while(k&&T.data[p].rchild)p=((ch>>--k)&1?T.data[p].rchild:T.data[p].lchild);//�ҵ�Ҷ�ӽڵ�
			if(!k){
				fread(&ch,1,1,fp1);		
				k=8;
				if(ftell(fp1)==end)break;		//ɾ���ַ����н�βc��0 
			}
			else {
				fwrite(&(T.data[p].ch),1,1,fp2); 
				p=T.root;
			}
		}
	}	//������8λ8λ���ַ� ʣ��һ������8λ�� 
	do{
		while(k>c&&T.data[p].rchild)p=((ch>>--k)&1?T.data[p].rchild:T.data[p].lchild);//�ҵ�Ҷ�ӽڵ�
		fwrite(&(T.data[p].ch),1,1,fp2); 
		p=T.root;
	}while(k>c);	//д�����λ 
	unsigned int ll=ftell(fp2); 
	fclose(fp2);
	return ll;
}

void discomp(FILE *&fp1){
	char tt[30],cc,c;
	unsigned int end;
	if((c=fgetc(fp1))==EOF){	printf("����һ�����ļ�!\n");	fclose(fp1); PAUSE}		//���ļ� 
	else if(c==0){	//���ַ��ļ� 
		FILE *fp2=openFile(fileName,"wb+"); //��һ��ѹ���ļ�
		fseek(fp1,0L,SEEK_SET);	
		fseek(fp1,0L,SEEK_CUR);		//�л�ָ��
		fscanf(fp1,"%d%c",&end,&c);	//���������ַ�
		while(end--)fwrite(&c,1,1,fp2);		//д�ַ�
		fclose(fp2);
		fclose(fp1);
		printf("��ѹ�ɹ���\n");
		PAUSE
	} 
	printf("�����ĵȴ�...");
	clock_t begin=clock(),endd;
	fseek(fp1,-1L,SEEK_END);	//�����˼���0 
	fseek(fp1,0L,SEEK_CUR);		//�л�ָ��
	end=ftell(fp1);
	fscanf(fp1,"%c",&c); 		//����0 
	fseek(fp1,0L,SEEK_SET);	
	fseek(fp1,0L,SEEK_CUR);		//�л�ָ��
	fscanf(fp1,"%c",&cc);	//��׺��λ�� 
	fread(tt,cc,1,fp1);		//����׺�� 
	fread(charNum,sizeof(charNum),1,fp1);	//��Ƶ���� 
	int l=strlen(fileName);
	while(--l&&fileName[l]!='.');	//�ҵ����һ����
	strcpy(fileName+l+1,tt);
	createHTree();		//����HuffmanTree 
	unsigned int ll=writeDiscompFile(fp1,c,end);
	endd=clock();
	CLEARSCREEN
	printf("��ѹ�ɹ���\n��ʱ%.1fs\n��ѹ����%.1fkb/s\n",(float)(endd-begin)/CLOCKS_PER_SEC,ll/(float)(endd-begin));
	PAUSE
}

main(){
	char n;
	printf("���Ϸ����ļ�\n");
	gets(fileName);	//�����ļ��� ���ܳ��� 
	FILE *fp1=openFile(fileName);	//���ļ� 
	CLEARSCREEN	//�����Ļ 
	printf("1. ѹ��\n2. ��ѹ\n���������"); 
	loop:scanf("%d",&n);
	switch(n){
		case 1:CLEARSCREEN	comp(fp1);break;		//ѹ��
		case 2:CLEARSCREEN	discomp(fp1);break;	//��ѹ
		default:printf("������������������\n");goto loop; 
	}
	fclose(fp1);
}
