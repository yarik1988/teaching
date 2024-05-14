#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

bool is_prime(int n)
{
bool isPrime = true;
for(int i = 2; i <= sqrt(n); ++i)
      if(n % i == 0)
      {
          isPrime = false;
          break;
      }
return isPrime;
}

int* get_first_m_primes(int m)
{
int* res=new int[m];
int cnt=0;
int i=2;
while (cnt<m)
{
   if (is_prime(i))
        {
        res[cnt]=i;
        cnt++;
        }
i++;
}
return res;
}


void gen_ham_nmb(int N,int m)
{
int* res=get_first_m_primes(m);
auto divp = new queue<int>[m];
for (int i=0;i<m;i++)
    divp[i].push(res[i]);

for (int k=0;k<N;k++)
    {
    int min_elem=divp[0].front();
    int q_pos=0;
    for (int i=0;i<m;i++)
        if (divp[i].front()<min_elem)
            {
            min_elem=divp[i].front();
            q_pos=i;
            }
    for (int i=q_pos;i<m;i++)
        divp[i].push(min_elem*res[i]);
    divp[q_pos].pop();
    cout<<min_elem<<" ";
    }
delete[] res;
}


int main()
{
    gen_ham_nmb(100,3);
    return 0;
}
