#include <unittest++/UnitTest++.h>
#include <iostream>
#include <cstdlib>
#include "Quaternion.hpp"
using namespace std;
using namespace m3g;

TEST (Quaternion_default_values)
{
  Quaternion q1;
  CHECK_EQUAL (0, q1.x);
  CHECK_EQUAL (0, q1.y);
  CHECK_EQUAL (0, q1.z);
  CHECK_EQUAL (1, q1.w);

  q1.setIdentity ();
  CHECK_EQUAL (0, q1.x);
  CHECK_EQUAL (0, q1.y);
  CHECK_EQUAL (0, q1.z);
  CHECK_EQUAL (1, q1.w);

  q1.setZero ();
  CHECK_EQUAL (0, q1.x);
  CHECK_EQUAL (0, q1.y);
  CHECK_EQUAL (0, q1.z);
  CHECK_EQUAL (0, q1.w);
}


TEST (Quaternion_set_values) 
{
  Quaternion q(30, 0.5, 0.3, 0.812403840463596);

  float a[4];
  q.getAngleAxis (a);
  
  CHECK_CLOSE (30.f, a[0], 0.00001);
  CHECK_CLOSE (0.5f, a[1], 0.00001);
  CHECK_CLOSE (0.3f, a[2], 0.00001);
  CHECK_CLOSE (0.812403840463596f, a[3], 0.00001);
  CHECK_CLOSE (1.f, q.getLength(), 0.00001);

  q.set (1,2,3,4);
  CHECK_CLOSE (0.182575f, q.x, 0.00001);
  CHECK_CLOSE (0.365148f, q.y, 0.00001);
  CHECK_CLOSE (0.547723f, q.z, 0.00001);
  CHECK_CLOSE (0.730297f, q.w, 0.00001);
  CHECK_CLOSE (1.f, q.getLength(), 0.00001);
}


TEST (Quaternion_multiply_float)
{
  Quaternion q (30, 0.5, 0.3, 0.812403840463596);
  Quaternion q0 = q*2;
  Quaternion q1 = 2*q;

  CHECK_EQUAL (q.x*2, q0.x);
  CHECK_EQUAL (q.y*2, q0.y);
  CHECK_EQUAL (q.z*2, q0.z);
  CHECK_EQUAL (q.w*2, q0.w);

  CHECK_EQUAL (q.x*2, q1.x);
  CHECK_EQUAL (q.y*2, q1.y);
  CHECK_EQUAL (q.z*2, q1.z);
  CHECK_EQUAL (q.w*2, q1.w);

  Quaternion q2 = q*0.5;
  float angle_axies[4];
  q.getAngleAxis(angle_axies);
  //cout << "q0 = " << angle_axies[0] << ", " << angle_axies[1] << ", " << angle_axies[2] << ", " << angle_axies[3] << "\n";
  q2.getAngleAxis(angle_axies);
  //cout << "q2 = " << angle_axies[0] << ", " << angle_axies[1] << ", " << angle_axies[2] << ", " << angle_axies[3] << "\n";
}

TEST (Quaternion_multiply_Quaternion)
{
  Quaternion q0 (30, 0.5, 0.3, 0.812403840463596);
  Quaternion q1 (90, 0.1, -0.2, 0.974679434480896);
  Quaternion q2 = q0*q1;
  Quaternion q3 = q1*q0;

  // Boost::quaternion (w,x,y,z) // 並び順注意。m3gとは違う
  // q0 = (0.965926,0.12941,0.0776457,0.210266)
  // q1 = (0.707107,0.0707107,-0.141421,0.689202)
  // q2 = (0.539927,0.243058,-0.15602,0.790607)
  // q3 = (0.539927,0.0765584,-0.00737683,0.83819)

  CHECK_CLOSE (0.243058f, q2.x, 0.00001);
  CHECK_CLOSE (-0.15602f, q2.y, 0.00001);
  CHECK_CLOSE (0.790607f, q2.z, 0.00001);
  CHECK_CLOSE (0.539927f, q2.w, 0.00001);

  CHECK_CLOSE (0.0765584f  , q3.x, 0.00001);
  CHECK_CLOSE (-0.00737683f, q3.y, 0.00001);
  CHECK_CLOSE (0.83819f    , q3.z, 0.00001);
  CHECK_CLOSE (0.539927f   , q3.w, 0.00001);

}

TEST (Quaternion_slerp)
{
  Quaternion q0 (0, 0,0,1);
  Quaternion q1 (90, 0,0,1);
  Quaternion q2 = slerp (q0, q1, 0.5);
  float a[4];
  q2.getAngleAxis (a);

  CHECK_CLOSE (45.f, a[0], 0.00001);
  CHECK_CLOSE (0.f, a[1], 0.00001);
  CHECK_CLOSE (0.f, a[2], 0.00001);
  CHECK_CLOSE (1.f, a[3], 0.00001);
}

TEST (Quaternion_slerp_2)
{
  // このデータは以前実際にアプリで使ってNAN化したもの。
  Quaternion q1;
  Quaternion q2;
  Quaternion q3;

  q1.set (0.60664, -0.79462, 0.0139204, 0.0191728);
  q2.set (0.605992, -0.795152, 0.01349, 0.01892);
  q3 = slerp (q1, q2, 0.5);

  CHECK_CLOSE (0.6063120, q3.x, 0.00001f);
  CHECK_CLOSE (-0.794881, q3.y, 0.00001f);
  CHECK_CLOSE (0.0137051, q3.z, 0.00001f);
  CHECK_CLOSE (0.0190463, q3.w, 0.00001f);

  q1.set (0.602674, -0.797573, 0.0156422,0.0205976);
  q2.set (0.602917, -0.797366, 0.0157651,0.0206895);
  q3 = slerp (q1, q2, 0);

  CHECK_CLOSE (0.602672,  q3.x, 0.00001f);
  CHECK_CLOSE (-0.79757,  q3.y, 0.00001f);
  CHECK_CLOSE (0.0156421, q3.z, 0.00001f);
  CHECK_CLOSE (0.0205975, q3.w, 0.00001f);

}
