#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define timepell 1；//时间片
struct process
{
   char name[100];
   float arrivetime;//到达时间
   float servertime;//服务时间
   float servertime1;//服务时间
   float finishtime;//完成时间
   float roundtime;//周转时间
   float daiquantime;//带权周转时间
   int flag;//是否完成
}a[100];
struct stud{
    struct process p;
    struct stud *next;
};
struct Queue {
    struct stud *front;
    struct stud *rear;
};

void swap(struct process *a,struct process *b)
{
    struct process c=*b;
    *b=*a;
    *a=c;
    return ;
}
void sortarrivetime(struct process a[],int n)
{
    for(int i=n-1;i>0;i--)
    {
        for(int j=0;j<i;j++)
        {
            if(a[j].arrivetime>a[j+1].arrivetime)
            swap(&a[j],&a[j+1]);
        }
    }
}

void initQueue(struct Queue *q) {
    q->front = q->rear = NULL;
}
 void InsertQue(struct Queue *q, struct stud *node) {
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}
 struct stud* DeleteQue(struct Queue* q) {
    if (q->front == NULL) {
        return NULL;
    }
    struct stud *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}
int IsEmpty(struct Queue *q) {
    return q->front == NULL;
}
// 计算周转时间和带权周转时间
// 打印队列中的所有元素
void PrintQue(struct Queue *q) {
    struct stud *p = q->front;
    while (p != NULL) {
        printf("%s %f %f %f\n", p->p.name, p->p.arrivetime, p->p.servertime);
        p = p->next;
    }
}

struct stud* create_node( int id) {
    struct stud *new_node = (struct stud*)malloc(sizeof(struct stud));
    if (!new_node) {
        printf("Memory allocation error\n");
        exit(1);
    }
	 printf("%d 进程名：",id+1);
	 scanf("%s",&new_node->p.name);
	 printf("到达时间：");
	 scanf("%f" ,&new_node->p.arrivetime);
	 printf("服务时间：");
     scanf("%f" ,&new_node->p.servertime);
	 new_node->p.servertime1=new_node->p.servertime;//服务时间
    new_node->p.flag = 0; // 初始化 process 的 id 字段
    new_node->next = NULL;
    return new_node;
}
struct stud* create(int a) {
    if (a <= 0) {
        return NULL; // 如果 a 小于等于 0，返回 NULL
    }
    struct stud *head = create_node(0); // 创建头节点，id 为 0
    struct stud *current = head;
    // 创建剩余的节点并链接起来
    for (int i = 1; i < a; i++) {
        current->next = create_node(i);
        current = current->next;
    }
    return head;
}
// 找到运行时间最短的进程的索引
void findShortestJob(struct process a[], int n, int *minIndex,float time) {
	float minBurst=99999.0;
    for (int i = 0; i < n; i++) {
        if (a[i].flag==0&&a[i].servertime< minBurst&&a[i].arrivetime<=time) {
            minBurst =a[i].servertime;
            *minIndex = i;
        }
    }
}

void FCFS(struct process a[],int n,float *t1,float *t2)
{
    a[0].finishtime=a[0].arrivetime+a[0].servertime;
    a[0].roundtime=a[0].finishtime-a[0].arrivetime;
    a[0].daiquantime=a[0].roundtime/a[0].servertime;
		for(int i=1;i<n;i++)
        {
			if(a[i].arrivetime<a[i-1].finishtime){
				a[i].finishtime=a[i-1].finishtime+a[i].servertime;
             a[i].roundtime=a[i].finishtime-a[i].arrivetime;
            a[i].daiquantime=a[i].roundtime/a[i].servertime;
			}
			else{
				a[i].finishtime=a[i].arrivetime+a[i].servertime;	
				a[i].roundtime=a[i].finishtime-a[i].arrivetime;
               a[i].daiquantime=a[i].roundtime/a[i].servertime;
			}
        }

		for(int i=0;i<n;i++)
		{
			printf("进程名：%s ",a[i].name);
			printf("到达时间:%.2f",a[i].arrivetime);
			printf("服务时间 %.2f",a[i].servertime);
			printf("完成时间 %.2f",a[i].finishtime);
			printf("周转时间 %.2f",a[i].roundtime);
			printf("带权周转时间 %.2f\n",a[i].daiquantime);
			*t1=*t1+a[i].roundtime;
			*t2=*t2+a[i].daiquantime;
		}
}

