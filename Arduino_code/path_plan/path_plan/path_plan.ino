#include<math.h>

int i, x, y, parent_x, parent_y;
float h_cost, g_cost, f_cost;
int node, count_2 = 0;
float min_dist = 999;
int count_1 = 1; // counter to store path
long int open_list[6]; // visited nodes
int mat[6][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}}; // nodes mapped
int adj[6][6] = {{0, 1, 0, 1, 1, 0}, {1, 0, 1, 1, 1, 1}, {0, 1, 0, 0, 1, 1}, {1, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 0, 1}, {0, 1, 1, 0, 1, 0}}; // adjacency matrix
int strt = 0; // a is start point received from node MCU
int dest = 5; //b is destination received from node MCU
int dest_x = mat[dest][0];
int dest_y = mat[dest][1];
int parent_node = strt;
int path[6] = {strt, 0, 0, 0, 0, 0};

void setup()
{
  Serial.begin(9600);
}

int flag = 1;
int count_2_store;
int flag_flag = 1;

void loop()
{
  if (flag_flag == 1)
  {
    while (parent_node != dest)
    {
      parent_x = mat[parent_node][0];
      parent_y = mat[parent_node][1];

      for (i = 0; i < 6; i++ )
      {
        // count to get next nodes for a parent node

        if (adj[parent_node][i] == 1)
        {
          open_list[count_2] = i;

          //          Serial.println("a");
          //          Serial.println(open_list[count_2]);
          count_2_store = count_2;
          count_2++;

        }

      }


      for (i = 0; i < count_2; i++)
      {
        node = open_list[i];
        //        Serial.println(node);
        if (node != dest)
        {
          x = mat[node][0];
          y = mat[node][1];
          //          Serial.println(x);
          //          Serial.println(y);

          /**** calculatation of  h_cost ****/

          h_cost = sqrt((dest_x - x) * (dest_x - x) + (dest_y - y) * (dest_y - y));
          //          Serial.println(h_cost);

          /***** calculation of g_cost ****/
          g_cost = sqrt((parent_x - x) * (parent_x - x) + (parent_y - y) * (parent_y - y));
          //          Serial.println(g_cost);

          /**** calculation off_cost ****/
          f_cost = h_cost + g_cost;
          //          Serial.println(f_cost);

          if (f_cost < min_dist)
          {
            min_dist = f_cost;
            //            Serial.println(min_dist);
            parent_node = node;
            path[count_1] = parent_node;
            Serial.println(parent_node);
            count_1++;
          }
          //  Serial.println(path[count_1]);
        }
      }
//      Serial.println(parent_node);
    }
//    else
//    {
//      //Serial.println("break");
//    }

    //  path[count_1] = parent_node;
    //  Serial.print(path[0]);
    //  count_1++;
    //
    //  for (i = 0; i <= count_2; i++)
    //  {
    //    open_list[i] = 0;
    //  }
    flag_flag++;
  }
}
