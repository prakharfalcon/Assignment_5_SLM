#include "TextGenerator.h"
#include <vector>
#include <numeric>
#include <stdexcept>

TextGenerator::TextGenerator(const LanguageModel& lm) 
    : model(lm), rng(std::random_device{}()) {}

std::string TextGenerator::select_initial_kgram() const {
    const auto& counts = model.get_kgram_counts();
    if (counts.empty()) {
        throw std::runtime_error("model has no k-grams trained.");
    }

    std::vector<std::string> kgrams;
    std::vector<int> weights;

    for (const auto& pair : counts) {
        kgrams.push_back(pair.first);
        weights.push_back(pair.second);
    }

    std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
    return kgrams[dist(rng)];
}

char TextGenerator::sample_next_char(const std::string& current_kgram) const {
    double alpha = model.get_alpha();
    const auto& alphabet = model.get_alphabet();

    if (alpha == 0.0) {
        auto transitions = model.get_transitions_for(current_kgram);
        if (transitions.empty()) {
            return '\0'; // Stuck: no transitions exist
        }

        std::vector<char> chars;
        std::vector<int> weights;
        for (const auto& pair : transitions) {
            chars.push_back(pair.first);
            weights.push_back(pair.second);
        }

        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        return chars[dist(rng)];
    } else {
        // alpha > 0: sample over full alphabet Sigma using smoothed probabilities
        std::vector<char> chars(alphabet.begin(), alphabet.end());
        std::vector<double> probs;

        for (char c : chars) {
            probs.push_back(model.get_probability(current_kgram, c));
        }

        std::discrete_distribution<size_t> dist(probs.begin(), probs.end());
        return chars[dist(rng)];
    }
}

std::string TextGenerator::generate(size_t length) const {
    int k = model.get_k();
    if (length < static_cast<size_t>(k)) {
        throw std::invalid_argument("output length must be at least k.");
    }

    std::string result = select_initial_kgram();
    
    while (result.length() < length) {
        std::string current_kgram = result.substr(result.length() - k, k);
        char next_char = sample_next_char(current_kgram);

        if (next_char == '\0') {
            break; // Stop early if stuck
        }

        result.push_back(next_char);
    }

    return result;
}