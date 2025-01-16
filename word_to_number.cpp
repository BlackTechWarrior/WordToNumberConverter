#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

const map<string, double> wordToNumber = {
    {"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4},
    {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
    {"ten", 10}, {"eleven", 11}, {"twelve", 12}, {"thirteen", 13},
    {"fourteen", 14}, {"fifteen", 15}, {"sixteen", 16}, {"seventeen", 17},
    {"eighteen", 18}, {"nineteen", 19}, {"twenty", 20}, {"thirty", 30},
    {"forty", 40}, {"fifty", 50}, {"sixty", 60}, {"seventy", 70},
    {"eighty", 80}, {"ninety", 90},
    {"half", 0.5}, {"quarter", 0.25}, {"quarters", 0.25}, {"third", 1.0/3.0},
    {"thirds", 1.0/3.0}, {"fourth", 0.25}, {"fourths", 0.25}, {"eighth", 0.125},
    {"eighths", 0.125}, {"halfs", 0.5}, {"halves", 0.5}
};

const map<string, long long> magnitudes = {
    {"billion", 1000000000}, {"billions", 1000000000},
    {"million", 1000000}, {"millions", 1000000},
    {"thousand", 1000}, {"thousands", 1000},
    {"hundred", 100}, {"hundreds", 100}
};

struct ParsingState {
    bool isNegative = false;
    double total = 0;
    long long currentNumber = 0;
    long long currentMagnitude = 1;
    bool foundPoint = false;
};

class NumberConverter {
private:
    static vector<string> tokenize(const string& input) {
        string processed = input;
        transform(processed.begin(), processed.end(), processed.begin(), ::tolower);
        
        for (char& c : processed) {
            if (ispunct(c)) c = ' ';
        }

        stringstream ss(processed);
        vector<string> tokens;
        string token;
        
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static bool isFraction(const string& token) {
        return wordToNumber.find(token) != wordToNumber.end() && wordToNumber.at(token) < 1.0;
    }

    static string formatNumber(double number) {
        bool isNegative = number < 0;
        number = abs(number);

        long long wholePart = (long long)number;
        double decimalPart = number - wholePart;

        string wholeStr = to_string(wholePart);
        int insertPosition = wholeStr.length() - 3;
        while (insertPosition > 0) {
            wholeStr.insert(insertPosition, ",");
            insertPosition -= 3;
        }

        if (decimalPart > 0) {
            ostringstream oss;
            oss << fixed << setprecision(10);
            oss << decimalPart;
            string decimalStr = oss.str();
            decimalStr = decimalStr.substr(1);

            while (decimalStr.back() == '0') {
                decimalStr.pop_back();
            }

            wholeStr += decimalStr;
        }

        return (isNegative ? "-" : "") + wholeStr;
    }

public:
    static double convertToNumber(const vector<string>& tokens) {
        ParsingState state;
        size_t i = 0;

        if (!tokens.empty() && tokens[0] == "negative") {
            state.isNegative = true;
            i++;
        }

        for (; i < tokens.size(); i++) {
            const string& token = tokens[i];

            if (token == "and") {
                // Handle "and a [fraction]" pattern
                if (i + 2 < tokens.size() && tokens[i + 1] == "a" && isFraction(tokens[i + 2])) {
                    double fraction = wordToNumber.at(tokens[i + 2]);
                    state.total += state.currentNumber + fraction;
                    state.currentNumber = 0;
                    i += 2;
                    continue;
                }
                // Handle "and [fraction]" pattern
                if (i + 1 < tokens.size() && isFraction(tokens[i + 1])) {
                    double fraction = wordToNumber.at(tokens[i + 1]);
                    state.total += state.currentNumber + fraction;
                    state.currentNumber = 0;
                    i++;
                }
                continue;
            }

            if (isFraction(token)) {
                double fraction = wordToNumber.at(token);
                if (state.currentNumber == 0) {
                    state.total += fraction;
                } else {
                    state.total += state.currentNumber * fraction;
                }
                state.currentNumber = 0;
                state.currentMagnitude = 1;
                continue;
            }

            if (wordToNumber.find(token) != wordToNumber.end()) {
                state.currentNumber += wordToNumber.at(token);
                continue;
            }

            if (magnitudes.find(token) != magnitudes.end()) {
                long long magnitude = magnitudes.at(token);
                
                if (magnitude == 100) {
                    state.currentNumber *= magnitude;
                } else {
                    if (state.currentNumber == 0) state.currentNumber = 1;
                    
                    // Check if next token is also a magnitude
                    if (i + 1 < tokens.size() && magnitudes.find(tokens[i + 1]) != magnitudes.end()) {
                        long long nextMagnitude = magnitudes.at(tokens[i + 1]);
                        if (magnitude < nextMagnitude) {
                            state.currentNumber *= magnitude;
                            continue;
                        }
                    }
                    
                    state.currentNumber *= magnitude;
                    state.total += state.currentNumber;
                    state.currentNumber = 0;
                }
                continue;
            }

            if (token == "point") {
                if (state.currentNumber > 0) {
                    state.total += state.currentNumber * state.currentMagnitude;
                    state.currentNumber = 0;
                }
                
                double decimalPart = 0;
                double position = 0.1;
                i++;
                while (i < tokens.size() && wordToNumber.find(tokens[i]) != wordToNumber.end()) {
                    decimalPart += wordToNumber.at(tokens[i]) * position;
                    position *= 0.1;
                    i++;
                }
                state.total += decimalPart;
                break;
            }

            throw runtime_error("Unknown token: " + token);
        }

        state.total += state.currentNumber * state.currentMagnitude;
        return state.isNegative ? -state.total : state.total;
    }

    static string convert(const string& input) {
        try {
            vector<string> tokens = tokenize(input);
            if (tokens.empty()) {
                throw runtime_error("No valid number words found");
            }
            double result = convertToNumber(tokens);
            return formatNumber(result);
        } catch (const exception& e) {
            throw runtime_error(string("Conversion error: ") + e.what());
        }
    }
};

int main() {
    cout << "Enter a number in words (e.g., 'negative three thousand point five' or 'two and a half'): ";
    cout << "\nTo exit, enter 'n', 'N' or 'exit'\n";

    string input;
    while (true) {
        cout << ">";
        getline(cin, input);

        if (input == "n" || input == "N" || input == "exit") break;

        try {
            string result = NumberConverter::convert(input);
            cout << "Numeric value: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
