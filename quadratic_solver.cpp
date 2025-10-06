#include "quadratic_solver.h"
#include <cmath>
#include <iostream>

using namespace std;

void inputCoefficients(Coefficients& coefs){
    cout << "Choose coefficient for the second degree equation ax^2+bx+c : "<< endl;
    cout << "Coefficient a: ";
    cin >> coefs.a;
    cout << "Coefficient b: ";
    cin >> coefs.b;
    cout << "Coefficient c: ";
    cin >> coefs.c;
}

tuple<double, double> find_summit(const Coefficients& coefs) {
    double xv = -coefs.b/(2*coefs.a);
    double yv = coefs.a*pow(xv, 2) + coefs.b*xv + coefs.c;
    return make_tuple(xv, yv);
}

int calc_real_roots_quantity(
    const Coefficients& coefs,
    tuple<double, double> summit
    ) {
    auto[xv, yv] = summit;
    const double eps = 1e-12;
    // 1 Root
    if (fabs(yv) < eps) {
        return 1;
    }
    // 2 Roots
    if ((coefs.a > 0 && yv < 0) || (coefs.a < 0 && yv > 0)){
        return 2;
    }
    // 0 Real roots
    return 0;
}

double calc_polynomial_eq(
    const Coefficients& coefs,
    double x
    ) {
    return  coefs.a * pow(x, 2) + coefs.b*x + coefs.c;
}

tuple<double, double> find_roots(
    const Coefficients& coefs,
    const tuple<double, double>& summit
    ) {
    auto[xv, yv] = summit;
    double x = xv;
    double y = yv;

    // Get increment resolution
    double resolution;
    cout << "Choose resolution : " << endl;
    cin >> resolution;

    // Main loop
    while (yv * y > 0) {
        y = calc_polynomial_eq(coefs, x+resolution);
        x += resolution;
    }
    return make_tuple(x, y);
}

int main_solv_quadratic_equation() {

    // Initialisation
    Coefficients coefs = {0.0, 0.0, 0.0};
    inputCoefficients(coefs);
    tuple<double, double> summmit = find_summit(coefs);
    auto[xv, yv] = summmit;
    const int real_roots_quantity = calc_real_roots_quantity(coefs, summmit);

    // Printing
    cout << "Coefficients are: " << "a = " << coefs.a << ", b = " << coefs.b << ", c = " << coefs.c << endl;
    cout << "Summit xv: " << xv << ", yv: " << yv << endl;
    cout << "Number of real roots: " << real_roots_quantity << endl;

    // if only 1 root its abscissa is its summit
    if (real_roots_quantity == 1) {
        cout << "1 root found: xv: " << xv << ", yv: "<<yv <<endl;
    }

    // if 2 roots they are symetrical with regard to the summit absicca axis
    // so we only to find one to have the other one easily.
    if (real_roots_quantity == 2) {
        tuple<double, double> root_1_approx = find_roots(coefs, summmit);
        auto[x, y] = root_1_approx;
        double x_root_2 = 2 * xv - x;
        cout << "Approximation of root 1 x = "<< x <<", root 2 x = " <<x_root_2 <<endl;
    }
    // End
    return 0;
}