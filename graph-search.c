#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

typedef struct node{                    // 노드 구조체
    int v;
    struct node *next;
}node;

typedef struct head{                    // 각 노드의 헤드 노드 구조체
    node *h; 
}head;

typedef struct graph{                   // 그래프 구조체
    head *list;
}graph;

#define MAX_QUEUE_SIZE 30

int queue[MAX_QUEUE_SIZE];
int rear = 0;
int front = 0;

int is_empty(){                     // 큐가 비었는지 확인
    if(rear == front) return 1;
    else return 0;
}

int is_full(){                      // 큐가 꽉 찼는지 확인
    if(front == MAX_QUEUE_SIZE) return 1;
    else return 0;
}

int dequeue() {                     // dequeue 함수
    if (front == rear) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    return item;
}

void enqueue(int item) {            // enqueue 함수
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        printf("Queue is full\n");
        return;
    }
    queue[rear] = item;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
}

#define MAX_STACK_SIZE 30

int stack[MAX_STACK_SIZE];
int top = -1;

int is_empty_stack(){               // 스택이 비었는지 확인
    if(top == -1) return 1;
    else return 0;
}

int is_full_stack(){                // 스택이 꽉 찼는지 확인
    if(top == MAX_STACK_SIZE) return 1;
    else return 0;
}

void push(int item){                // push 함수
    if(is_full_stack() == 1){
        printf("Stack is full\n");
        return;
    }
    stack[++top] = item;
}

int pop(){                          // pop 함수
    if(is_empty_stack() == 1){
        printf("Stack is empty\n");
        return -1;
    }
    return stack[top--];
}

graph* init_graph(graph *);                 // graph initalize 하는 함수
void insert_vertex(graph * , int);          // vertex를 생성하는 함수
void insert_edge(graph*, int, int);         // edge를 생성하는 함수
void dfs(graph*, int);                      // dfs 함수
void bfs(graph*,int);                       // bfs 함수
void print_graph(graph *);                  // graph를 출력하는 함수
void dfs_recursive(graph*, int, int*);      // dfs를 재귀적으로 호출하는 함수

int main(){                                 // main 함수는 이전 과제를 참고했습니다.
    char command;
    graph *gra = NULL;
    int v1, v2;
    int start;
    do{
        printf("-------------------- [이한결]  [2021041055] --------------------\n");
        printf("                         Graph Searches                       \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph     = z\n");
        printf(" Insert Vertex        = v          Insert Edge              = e \n");
        printf(" Depth First Search   = d          Breath First Search      = b \n");
        printf(" Print Graph          = p          Quit                     = q \n");
        printf("----------------------------------------------------------------\n");
        printf("Command = ");
	    scanf(" %c", &command);
        switch(command){
        case 'z': case 'Z':
            gra = init_graph(gra);
            break;
        case 'v': case 'V':
            printf("Insert number of Vertex: ");
            scanf("%d", &v1);
            insert_vertex(gra, v1);
            break;
        case 'e': case 'E':
            printf("Insert two number of Vertex: ");
            scanf("%d %d", &v1, &v2);
            insert_edge(gra, v1, v2);
            break;
        case 'd': case 'D':
            printf("Insert starting number: ");
            scanf("%d", &start);
            dfs(gra, start);
            printf("\n");
            for(int i = 0 ; i < MAX_STACK_SIZE ; i++) stack[i] = 0;
            top = -1;
            break;
        case 'b': case 'B':
            printf("Insert starting number: ");
            scanf("%d", &start);
            bfs(gra, start);
            printf("\n");
            for(int i = 0 ; i < MAX_QUEUE_SIZE ; i++) queue[i] = 0;
            front = 0;
            rear = 0;
            break;
        case 'p': case 'P':
            print_graph(gra);
            break;
        case 'q': case 'Q':
            break;
        default:
	    	printf("\n       >>>>>   Concentration!!   <<<<<     \n");
	    	break;
        }
    }while(command != 'q' && command != 'Q');
}

graph* init_graph(graph* g){
    if(g != NULL){                              // graph가 이미 있으면 free를 통해 메모리 해제
        for(int i = 0; i < MAX_VERTEX; i++){    
            free(g->list[i].h);                 // vertex에 연결된 edge들을 free
        }
        free(g->list);                    // vertex들을 free    
        free(g);                        // graph를 free
    }
    g = (graph*)malloc(sizeof(graph));      // graph를 생성
    g->list = (head*)malloc(sizeof(head)*MAX_VERTEX);       // vertex를 생성
    for(int i = 0 ; i < MAX_VERTEX ; i++){      
        g->list[i].h = NULL;                    // vertex들을 NULL로 초기화
    }
    return g;
}

