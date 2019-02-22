/* $COPYRIGHT SKOLTECH
 * $LICENSE_LGPL
 *
 * example_synthetic_plane_traj.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech
 */


#include "mrob/create_points.hpp"
#include "mrob/plane_registration.hpp"
#include <iostream>


int main()
{
    uint_t numPlanes = 4, numPoses = 2;
    // 1) define problem conditions
    mrob::CreatePoints scene(40,numPlanes,numPoses,0.001, 0.1);
    //scene.print();

    mrob::PlaneRegistration contPlanes;
    scene.create_plane_registration(contPlanes);
    contPlanes.print();


    // 3) evaluate alignment
    contPlanes.solve();
    contPlanes.print(false);
    contPlanes.solve();
    contPlanes.print(false);
    contPlanes.solve();
    contPlanes.print(false);
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.solve();
    contPlanes.print(false);


    return 1;
}
