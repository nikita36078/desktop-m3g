#include "Quaternion.hpp"
#include "Exception.hpp"
#include "m3ginternal.hpp"
#include "Vector.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using namespace m3g;

Quaternion:: Quaternion () :
    x(0), y(0), z(0), w(1)
{
}

Quaternion:: Quaternion (float angle, float ax, float ay, float az) :
    x(0), y(0), z(0), w(1)
{
    Vector axis (ax, ay, az);
    axis.normalize ();
  
    if (angle != 0) {
        float th = 2*M_PI*angle/360.f;
        x = axis.x * sinf (th/2.f);
        y = axis.y * sinf (th/2.f);
        z = axis.z * sinf (th/2.f);
        w =          cosf (th/2.f);
    }
}

Quaternion:: ~Quaternion ()
{
}

void Quaternion:: setIdentity ()
{
    x = y = z = 0;
    w = 1;
}

void Quaternion:: setZero ()
{
    x = y = z = w  = 0;
}

Quaternion& Quaternion:: normalize ()
{
    float len = sqrtf(x*x + y*y + z*z + w*w);
    x = x/len;
    y = y/len;
    z = z/len;
    w = w/len;
    return *this;
}

void Quaternion:: set (float qx, float qy, float qz, float qw)
{
    x = qx;
    y = qy;
    z = qz;
    w = qw;
    //normalize ();
}

float Quaternion:: getLength () const
{
    return sqrtf(x*x+y*y+z*z+w*w);
}

/**
 * (補足) 回転角度が0の場合はangle=0,axis=(x,y,z)が返る.
 */
void Quaternion:: getAngleAxis (float* angle_axis) const
{
    if (angle_axis == NULL) {
        throw NullPointerException (__FILE__, __func__, "Angle_axis is NULL.");
    }

    float th = 2*acosf(w);
    angle_axis[0] = th*360/(2.f*M_PI);
    if (sinf(th/2.f) != 0) {
        angle_axis[1] = x/sinf(th/2.f);
        angle_axis[2] = y/sinf(th/2.f);
        angle_axis[3] = z/sinf(th/2.f);
    } else {
        angle_axis[1] = x;
        angle_axis[2] = y;
        angle_axis[3] = z;
    }
}

/**
 * a=0のときp, a=1のときq
 */
Quaternion m3g::slerp (const Quaternion& p, const Quaternion& q, float a)
{
    float th = acosf(p.x*q.x + p.y*q.y + p.z*q.z + p.w*q.w);
    if (sinf(th) > 0 || sinf(th) < 0) {
        float w0 = sinf((1-a)*th)/sinf(th);
        float w1 = sinf(a*th)/sinf(th);
        return w0*p + w1*q;
    } else {
        // 注意：NANを含む比較はfalseを返す。
        // thが0もしくはNAN化した時こちらが実行される
        // p,qがほぼ等しい時に内積が1を超える事はあり得る。
        return (a < 0.5) ? p : q;
    }
}

/**
 * Spherical and quadrangle interpolation.
 * For more information, See 3D Math Primer for Graphics and Game Develpment
 */
m3g::Quaternion m3g::squad (
    const m3g::Quaternion& q0,
    const m3g::Quaternion& q1,
    const m3g::Quaternion& q2,
    const m3g::Quaternion& q3,
    float t0, float t1, float t2, float t3, float s)
{
    //cout << "s = " << s << ", t = " << t0 << ", " << t1 << ", " << t2 << ", " << t3 << "\n";

    float f1 = 2*(t2-t1)/(t2-t0);
    float f2 = 2*(t2-t1)/(t3-t1);
    //cout << "f1 = " << f1 << ", f2 = " << f2 << "\n";
    Quaternion a1 = q1 * exp( (f1*(log(q1.inv()*q2) + log(q0.inv()*q1))*0.5f - log(q1.inv()*q2))*0.5f );
    Quaternion b2 = q2 * exp( (log(q1.inv()*q2) - f2*(log(q2.inv()*q3) + log(q1.inv()*q2))*0.5f)*0.5f );
    //cout << "a1 = " << a1 << "\n";
    //cout << "b2 = " << b2 << "\n";
    Quaternion q = slerp (slerp(q1,q2,s), slerp(a1,b2,s), 2*s*(1-s));
    return q;
}

m3g::Quaternion Quaternion::inv () const
{
    Quaternion q;
    q.x = -x;
    q.y = -y;
    q.z = -z;
    q.w =  w;
    return q;
}


m3g::Quaternion m3g::log (const m3g::Quaternion& q)
{
    float x,y,z,w;
    float alpha = acosf(q.w);
    if (alpha > 0 || alpha < 0) {
        x = alpha*q.x/sinf(alpha);
        y = alpha*q.y/sinf(alpha);
        z = alpha*q.z/sinf(alpha);
        w = 0;
    } else {
        // 注意：NANを含む比較はfalseを返す。
        // alphaが0もしくはNAN化した時こちらが実行される
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    Quaternion q2;
    q2.set (x, y, z, w);
    return q2;
}


m3g::Quaternion m3g::exp (const m3g::Quaternion& q)
{
    if (q.w != 0) {
        throw ArithmeticException (__FILE__, __func__, "W component of quaternion must be 0 for exponential, w=%f.", q.w);
    }

    float x,y,z,w;
    float alpha = q.getLength();
    if (alpha > 0 || alpha < 0) {
        x     = q.x*sinf(alpha)/alpha;
        y     = q.y*sinf(alpha)/alpha;
        z     = q.z*sinf(alpha)/alpha;
        w     = cosf(alpha);
    } else {
        // 注意：NANを含む比較はfalseを返す。
        // thが0もしくはNAN化した時こちらが実行される
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }
    Quaternion q2;
    q2.set (x, y, z, w);
    return q2;
}



Quaternion operator* (float f, const Quaternion& p)
{
    Quaternion r;
    r.x = f*p.x;
    r.y = f*p.y;
    r.z = f*p.z;
    r.w = f*p.w;
    return r;
}

Quaternion operator* (const Quaternion& p, float f)
{
    return f*p;
}

/**
 * クォータニオンpを適応してからクォータニオンqを適応する.
 */
Quaternion operator* (const Quaternion& p, const Quaternion& q)
{
    Quaternion r;
    r.x = p.w*q.x + q.w*p.x + p.y*q.z - p.z*q.y;
    r.y = p.w*q.y + q.w*p.y + p.z*q.x - p.x*q.z;
    r.z = p.w*q.z + q.w*p.z + p.x*q.y - p.y*q.x;
    r.w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;
    return r;
}

Quaternion operator+ (const Quaternion& p, const Quaternion& q)
{
    Quaternion r;
    r.x = p.x + q.x;
    r.y = p.y + q.y;
    r.z = p.z + q.z;
    r.w = p.w + q.w;
    return r;
}

Quaternion operator- (const Quaternion& p, const Quaternion& q)
{
    Quaternion r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    r.w = p.w - q.w;
    return r;
}

std::ostream& Quaternion:: print (std::ostream& out) const
{
    out << "[ " << x << "," << y << "," << z << ", " << w << " or ";
    float a[4];
    getAngleAxis (a);
    out << "" << a[0] << " ," << a[1] << "," << a[2] << "," << a[3] << " ]";
    return out;
}

std::ostream& operator<< (std::ostream& out, const Quaternion& q)
{
    return q.print (out);
}
