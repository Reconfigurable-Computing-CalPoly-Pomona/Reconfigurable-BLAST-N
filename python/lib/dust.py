from collections import defaultdict
import json
import os
import tqdm
from typing import Dict, List

from .split import split_to_words

"""
Internal
"""

def dust(word: str, pattern_len: int=3):
    """
    @brief: Perform the DUST algorithm on a single word. \\
    @param word:        The word to perform the DUST algorithm on. \\
    @param pattern_len: The length of segments to check for self similarity in word. \\
    @return: The DUST score of the word's self similarity.
    """
    total_score = 0
    # triplet is a tuple of the 11-letter words split into subsequences of length 3 (triplet)
    triplets = split_to_words(word, pattern_len)
    record = defaultdict(int)
    for triplet in triplets:
        if triplet not in record:
            occurrance = triplets.count(triplet)
            record[triplet] = occurrance * (occurrance - 1) / 2
    total_score = sum(record.values()) / (len(word) - pattern_len)
    return total_score

"""
External
"""

def dust_filter(data: Dict[str, Dict[str, List[int]]], threshold: float, word_len: int) -> Dict[str, Dict[str, List[int]]]:
    """
    @brief: Perform the DUST algorithm on formatted data, and remove words which score below the threshold. \\
            It scores using a self similarity equation (refer to SDUST paper) and removes words under threshold. \\
    @param data:      The formatted data to perform DUST on \\
    @param threshold: The DUST score threshold in percent to remove words at \\
    @return: The input dictionary without words which scored below the threshold.
    """
    result: Dict[str, Dict[str, List[int]]] = {}
    total_score: int = 0

    # breaks words of 11 into subsequences of tuple triplets (length 3)
    for qname, words in tqdm.tqdm(data.items()):
        temp: Dict[str, List[int]] = {}
        for word, indices in words.items():
            total_score = dust(word)
            # words that score above the threshold will not be added to the filtered list
            if (total_score < threshold):
                temp[word] = indices
        if temp:
            result[qname] = temp
    return result
