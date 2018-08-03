/* $COPYRIGHT_SKOLTECH
 * $LICENSE_LGPL
 *
 * SE3.cpp
 *
 *  Created on: Feb 12, 2018
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech
 */

#include "skmr/SE3.hpp"
#include "skmr/SO3.hpp"
#include <cmath>
#include <iostream>


using namespace skmr;

SE3::SE3(const Mat61 &xi) : Mat4(Mat4::Identity())
{
    //std::cout << "SE3 MAT31" << std::endl;
    this->exp(this->hat(xi));
}

template<typename OtherDerived>
SE3::SE3(const Eigen::MatrixBase<OtherDerived>& other)  :
Mat4(other)
{    std::cout << "SE3 MAT4" << std::endl;
}

template<typename OtherDerived>
SE3& SE3::operator=(const Eigen::MatrixBase <OtherDerived>& other)
{
    std::cout << "SE3 operator =" << std::endl;
    this->Mat4::operator=(other);
    return *this;
}

void SE3::update(const Mat61 &dxi)
{
    SE3 dT(dxi);
    *this << dT * (*this);
}

Mat61 SE3::vee(const Mat4 &xi_hat) const
{
    Mat61 xi;
    xi << -xi_hat(1,2), xi_hat(0,2), -xi_hat(0,1),
           xi_hat(0,3), xi_hat(2,3), xi_hat(2,3);
    return xi;
}

Mat4 SE3::hat(const Mat61 &xi) const
{
    Mat4 xi_hat;
    xi_hat  <<    0.0, -xi(2),  xi(1), xi(3),
                xi(2),    0.0, -xi(0), xi(4),
               -xi(1),  xi(0),    0.0, xi(5),
                    0,      0,      0,    0;
    return xi_hat;
}

void SE3::exp(const Mat4 &xi_hat)
{
    // Calculating xi = [w, v]
    Mat61 xi = this->vee(xi_hat);
    Mat31 w = xi.head<3>();
    Mat31 v = xi.tail<3>();
    SO3 R(w);
    Mat3 w_hat = xi_hat.topLeftCorner<3,3>();

    // Calculate the closed form of V
    // V = I + c2*(w^) + c3*(w^)^2   ,
    // where o = norm(w), c2 = (1 - cos(o))/o^2, c3 = (o- sin(o) / o^3
    Mat3 V = Mat3::Identity();
    double o = w.norm();
    if ( o > 1e-9) //not zero
    {
        double c2 = (1 - std::cos(o))/o/o;
        double c3 = (o - std::sin(o))/o/o/o;
        V += c2*w_hat + c3*w_hat*w_hat;
    }

    // Calculate the translation component t = Vv
    Mat31 t = V*v;

    // compose the rigid body motion matrix T = [R, t]
    //this->topLeftCorner<3,3>() = R;
    //this->topRightCorner<3,1>() = t;
    *this << R, t,
             0,0,0,1;
}

Mat4 SE3::ln(void) const
{
    //Mat3 RR = );
    SO3 R;
    R << this->topLeftCorner<3,3>();
    // Logarithmic mapping of the rotations
    double o;
    Mat3 w_hat = R.ln(&o);

    // calculate v = V^1 t
    // V^-1 = I - 0.5w^ + k1 (w^)^2
    // k1 = 1/o^2 * (1 - c1/(2c2) ) ,    c1 =sin(o)/o and c2 = (1 - cos(o))/o^2 from so3_exp
    Mat3 Vinv = Mat3::Identity();
    if (o > 1e-9)
    {
        double c1 = std::sin(o)/o;
        double c2 = (1 - std::cos(o))/o/o;
        double k1 = 1/o/o*(1 - 0.5*c1/c2);
        Vinv += -0.5*w_hat + k1* w_hat*w_hat;
    }

    // v = V^-1 t
    Mat31 v = Vinv * this->topRightCorner<3,1>();

    // create a vector containing the components
    Mat4 xi_hat = Mat4::Zero();
    xi_hat << w_hat, v,
              0,0,0,0;
    return xi_hat;
}

Mat61 SE3::ln_vee() const
{
    Mat4 xi_hat = this->ln();
    return this->vee(xi_hat);
}

SE3 SE3::inv(void) const
{
    SE3 inv(Mat4::Identity());
    Mat3 R = this->topLeftCorner<3,3>();
    R.transposeInPlace();
    Mat31 t = this->topRightCorner<3,1>();
    inv.topLeftCorner<3,4>() << R , -R*t;
    return inv;

}

void SE3::print(void) const
{
    std::cout << *this << std::endl;
}


void SE3::print_lie(void) const
{

    std::cout << this->ln_vee() << std::endl;
}