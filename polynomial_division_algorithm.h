#ifndef POLYNOMIAL_DIVISION_ALGORITHM_H
#define POLYNOMIAL_DIVISION_ALGORITHM_H

#include <string>
#include <vector>

using namespace std;

struct Monomial {
    double coef;
    int degree;
};

string inputPolynomial(const string& header);

vector<Monomial> parseMonomials(string& input);

int identifyMaxDegree(const vector<Monomial>& poly);

Monomial indentifyDominantTerms(const vector<Monomial>& polynomial);

Monomial divideDominantTerms(
    const Monomial& dominantTermNumerator,
    const Monomial& dominantTermDenominator
    );

void sortPolynomial(vector<Monomial>& polynomial);

void printVector(const vector<double>& v, const string& name);

vector<double> subtractVectors(const vector<double>& v1, const vector<double>& v2);

vector<Monomial> fromVectorToMonomials(const vector<double>& v);

string cleanNumber(const double number);

void printPolynomial(const vector<Monomial>& poly, const string& name);

void mainPolynomialDivisionAlgorithm();

#endif