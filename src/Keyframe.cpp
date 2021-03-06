#include "m3g/Keyframe.hpp"
#include "m3g/Quaternion.hpp"
#include "m3g/Exception.hpp"
#include <iostream>
using namespace std;
using namespace m3g;

Keyframe:: Keyframe () : time(-1), value(0)
{
}


Keyframe:: Keyframe (int t, float* v) : time(t), value(v)
{
}

Keyframe:: ~Keyframe ()
{
}

void m3g::step   (float s, const Keyframe& k0, const Keyframe& k1, int component_count, float* value)
{
    if (value == NULL) {
        throw NullPointerException (__FILE__, __func__, "Value is NULL.");
    }
    if (s < 0 || s > 1) {
        throw IllegalArgumentException (__FILE__, __func__, "S is invalid, s=%f.", s);
    }

    for (int i = 0; i < component_count; i++) {
        value[i] = (s < 1) ? k0.value[i] : k1.value[i];
    }
}

void m3g::linear (float s, const Keyframe& k0, const Keyframe& k1, int component_count, float* value)
{
    if (value == NULL) {
        throw NullPointerException (__FILE__, __func__, "Value is NULL.");
    }
    if (s < 0 || s > 1) {
        throw IllegalArgumentException (__FILE__, __func__, "S is invalid, s=%f.", s);
    }

    for (int i = 0; i < component_count; i++) {
        value[i] = k0.value[i]*(1-s) + k1.value[i]*(s);
    }
}

void m3g::slerp  (float s, const Keyframe& k0, const Keyframe& k1, int component_count, float* value)
{
    if (value == NULL) {
        throw NullPointerException (__FILE__, __func__, "Value is NULL.");
    }
    if (s < 0 || s > 1) {
        throw IllegalArgumentException (__FILE__, __func__, "S is invalid, s=%f.", s);
    }
    if (component_count != 4) {
        throw IllegalArgumentException (__FILE__, __func__, "Component count must be 4, num=%d.", component_count);
    }

    Quaternion q0, q1, q2;
    q0.set (k0.value[0], k0.value[1], k0.value[2], k0.value[3]);
    q1.set (k1.value[0], k1.value[1], k1.value[2], k1.value[3]);
    q2 = slerp (q0, q1, s);
    value[0] = q2.x;
    value[1] = q2.y;
    value[2] = q2.z;
    value[3] = q2.w;
    //cout << "a = " << a << "\n";
    //cout << "Quaternion q0 = " << q0 << "\n";
    //cout << "Quaternion q1 = " << q1 << "\n";
    //cout << "Quaternion by SLEPR = " << q2 << "\n";
}

void m3g::spline (float s, const Keyframe& k0, const Keyframe& k1, const Keyframe& k2, const Keyframe& k3, int component_count, float* value)
{
    if (value == NULL) {
        throw NullPointerException (__FILE__, __func__, "Value is NULL.");
    }
    if (s < 0 || s > 1) {
        throw IllegalArgumentException (__FILE__, __func__, "S is invalid, s=%f.", s);
    }

    float sh0 = 2*s*s*s - 3*s*s + 1;
    float sh1 = -2*s*s*s + 3*s*s;
    float sh2 = s*s*s - 2*s*s + s;
    float sh3 = s*s*s - s*s;

    for (int i = 0; i < component_count; i++) {
        float tan1 = (k0.time == -1) ? 0 : (k2.time-k1.time)/(float)(k2.time-k0.time)*(k2.value[i] - k0.value[i]);
        float tan2 = (k3.time == -1) ? 0 : (k2.time-k1.time)/(float)(k3.time-k1.time)*(k3.value[i] - k1.value[i]);

        value[i] = sh0*k1.value[i] + sh1*k2.value[i] + sh2*tan1 + sh3*tan2;
    }
}

void m3g::squad  (float s, const Keyframe& k0, const Keyframe& k1, const Keyframe& k2, const Keyframe& k3, int component_count, float* value)
{
    if (value == NULL) {
        throw NullPointerException (__FILE__, __func__, "Value is NULL.");
    }
    if (s < 0 || s > 1) {
        throw IllegalArgumentException (__FILE__, __func__, "S is invalid, s=%f.", s);
    }
    if (component_count != 4) {
        throw IllegalArgumentException (__FILE__, __func__, "Component count must be 4, num=%d.", component_count);
    }

    Quaternion q0, q1, q2, q3;
    if (k0.time == -1)
        q0.set (k1.value[0], k1.value[1], k1.value[2], k1.value[3]);
    else
        q0.set (k0.value[0], k0.value[1], k0.value[2], k0.value[3]);
    q1.set (k1.value[0], k1.value[1], k1.value[2], k1.value[3]);
    q2.set (k2.value[0], k2.value[1], k2.value[2], k2.value[3]);
    if (k3.time == -1)
        q3.set (k2.value[0], k2.value[1], k2.value[2], k2.value[3]);
    else
        q3.set (k3.value[0], k3.value[1], k3.value[2], k3.value[3]);
  
    float t0, t1, t2, t3;
    t0 = (k0.time==-1) ? k1.time : k0.time;
    t1 = k1.time;
    t2 = k2.time;
    t3 = (k3.time==-1) ? k2.time : k3.time;

    Quaternion q4 = squad (q0, q1, q2, q3, t0, t1, t2, t3, s);
    value[0] = q4.x;
    value[1] = q4.y;
    value[2] = q4.z;
    value[3] = q4.w;
}


std::ostream& Keyframe:: print (std::ostream& out) const
{
    out << "time=" << time << ",value=";
    if (value == NULL)
        out << "0";
    else
        out << value[0] << "," << value[1] << "," << value[2] << ",...";
    return out;
}


std::ostream& operator<< (std::ostream& out, const m3g::Keyframe& key)
{
    return key.print (out);
}
