#include<string>
#include<vector>

using namespace std;


class PRP{
 public:

  int n;     // number of clients
  int l;     // number of time periods

  float u;  // unit production cost
  float f;   // fixed production setup cost
  float C;   // production capacity
  float Q;   // vehicle capacity
  int k;     // number of vehicles

  vector<pair<float,float> > xy; // clients coordinates
  vector<float> h;  // h_i unit inventory cost at node i
  vector<vector<float> > d; // d_it demand at customer i in period t
  vector<float> L; // maximum inventory level at node i
  vector<float> L0; //initial inventory at node i

  int dist;  // 1 if the instance is from Archetti et al. 
             // 2 if the instance is from Boudia et al.,
  // if dist==1
      // The transportation cost c_ij is 
      // INT((SQRT((X(I)-X(J))**2+(Y(I)-Y(J))**2))+.5)
  // if dist ==2:
       float mc;   // kilometric cost
       // The transportation cost c_ij is mc times the euclidian cost
  

  PRP(int n, int l);
  PRP(istream&);
  void write_screen_txt();
  void write_instance_file(string);
  

};
