//this code writes spike time
#include<bits/stdc++.h>
#include<ctime>
#include<random>
using namespace std;



//const int n=40;    //number of neurons
float S=1.00;
float L=1.00;
float h=0.002;    //time steps
float T=1000.00; //updated should be 10000
float thresh=0.80;         // value above which the particle jumps assuming the particle will jump
float excite=0.01;     //excitation in each particle due to one particle crossing threshold value
float error =0.001;
//const int K=13;
int arr[100][100];        //mapping array


void iterate(float pos[][2],float pos_ini[][2],int times, ofstream &spike,vector<vector<float> > &spike_time,int n);
int is_sync(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,ofstream &spike,vector<vector<float> > &spike_time,int n);
void map_neuron(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,int n);
void run(float pos[][2],float pos_ini[][2],ofstream &myfile,ofstream &spike,vector<vector<float> > &spike_time,int n);
float time(float v,float v_o,int n);
float dydx(float v,int n);
void rungekutta(int i,float p[][2],int n);







void rungekutta(int i,float pos[][2],int n)
{
  float k1,k2,k3,k4,k5;
  float v=pos[i][0];
  k1=h*(S-L*v);
  k2=h*(S-L*(v+0.5*k1));
  k3=h*(S-L*(v+0.5*k2));
  k4=h*(S-L*(v+k3));
  pos[i][0]+=(1.0/6.0)*(k1+2*k2+2*k3+k4);
  pos[i][1]+=h;

}

void iterate(float pos[][2],float pos_ini[][2], int times, ofstream &spike,vector<vector<float> > &spike_time,int n)
{


	int L[n+1]={0};
	for(int i=0;i<n;i++)
	{

    rungekutta(i,pos,n);

	}
  for(int i=0;i<n;i++)
  {
    if(pos[i][0]>=thresh)
      L[i]=1;
  }
  for(int i=0;i<n;i++)
  {
    int cnt=0;
    for(int j=0;j<n;j++)
    {
      if(i==j)
      continue;
      else
      {
         if(L[j]==1 && arr[i][j]==0)
         {

           cnt++;

         }
      }
    }
      if((pos[i][0]+cnt*excite)>=thresh )
        {
          pos[i][0]=0.000;
        //  if(times<=10)
          {
            spike_time[i].push_back(pos[i][1]);
          }

        }
        else
        {
          pos[i][0]+=cnt*excite ;

        }
    }

}


int is_sync(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,ofstream &spike,vector<vector<float> > &spike_time,int n)
{

	run(pos,pos_ini,myfile,spike,spike_time,n);
  	int count=0;
	  for(int i=1;i<n;i++)
	  {
	    if(fabs(pos[i][0]-pos[0][0])>error)
	    {
	        count++;
	    }
	  }

	    if(count==0)
	    {
	      return 1;

	    }

	    else
	    return 0;

}



void map_neuron(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,int n)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0,(n-1));
 for(int i=0;i<n;i++)
 {
 	for(int j=0;j<n;j++)
 		arr[i][j]=0;
 }
  for(int i=0;i<num;i++)
  {

   while(1)
    {


      int v1= dis(gen);
      int v2= dis(gen);
      if(arr[v1][v2]==1)
        continue;
      if(v1!=v2)
      {
        arr[v1][v2]=1;
       arr[v2][v1]=1; //condition for second case
        break;
      }

    }

  }
  for(int i=0;i<n;i++)
  {
    for(int j=0;j<n;j++)
    {
      char s=arr[i][j]+'0';
      myfile<<s<<" ";
    }
    myfile<<"\n";
  }
  myfile<<"\n";
}

void run(float pos[][2],float pos_ini[][2],ofstream  &myfile,ofstream &spike,vector<vector<float> > &spike_time,int n)
{
  int count=0;
  int times=500000;//(int)T/h;
  int i=0;
  while(times--)
  {
    iterate(pos,pos_ini,times,spike,spike_time,n); //mod
  }
}
void generate_rand(float pos[][2],float pos_ini[][2],int n)
{

	for(int i=0;i<n;i++)
  {
    pos[i][0]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/thresh));
    pos[i][1]= 0.0;
    pos_ini[i][0]=pos[i][0];
    pos_ini[i][1]=0.0;
  }
}



int main()
{
    int n;
  	float pos[101][2];
    float pos_ini[101][2];
    ofstream myfile,spike;

    myfile.open("map1.txt");

    spike.open("spike_time1.txt");
    int seed;
    cin>>seed;
    srand(seed);

  for(n=10;n<=50;n+=5)
  {
    int N1;           // no of graphs generated
    int N2;
  myfile<<"Number of neurons: "<<to_string(n)<<"\n";
  spike<<"Number of neurons: "<<to_string(n)<<"\n";
    int k=0;
    int incre= (n*(n-1))/40;
    int incre1= 90/40;

for(int i1=0,N1=10;2*i1<=(n*(n-1)),N1<=100;i1+=incre,N1+=incre1)
  {

    float p=0.0;
     int sum=0;
     myfile<<"Number of synapses broken: "<<to_string(2*i1)<<"\n";
    spike<<"Number of synapses broken: "<<to_string(2*i1)<<"\n";
    for(int i=0;i<N1;i++)
      {
        map_neuron(pos,pos_ini,i1,myfile,n); //

        for(int j=0;j<N2;j++)
        {
          vector<vector <float > > spike_time; //mod

          for(int i11=0;i11<n;i11++)
          {
            vector<float > v;
            spike_time.push_back(v);
          }

          generate_rand(pos,pos_ini,n);

          sum+=is_sync(pos,pos_ini,i1,myfile,spike,spike_time,n);

          for(int i11=0;i11<n;i11++)
          {

            int spikeI=spike_time[i11].size();
            if(spikeI<11)
            {
            for(int j1=0;j1<spikeI;j1++)
            {
              spike<<(spike_time[i11][j1])<<" ";
            }
            spike<<"\n";
          }
          else
          {
            for(int j1=spikeI-10;j1<spikeI;j1++)
            {
              spike<<(spike_time[i11][j1])<<" ";
            }
            spike<<"\n";
          }

          }
            spike<<"\n";
        }
      }
       p= (float)sum/(N1*N2);

       float fraction= (float)(2*i1)/(n*(n-1));
       if(fabs(1-p)>0.01)
       {
        cout<<n<<" "<<fraction<<endl;
        break;
       }

}
}
spike.close();
//myfile.close();
myfile.close();



}
