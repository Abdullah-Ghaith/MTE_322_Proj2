#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

//CONSTANTS
int Fr = -1200;
int Pr = 400; 
float lp = 0.29;
int Lh = 3000;
int max_speed = 8000;
int FaA = 600;
int fs = 3;

std::vector<float> find_radial_forces(float la, float lb)
{
    
  vector<float> radial_forces;
  radial_forces.push_back((Pr * (lp-lb) - Fr * lb)/(la-lb));
  radial_forces.push_back(-1*radial_forces.at(0) + Pr - Fr);

  return radial_forces;
}

int main()
{
  //CURRENT PARAMS
  float la = 0.085;
  float lb = 0.225;
  float C0RA = 15200;
  float CRA = 13700;
  float f0A = 15.3;

  std::vector<float> radial_forces;
  radial_forces = find_radial_forces(la,lb);
  printf("FrA = %f, FrB = %f\n", radial_forces[0], radial_forces[1]);

  float design_life = (Lh*60*max_speed)/(1000000);
  printf("design_life = %f\n", design_life);
  
  float rad_axial_ratio_A = FaA/abs(radial_forces[0]);
  printf("rad_axial ratio = %f\n", rad_axial_ratio_A);
  
  float P0A;
  if (rad_axial_ratio_A < 0.8)
  {
    P0A = radial_forces[0];
  }
  else 
  {
    P0A = 0.6*radial_forces[0] + 0.5*FaA;
  }

  float min_C0A = fs*abs(P0A);
  printf("C0A must be greater than = %f\n", min_C0A);
  
  float dynamic_load_ratio_a = (f0A*FaA)/C0RA;
  printf("dynamic load ratio = %f\n", dynamic_load_ratio_a);

  const float RATIO_OPTIONS[9] = {0.172,0.345,0.689,1.03,1.38,2.07,3.45,5.17,6.89};
  int closest_match_a = 0;
  bool found_match = false;
  for (int i = 0; i < sizeof(RATIO_OPTIONS)/sizeof(float) && !found_match; i++)
  {
    float diff = RATIO_OPTIONS[i]-dynamic_load_ratio_a;
    if(diff > 0)
    {
        closest_match_a = i;
        found_match = true;
    }
  }
  found_match = false;

  const float e_OPTIONS[9] = {0.19,0.22,0.26,0.28,0.30,0.34,0.38,0.42,0.44};  
  float e_a = e_OPTIONS[closest_match_a];
  printf("e_a = %f\n", e_a);

  float XA = 0;
  float YA = 0;
  const float Y_OPTIONS[9] = {2.30, 1.99, 1.71, 1.55, 1.45, 1.31, 1.15, 1.04, 1.00};

  if(abs(FaA/radial_forces[0]) < e_a)
  {
    XA = 1;
    YA = 0;
  }
  else
  {
    XA = 0.56;
    YA = Y_OPTIONS[closest_match_a];
  }
  float dynamic_eq_load = XA*abs(radial_forces[0])+YA*FaA;
  printf("dynamic equivalent load = %f\n", dynamic_eq_load);

  float min_CA = dynamic_eq_load*pow(design_life, 0.3);
  printf("min. dynamic load rating = %f\n", min_CA);
}
