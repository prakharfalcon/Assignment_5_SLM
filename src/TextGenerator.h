#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include "LanguageModel.h"
#include <string>
#include <random>

class TextGenerator {
private:
    const LanguageModel& model;
    mutable std::mt19937 rng;

    // Pick starting k-gram according to k-gram frequencies
    std::string select_initial_kgram() const;

    // Sample next character given current k-gram
    // Returns '\0' if generator gets stuck (alpha = 0 and no transitions)
    char sample_next_char(const std::string& current_kgram) const;

public:
    explicit TextGenerator(const LanguageModel& lm);

    // Generates output text up to 'length' characters
    std::string generate(size_t length) const;
};

#endif