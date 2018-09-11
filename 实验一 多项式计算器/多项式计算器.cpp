#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<conio.h>

//#define debug            //测试除法用 
#define MAX_LENGTH 10000   //多项式最大长度 （字符数） 
#define LLL "2"           //系数保留的小数位数 
#define MOOD1             //在该模式下，系数（指数）为 1 或 -1 时省略系数（指数），常数项省略指数，注释掉这行即可不省略 
#define MIN_FLOAT 1e-20  //除法运算能算的最小数 
#define MAX_FLOAT 1e20   //除法运算能算的最大数 


#define FLOATX "%."LLL"f"
#define INPUTERROR {printf("输入有误，请重新输入\n");goto again;}
#define CLEARSCREEN {system("cls");printList(list1,list2);} 

typedef const char* String; 

String MY_MENU;

typedef struct e{
	float x;//系数 
	int k;//指数
	struct e *next; 
}elem;

typedef struct{
	elem *head;
	int length;
	String op;
}list;

void initList(list &myList){  //初始化，创建头结点 
	myList.head=(elem *)malloc(sizeof(elem));
	myList.head->next=NULL;
	myList.length=0;
	myList.op="";   //没有任何操作 
}

void addElem(float a,int b,list &myList){   //a系数 b指数 指数递减顺序插入链表 
	elem *p=myList.head,*q=myList.head; 
	while(p=p->next){
		if(p->k==b){   //指数相同直接相加 
			if((p->x+=a)==0){  //相加后系数为0 
				q->next=p->next;  //删除该项 
				free(p); 
				myList.length--; 
			}
			return;    //无需改变listlength 
		} 
		if(p->k<b)break;
		q=p;   //找到位置， 插入到q->next 
	}
	q=q->next=(elem *)malloc(sizeof(elem));
	q->x=a;
	q->k=b;
	q->next=p; 
	myList.length++;
}

