#include<bits/stdc++.h>
#include<ctime>
#include<random>
using namespace std;



const int n=35;    //number of neurons
float S=1.000;
float L=1.00;
float h=0.002;    //time steps
float T=1000.00; //updated should be 10000
float thresh=0.80;         // value above which the particle jumps assuming the particle will jump
float excite=0.01;     //excitation in each particle due to one particle crossing threshold value
float error =0.001;
//const int K=13;
int arr[n+1][n+1];        //mapping array


void iterate(float pos[][2],float pos_ini[][2],vector<vector<float > > &spike_time);
int is_sync(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,vector<vector<float > > &spike_time);
void map_neuron(float pos[][2],float pos_ini[][2],int num,ofstream& myfile1);
void run(float pos[][2],float pos_ini[][2],ofstream &myfile,vector<vector<float > > &spike_time);
float time(float v,float v_o);
float dydx(float v);
void rungekutta(int i,float p[][2]);







void rungekutta(int i,float pos[][2])
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

void iterate(float pos[][2],float pos_ini[][2],vector<vector<float > > &spike_time)
{


	int L[n+1]={0};
	for(int i=0;i<n;i++)
	{

    rungekutta(i,pos);

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
      if(pos[i][0]+cnt*excite>=thresh )
        {
          pos[i][0]=0.0;
          spike_time[i].push_back(pos[i][1]);

        }
        else
        {
          pos[i][0]+=cnt*excite ;

        }
    }

}


int is_sync(float pos[][2],float pos_ini[][2],int num,ofstream &myfile,vector<vector<float > > &spike_time)
{

	run(pos,pos_ini,myfile,spike_time);
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



void map_neuron(float pos[][2],int num,ofstream &myfile1)
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
      myfile1<<s<<" ";
    }
    myfile1<<"\n";
  }
  myfile1<<"\n";
}

void run(float pos[][2],float pos_ini[][2],ofstream  &myfile,vector<vector<float > > &spike_time)
{
  int count=0;
  int times=T/h;
  int i=0;
  while(times--)
  {
    iterate(pos,pos_ini,spike_time);
  }
}
void generate_rand(float pos[][2],float pos_ini[][2])
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
  	float pos[201][2];
    float pos_ini[201][2];
    ofstream myfile,myfile1,spike;
    myfile.open("endpositions.txt");
    myfile1.open("map.txt");
    spike.open("spike_time.txt");
    int seed;
    cin>>seed;
    srand(seed);
    //cout<<"done"<<endl;
    int k=0;
    int incre= (n*(n-1))/80;
    //int incre1= 90/20;

    int N1=50;           // no of graphs generated
    int N2=10;
    myfile1<<"neurons: "<<to_string(n);
    myfile1<<"\n";
    spike<<"neurons: "<<to_string(n);
    spike<<"\n";

  //  cout<<N1<<" "<<N2<<" "<<T<<" "<<n<<endl;
  //  for(int i1=144;2*i1<=390;i1+=incre)
   int start=(n*(n-1)/2)*(40/100);
for(int i1=start;2*i1<=(n*(n-1));i1+=incre)
  {
      float p=0.0;
      int sum=0;
      myfile1<<"Synapses broken: "<<to_string(i1);
      myfile1<<"\n";
      spike<<"Synapses broken: "<<to_string(i1);
      spike<<"\n";
    for(int i=0;i<N1;i++)
          {

            map_neuron(pos,i1,myfile1);

            for(int j=0;j<N2;j++)
            {
              vector<vector<float > > spike_time;
              for(int counter=0;counter<n;counter++)
              {
                vector<float > v;
                spike_time.push_back(v);
              }

              generate_rand(pos,pos_ini);
              sum+=is_sync(pos,pos_ini,i1,myfile,spike_time);
              int spikeI=spike_time[0].size();
              if(spikeI<10)
              {
                for(int ii1=0;ii1<n;ii1++)
                {
                  for(int jj1=0;jj1<spikeI;jj1++)
                  {
                    spike<<spike_time[ii1][jj1]<<" ";
                  }
                  spike<<"\n";
                }
              }

              else
              {
                for(int ii1=0;ii1<n;ii1++)
                {
                  for(int jj1=spikeI-10;jj1<spikeI;jj1++)
                  {
                    spike<<spike_time[ii1][jj1]<<" ";
                  }
                  spike<<"\n";
                }
              }
              spike<<"\n";
            }
          }
         float fraction = (float)(2*i1)/(n*(n-1));
           p= (float)sum/(N1*N2);
           cout<<fraction<<" "<<p<<endl;
          if(fabs(1-p)>0.1)
          {
            //cout<<n<<" "<<fraction;
            break;
          }
}


myfile.close();
myfile1.close();
spike.close();


}
