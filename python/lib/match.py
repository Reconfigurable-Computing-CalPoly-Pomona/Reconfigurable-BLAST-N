from collections import defaultdict
import json
import os
import sys
import tqdm
from typing import DefaultDict, Dict, List

"""
Internal
"""

class MatchStruct:
    """
    @brief The details for what a match has, a word, data indices, and query indices
    """
    def __init__(self, word: str=None, dindices: List[int]=None, qindices: List[int]=None):
        """
        @param word:     The exact match's word
        @param dindices: The data base indices where the word was found
        @param qindices: The query indices where the word was found
        """
        self.word: str = word
        self.data_indices: List[int] = dindices
        self.query_indices: List[int] = qindices
    
    def __str__(self):
        return str(self.__dict__)
    def __repr__(self):
        return self.__str__()
"""
External
"""

def get_exact_matches(query: Dict[str, Dict[str, List[int]]],
                      data: Dict[str, Dict[str, List[int]]]) -> Dict[str, Dict[str, List[MatchStruct]]]:
    """
    @brief: Find all the exact matches of each query word in all data bases \\
    @param query: The query to find exact matches for \\
                  {qname : {word : [indices], word : [indices], ...}, qname : ..., } \\
    @param data:  The data base to look for the query words in \\
                  {dname : {word : [indices], word : [indices], ...}, dname : ..., } \\
    @return: The exact matches with the indicies of both the query and data base recorded \\
             {dname : {qname : [MatchStruct(word, dindices, qindices), ...], ...}, ...}
    """

    exact_matches: Dict[str, Dict[str, List[MatchStruct]]] = {}

    # traverse each data set
    for d_name, d_word_dict in tqdm.tqdm(data.items()):

        matches: DefaultDict[str, List[MatchStruct]] = defaultdict(list)

        # traverse each query set
        for q_name, q_word_dict in query.items():
            # traverse each word in the query set
            for q_word, q_indice_list in q_word_dict.items():
                # the current word in the query is also in the data set
                if q_word in d_word_dict.keys():
                    matches[q_name].append(MatchStruct(word=q_word,
                                                       dindices=d_word_dict[q_word],
                                                       qindices=q_indice_list))
        
        # record if there were matches are found (not empty)
        if matches:
            exact_matches[d_name] = dict(matches) 

    return exact_matches
