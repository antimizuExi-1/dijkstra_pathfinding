#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>

#include "graph.h"
#define INF INT_MAX

const int screen_width = 1280;
const int screen_height = 760;

// 迪杰斯特拉求最短路径
bool dijkstra_shortest_path(Graph *graph, int start_vertex, int end_vertex, int rest[])
{
    int dist[graph->vertex_num];
    int parent[graph->vertex_num];
    bool final[graph->vertex_num];

    for (int i = 0; i < graph->vertex_num; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        final[i] = false;
    }
    dist[start_vertex] = 0;
    final[start_vertex] = true;

    for (int select = start_vertex;;)
    {
        // 当前选择点到达所有的邻接顶点
        if (graph->adjacency_list[select].first_edge != NULL)
        {
            EdgeNode *edge = graph->adjacency_list[select].first_edge;
            while (edge)
            {
                if (!final[edge->vertex_node_index] && dist[edge->vertex_node_index] > edge->weight + dist[select])
                {
                    // 更新当前节点连接的所有点
                    dist[edge->vertex_node_index] = edge->weight + dist[select];
                    parent[edge->vertex_node_index] = select;
                }
                edge = edge->next_edge;
            }
        }
        // 查询到目标顶点已经是最短路线，结束程序，返回
        if (select == end_vertex && final[end_vertex])
        {
            memcpy(rest, parent, sizeof(int) * graph->vertex_num);
            return true;
        }
        int min = INF;
        int id = -1;
        for (int i = 0; i < graph->vertex_num; i++)
        {
            if (!final[i] && dist[i] < min)
            {
                min = dist[i];
                id = i;
            }
        }
        // 没有可以到达的路线
        if (id == -1)
        {
            return false;
        }
        final[id] = true;
        select = id;
    }
}

