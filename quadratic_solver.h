#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H


# include <tuple>
using namespace std;

/**
   * @struct Coefficients
   * @brief Container for quadratic coefficients (a, b, c).
*/
struct Coefficients {
    double a, b, c;
};

/**
     * @brief Requests coefficients input from user.
     * @param[out] coefs Coefficients structure to fill.
*/
void inputCoefficients(Coefficients& coefs);

/**
    * @brief Computes the summit (vertex) of the parabola.
    * @param[in] coefs Coefficients of the quadratic equation.
    * @return (xv, yv) coordinates of the summit.
*/
tuple<double, double> find_summit(const Coefficients& coefs);


/**
 * @brief Determines the number of real roots of the polynomial.
 * @param[in] coefs Coefficients of the equation.
 * @param[in] summit Tuple (xv, yv) of the vertex.
 * @return 0, 1, or 2 depending on the discriminant sign.
*/
int calc_real_roots_quantity(
    const Coefficients& coefs,
    tuple<double, double> summit
    );

/**
 * @brief Evaluates polynomial value at a given x.
 * @param[in] coefs Polynomial coefficients.
 * @param[in] x Point where to evaluate.
 * @return Value f(x).
 */
double calc_polynomial_eq(
    const Coefficients& coefs,
    double x
    );

/**
 * @brief Numerical root-finding by directional iteration.
 * @param[in] coefs Polynomial coefficients.
 * @param[in] summit Vertex tuple (xv, yv).
 * @return Approximation of (x, f(x)) near a root.
*/
tuple<double, double> find_roots(
    const Coefficients& coefs,
    const tuple<double, double>& summit
    );

/**
    * @brief Main interactive solver routine.
    *        Requests user input and outputs analysis results.
    * @return 0 on success.
*/
int main_solv_quadratic_equation();

#endif