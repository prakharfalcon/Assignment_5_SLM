#ifndef LANGUAGE_MODEL_H
#define LANGUAGE_MODEL_H

#include <string>
#include <map>
#include <set>
#include <vector>

class LanguageModel {
private:
    int k;
    double alpha;
    
    std::map<std::string, int> kgram_counts;
    std::map<std::string, std::map<char, int>> transition_counts;
    std::set<char> alphabet;

public:
    LanguageModel(int k_val, double alpha_val = 0.0);
    
    void train(const std::string& filename);
    
    int get_k() const;
    double get_alpha() const;
    const std::set<char>& get_alphabet() const;
    const std::map<std::string, int>& get_kgram_counts() const;
    
    double get_probability(const std::string& kgram, char c) const;
    std::map<char, int> get_transitions_for(const std::string& kgram) const;
    bool has_kgram(const std::string& kgram) const;
};

#endif