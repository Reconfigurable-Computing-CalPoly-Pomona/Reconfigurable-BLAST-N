from collections import defaultdict
from typing import Dict, List
import tqdm

import random

from .extend import Extended
from .smith_waterman import smith_waterman

class Sorted:
    def __init__(self, extended_pair, dindex, qindex, score):
        self.extended_pair = extended_pair
        self.dindex = dindex
        self.qindex = qindex
        self.score = score
    
    def __str__(self):
        return str(self.__dict__)
    
    def __repr__(self):
        return self.__str__()

def sort_filter(extended_pairs: Dict[str, Dict[str, List[Extended]]],
                query: Dict[str, str],
                match: int,
                mismatch: int,
                gap: int) -> Dict[str, Dict[str, List[Sorted]]]:
    result: Dict[str, Dict[str, List[Sorted]]] = {}
    for dname, queries in tqdm.tqdm(extended_pairs.items()):
        temp = defaultdict(list)
        for qname, epairs in queries.items():
            for epair in epairs:
                temp[qname].append(Sorted(
                    epair.extended_pair,
                    epair.dindex,
                    epair.qindex,
                    random.randint(1,100)))
                """smith_waterman(seq1=epair.extended_pair,
                                seq2=query[qname],
                                match=match,
                                mismatch=mismatch,
                                gap=gap,
                                just_score=True)))"""
            temp[qname].sort(key=lambda scored: scored.score)
        if temp:
            result[dname] = dict(temp)
    return result

