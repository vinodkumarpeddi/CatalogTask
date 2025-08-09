#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <algorithm> // for remove_if


using namespace std;

// Convert a string number from a given base to decimal integer
long long baseToDecimal(const string& value, int base) {
    long long result = 0;
    for (char ch : value) {
        int digit = (ch >= '0' && ch <= '9') ? ch - '0' : (toupper(ch) - 'A' + 10);
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation to find P(0)
double lagrangeConstantTerm(const vector<pair<long long, long long>>& points) {
    double c = 0.0;
    int k = points.size();
    for (int i = 0; i < k; i++) {
        double term = points[i].second;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        c += term;
    }
    return c;
}

int main() {
    ifstream file("testcases.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open testcases.json\n";
        return 1;
    }

    string line;
    int n = 0, k = 0;
    vector<pair<long long, long long>> points;

    while (getline(file, line)) {
        // Remove spaces
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Parse n and k
        if (line.find("\"n\":") != string::npos) {
            n = stoi(line.substr(line.find(":") + 1));
        } else if (line.find("\"k\":") != string::npos) {
            k = stoi(line.substr(line.find(":") + 1));
        }
        // Parse each root entry
        else if (line.size() > 1 && line[0] == '\"' && isdigit(line[1])) {
            // Extract x from key
            size_t posQuote = line.find("\"", 1);
            long long x = stoll(line.substr(1, posQuote - 1));

            // Read base line
            string baseLine, valueLine;
            getline(file, baseLine);
            baseLine.erase(remove_if(baseLine.begin(), baseLine.end(), ::isspace), baseLine.end());
            int base = stoi(baseLine.substr(baseLine.find(":\"") + 2));

            // Read value line
            getline(file, valueLine);
            valueLine.erase(remove_if(valueLine.begin(), valueLine.end(), ::isspace), valueLine.end());
            string valStr = valueLine.substr(valueLine.find(":\"") + 2);
            if (valStr.back() == ',' || valStr.back() == '}')
                valStr.pop_back();
            if (!valStr.empty() && valStr.back() == '\"')
                valStr.pop_back();

            long long y = baseToDecimal(valStr, base);
            points.push_back({x, y});
        }
    }

    // Only take first k points
    if ((int)points.size() > k) {
        points.resize(k);
    }

    double c = lagrangeConstantTerm(points);
    cout << "Secret (c) = " << llround(c) << "\n";

    return 0;
}
