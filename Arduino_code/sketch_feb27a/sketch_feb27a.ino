#include<math.h>

int i, dest_x, dest_y, x, y;
float h_cost, g_cost, f_cost;
int node, count_2;
float min_dist = 999;
int count_1 = 0; // counter to store path
long int open_list[6]; // visited nodes
int parent_node, strt, dest;
int path[6];
int mat[6][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}}; // nodes mapped
int adj[6][6] = {{0, 1, 0, 1, 1, 0}, {1, 0, 1, 1, 1, 1}, {0, 1, 0, 0, 1, 1}, {1, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 0, 1}, {0, 1, 1, 0, 1, 0}}; // adjacency matrix

Serial.begin(9600);

int strt = a; // a is start point received from node MCU
int dest = b; //b is destination received from node MCU
int dest_x = mat[dest][0];
int dest_y = mat[dest][1];
parent_node = strt;

int main()
{
  while (parent_node != dest)
  {
    int count_2 = 0; // count to get next nodes for a parent node
    parent_x = mat[parent_node][0];
    parent_y = mat[parent_node][1];

    for (i = 0; i < 6; i++ )
    {
      if (adj[parent_node][i] == 1)
      {
        open_list[count_2] = i;
        count_2 + +;
      }
    }

    for (i = 0; i < count_2; i++)
    {
      node = open_list[i];
      if (node != dest)
      {
        x = mat[node][0];
        y = mat[node][1];

        /**** calculatation of  h_cost ****/

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
        break;
      }
    }
  }

  path[count_1] = parent_node;
  Serial.println(path);
  count_1++;

  for (i = 0; i <= count_2; i++)
  {
    open_list[i] = 0;
  }
}