void DrawArrow(Vector2 start, Vector2 end,
               float thickness,
               float headLength,
               float headAngleDeg,
               float backOffset,
               Color color)
{
    Vector2 dir = {end.x - start.x, end.y - start.y};
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len < 1e-3f) return;
    dir.x /= len;
    dir.y /= len;

    // 箭头真正的顶点位置：整体往回缩 backOffset
    Vector2 arrowTip = {
        end.x - dir.x * backOffset,
        end.y - dir.y * backOffset
    };

    // 主体线：缩到箭头根部
    Vector2 lineEnd = {
        arrowTip.x - dir.x * headLength,
        arrowTip.y - dir.y * headLength
    };
    DrawLineEx(start, lineEnd, thickness, color);

    float ang = headAngleDeg * (PI / 180.0f);
    float cosA = cosf(ang), sinA = sinf(ang);
    Vector2 side1 = {
        dir.x * cosA - dir.y * sinA,
        dir.x * sinA + dir.y * cosA
    };
    Vector2 side2 = {
        dir.x * cosA + dir.y * sinA,
        -dir.x * sinA + dir.y * cosA
    };

    Vector2 p1 = arrowTip;
    Vector2 p2 = {arrowTip.x - side1.x * headLength, arrowTip.y - side1.y * headLength};
    Vector2 p3 = {arrowTip.x - side2.x * headLength, arrowTip.y - side2.y * headLength};

    DrawTriangle(p1, p2, p3, color);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Graph Structure");

    VertexData DrawVertex[5] = {
        {{200, 200},BLUE},
        {{1200, 400},BLUE},
        {{600, 200},BLUE},
        {{700, 100},BLUE},
        {{800, 600},BLUE},
    };

    Graph graph;
    create_graph(&graph, 5, DrawVertex);

    add_edge(&graph, 1, 2);
    add_edge(&graph, 3, 2);
    add_edge(&graph, 3, 1);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 0, 3);
    add_edge(&graph, 0, 4);
    add_edge(&graph, 2, 4);

    int start_index = 0;
    int end_index = 3;
    int select_node = -1;

    int add_edge_start = -1;
    int delete_edge_start = -1;

    while (!WindowShouldClose())
    {
        Vector2 mos_pos = GetMousePosition();
        for (int i = 0; i < graph.vertex_num; i++)
        {
            if (CheckCollisionPointCircle(mos_pos, graph.adjacency_list[i].data.pos, 15))
            {
                graph.adjacency_list[i].data.color = SKYBLUE;
                select_node = i;
                break;
            }
            graph.adjacency_list[i].data.color = BLUE;
            select_node = -1;
        }
        // 设定起点和终点
        if (select_node != -1 && IsKeyPressed(KEY_S) && select_node != end_index)
        {
            start_index = select_node;
        }
        if (select_node != -1 && IsKeyPressed(KEY_E) && select_node != start_index)
        {
            end_index = select_node;
        }
        // 拖拽节点
        if (select_node != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            graph.adjacency_list[select_node].data.pos = mos_pos;
        }
        // 添加新节点
        if (select_node == -1 && IsKeyPressed(KEY_N))
        {
            add_node(&graph, (VertexData){mos_pos,BLUE});
        }
        // 添加边
        if (select_node != -1 && IsKeyPressed(KEY_K))
        {
            // 选择第一个点
            if (add_edge_start == -1)
            {
                add_edge_start = select_node;
            } else
            {
                // 选定第二点 创建边
                add_edge(&graph, add_edge_start, select_node);
                add_edge_start = -1;
            }
        }
        // 删除边
        if (select_node != -1 && IsKeyPressed(KEY_D))
        {
            // 选择第一个点
            if (delete_edge_start == -1)
            {
                delete_edge_start = select_node;
            } else
            {
                // 选定第二点删除
                delete_edge(&graph, delete_edge_start, select_node);
                delete_edge_start = -1;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleV(graph.adjacency_list[start_index].data.pos, 20, GREEN);
        DrawCircleV(graph.adjacency_list[end_index].data.pos, 20, RED);
        if (add_edge_start != -1)
        {
            DrawCircleV(graph.adjacency_list[add_edge_start].data.pos, 20, YELLOW);
        }

        if (delete_edge_start != -1)
        {
            DrawCircleV(graph.adjacency_list[delete_edge_start].data.pos, 20, GRAY);
        }

        for (int i = 0; i < graph.vertex_num; i++)
        {
            if (graph.adjacency_list[i].first_edge != NULL)
            {
                EdgeNode *current = graph.adjacency_list[i].first_edge;
                while (current != NULL)
                {
                    DrawArrow(
                        graph.adjacency_list[i].data.pos,
                        graph.adjacency_list[current->vertex_node_index].data.pos,
                        3.0f, 20.0f, 25.0f, 16.0f, WHITE);
                    current->weight = (int) Vector2Distance(graph.adjacency_list[i].data.pos,
                                                            graph.adjacency_list[current->vertex_node_index].data.pos);
                    Vector2 mid = Vector2Scale(
                        Vector2Add(graph.adjacency_list[i].data.pos,
                                   graph.adjacency_list[current->vertex_node_index].data.pos), 0.5f);
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%.0f", (float) current->weight);

                    DrawText(buf, (int) mid.x - 12, (int) mid.y - 8, 18, GREEN);
                    current = current->next_edge;
                }
            }
        }

        for (int i = 0; i < graph.vertex_num; i++)
        {
            DrawCircleV(graph.adjacency_list[i].data.pos, 15, graph.adjacency_list[i].data.color);

            char buf[32];
            snprintf(buf, sizeof(buf), "%d", i);
            DrawText(buf,
                     (int) graph.adjacency_list[i].data.pos.x,
                     (int) graph.adjacency_list[i].data.pos.y,
                     18, GREEN);
        }
        int temp[graph.vertex_num];
        if (dijkstra_shortest_path(&graph, start_index, end_index, temp))
        {
            for (int i = end_index;;)
            {
                if (temp[i] == -1) break;
                DrawLineEx(graph.adjacency_list[i].data.pos, graph.adjacency_list[temp[i]].data.pos, 3.0f, RED);
                i = temp[i];
            }
        }

        EndDrawing();
    }
    CloseWindow();
    destroy_graph(&graph);
    return 0;
}
