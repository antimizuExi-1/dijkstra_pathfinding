#include <stdlib.h>
#include "graph.h"

void create_graph(Graph *graph, int vertex_num, VertexData data[])
{
    graph->vertex_num = vertex_num;
    graph->edge_num = 0;
    // 初始化邻接表
    graph->adjacency_list = malloc(sizeof(VertexNode) * vertex_num);
    for (int i = 0; i < graph->vertex_num; i++)
    {
        graph->adjacency_list[i].data = data[i];
        graph->adjacency_list[i].first_edge = NULL;
    }
}

void add_node(Graph *graph, VertexData data)
{
    graph->vertex_num++;
    // 重新分配一个更大的邻接表
    graph->adjacency_list = realloc(graph->adjacency_list, sizeof(VertexNode) * graph->vertex_num);
    graph->adjacency_list[graph->vertex_num - 1].data = data;
    graph->adjacency_list[graph->vertex_num - 1].first_edge = NULL;
}

bool add_edge(Graph *graph, int vertex_index_1, int vertex_index_2)
{
    if (vertex_index_1 < 0 || vertex_index_2 < 0 ||
        vertex_index_1 >= graph->vertex_num || vertex_index_2 >= graph->vertex_num ||
        vertex_index_1 == vertex_index_2)
    {
        return false;
    }
    // 判断边是否存在
    EdgeNode *current = graph->adjacency_list[vertex_index_1].first_edge;
    while (current != NULL)
    {
        if (current->vertex_node_index == vertex_index_2)
        {
            // 已经存在这条边了，取消添加
            return false;
        }
        current = current->next_edge;
    }

    // 创建边
    EdgeNode *new_edge_node = malloc(sizeof(EdgeNode));
    new_edge_node->next_edge = NULL;
    // 边权重
    new_edge_node->weight = 0;
    // 这条边指向的顶点
    new_edge_node->vertex_node_index = vertex_index_2;

    // 如果是第一次添加，直接把first指向新创建的节点上
    if (graph->adjacency_list[vertex_index_1].first_edge == NULL)
    {
        graph->adjacency_list[vertex_index_1].first_edge = new_edge_node;
    } else
    {
        // 否则的话，添加到链表尾部
        EdgeNode *current_edge_node = graph->adjacency_list[vertex_index_1].first_edge;
        while (current_edge_node->next_edge != NULL)
        {
            current_edge_node = current_edge_node->next_edge;
        }
        current_edge_node->next_edge = new_edge_node;
    }
    graph->edge_num++;
    return true;
}

void delete_edge(Graph *graph, int vertex_index_1, int vertex_index_2)
{
    EdgeNode *prev = NULL;
    EdgeNode *current = graph->adjacency_list[vertex_index_1].first_edge;
    while (current != NULL)
    {
        if (current->vertex_node_index == vertex_index_2)
        {
            break;
        }
        prev = current;
        current = current->next_edge;
    }
    if (current == NULL || prev == NULL)
    {
        if (prev == NULL && current != NULL && current->vertex_node_index == vertex_index_2)
        {
            EdgeNode* temp = current->next_edge;
            free(graph->adjacency_list[vertex_index_1].first_edge);
            graph->adjacency_list[vertex_index_1].first_edge = NULL;
            graph->adjacency_list[vertex_index_1].first_edge = temp;
        }
        return;
    }
    graph->edge_num--;
    prev->next_edge = current->next_edge;
    free(current);
}

void destroy_graph(Graph *graph)
{
    for (int i = 0; i < graph->vertex_num; i++)
    {
        // 空链表，跳过
        if (graph->adjacency_list[i].first_edge == NULL)
        {
            continue;
        }
        // 释放每条边节点的链表
        EdgeNode *current = graph->adjacency_list[i].first_edge->next_edge;
        EdgeNode *prev = graph->adjacency_list[i].first_edge;
        while (current != NULL)
        {
            free(prev);
            prev = current;
            current = current->next_edge;
        }
        free(prev);
    }
    // 释放邻接表
    free(graph->adjacency_list);
}
