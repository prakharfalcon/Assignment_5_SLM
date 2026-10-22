#include "LanguageModel.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

LanguageModel::LanguageModel(int k_val, double alpha_val) : k(k_val), alpha(alpha_val) {
    if (k < 1) {
        throw std::invalid_argument("k must be a positive integer.");
    }
    if (alpha < 0.0) {
        throw std::invalid_argument("alpha must be non-negative.");
    }
}

void LanguageModel::train(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("cannot open file '" + filename + "'");
    }

    // Read full file content including all spaces and newlines
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    // Populate alphabet with all unique characters in training text
    for (char c : text) {
        alphabet.insert(c);
    }

    // Check if training text is long enough to have at least one k-gram followed by a character
    if (text.length() < static_cast<size_t>(k + 1)) {
        throw std::runtime_error("training text too short for k = " + std::to_string(k));
    }

    // Process all k-grams followed by a character
    for (size_t i = 0; i <= text.length() - k - 1; ++i) {
        std::string kgram = text.substr(i, k);
        char next_char = text[i + k];

        kgram_counts[kgram]++;
        transition_counts[kgram][next_char]++;
    }
}

int LanguageModel::get_k() const {
    return k;
}

double LanguageModel::get_alpha() const {
    return alpha;
}

const std::set<char>& LanguageModel::get_alphabet() const {
    return alphabet;
}

const std::map<std::string, int>& LanguageModel::get_kgram_counts() const {
    return kgram_counts;
}

bool LanguageModel::has_kgram(const std::string& kgram) const {
    return kgram_counts.find(kgram) != kgram_counts.end();
}

std::map<char, int> LanguageModel::get_transitions_for(const std::string& kgram) const {
    auto it = transition_counts.find(kgram);
    if (it != transition_counts.end()) {
        return it->second;
    }
    return {};
}

double LanguageModel::get_probability(const std::string& kgram, char c) const {
    int nw = 0;
    auto k_it = kgram_counts.find(kgram);
    if (k_it != kgram_counts.end()) {
        nw = k_it->second;
    }

    int nwc = 0;
    auto t_it = transition_counts.find(kgram);
    if (t_it != transition_counts.end()) {
        auto c_it = t_it->second.find(c);
        if (c_it != t_it->second.end()) {
            nwc = c_it->second;
        }
    }

    size_t alphabet_size = alphabet.size();
    
    // Formula: (n(w, c) + alpha) / (n(w) + alpha * |Sigma|)
    double denominator = nw + alpha * alphabet_size;
    if (denominator == 0.0) {
        return 0.0;
    }

    return (nwc + alpha) / denominator;
}