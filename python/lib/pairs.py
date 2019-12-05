from typing import Dict, List, DefaultDict
import tqdm
from collections import defaultdict
from .match import MatchStruct

"""
Internal
"""
class MatchSingleton:
    def __init__(self, word: str, dindex: int, qindex: int):
        """
        @brief: Hold both words from the pair, how long they are, and where they both are in the query and database.
        """
        self.word: str = word
        self.dindex: int = dindex
        self.qindex: int = qindex

    def __str__(self):
        return str(self.__dict__)
    
    def __repr__(self):
        return self.__str__()


class AdjacentPair:
    def __init__(self, word1: str, word2: str, dindex1: int, qindex1: int, dindex2: int, qindex2: int):
        """
        @brief: Hold both words from the pair, how long they are, and where they both are in the query and database.
        """
        self.word1: str = word1
        self.word2: str = word2
        self.length = len(self.word1)
        self.dindex1: int = dindex1
        self.qindex1: int = qindex1
        self.dindex2: int = dindex2
        self.qindex2: int = qindex2

    def __str__(self):
        return str(self.__dict__)
    
    def __repr__(self):
        return self.__str__()

def append(flattened: List[MatchSingleton], result: list, query_len: int):
    for index1, value1 in enumerate(flattened):
        for index2 in range (index1 + 1, len(flattened)):
            if abs(value1.dindex - flattened[index2].dindex) >= len(value1.word) \
                and abs(value1.dindex - flattened[index2].dindex) <= query_len - len(value1.word):
                
                result.append(AdjacentPair(word1   = value1.word,       word2   = flattened[index2].word,
                                           dindex1 = value1.dindex, qindex1 = value1.qindex,
                                           dindex2 = flattened[index2].dindex, qindex2 = flattened[index2].qindex))
                break

def flatten(match_structs: List[MatchStruct], query_len: int):
    flat = []
    result: List[AdjacentPair] = []
    for match in match_structs:
        for dindex in match.data_indices:
            for qindex in match.query_indices:
                flat.append(MatchSingleton(match.word, dindex, qindex))
    flat.sort(key=lambda m: m.dindex)
    append(flat, result, query_len)
    
    return result

"""
External
"""

def pair_filter(matches: Dict[str, Dict[str, List[MatchStruct]]],
                query: Dict[str, str]) -> Dict[str, Dict[str, List[AdjacentPair]]]:

    filtered_pairs: Dict[str, Dict[str, List[AdjacentPair]]] = {}
    
    for dname, queries in tqdm.tqdm(matches.items()):
        pairs = defaultdict(list)
        for qname, match_structs in queries.items():
            for pair in flatten(match_structs, len(query[qname])):
                #                                    distance threshold
                if abs(pair.dindex1 - pair.dindex2) <= len(query[qname]) - pair.length \
                    or abs(pair.qindex1 - pair.qindex2) >= pair.length \
                    or abs(pair.dindex1 - pair.dindex2) >= pair.length:
                    # only append if the pair isn't already recorded
                    if not any(pair.dindex1 == p.dindex1 for p in pairs[qname]):
                        pairs[qname].append(pair)
        if pairs:
            filtered_pairs[dname] = dict(pairs)
    return filtered_pairs
