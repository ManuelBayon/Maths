#include <regex>
#include <string>
#include <iostream>
using namespace std;

struct Monomial {
    double coef;
    int degree;
};

string inputPolynomial() {
    cout << "Enter polynomial (e.g. 2x^3 - 3x^2 + 4x - 5) :";
    string input;
    getline(cin, input);
    erase(input, ' ');
    return input;
}

vector<Monomial> parseMonomials(string& input) {
    vector<Monomial> monomials;
    const regex outerReg(R"(([+-]?(?:\d+(?:\.\d+)?)(?:x(\^(\d+))?)?|[+-]?x(\^(\d+))?))");
    const regex innerReg(R"(([+-]?\d*\.?\d*)x?\^?(\d*)?)");
    sregex_iterator iterator(input.begin(), input.end(), outerReg);
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
            cout << "match: " << matchStr << endl;
            cout << "  match[1] (coefStr): " << coefStr << endl;
            cout << "  match[2] (expStr): " << expStr << endl;
            cout << "  => coef = " << coef << ", degree = " << degree << endl;
        }
    }
    return monomials;
}

int findMaxDegree(const vector<Monomial>& poly) {
    int maxDeg = 0;
    for (const auto& m : poly)
        if (m.degree > maxDeg)
            maxDeg = m.degree;
    return maxDeg;
}

bool verifyMaxDegree(const int& degA, const int& degB) {
    cout << endl;
    cout << "deg(A) = " << degA << ", deg(B) = " << degB << endl;

    if (degA < degB) {
        cout << "Impossible to divide : deg(A) < deg(B)." << endl;
        cout << "Quotient Q(x) = 0, Remainder R(x) = A(x)" << endl;
        return false;
    }
    return true;
}

int main_polynomial_division_algorithm(){

    // Numerator input
    cout << "Enter the numerator polynomial :" << endl;
    string numerator_input = inputPolynomial();
    const vector<Monomial> numerator_monomials = parseMonomials(numerator_input);

    // Denominator input
    cout << endl;
    cout << "Enter the denominator polynomial :" << endl;
    string denominator_input = inputPolynomial();
    const vector<Monomial> denominator_monomials = parseMonomials(denominator_input);

    // Verify Numerator/Denominator degrees
    const int degA = findMaxDegree(numerator_monomials);
    const int degB = findMaxDegree(denominator_monomials);
    verifyMaxDegree(degA, degB);

    // Start Algorithm



    return 0;
}
