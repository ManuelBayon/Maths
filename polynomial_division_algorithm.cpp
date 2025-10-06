#include <regex>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

struct Monomial {
    double coef;
    int degree;
};

string inputPolynomial(const string& header) {
    cout << header;
    string input;
    getline(cin, input);
    erase(input, ' ');
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
            cout << "Monomial: " << matchStr << "  => coef = " << coef << ", degree = " << degree << endl;
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

int mainPolynomialDivisionAlgorithm(){
    // Main header
    cout << endl << "-----   POLYNOMIAL DIVISOR   -----" << endl;

    // Numerator input
    string header = "\nEnter numerator polynomial (e.g. 2x^3 + 4x - 5) : ";
    string numerator_input = inputPolynomial(header);
    vector<Monomial> numerator_monomials = parseMonomials(numerator_input);

    // Denominator input
    header = "\nEnter denominator polynomial (e.g. 2x^3 + 4x - 5) :";
    string denominator_input = inputPolynomial(header);
    vector<Monomial> denominator_monomials = parseMonomials(denominator_input);

    // Find Numerator/Denominator degrees
    const int degA = identifyMaxDegree(numerator_monomials);
    int degB = identifyMaxDegree(denominator_monomials);

    // Identify dominant terms
    const Monomial dominantTermNumerator = indentifyDominantTerms(numerator_monomials);
    Monomial dominantTermDenominator = indentifyDominantTerms(denominator_monomials);

    // Calculate Quotient
    Monomial quotient = divideDominantTerms(dominantTermNumerator, dominantTermDenominator);
    cout << "\nQuotient: "<<quotient.coef <<"x^" << quotient.degree << endl;

    // Calculate partial product
    vector<Monomial> partialproduct;
    cout << endl;
    for (const auto& monomial : denominator_monomials) {
        double partialProductCoef = quotient.coef * monomial.coef;
        int partialProductDeg = quotient.degree + monomial.degree;
        Monomial partialProduct{partialProductCoef, partialProductDeg};
        partialproduct.push_back(partialProduct);
        cout << "Partial product: " << partialProductCoef << "x^" << partialProductDeg << endl;
    }

    // Sort Polynomials
    sortPolynomial(numerator_monomials);
    sortPolynomial(denominator_monomials);
    sortPolynomial(partialproduct);

    // Calculate remainder (Numerator - PartialProduct) for the terms of the same degree.


    return 0;
}
