#include<math.h>

void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  int i, dest_x, dest_y, x, y, parent_x, parent_y;
  float h_cost, g_cost, f_cost;
  int node;
  float min_dist = 99999;
  int count_1 = 0; // counter to store path
  int open_list[6]; // visited nodes
  int parent_node, strt, dest; // strt is starting node, dest is destination node
  int path[6]; // array to store shortest path
  int mat[6][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}}; //  mapped nodes
  int adj[6][6] = {{0, 1, 0, 1, 1, 0}, {1, 0, 1, 1, 1, 1}, {0, 1, 0, 0, 1, 1}, {1, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 0, 1}, {0, 1, 1, 0, 1, 0}}; // adjacency matrix

  strt = 2;
  dest = 3;
  dest_x = mat[dest][0];
  dest_y = mat[dest][1];
  parent_node = strt;
  path[count_1] = {strt};
  count_1++;

  while (parent_node != dest)
  {
    int count_2 = 0; // counter to get next nodes for given parent node
    for (i = 0; i < 6; i++ )
    {
      if (adj[parent_node][i] == 1)
      {
        open_list[count_2] = i;
        count_2 ++;
      }
    }

    for (i = 0; i < count_2; i++)
    {
      node = open_list[i];
      if (node != dest)
      {
        parent_x = mat[parent_node][0];
        parent_y = mat[parent_node][1];

        x = mat[node][0];
        y = mat[node][1];

        /**** calculate h_cost ****/
        h_cost = sqrt((dest_x - x) * (dest_x - x) + (dest_y - y) * (dest_y - y));

        /***** calculation of g_cost ****/
        g_cost = sqrt((parent_x - x) * (parent_x - x) + (parent_y - y) * (parent_y - y));

        /**** calculation off_cost ****/
        f_cost = h_cost + g_cost;

        if (f_cost < min_dist)
        {
          min_dist = f_cost;
          parent_node = node;
        }
      }
      else
      {
        parent_node = node;
        path[count_1] = node;
        count_1++;

        for (i = 0; i < count_1; i++)
        {
          Serial.println(path[i]);
        }
        break;
      }
    }

    path[count_1] = parent_node;
    count_1++;

    for (i = 0; i < count_1; i++)
    {
      open_list[i] = 0;
    }
  }
}

void loop() {
}

