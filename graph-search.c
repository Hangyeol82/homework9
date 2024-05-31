#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

typedef struct Graph{
    int v[MAX_VERTEX];                      // 생성된 vertex를 배열에서 1로 표현하려고 한다.
    int edge[MAX_VERTEX][MAX_VERTEX];       // edge를 2차원 배열에서 1로 표현하려고 한다
} Graph;

Graph* init_graph(Graph *);                 // graph initalize 하는 함수
void insert_vertex(Graph * , int);          // vertex를 생성하는 함수
void insert_edge(Graph*, int, int);         // edge를 생성하는 함수
void dfs(Graph*, int);
void bfs();
void print_graph();
void quit_graph();

int check[MAX_VERTEX] = {0,};               // dfs, bfs를 사용할때 vertex를 방문했는지 확인하기 위한 check 배열 생성

int main(){                                 // main 함수는 이전 과제를 참고했습니다.
    char command;
    Graph *gra = NULL;
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
            for(int i = 0 ; i < MAX_VERTEX ; i++){
                check[i] = 0;                           // check 배열 0으로 초기화
            }
            printf("\n");
        case 'b': case 'B':
        case 'p': case 'P':
        case 'q': case 'Q':
            break;
        default:
	    	printf("\n       >>>>>   Concentration!!   <<<<<     \n");
	    	break;
        }
    }while(command != 'q' && command != 'Q');
}

Graph* init_graph(Graph* graph){
    if(graph != NULL){
        free(graph);                            // graph가 NULL 이 아니면 할당을 해제한다.
    }
    graph = (Graph*)malloc(sizeof(Graph));      // graph를 동적으로 할당한다.
    for(int i = 0 ; i < MAX_VERTEX ; i++){
        graph->v[i]=0;                          // v 배열을 다 0으로 초기화한다.
    }
    for(int i = 0 ; i < MAX_VERTEX ; i++){
        for(int j = 0 ; j < MAX_VERTEX ; j++){
            graph->edge[i][j] = 0;              // edge 배열도 다 0으로 초기화한다.
        }
    }
    return graph;
}

void insert_vertex(Graph* g, int n){
    g->v[n-1] = 1;                          // n에 해당하는 배열 v 인덱스를 1로 바꿔준다.
}

void insert_edge(Graph* g, int v1, int v2){
    g->edge[v1-1][v2-1] = 1;                
    g->edge[v2-1][v1-1] = 1;                // 무방향 그래프이므로 양쪽으로 이어준다.
}

void dfs(Graph* g, int start){
    check[start-1] = 1;                     // 방문한 vertex 기록 
    printf("[%d] ", start);                 // 방문한 vertex 출력
    for(int i = 0 ; i < MAX_VERTEX; i++){
        if(g->edge[start-1][i] == 1 && check[i] != 1){      // edge가 연결되어 있고 방문하지 않은 vertex 일때
            dfs(g, i+1);                    // recursive 방식으로 dfs 구현
        }
    }
}
