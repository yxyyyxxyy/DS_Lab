#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<conio.h>

//#define debug            //���Գ����� 
#define MAX_LENGTH 10000   //����ʽ��󳤶� ���ַ����� 
#define LLL "2"           //ϵ��������С��λ�� 
#define MOOD1             //�ڸ�ģʽ�£�ϵ����ָ����Ϊ 1 �� -1 ʱʡ��ϵ����ָ������������ʡ��ָ����ע�͵����м��ɲ�ʡ�� 
#define MIN_FLOAT 1e-20  //���������������С�� 
#define MAX_FLOAT 1e20   //������������������ 


#define FLOATX "%."LLL"f"
#define INPUTERROR {printf("������������������\n");goto again;}
#define CLEARSCREEN {system("cls");printList(list1,list2);} 

typedef const char* String; 

String MY_MENU;

typedef struct e{
	float x;//ϵ�� 
	int k;//ָ��
	struct e *next; 
}elem;

typedef struct{
	elem *head;
	int length;
	String op;
}list;

void initList(list &myList){  //��ʼ��������ͷ��� 
	myList.head=(elem *)malloc(sizeof(elem));
	myList.head->next=NULL;
	myList.length=0;
	myList.op="";   //û���κβ��� 
}

void addElem(float a,int b,list &myList){   //aϵ�� bָ�� ָ���ݼ�˳��������� 
	elem *p=myList.head,*q=myList.head; 
	while(p=p->next){
		if(p->k==b){   //ָ����ֱͬ����� 
			if((p->x+=a)==0){  //��Ӻ�ϵ��Ϊ0 
				q->next=p->next;  //ɾ������ 
				free(p); 
				myList.length--; 
			}
			return;    //����ı�listlength 
		} 
		if(p->k<b)break;
		q=p;   //�ҵ�λ�ã� ���뵽q->next 
	}
	q=q->next=(elem *)malloc(sizeof(elem));
	q->x=a;
	q->k=b;
	q->next=p; 
	myList.length++;
}

int changeElem(float a,int b,list &myList){   //aϵ�� bָ�� �޸�ָ��Ϊb�����ϵ��Ϊa������0��ʾû�ҵ�
	elem *p=myList.head,*q=myList.head;
	while(p=p->next){
		if(p->k==b){   //�ҵ� 
			if((p->x=a)==0){  //ɾ������ 
				q->next=p->next;
				free(p);
				myList.length--;
			}
			return 1;    
		} 
		q=p;
	}
	return 0;
} 

void clearList(list &myList){   //������� 
	myList.length=0;
	elem *p=myList.head->next,*q=myList.head->next;
	while(p){
		p=p->next;
		free(q);
		q=p;
	}
	myList.head->next=NULL; 
}


String toString(list myList){  //����ʽת��Ϊ�ַ��� 
	elem *p; 
	static char a[MAX_LENGTH];
	char *s=a;
	if(myList.length==0)s+=sprintf(s,"δ����"); 
	else{
		p=myList.head->next;
		if(p->x<0)sprintf(s++,"-");  //��һ������ 
		while(1){
#ifdef MOOD1
			if(fabs(p->x)!=1.0){
				if(p->k==1)s+=sprintf(s,FLOATX"x",fabs(p->x));    //FLOATX�Ǿ��� 
				else if(p->k==0)s+=sprintf(s,FLOATX,fabs(p->x)); 
				else s+=sprintf(s,FLOATX"x^%d",fabs(p->x),p->k);   //sʼ��ָ����һ���ַ�����λ�� 
			}
			else{
				if(p->k==1)s+=sprintf(s,"x");    //FLOATX�Ǿ��� 
				else if(p->k==0)s+=sprintf(s,FLOATX,fabs(p->x)); 
				else s+=sprintf(s,"x^%d",p->k);   //sʼ��ָ����һ���ַ�����λ�� 
			}
#else
			s+=sprintf(s,FLOATX"x^%d",fabs(p->x),p->k);   //sʼ��ָ����һ���ַ�����λ�� 
#endif 
			if((p=p->next)==NULL)break;
			s+=sprintf(s,p->x>0?" + ":" - ");
		}
	}
	sprintf(s,"   %s",myList.op); 
	return(a);
}

void printList(list list1,list list2){
	printf("****************************************\n");
	printf("����ʽ1    ����%3d    %s\n",list1.length,toString(list1));
	printf("����ʽ2    ����%3d    %s\n",list2.length,toString(list2));
	printf("****************************************\n");
}

void printMenu(list list1,list list2){   //��ӡ�˵� 
	CLEARSCREEN
	printf(MY_MENU);
	printf("****************************************\n");
	printf("��������Ž���ѡ��\n");
}

