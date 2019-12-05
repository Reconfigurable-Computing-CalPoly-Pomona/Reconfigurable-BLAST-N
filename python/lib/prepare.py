from collections import defaultdict
from copy import copy
import json
import os
import sys
import tqdm
from typing import DefaultDict, Dict, List

from .split import split_to_words

"""
Internal
"""

def build_sequence(path: str, sep: str='>') -> Dict[str, str]:
    """
    @brief: Given a *.fa or *.fasta file, turn it into *.json format \\
            where each sequence name maps to its sequence \\
    @param path: The *.fa or *.fasta file path \\
    @param sep:  The seperator character which denotes a different sequence \\
    @return: A dictionary where each sequence name is mapped to its sequence \\
             {'sequence name' : 'sequence'}
    """
    seq_file = open(path, 'r')

    # {'sequence name' : 'sequence letters'}
    result: Dict[str, str] = {}
    name: str = ''

    # put all sequence letters into a single string for each sequence
    for line in seq_file.readlines():
        # a sequence name is found
        if line[0] == sep:
            # start after the seperator and remove newlines
            name = copy(line.rstrip('\n\r')[len(sep):])
            result[name] = []
        # sequence letters are found
        else:
            # remove newlines and append
            result[name].append(line.rstrip('\n\r'))

    # change ['sequence'] -> 'sequence'
    for name, sequence in result.items():
        result[name] = ''.join(sequence)

    seq_file.close()

    return result

def split_sequence(data: Dict[str, str], length: int=11) -> Dict[str, Dict[str, List[int]]]:
    """
    @brief: Given a dictionary of {'name' : 'sequence'}, split \\
            the sequence into words of a given length \\
    @param data:   A dictionary of {'sequence name': 'sequence string', ...} \\
    @param length: The length of words to split each sequence string into \\
    @return: A dictionary where all sequences are mapped to all of their split words and the corresponding \\
             indices where each word appears in the sequence \\
             {'sequence name': {'split word': [indices], 'split word': [indices], ...}, ...}
    """
    result: Dict[str, Dict[str, List[int]]] = {}
    
    # traverse the sequence
    for name, sequence in tqdm.tqdm(data.items()):
        # get all the words and find their indices in that data set
        words_with_indices: DefaultDict[str, List[int]] = defaultdict(list)
        words: list = split_to_words(iterable=sequence, length=length)
        
        # map each word to all of their indices each time the word appears
        for i, word in enumerate(words):
            words_with_indices[word].append(i)
        
        # cast each DefaultDict to a standard Dict to ensure proper return type
        result[name] = dict(words_with_indices)

    return result

"""
External
"""

def prepare_sequence(path: str, length: int=11, sep: str='>', write: bool=False, formatted: bool=False) -> Dict[str, Dict[str, List[int]]]:
    """
    @brief: Read in a *.fa or *.fasta file, split it by sequence \\
            name and split the sequence into words of a given length \\
    @param path:      The *.fa or *.fasta file path \\
    @param length:    The word length to split the sequences into \\
    @param sep:       The seperator character which denotes a different sequence \\
    @param write:     Write the output dictionary to a *.json file
    @param formatted: If write is true, write the *.json on many lines with indentation, else one line \\
    @return: The *.fa or *.fasta file formatted into all of its unique words matched with all indices the word appears at \\
             {'sequence name': {'split word': [indices], 'split word': [indices], ...}, ...}
    """
    # read data to a dict {'name' : 'sequence'}
    built_data: Dict[str, str] = build_sequence(path=path, sep=sep)
    # {'sequence name': {'split word': [indices], 'split word': [indices], ...}, ...}
    split_data: Dict[str, Dict[str, List[int]]] = split_sequence(data=built_data, length=length)

    # write to *.json file
    if write:
        with open(path + '.json', 'w') as d_json:
            if formatted:
                json.dump(split_data, d_json, indent=4, separators=(',', ': '))
            else:
                json.dump(split_data, d_json)
        
    return split_data
