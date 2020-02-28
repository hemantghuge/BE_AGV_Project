


int angle_list[5] = {1, 1, 1, 1, 10};
int n = sizeof(angle_list) / sizeof(angle_list[0]);

int large;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  Mode(angle_list , n);
}

void loop() {
  // put your main code here, to run repeatedly:



}

void Mode(int [], int )
{

  large = Max(angle_list , n);
  Serial.println(large);

  int len = large + 1;
  int count[len];
  for (int i = 0; i < len; i++)
    count[i] = 0;

  for (int i = 0; i < n; i++)
    count[angle_list[i]]++;

  int mode = 0;
  int k = count[0];
  for (int i = 1; i < len; i++)
  {
    if (count[i] > k)
    {
      k = count[i];
      mode=i;
    }
  }

  Serial.println("MODE");
  Serial.println(mode);

}


int Max(int arr[], int n)
{
  Serial.println("In Max Function");
  int i;

  int big = arr[0];

  for (i = 1; i < n; i++)
    if (arr[i] > big)
      big = arr[i];

  return big;
}