void setList(list &list1,list &list2){   //����1 �޸Ķ���ʽ 
	int n1,n2;
	list *clist;
	loop:CLEARSCREEN
	printf("1.���Ķ���ʽ1\n2.���Ķ���ʽ2\n3.�������˵�\n");
	again:scanf("%d",&n1);
	switch(n1){
		case 1:clist=&list1;list1.op="���ڸ���";break;
		case 2:clist=&list2;list2.op="���ڸ���";break;
		case 3:return;
		default:INPUTERROR
	}
	CLEARSCREEN
	printf("1.����\n2.�޸�\n3.׷��\n4.������һ��\n5.�������˵�\n");
	again2:scanf("%d",&n2);
	switch(n2){
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:list1.op=list2.op="";goto loop;
		case 5:list1.op=list2.op="";return;
		default:printf("������������������\n");goto again2;
	}
	{    //�����޸� 
		 if(n2==1){  //���� 
			clearList(*clist);
			int number=0;
			while(++number){
				CLEARSCREEN
				printf("ϵ��Ϊ0ʱ��Ϊ������������������룬ָ����ͬ���������\n");
		 		printf("��ֱ������%d���ϵ����ָ��\n",number);
		 		float ta;
				int tn;
				scanf("%f%d",&ta,&tn);
				if(ta==0)break;
				addElem(ta,tn,*clist);
			}
			(*clist).op=""; 
		 }
		 if(n2==3){  //׷��
			while(1){
				CLEARSCREEN
				printf("ϵ��Ϊ0ʱ��Ϊ������������������룬ָ����ͬ���������\n");
		 		printf("��ֱ�����Ҫ���ӵ����ϵ����ָ��\n");
		 		float ta;
				int tn;
				scanf("%f%d",&ta,&tn);
				if(ta==0)break;
				addElem(ta,tn,*clist);
			}
			(*clist).op=""; 
		 } 
		 if(n2==2){  //�޸�
			while(1){
				CLEARSCREEN
		 		printf("������Ҫ�޸ĵ����ָ��\n");
		 		float ta;
				int tn,num;
				scanf("%d",&tn);
				printf("ϵ��Ҫ�޸ĳ�\n");
				scanf("%f",&ta);
				int flag=(changeElem(ta,tn,*clist)==0);
				CLEARSCREEN
				if(flag)printf("û���ҵ�����\n");
				printf("1.�����޸�\n2.�������˵�\n");
				scanf("%d",&num);
				if(num!=1)break;
			}
			(*clist).op=""; 
		 } 
	} 
	
}

void destoryList(list &list1,list &list2){    //����ֻ����ն���ʽ���˳�ʱ�����ٶ���ʽ 
	CLEARSCREEN
	printf("���������ĸ�����ʽ��\n1.����ʽ1\n2.����ʽ2\n3.ȫ��\n4.�������˵�\n"); 
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 3:clearList(list2);
		case 1:clearList(list1);break;
		case 2:clearList(list2);break;
		case 4:return;
		default:INPUTERROR
	}
}

void addList(list &list1,list &list2){
	list1.op=list2.op="�������"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("���ȴ�����������ʽ��\n�����������\n");
		getchar();   //�����س� 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	while(p=p->next)addElem(p->x,p->k,tempList);
	while(q=q->next)addElem(q->x,q->k,tempList);
	printf("����ʽ֮��    ����%3d    %s\n",tempList.length,toString(tempList));
	printf("\n�Ƿ�Ҫ�����д�����ʽ1�����ʽ2\n1.д�����ʽ1\n2.д�����ʽ2\n3.�������˵�\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://д�����ʽ1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://д�����ʽ2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://��д�룬����tempList 
		{
			clearList(tempList);
			free(tempList.head);
			tempList.head=NULL;
			break;
		}
		
		default :INPUTERROR
	}
	list1.op=list2.op=""; 
}

void minusList(list &list1,list &list2){
	list1.op=list2.op="�������"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("���ȴ�����������ʽ��\n�����������\n");
		getchar();   //�����س� 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	int mood;
	printf("1.����ʽ1 - ����ʽ2\n2.����ʽ2 - ����ʽ1\n");
	again2:scanf("%d",&mood);
	switch(mood){
		case 1:list1.op="������� ������"; list2.op="������� ����"; break;
		case 2:list2.op="������� ������"; list1.op="������� ����"; break;
		default:printf("������������������\n");goto again2;
	}
	CLEARSCREEN
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	while(p=p->next)addElem((mood==1?p->x:-p->x),p->k,tempList);
	while(q=q->next)addElem((mood==1?-q->x:q->x),q->k,tempList);
	printf("����ʽ֮��    ����%3d    %s\n",tempList.length,toString(tempList));
	printf("\n�Ƿ�Ҫ�����д�����ʽ1�����ʽ2\n1.д�����ʽ1\n2.д�����ʽ2\n3.�������˵�\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://д�����ʽ1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://д�����ʽ2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://��д�룬����tempList 
		{
			clearList(tempList);
			free(tempList.head);
			tempList.head=NULL;
			break;
		}
		
		default :INPUTERROR
	}
	list1.op=list2.op=""; 
}

