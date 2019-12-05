import sys

"""
External
"""

def split_to_words(iterable: iter, length: int) -> list:
    """
    @brief: Split an iterable into all possible slices of given length \\
    @param iterable: A Python iterable to split into a list of words of length \\
    @param length:   The length of the words to extract from the iterable \\
    @return: A list of words of given length split from the iterable
    """
    num_words: int = len(iterable) - length + 1
    words: list = []
    for i in range(num_words):
        words.append(iterable[i:i + length])
    
    return words
