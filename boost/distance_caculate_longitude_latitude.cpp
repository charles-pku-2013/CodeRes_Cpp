#include <string>
#include <iostream>
#include <boost/geometry.hpp>

using namespace std;

int main() {
    using spherical_point =
        boost::geometry::model::point<double, 2, boost::geometry::cs::spherical_equatorial<boost::geometry::degree>>;

    spherical_point p(116.394476, 40.00694);
    spherical_point q(121.506379, 31.245414);
    double dist = boost::geometry::distance(p, q);
    const double earth_radius = 6371.0; // Km
    double dist_km = dist * earth_radius;

    cout << dist_km << endl;
    return 0;
}