void minusListNow(list &list1,list &list2){  //list1=list1-list2; ����� 
	int mood=1;
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	while(p=p->next)addElem((mood==1?p->x:-p->x),p->k,tempList);
	while(q=q->next)addElem((mood==1?-q->x:q->x),q->k,tempList);
	clearList(list1);
	list1.length=tempList.length;
	list1.head=tempList.head;	
}


void multList(list &list1,list &list2){
	list1.op=list2.op="�������"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("���ȴ�����������ʽ��\n�����������\n");
		getchar();   //�����س� 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	while(p=p->next){
		q=list2.head; 
		while(q=q->next){
			addElem(q->x*p->x,q->k+p->k,tempList);	
		}
	}
	printf("����ʽ֮��    ����%3d    %s\n",tempList.length,toString(tempList));
	printf("\n�Ƿ�Ҫ�����д�����ʽ1�����ʽ2\n1.д�����ʽ1\n2.д�����ʽ2\n3.�������˵�\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://д�����ʽ1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://д�����ʽ2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://��д�룬����tempList 
		{
			clearList(tempList);
			free(tempList.head);
			tempList.head=NULL;
			break;
		}
		
		default :INPUTERROR
	}
	list1.op=list2.op=""; 
}

void multListNow(list &list1,list &list2){    //�������˷���д��list1 
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	while(p=p->next){
		q=list2.head; 
		while(q=q->next){
			addElem(q->x*p->x,q->k+p->k,tempList);	
		}
	}
	clearList(list1);//д�����ʽ1 
	list1.length=tempList.length;
	list1.head=tempList.head;
}

float calcList(list myList,float x){  //��myList �� x ����ֵ 
	elem *p=myList.head;
	float sum=0;
	while(p=p->next){
		sum+=p->x*pow(x,p->k);
	}
	return(sum);
}

void calculateList(list &list1,list &list2){
	CLEARSCREEN
	printf("1.�����ʽ1\n2.�����ʽ2\n3.�������˵�\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1: 
		{
			list1.op="������ֵ";
			CLEARSCREEN
			if(list1.length==0){
				printf("���ȴ�������ʽ1\n��������������˵�\n");
				getchar();
				getch();
				list1.op=""; 
				return;
			}
			break;
		}
		
		case 2: 
		{
			list2.op="������ֵ";
			CLEARSCREEN
			if(list2.length==0){
				printf("���ȴ�������ʽ2\n��������������˵�\n");
				getchar();
				getch();
				list2.op=""; 
				return;
			}
			break;
		}
		
		case 3: return;
		case 4: INPUTERROR
	}
	printf("������xֵ\n");
	float xx;
	scanf("%f",&xx);
	CLEARSCREEN
	if(n==1)printf("����ʽ1 �� x="FLOATX" ��ֵΪ "FLOATX,xx,calcList(list1,xx));
	else printf("����ʽ2 �� x="FLOATX" ��ֵΪ "FLOATX,xx,calcList(list2,xx));
	printf("\n\n�����������\n");
	getchar();
	getch();
	list1.op=list2.op="";
}

