from collections import defaultdict
import json
import os
import tqdm
from typing import Dict, List

if __name__ == '__main__':
    from split import split_to_words
else:
    from .split import split_to_words

"""
Internal
"""

def _dust(word: str, pattern_len: int=3):
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
    @return: The input dictionary without words which scored below the threshold
    """
    filtered_dictionary: Dict[str, Dict[str, List[int]]] = {}
    total_score: int = 0
    #threshold = threshold * (word_len - 2) / 2
    print(threshold)
    print("hello")

    # breaks words of 11 into subsequences of tuple triplets (length 3)
    for key, values in tqdm.tqdm(data.items()):
        for word, v in values.items():
            total_score = _dust(word)
            # words that score above the threshold will not be added to the filtered list
            if (total_score < threshold):
                filtered_dictionary[key] = {word: v}
    return filtered_dictionary

"""
Test
"""

if __name__ == '__main__':
    """
    thisfilepath = os.path.dirname(os.path.abspath(__file__))

    test_data_path: str = 'data/dusttestdata.json'
    filtered_data_path: str = 'data/filtereddictionary.json'

    # opening the file
    with open(test_data_path) as json_file:
        data = json.load(json_file)

    # {name : {word : [indices], word : [indices], ...}}
    filtered_dictionary: Dict[str, Dict[str, List[int]]] = {}
    threshold_score = 2
    dust(data, threshold_score)

    # making filtered_dictionary a *.json file
    with open(filtered_data_path, 'w') as filtered_json:
        json.dump(filtered_dictionary, filtered_json)
    """
    words = [
        'AACAACAACAAA',
        'AAAAAAAAAAAA',
        'AGCTCGATGTAG',
    ]
    results = [_dust(word) for word in words]
    for result in results:
        print(result)