void insert_vertex(graph* g, int n){
    if (n < 0 || n > 9) {
        printf("Wrong Number!\n");              // vertex의 범위가 0~9가 아니면 오류 출력
        return;
    }
    g->list[n].h = (node*)malloc(sizeof(node));      // vertex를 생성
    g->list[n].h->v = n;                         // vertex에 번호를 저장        
    g->list[n].h->next = NULL;                  // vertex에 연결된 edge를 NULL로 초기화
}

void insert_edge(graph* g, int v1, int v2){ 
    if(v1 == v2) return;
    node* point = g->list[v1].h;                 // vertex에 연결된 edge를 저장
    node* pre = NULL;                            // point의 이전을 저장
    node* new1 = (node*)malloc(sizeof(node));    // edge를 생성
    new1->v = v2;                              
    new1->next = NULL;                          // edge를 초기화
    node* new2 = (node*)malloc(sizeof(node));   // edge를 생성
    new2->v = v1;   
    new2->next = NULL;                          // edge를 초기화
    int i = 0;                                  // 첫번째 edge를 연결하기 위한 변수
    while(1){
        i++;
        if(point->next == NULL){                // vertex에 연결된 edge가 없으면 연결
            point->next = new1;
            break;
        }
        else if(point->v > v2 && i != 1){       // 크기가 작은 순으로 연결하기 위한 조건문 (head는 제외)
            pre->next = new1;
            new1->next = point;
            break;
        }
        else if(point->v == v2) break;          // 이미 연결된 edge가 있으면 break
        pre = point;
        point = point->next;
    }
    point = g->list[v2].h;
    pre =  NULL;
    i = 0;
    while(1){
        i++;
        if(point->next == NULL){                // vertex에 연결된 edge가 없으면 연결
            point->v = v1;
            point->next = new2;
            break;
        }
        else if(point->v > v1 && i != 1){       // 크기가 작은 순으로 연결하기 위한 조건문 (head는 제외)
            pre->next = new2;                   
            new2->next = point;
            break;
        }
        else if(point->v == v1) break;          // 이미 연결된 edge가 있으면 break
        pre = point;                            
        point = point->next;
    }
}

void dfs(graph* g, int start){
    int check[MAX_VERTEX] = {0,};        // 방문했는지 확인하기 위한 check 배열 생성
    dfs_recursive(g, start, check);      // 재귀 함수 호출
}

void dfs_recursive(graph* g, int start, int* check){
    node *point = g->list[start].h;      // 시작점을 point에 저장
    printf("[%d] ", start);              // 시작점 출력
    check[start] = 1;                     // 시작점을 방문했다고 표시
    while(point != NULL){                 // point가 NULL이 아닐때까지 반복
        if(check[point->v] == 0) {
            dfs_recursive(g, point->v, check); // point의 vertex를 방문하지 않았다면 재귀 호출
        }
        point = point->next;             // point를 다음으로 이동
    }
}

void bfs(graph *g , int start){
    int check[MAX_VERTEX] = {0,};       // 방문했는지 확인하기 위한 check 배열 생성
    node *point = g->list[start].h;     // 시작점을 point에 저장
    enqueue(start);                     // 시작점을 큐에 넣음
    check[start] = 1;                   // 시작점을 방문했다고 표시
    while(is_empty() != 1){
        start = dequeue();              // 큐에서 하나를 빼서 시작점으로 설정
        point = g->list[start].h;       // 시작점을 point에 저장
        printf("[%d] ", start);         // 시작점 출력
        while(point!=NULL){             // point가 NULL이 아닐때까지 반복
            if(check[point->v] == 0) {
                enqueue(point->v);      // point의 vertex를 큐에 넣음
                check[point->v] = 1;    // point의 vertex를 방문했다고 표시
            }
            point = point->next;        // point를 다음으로 이동
        }
    }
}

void print_graph(graph *g){
    for(int i = 0 ; i < MAX_VERTEX ; i++){      // vertex에 연결된 edge들을 출력
        if(g->list[i].h == NULL) continue;      // vertex가 없으면 다음 vertex로 넘어감
        printf("vertex edge with %d vertex: ", i);       // vertex 출력
        node* point = g->list[i].h;                      // point에 vertex의 head를 저장
        if(point->next == NULL) printf("None\n");       // vertex에 연결된 edge가 없으면 None 출력
        else{
            point= point->next;                 // point를 다음으로 이동
            while(point != NULL){               // point가 NULL이 아닐때까지 반복    
                printf("%d ", point->v);        // point의 vertex 출력
                point=point->next;              // point를 다음으로 이동
            }
        }
        printf("\n");
    }
}