void divList(list &list1,list &list2){
	list1.op=list2.op="��������"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("���ȴ�����������ʽ��\n�����������\n");
		getchar();   //�����س� 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	int mood;
	printf("1.����ʽ1 / ����ʽ2\n2.����ʽ2 / ����ʽ1\n");
	again2:scanf("%d",&mood);
	switch(mood){
		case 1:list1.op="�������� ������"; list2.op="�������� ����"; break;
		case 2:list2.op="�������� ������"; list1.op="�������� ����"; break;
		default:printf("������������������\n");goto again2;
	}
	CLEARSCREEN
	list tempList;
	tempList.head=(elem *)malloc(sizeof(elem));
	tempList.head->next=NULL;
	tempList.length=0;
	tempList.op="";
	elem *p=list1.head,*q=list2.head;
	list copyList;
	copyList.head=(elem *)malloc(sizeof(elem));
	copyList.head->next=NULL;
	copyList.length=0;
	copyList.op="";
	list resultList;
	resultList.head=(elem *)malloc(sizeof(elem));
	resultList.head->next=NULL;
	resultList.length=0;
	resultList.op="";
	bool tooSmall=false,tooLarge=false;
	if(mood==1){   //   1/2
		while(p=p->next)addElem(p->x,p->k,copyList); //����һ��list1
		q=q->next;  //����ͷ��� 
		while(1){
			p=copyList.head->next;
			if(p==NULL||p->k<q->k)break;
			if(fabs(p->x)<MIN_FLOAT){
				tooSmall=true;
				break;			
			}
			if(fabs(p->x)>MAX_FLOAT){
				tooLarge=true;
				break;			
			}
			if(resultList.length>=MAX_LENGTH){
				printf("��������趨������������Դ�ļ����޸Ķ���ʽ��󳤶� MAX_LENGTH ��\n�����������\n");
				getchar();
				getch();
				goto my_exit;
			}
			clearList(tempList);
			addElem(p->x/q->x,p->k-q->k,tempList);
			addElem(p->x/q->x,p->k-q->k,resultList);
			multListNow(tempList,list2);  //templist=tempList*list2
			minusListNow(copyList,tempList);//copyList=copyList-templist; 
#ifdef debug       //���Գ��� list1 / list2 
/*�������� ��С 
1 1  1 1 1000000 0 0
1 2 1 10 10 1 1 0 0
6 1
�������� ����
1 1  1 1 1000000 0 0
1 2 1 0.1 10 1 1 0 0
6 1
*/ 
printf("��      ����%3d    %s\n",resultList.length,resultList.length?toString(resultList):"��");
printf("����    ����%3d    %s\n",list2.length,list2.length?toString(list2):"��");
printf("ʣ����  ����%3d    %s\n",copyList.length,copyList.length?toString(copyList):"��");
getchar(); 
#endif	
		}
	} 
	else{     //   2/1 
		while(q=q->next)addElem(q->x,q->k,copyList); //����һ��list2
		p=p->next;  //����ͷ��� 
		while(1){
			q=copyList.head->next;
			if(q==NULL||q->k<p->k)break;
			if(fabs(q->x)<MIN_FLOAT){
				tooSmall=true;
				break;			
			}
			if(fabs(p->x)>MAX_FLOAT){
				tooLarge=true;
				break;			
			}
			if(resultList.length>=MAX_LENGTH){
				printf("��������趨������������Դ�ļ����޸Ķ���ʽ��󳤶� MAX_LENGTH ��\n�����������\n");
				getchar();
				getch();
				goto my_exit;
			}
			clearList(tempList);
			addElem(q->x/p->x,q->k-p->k,tempList);
			addElem(q->x/p->x,q->k-p->k,resultList);
			multListNow(tempList,list1);  //templist=tempList*list2
			minusListNow(copyList,tempList);//copyList=copyList-templist; 	
		}
	}
	if(tooSmall==true)printf("��̫С�ˣ��Ѿ��޷��������������С�����޸�Դ�ļ��е� MIN_FLOAT\n");
	if(tooLarge==true)printf("��̫���ˣ��Ѿ��޷�������������������޸�Դ�ļ��е� MAX_FLOAT\n");
	printf("��      ����%3d    %s\n",resultList.length,resultList.length?toString(resultList):"��");
	printf("����    ����%3d    %s\n",copyList.length,copyList.length?toString(copyList):"��");
	printf("�����������\n");
	getchar();
	getch();
	my_exit:clearList(tempList);
	clearList(copyList);
	clearList(resultList);
	free(tempList.head);
	free(copyList.head);
	free(resultList.head);     //����������ʱ���� 
	list1.op=list2.op=""; 
}

void exitList(list &list1,list &list2){  //�˳������������� 
	clearList(list1);
	clearList(list2);
	free(list1.head);
	free(list2.head);
	list1.head=NULL;
	list2.head=NULL;
	system("cls");
	printf("��лʹ��!\n");
	getchar();
	getch(); 
	exit(0);
}

main(){
	list list1,list2;     //��������ʽ 
	int num;
	const int funcNum=8;     //�������� 
	MY_MENU="1.��������ʽ\n2.���ٶ���ʽ\n3.����ʽ���\n4.����ʽ���\n5.����ʽ���\n6.����ʽ���\n7.����ʽ��ֵ\n8.�˳�\n";    //������ 
	void (*function[funcNum])(list &,list &)={setList,destoryList,addList,minusList,multList,divList,calculateList,exitList};    //���ܺ����� 
	initList(list1);       //��ʼ�� 
	initList(list2);
	while(1){
		printMenu(list1,list2);   //��ӡ�˵� 
		again:scanf("%d",&num);    //���빦����� 
		if(num<1||num>funcNum)INPUTERROR   //������������ 
		function[num-1](list1,list2);     //���ù��ܵĺ��� 
	}
} 