void SPN(struct process a[],int n,float *t1,float *t2)
{
    float currentTime = 0.0;  // 当前时间
    for (int i = 0; i < n; i++) {
        int minIndex;

        findShortestJob(a, n, &minIndex,currentTime);

		a[minIndex].finishtime= currentTime+a[minIndex].servertime;
		a[minIndex].roundtime=a[minIndex].finishtime-a[minIndex].arrivetime;
		a[minIndex].daiquantime=a[minIndex].roundtime/a[minIndex].servertime;
        
        currentTime += a[minIndex].servertime;// 更新当前时间
        a[minIndex].flag= 1;//标记已完成进程
		}
		for(int i=0;i<n;i++)
		{
			printf("进程名：%s ",a[i].name);
			printf("到达时间:%.2f",a[i].arrivetime);
			printf("服务时间 %.2f",a[i].servertime);
			printf("完成时间 %.2f",a[i].finishtime);
			printf("周转时间 %.2f",a[i].roundtime);
			printf("带权周转时间 %.2f\n",a[i].daiquantime);
			*t1=*t1+a[i].roundtime;
			*t2=*t2+a[i].daiquantime;
		}
}
void RR(struct stud *head,int n)
{
    struct stud  *t, *r, *p;
	struct Queue q;
	 initQueue(&q);
	float slice =0.0f;
	printf("请输入时间片大小：");
	scanf("%f",&slice);
     float current_time = 0.0f;
    int count = 0;
    t =head;
	head=head->next;
	t->next=NULL;
	InsertQue(&q,t);
    while (count < n &&!IsEmpty(&q) ) {
             struct stud *a = DeleteQue(&q); // 从队列中取出当前进程
			 if (a->p.arrivetime > current_time) {
            current_time = a->p.arrivetime; // 如果到达时间大于当前时间，则更新当前时间
        }
		   if(head!=NULL)
			if(head->p.arrivetime<=current_time)
			{
				t =head;
	            head=head->next;
				t->next=NULL;
	            InsertQue(&q,t);
			}

        float executed = slice;
        if (executed > a->p.servertime) {
            executed = a->p.servertime; // 如果时间片大于剩余服务时间，则只执行剩余服务时间
            a->p.flag = 1; // 标记为已完成

        } else {
            a->p.servertime -= executed; // 减去已执行的时间
            a->p.flag = 0; // 标记为未完成
         }  
        current_time += executed; 
		       // 更新当前时间
		  if(a->p.servertime!=0) 
		    if(head!=NULL)
			if(head->p.arrivetime<=current_time)
			{
				t =head;
	            head=head->next;
				 t->next=NULL;
	            InsertQue(&q,t);	
	    	}

			if(a->p.flag==0) InsertQue(&q,a);	

        if (a->p.flag) { // 如果进程已完成
            a->p.finishtime = current_time;
            a->p.roundtime = a->p.finishtime - a->p.arrivetime;
            a->p.daiquantime = a->p.roundtime / a->p.servertime1; // 计算带权周转时间
            // 这里可以打印或存储进程信息
            printf("进程 %s 完成，完成时间: %.2f, 周转时间: %.2f, 带权周转时间: %.2f\n",
                   a->p.name, a->p.finishtime, a->p.roundtime, a->p.daiquantime);
            count++; // 增加已完成进程计数
            //free(a);
		}	   
}
		
}
int main()
{
	float t1 = 0.0f;
	float t2 = 0.0f;
	float avr_t1 = 0.0f;
	float avr_t2 = 0.0f;
	
	int n,i;
	char select = ' ';
	while (select != 'e' && select != 'E')
	{
		printf("\n....................................\n");
		printf("请选择算法 ：\na.先来先服务算法\nb.短作业优先算法\nc.时间片轮转算法\ne.退出程序\n输入选择");
		scanf("%c", &select);
		if (select == 'a' || select =='A') 
		{
			printf("\n\n**************先来先服务算法************\n\n");
			printf("请输入进程数 ：");
			scanf("%d", &n);
			for(i=0;i<n;i++)
			{
				printf("%d 进程名：",i+1);
				scanf("%s",&a[i].name);
				printf("到达时间：");
				scanf("%f" ,&a[i].arrivetime);
				printf("服务时间：");
                scanf("%f" ,&a[i].servertime);
			}
			
			sortarrivetime(a,n);
			FCFS(a,n,&t1,&t2);
			avr_t1 =t1/n;
			avr_t2 =t2/n;
			printf("\n");
			printf("平均周转时间为：%.2f \n", avr_t1);
			printf("平均带权周转时间为：%.2f \n", avr_t2);
		}

		else if (select == 'b' || select =='B') 
		{
			printf("\n\n**************短作业优先算法************\n\n");
			printf("请输入进程数 ：");
			scanf("%d", &n);
			for(i=0;i<n;i++)
			{
				printf("%d 进程名：",i+1);
				scanf("%s",&a[i].name);
				printf("到达时间：");
				scanf("%f" ,&a[i].arrivetime);
				printf("服务时间：");
                scanf("%f" ,&a[i].servertime);
				a[i].flag = 0;
			}
			
			sortarrivetime(a,n);
			SPN(a,n,&t1,&t2);
			avr_t1 =t1/n;
			avr_t2 =t2/n;
			printf("\n");
			printf("平均周转时间为：%.2f \n", avr_t1);
			printf("平均带权周转时间为：%.2f \n", avr_t2);
		}
		
		else if (select == 'c' || select =='C') 
		{
			printf("\n\n**************时间片轮转算法************\n\n");
			int a;
			printf("请输入进程数 ：");
			scanf("%d", &a);
			struct stud *head;
			head =create(a);
			RR(head,a);
	 } 
	 getchar();
	 //else if(select == 'e' || select =='E')  break;
	 t1=0;
	 t2=0;
    } 
 return 0;
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
