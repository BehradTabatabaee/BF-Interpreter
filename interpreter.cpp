#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

const size_t TAPE_SIZE = 10000;

void runBrainfuck(const string& input) {
    vector<char> tape(TAPE_SIZE, 0);
    size_t pointer = 0;
    size_t unmatchedBrackets = 0;

    for (size_t i = 0; i < input.length(); i++) {
        char command = input[i];
        switch (command) {
        case '>':
            if (pointer >= TAPE_SIZE - 1) {
                throw out_of_range("Pointer moved out of tape bounds.");
            }
            pointer++;
            break;

        case '<':
            if (pointer == 0) {
                throw out_of_range("Pointer moved out of tape bounds.");
            }
            pointer--;
            break;

        case '+':
            tape[pointer]++;
            break;

        case '-':
            tape[pointer]--;
            break;

        case '.':
            cout << tape[pointer];
            break;

        case ',':
            char inputChar;
            cin.get(inputChar);
            tape[pointer] = inputChar;
            break;

        case '[':
            if (tape[pointer] == 0) {
                size_t unmatched = 1;
                while (unmatched > 0) {
                    i++;
                    if (i >= input.length()) {
                        throw runtime_error("Unmatched '[' encountered.");
                    }
                    if (input[i] == '[') unmatched++;
                    if (input[i] == ']') unmatched--;
                }
            }
            break;

        case ']':
            if (tape[pointer] != 0) {
                size_t unmatched = 1;
                while (unmatched > 0) {
                    i--;
                    if (i >= input.length()) {
                        throw runtime_error("Unmatched ']' encountered.");
                    }
                    if (input[i] == '[') unmatched--;
                    if (input[i] == ']') unmatched++;
                }
            }
            break;

        default:
            throw runtime_error("Invalid character!");
        }
    }

    unmatchedBrackets = 0;
    for (char command : input) {
        if (command == '[') unmatchedBrackets++;
        if (command == ']') unmatchedBrackets--;
    }
    if (unmatchedBrackets != 0) {
        throw runtime_error("Unmatched brackets");
    }
}

std::string filterBrainfuckCode(const std::string& input) {
    std::string filtered;
    for (char c : input) {
        if (std::string("><+-.,[]").find(c) != std::string::npos) {
            filtered += c;
        }
    }
    return filtered;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <brainfuck_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    code = filterBrainfuckCode(code);

    try {
        runBrainfuck(code);
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
