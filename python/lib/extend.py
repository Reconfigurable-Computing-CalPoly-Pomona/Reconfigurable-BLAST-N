from collections import defaultdict
from copy import copy
from typing import Dict, List
import tqdm

from .smith_waterman import smith_waterman
from .pairs import AdjacentPair

"""
Internal
"""

class Extended:
    def __init__(self, extended_pair, dindex, qindex):
        """
        @brief: Hold the extended pair and where it was found in the database.
        """
        self.extended_pair = extended_pair
        self.dindex = dindex
        self.qindex = qindex
    
    def __str__(self):
        return str(self.__dict__)
    def __repr__(self):
        return self.__str__()

"""
2 adjacent pairs
LEFT (lesser index)
    extend leftward one at a time and score
MIDDLE
    fill with gap characters
RIGHT (greater index)
    extend rightward one at a time and score
"""

def extend_and_score(pair: AdjacentPair,
                     query: str,
                     data: str,
                     match: int,
                     mismatch: int,
                     gap: int,
                     minscore: int,
                     score=False,
                     printing=True) -> Extended:
    """
    @brief: Extend an adjacent pair while scoring each extension. Stop extending if the extended word \\
            scores too low. \\
    @param pair:     The adjacent pair of words and their indices. \\
    @param query:    The section the adjacent pair is from. \\
    @param data:     The section the adjacent pair is going to be extended to. \\
    @param match:    Smith Waterman score for a match. \\
    @param mismatch: Smith Waterman score for a mismatch. \\
    @param gap:      Smith Waterman score for a gap. \\
    @param minscore: Minimum Smith Waterman score before the extending stops. \\
    @param score:    Perform Smith Waterman scoring on true. \\
    @param printing: Print the data after extending on true. \\
    @return: The string containing both pairs extended to the query from the database and the database index. \\
    """
    # find left-most indices
    dleftindex = min([pair.dindex1, pair.dindex2])
    qleftindex = min([pair.qindex1, pair.qindex2])
    drightindex = max([pair.dindex1, pair.dindex2])
    qrightindex = max([pair.qindex1, pair.qindex2])

    # build string
    qextended = query[qleftindex:qleftindex + pair.length]
    dextended = data[dleftindex:dleftindex + pair.length]

    # extend left
    qexindex = copy(qleftindex)
    dexindex = copy(dleftindex)
    while qexindex - 1 >= 0 and dexindex - 1 >= 0:
        qexindex -= 1
        dexindex -= 1
        qextended = query[qexindex] + qextended
        dextended = data[dexindex] + dextended
        if score:
            s = smith_waterman(qextended, dextended, match=match, mismatch=mismatch, gap=gap, just_score=True)
            if s < minscore:
                return None

    # the left-most index in the database
    dindex: int = copy(dexindex)
    qindex: int = copy(qexindex)
    
    # extend left pair to the right
    qexindex = qleftindex + pair.length - 1
    dexindex = dleftindex + pair.length - 1
    while qexindex + 1 < qrightindex and dexindex + 1 < drightindex:
        qexindex += 1
        dexindex += 1
        qextended = qextended + query[qexindex]
        dextended = dextended + data[dexindex] # OOB issue
    
    # extend right with gaps until qextended aligns with data
    while dexindex + 1 < drightindex:
        qexindex += 1
        dexindex += 1
        qextended = qextended + '-'
        dextended = dextended + data[dexindex]
    
    # append the right pair
    qextended = qextended + query[qrightindex:qrightindex + pair.length]
    dextended = dextended + data[drightindex:drightindex + pair.length]
    
    # extend right
    qexindex = qrightindex + pair.length - 1
    dexindex = drightindex + pair.length - 1
    while qexindex + 1 < len(query) and dexindex + 1 < len(data):
        qexindex += 1
        dexindex += 1
        qextended = qextended + query[qexindex]
        dextended = dextended + data[dexindex]
        if score:
            s = smith_waterman(qextended, dextended, match=match, mismatch=mismatch, gap=gap, just_score=True)
            if s < minscore:
                return None
    
    if printing:
        print(f"Data Ext:\t{dextended}")
        print(f"Quer Ext:\t{qextended}")
    
    return Extended(qextended, dindex, qindex)

"""
External
"""

def extend_filter(pairs: Dict[str, Dict[str, List[AdjacentPair]]],
                  query: Dict[str, str],
                  data: Dict[str, str],
                  minscore: int,
                  match: int,
                  mismatch: int,
                  gap: int) -> Dict[str, Dict[str, List[Extended]]]:
    """
    @brief: Given adjacent pairs, the database and query, extend the pairs from the query to the database.
    @param pairs:    The mapped data of adjacent pairs to extend and filter while extending.
    @param query:    The map of query names to their entire query sequence.
    @param data:     The map of data names to their entire data sequence.
    @param minscore: The minimum smith waterman score allowed before needing to remove the word.
    @param match:    The smith waterman score when two characters are the same.
    @param mismatch: The smith waterman score when two characters are not the same.
    @param gap:      The smith waterman score when there is a gap character.
    @return A map of data names to query names to a list of extended matches with their data base index.
    """
    result: Dict[str, Dict[str, List[Extended]]] = {}
    
    for dname, queries in tqdm.tqdm(pairs.items()):
        temp = defaultdict(list)
        for qname, adjacent_pairs in queries.items():
            for adjacent_pair in adjacent_pairs:
                extended_pair = extend_and_score(
                    adjacent_pair,
                    query[qname],
                    data[dname],
                    match,
                    mismatch,
                    gap,
                    minscore,
                    score=False,
                    printing=False
                )
                # the word scored above minscore and is not None
                if extended_pair:
                    # don't add repeat extended_pairs
                    if not any(extended_pair.dindex == ep.dindex for ep in temp[qname]):
                        temp[qname].append(extended_pair)
        # temp has items in it, so record it
        if temp:
            result[dname] = dict(temp)
    
    return result
