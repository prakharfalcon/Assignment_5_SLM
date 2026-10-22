# Small Language Model (SLM)

We implement a Small Language Model (SLM) in C++ that generates pseudo-random text modeled on an input training text document. The model uses a k-gram algorithm with Laplace smoothing.


We used 'make' to compile the model
An example of how to use the model is:
    ./slm [-a alpha] k training_file output_length
    which in practice looks like:
    ./slm -a 0.5 4 moby_dick.txt 500

The LanguageModel reads the training files. Then tracks the k-gram frequencies and calculates the Laplace smoothed probabilities.
TextGenerator handlesinitial k-gram selection and random character samples.