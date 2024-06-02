#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

typedef struct node{
    int v;
    struct node *next;
}node;

typedef struct head{
    node *h; 
}head;

typedef struct graph{
    head *list;
}graph;

#define MAX_QUEUE_SIZE 30

int queue[MAX_QUEUE_SIZE];
int rear = 0;
int front = 0;

int is_empty(){
    if(rear == front) return 1;
    else return 0;
}

int is_full(){
    if(front == MAX_QUEUE_SIZE) return 1;
    else return 0;
}

int dequeue() {
    if (front == rear) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    return item;
}

void enqueue(int item) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        printf("Queue is full\n");
        return;
    }
    queue[rear] = item;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
}

graph* init_graph(graph *);                 // graph initalize 하는 함수
void insert_vertex(graph * , int);          // vertex를 생성하는 함수
void insert_edge(graph*, int, int);         // edge를 생성하는 함수
void dfs(graph*, int);
void bfs(graph*,int);
void print_graph(graph *);
void quit_graph();

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
            for(int i = 0 ; i < MAX_QUEUE_SIZE ; i++) queue[i] = 0;
            front = 0;
            rear = 0;
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
    if(g != NULL){
        for(int i = 0; i < MAX_VERTEX; i++){
            free(g->list[i].h);
        }
        free(g->list);
        free(g);
    }
    g = (graph*)malloc(sizeof(graph));
    g->list = (head*)malloc(sizeof(head)*MAX_VERTEX);
    for(int i = 0 ; i < MAX_VERTEX ; i++){
        g->list[i].h = NULL;
    }
    return g;
}

void insert_vertex(graph* g, int n){
    if (n < 0 || n > 9) {
        printf("Wrong Number!\n");
        return;
    }
    g->list[n].h = (node*)malloc(sizeof(node));
    g->list[n].h->v = n;
    g->list[n].h->next = NULL;
}

void insert_edge(graph* g, int v1, int v2){ 
    if(v1 == v2) return;
    node* point = g->list[v1].h;
    node* pre = NULL;
    node* new1 = (node*)malloc(sizeof(node));
    new1->v = v2;
    new1->next = NULL;
    node* new2 = (node*)malloc(sizeof(node));
    new2->v = v1;
    new2->next = NULL;
    int i = 0;
    while(1){
        i++;
        if(point->next == NULL){
            point->next = new1;
            break;
        }
        else if(point->v > v2 && i != 1){         // 크기가 작은 순으로 연결하기 위해
            pre->next = new1;
            new1->next = point;
            break;
        }
        else if(point->v == v2) break;
        pre = point;
        point = point->next;
    }
    point = g->list[v2].h;
    pre =  NULL;
    i = 0;
    while(1){
        i++;
        if(point->next == NULL){        
            point->v = v1;
            point->next = new2;
            break;
        }
        else if(point->v > v1 && i != 1){         // 크기가 작은 순으로 연결하기 위해
            pre->next = new2;
            new2->next = point;
            break;
        }
        else if(point->v == v1) break;
        pre = point;
        point = point->next;
    }
}

void dfs(graph* g, int start){
    int check[MAX_VERTEX] = {0,};        // 방문했는지 확인하기 위한 check 배열 생성
    enqueue(start);
    int current;
    while(is_empty() != 1){
        current = dequeue();
        if(check[current] == 1) continue;
        check[current] = 1;
        printf("[%d] ", current);
        node* point = g->list[current].h;
        while(point != NULL){
            if(check[point->v] == 0) {
                enqueue(point->v);
                break;
            }
            point = point->next;
        }
    }
}

void bfs(graph *g , int start){
    int check[MAX_VERTEX] = {0,};
    node *point = g->list[start].h;
    enqueue(start);
    check[start] = 1;
    while(is_empty() != 1){
        start = dequeue();
        point = g->list[start].h;
        printf("[%d] ", start);
        while(point!=NULL){
            if(check[point->v] == 0) {
                enqueue(point->v);
                check[point->v] = 1;
            }
            point = point->next;
        }
    }
}

void print_graph(graph *g){
    for(int i = 0 ; i < MAX_VERTEX ; i++){
        if(g->list[i].h == NULL) continue;
        printf("vertex edge with %d vertex: ", i);
        node* point = g->list[i].h;
        if(point->next == NULL) printf("None\n");
        else{
            point= point->next;
            while(point != NULL){
                printf("%d ", point->v);
                point=point->next;
            }
        }
        printf("\n");
    }
}