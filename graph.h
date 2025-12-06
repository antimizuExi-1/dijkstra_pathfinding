#ifndef GRAPH_H
#define GRAPH_H
#include <stdbool.h>
#include "raylib.h"

typedef struct
{
    Vector2 pos;
    Color color;
} VertexData;

typedef struct EdgeNode // 边节点
{
    int weight; // 边权值
    int vertex_node_index; // 这条边所指向的顶点
    struct EdgeNode *next_edge; // 下一条边
} EdgeNode;

typedef struct VertexNode // 顶点
{
    VertexData data;
    EdgeNode *first_edge; // 指向第一条从这个顶点出发的边
} VertexNode;

typedef struct Graph
{
    int vertex_num; // 顶点数
    int edge_num; // 边数
    VertexNode *adjacency_list; // 邻接表
} Graph;

// 创建图
void create_graph(Graph *graph, int vertex_num, VertexData data[]);

// 添加节点
void add_node(Graph *graph, VertexData data);

// 通过顶点索引添加边
// 如果添加的顶点索引超过顶点数或者输入负数 添加失败 返回false
bool add_edge(Graph *graph, int vertex_index_1, int vertex_index_2);

// 删除边
void delete_edge(Graph *graph, int vertex_index_1, int vertex_index_2);

// 销毁图
void destroy_graph(Graph *graph);

#endif //GRAPH_H
