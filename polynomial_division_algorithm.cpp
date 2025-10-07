#include <regex>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ranges>
#include <cmath>

#include "polynomial_division_algorithm.h"

using namespace std;

string inputPolynomial(const string& header) {
    cout << header;
    string input;
    getline(cin, input);
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

vector<Monomial> parseMonomials(string& input) {

    // Initialisation
    vector<Monomial> monomials;
    const regex outerReg(R"(([+-]?(?:\d+(?:\.\d+)?)(?:x(\^(\d+))?)?|[+-]?x(\^(\d+))?))");
    const regex innerReg(R"(([+-]?\d*\.?\d*)x?\^?(\d*)?)");
    sregex_iterator iterator(input.begin(), input.end(), outerReg);

    // Main Loop
    for (sregex_iterator end; iterator != end; ++iterator) {
        const smatch& m = *iterator;
        string matchStr = m.str();
        smatch innerMatch;
        if (regex_match(matchStr, innerMatch, innerReg)) {
            string coefStr = innerMatch[1].str();
            string expStr  = innerMatch[2].str();
            double coef;
            int degree;

            // ---------- Coefficient ----------
            if (coefStr.empty() || coefStr == "+") coef = 1.0;
            else if (coefStr == "-") coef = -1.0;
            else coef = stod(coefStr);

            // ---------- Degree ----------
            if (matchStr.find('x') == string::npos)
                degree = 0;
            else if (expStr.empty())
                degree = 1;
            else
                degree = stoi(expStr);
            monomials.push_back(Monomial(coef, degree));
        }
    }
    return monomials;
}

int identifyMaxDegree(const vector<Monomial>& poly) {
    int maxDeg = 0;
    for (const auto& m : poly)
        if (m.degree > maxDeg)
            maxDeg = m.degree;
    return maxDeg;
}

Monomial indentifyDominantTerms(const vector<Monomial>& polynomial) {
    const auto iterator = ranges::max_element(
        polynomial.begin(),
        polynomial.end(),
        [](const Monomial& a, const Monomial& b) -> bool {
            return a.degree < b.degree;
        });
    Monomial result = *iterator;
    return {result};
}

Monomial divideDominantTerms(
    const Monomial& dominantTermNumerator,
    const Monomial& dominantTermDenominator) {
    double qCoef = dominantTermNumerator.coef / dominantTermDenominator.coef;
    int qDeg = dominantTermNumerator.degree - dominantTermDenominator.degree;
    return Monomial{qCoef, qDeg};
}

void sortPolynomial(vector<Monomial>& polynomial) {
    ranges::sort(polynomial, ranges::greater(), &Monomial::degree);
}

void printVector(const vector<double>& v, const string& name) {
    if (!name.empty()) cout << name << " = ";
    cout << "[ ";
    for (auto value : v | views::all) cout << value << " ";
    cout << "]\n";
}

vector<double> subtractVectors(const vector<double>& v1, const vector<double>& v2) {
    vector<double> result(v1.size());
    ranges::transform(v1, v2, result.begin(), std::minus<>{});
    return result;
}

vector<Monomial> fromVectorToMonomials(const vector<double>& v) {
    vector<Monomial> monomials;
    for (auto degree: views::iota(0, static_cast<int>(v.size()))) {
        double coef = v[degree];
        if (coef != 0.0) monomials.push_back(Monomial(coef, degree));
    }
    return monomials;
}
string cleanNumber(const double number) {
    if (std::floor(number) == number)
        return std::to_string(static_cast<int>(number));
    return std::to_string(number);
}

void printPolynomial(const vector<Monomial>& poly, const string& name) {
    string result;
    for (auto m : poly) {
        if (m.coef == 0.0) continue;
        if (!result.empty()) {
            result += (m.coef > 0 ? " + " : " - ");
        } else if (m.coef < 0) {
            result += "-";
        }
        double absCoef = abs(m.coef);
        if (!(absCoef == 1.0 && m.degree != 0)) {
            result += cleanNumber(absCoef);
        }
        if (m.degree > 0) {
            result += "x";
            if (m.degree > 1)
                result += "^" + to_string(m.degree);
        }
    }
    if (result.empty()) result = "0";
    cout << name << ": " << result << endl;
}

void mainPolynomialDivisionAlgorithm(){

    // Main header
    cout << endl << "-----   POLYNOMIAL DIVISOR   -----" << endl;

    // Numerator input
    string header = "\nEnter numerator polynomial (e.g. 2x^3 + 4x - 5) : ";
    string numerator_input = inputPolynomial(header);
    const vector<Monomial> numerator_monomials = parseMonomials(numerator_input);

    // Denominator input
    header = "Enter denominator polynomial (e.g. 2x^3 + 4x - 5) :";
    string denominator_input = inputPolynomial(header);
    vector<Monomial> denominator_monomials = parseMonomials(denominator_input);

    // Initial degrees
    int degA = identifyMaxDegree(numerator_monomials);
    int degB = identifyMaxDegree(denominator_monomials);
    int degR = degA;

    // Initialize remainder
    vector<Monomial> remainder_monomials = numerator_monomials;
    vector<Monomial> quotient_monomials;

    // Main Loop
    while (degR >= degB) {
        // Identify dominant terms
        const Monomial dominantTermNumerator = indentifyDominantTerms(remainder_monomials);
        Monomial dominantTermDenominator = indentifyDominantTerms(denominator_monomials);

        // Calculate Quotient
        Monomial quotient = divideDominantTerms(dominantTermNumerator, dominantTermDenominator);
        quotient_monomials.push_back(quotient);

        // Calculate partial product
        vector<Monomial> partialproduct;
        for (const auto& monomial : denominator_monomials) {
            double partialProductCoef = quotient.coef * monomial.coef;
            int partialProductDeg = quotient.degree + monomial.degree;
            Monomial partialProduct{partialProductCoef, partialProductDeg};
            partialproduct.push_back(partialProduct);
        }

        // Vectorise partial product and numerator to calculate the remainder in the next step
        degA = identifyMaxDegree(remainder_monomials);
        vector<double> NumeratorVectorized(degA+1, 0.0);
        vector<double> PartialProdVectorized(degA+1,0.0);
        for (const auto& monomial : remainder_monomials) {
            NumeratorVectorized[monomial.degree] = monomial.coef;
        }
        for (const auto& monomial : partialproduct) {
            PartialProdVectorized[monomial.degree] = monomial.coef;
        }

        // Calculate remainder (Numerator - PartialProduct) for the terms of the same degree.
        vector<double> remainder = subtractVectors(NumeratorVectorized, PartialProdVectorized);
        remainder_monomials = fromVectorToMonomials(remainder);
        degR = identifyMaxDegree(remainder_monomials);

    }
    printPolynomial(quotient_monomials, "Quotient");
    printPolynomial(remainder_monomials, "Remainder");
}