int changeElem(float a,int b,list &myList){   //a系数 b指数 修改指数为b的项的系数为a，返回0表示没找到
	elem *p=myList.head,*q=myList.head;
	while(p=p->next){
		if(p->k==b){   //找到 
			if((p->x=a)==0){  //删除该项 
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

void clearList(list &myList){   //清空链表 
	myList.length=0;
	elem *p=myList.head->next,*q=myList.head->next;
	while(p){
		p=p->next;
		free(q);
		q=p;
	}
	myList.head->next=NULL; 
}


String toString(list myList){  //多项式转换为字符串 
	elem *p; 
	static char a[MAX_LENGTH];
	char *s=a;
	if(myList.length==0)s+=sprintf(s,"未输入"); 
	else{
		p=myList.head->next;
		if(p->x<0)sprintf(s++,"-");  //第一个负号 
		while(1){
#ifdef MOOD1
			if(fabs(p->x)!=1.0){
				if(p->k==1)s+=sprintf(s,FLOATX"x",fabs(p->x));    //FLOATX是精度 
				else if(p->k==0)s+=sprintf(s,FLOATX,fabs(p->x)); 
				else s+=sprintf(s,FLOATX"x^%d",fabs(p->x),p->k);   //s始终指向下一个字符输入位置 
			}
			else{
				if(p->k==1)s+=sprintf(s,"x");    //FLOATX是精度 
				else if(p->k==0)s+=sprintf(s,FLOATX,fabs(p->x)); 
				else s+=sprintf(s,"x^%d",p->k);   //s始终指向下一个字符输入位置 
			}
#else
			s+=sprintf(s,FLOATX"x^%d",fabs(p->x),p->k);   //s始终指向下一个字符输入位置 
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
	printf("多项式1    项数%3d    %s\n",list1.length,toString(list1));
	printf("多项式2    项数%3d    %s\n",list2.length,toString(list2));
	printf("****************************************\n");
}

void printMenu(list list1,list list2){   //打印菜单 
	CLEARSCREEN
	printf(MY_MENU);
	printf("****************************************\n");
	printf("请输入序号进行选择\n");
}

void setList(list &list1,list &list2){   //功能1 修改多项式 
	int n1,n2;
	list *clist;
	loop:CLEARSCREEN
	printf("1.更改多项式1\n2.更改多项式2\n3.返回主菜单\n");
	again:scanf("%d",&n1);
	switch(n1){
		case 1:clist=&list1;list1.op="正在更改";break;
		case 2:clist=&list2;list2.op="正在更改";break;
		case 3:return;
		default:INPUTERROR
	}
	CLEARSCREEN
	printf("1.创建\n2.修改\n3.追加\n4.返回上一级\n5.返回主菜单\n");
	again2:scanf("%d",&n2);
	switch(n2){
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:list1.op=list2.op="";goto loop;
		case 5:list1.op=list2.op="";return;
		default:printf("输入有误，请重新输入\n");goto again2;
	}
	{    //进行修改 
		 if(n2==1){  //创建 
			clearList(*clist);
			int number=0;
			while(++number){
				CLEARSCREEN
				printf("系数为0时认为输入结束，可乱序输入，指数相同则相加两项\n");
		 		printf("请分别输入第%d项的系数和指数\n",number);
		 		float ta;
				int tn;
				scanf("%f%d",&ta,&tn);
				if(ta==0)break;
				addElem(ta,tn,*clist);
			}
			(*clist).op=""; 
		 }
		 if(n2==3){  //追加
			while(1){
				CLEARSCREEN
				printf("系数为0时认为输入结束，可乱序输入，指数相同则相加两项\n");
		 		printf("请分别输入要增加的项的系数和指数\n");
		 		float ta;
				int tn;
				scanf("%f%d",&ta,&tn);
				if(ta==0)break;
				addElem(ta,tn,*clist);
			}
			(*clist).op=""; 
		 } 
		 if(n2==2){  //修改
			while(1){
				CLEARSCREEN
		 		printf("请输入要修改的项的指数\n");
		 		float ta;
				int tn,num;
				scanf("%d",&tn);
				printf("系数要修改成\n");
				scanf("%f",&ta);
				int flag=(changeElem(ta,tn,*clist)==0);
				CLEARSCREEN
				if(flag)printf("没有找到该项\n");
				printf("1.继续修改\n2.返回主菜单\n");
				scanf("%d",&num);
				if(num!=1)break;
			}
			(*clist).op=""; 
		 } 
	} 
	
}

void destoryList(list &list1,list &list2){    //这里只是清空多项式。退出时将销毁多项式 
	CLEARSCREEN
	printf("你想销毁哪个多项式？\n1.多项式1\n2.多项式2\n3.全部\n4.返回主菜单\n"); 
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
	list1.op=list2.op="正在求和"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("请先创建两个多项式。\n按任意键返回\n");
		getchar();   //消除回车 
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
	printf("多项式之和    项数%3d    %s\n",tempList.length,toString(tempList));
	printf("\n是否要将结果写入多项式1或多项式2\n1.写入多项式1\n2.写入多项式2\n3.返回主菜单\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://写入多项式1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://写入多项式2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://不写入，销毁tempList 
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
	list1.op=list2.op="正在求差"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("请先创建两个多项式。\n按任意键返回\n");
		getchar();   //消除回车 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	int mood;
	printf("1.多项式1 - 多项式2\n2.多项式2 - 多项式1\n");
	again2:scanf("%d",&mood);
	switch(mood){
		case 1:list1.op="正在求差 被减数"; list2.op="正在求差 减数"; break;
		case 2:list2.op="正在求差 被减数"; list1.op="正在求差 减数"; break;
		default:printf("输入有误，请重新输入\n");goto again2;
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
	printf("多项式之差    项数%3d    %s\n",tempList.length,toString(tempList));
	printf("\n是否要将结果写入多项式1或多项式2\n1.写入多项式1\n2.写入多项式2\n3.返回主菜单\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://写入多项式1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://写入多项式2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://不写入，销毁tempList 
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

void minusListNow(list &list1,list &list2){  //list1=list1-list2; 无输出 
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
	list1.op=list2.op="正在求积"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("请先创建两个多项式。\n按任意键返回\n");
		getchar();   //消除回车 
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
	printf("多项式之积    项数%3d    %s\n",tempList.length,toString(tempList));
	printf("\n是否要将结果写入多项式1或多项式2\n1.写入多项式1\n2.写入多项式2\n3.返回主菜单\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1://写入多项式1 
		{
			clearList(list1);
			list1.length=tempList.length;
			list1.head=tempList.head;
			break;
		}
		
		case 2://写入多项式2 
		{
			clearList(list2);
			list2.length=tempList.length;
			list2.head=tempList.head;
			break;
		}	
		
		case 3://不写入，销毁tempList 
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

void multListNow(list &list1,list &list2){    //立即做乘法并写入list1 
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
	clearList(list1);//写入多项式1 
	list1.length=tempList.length;
	list1.head=tempList.head;
}

float calcList(list myList,float x){  //求myList 在 x 处的值 
	elem *p=myList.head;
	float sum=0;
	while(p=p->next){
		sum+=p->x*pow(x,p->k);
	}
	return(sum);
}

void calculateList(list &list1,list &list2){
	CLEARSCREEN
	printf("1.求多项式1\n2.求多项式2\n3.返回主菜单\n");
	int n;
	again:scanf("%d",&n);
	switch(n){
		case 1: 
		{
			list1.op="正在求值";
			CLEARSCREEN
			if(list1.length==0){
				printf("请先创建多项式1\n按任意键返回主菜单\n");
				getchar();
				getch();
				list1.op=""; 
				return;
			}
			break;
		}
		
		case 2: 
		{
			list2.op="正在求值";
			CLEARSCREEN
			if(list2.length==0){
				printf("请先创建多项式2\n按任意键返回主菜单\n");
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
	printf("请输入x值\n");
	float xx;
	scanf("%f",&xx);
	CLEARSCREEN
	if(n==1)printf("多项式1 在 x="FLOATX" 的值为 "FLOATX,xx,calcList(list1,xx));
	else printf("多项式2 在 x="FLOATX" 的值为 "FLOATX,xx,calcList(list2,xx));
	printf("\n\n按任意键返回\n");
	getchar();
	getch();
	list1.op=list2.op="";
}

void divList(list &list1,list &list2){
	list1.op=list2.op="正在求商"; 
	CLEARSCREEN
	if(list1.length==0||list2.length==0){
		printf("请先创建两个多项式。\n按任意键返回\n");
		getchar();   //消除回车 
		getch();
		list1.op=list2.op=""; 
		return; 
	}
	int mood;
	printf("1.多项式1 / 多项式2\n2.多项式2 / 多项式1\n");
	again2:scanf("%d",&mood);
	switch(mood){
		case 1:list1.op="正在求商 被除数"; list2.op="正在求商 除数"; break;
		case 2:list2.op="正在求商 被除数"; list1.op="正在求商 除数"; break;
		default:printf("输入有误，请重新输入\n");goto again2;
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
		while(p=p->next)addElem(p->x,p->k,copyList); //复制一份list1
		q=q->next;  //跳过头结点 
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
				printf("结果超出设定长度啦！请在源文件中修改多项式最大长度 MAX_LENGTH ！\n按任意键返回\n");
				getchar();
				getch();
				goto my_exit;
			}
			clearList(tempList);
			addElem(p->x/q->x,p->k-q->k,tempList);
			addElem(p->x/q->x,p->k-q->k,resultList);
			multListNow(tempList,list2);  //templist=tempList*list2
			minusListNow(copyList,tempList);//copyList=copyList-templist; 
#ifdef debug       //测试除法 list1 / list2 
/*测试数据 极小 
1 1  1 1 1000000 0 0
1 2 1 10 10 1 1 0 0
6 1
测试数据 极大
1 1  1 1 1000000 0 0
1 2 1 0.1 10 1 1 0 0
6 1
*/ 
printf("商      项数%3d    %s\n",resultList.length,resultList.length?toString(resultList):"无");
printf("除数    项数%3d    %s\n",list2.length,list2.length?toString(list2):"无");
printf("剩余数  项数%3d    %s\n",copyList.length,copyList.length?toString(copyList):"无");
getchar(); 
#endif	
		}
	} 
	else{     //   2/1 
		while(q=q->next)addElem(q->x,q->k,copyList); //复制一份list2
		p=p->next;  //跳过头结点 
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
				printf("结果超出设定长度啦！请在源文件中修改多项式最大长度 MAX_LENGTH ！\n按任意键返回\n");
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
	if(tooSmall==true)printf("数太小了，已经无法运算啦！如需更小，请修改源文件中的 MIN_FLOAT\n");
	if(tooLarge==true)printf("数太大了，已经无法运算啦！如需更大，请修改源文件中的 MAX_FLOAT\n");
	printf("商      项数%3d    %s\n",resultList.length,resultList.length?toString(resultList):"无");
	printf("余项    项数%3d    %s\n",copyList.length,copyList.length?toString(copyList):"无");
	printf("按任意键返回\n");
	getchar();
	getch();
	my_exit:clearList(tempList);
	clearList(copyList);
	clearList(resultList);
	free(tempList.head);
	free(copyList.head);
	free(resultList.head);     //销毁三个临时链表 
	list1.op=list2.op=""; 
}

void exitList(list &list1,list &list2){  //退出程序，销毁链表 
	clearList(list1);
	clearList(list2);
	free(list1.head);
	free(list2.head);
	list1.head=NULL;
	list2.head=NULL;
	system("cls");
	printf("感谢使用!\n");
	getchar();
	getch(); 
	exit(0);
}

main(){
	list list1,list2;     //两个多项式 
	int num;
	const int funcNum=8;     //功能总数 
	MY_MENU="1.创建多项式\n2.销毁多项式\n3.多项式相加\n4.多项式相减\n5.多项式相乘\n6.多项式相除\n7.多项式求值\n8.退出\n";    //功能名 
	void (*function[funcNum])(list &,list &)={setList,destoryList,addList,minusList,multList,divList,calculateList,exitList};    //功能函数名 
	initList(list1);       //初始化 
	initList(list2);
	while(1){
		printMenu(list1,list2);   //打印菜单 
		again:scanf("%d",&num);    //输入功能序号 
		if(num<1||num>funcNum)INPUTERROR   //输入的序号有误 
		function[num-1](list1,list2);     //调用功能的函数 
	}
} 
