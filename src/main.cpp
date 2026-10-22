#include "LanguageModel.h"
#include "TextGenerator.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }

        double alpha = 0.0;
        size_t idx = 0;

        // Parse optional -a alpha argument
        if (idx < args.size() && args[idx] == "-a") {
            if (idx + 1 >= args.size()) {
                throw std::invalid_argument("option -a requires a non-negative number");
            }
            try {
                size_t processed = 0;
                alpha = std::stod(args[idx + 1], &processed);
                if (processed != args[idx + 1].length() || alpha < 0.0) {
                    throw std::invalid_argument("invalid alpha value");
                }
            } catch (...) {
                throw std::invalid_argument("option -a requires a non-negative number");
            }
            idx += 2;
        } else if (idx < args.size() && !args[idx].empty() && args[idx][0] == '-') {
            throw std::invalid_argument("unknown option '" + args[idx] + "'");
        }

        // Must have exactly 3 positional arguments left: k, training_file, output_length
        if (args.size() - idx != 3) {
            throw std::invalid_argument("invalid number of arguments");
        }

        std::string k_str = args[idx];
        std::string filename = args[idx + 1];
        std::string len_str = args[idx + 2];

        int k = 0;
        try {
            size_t processed = 0;
            k = std::stoi(k_str, &processed);
            if (processed != k_str.length() || k < 1) {
                throw std::invalid_argument("k must be a positive integer");
            }
        } catch (...) {
            throw std::invalid_argument("k must be a positive integer");
        }

        int output_length = 0;
        try {
            size_t processed = 0;
            output_length = std::stoi(len_str, &processed);
            if (processed != len_str.length() || output_length < k) {
                throw std::invalid_argument("output length must be an integer >= k");
            }
        } catch (...) {
            throw std::invalid_argument("output length must be an integer >= k");
        }

        // Train Language Model
        LanguageModel model(k, alpha);
        model.train(filename);

        // Generate Text
        TextGenerator generator(model);
        std::string generated_text = generator.generate(output_length);

        std::cout << generated_text << '\n';

    } catch (const std::exception& e) {
        std::cerr << "slm: " << e.what() << '\n';
        return 1;
    }

    return 0;
}