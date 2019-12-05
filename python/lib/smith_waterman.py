import numpy as np
import sys
from typing import Dict, List
import tqdm

"""
Internal
"""
GAP_CHAR = '-'

def score_alignment(alpha: str, beta: str, match: int, mismatch: int, gap: int) -> int:
    if alpha == beta:
        return match
    elif alpha == GAP_CHAR or beta == GAP_CHAR:
        return gap
    else:
        return mismatch

def smith_waterman(seq1: str,
                   seq2: str,
                   match: int=2,
                   mismatch: int=-1,
                   gap: int=-1,
                   just_score: bool=False,
                   printing: bool=False) -> int:
    """
    @brief: Perform the Smith-Waterman algorithm on sequence 1 and sequence 2 with \\
            the given scores \\
    @param seq1:       The first sequence to score with \\
    @param seq2:       The second sequence to score with \\
    @param match:      The score gained when two characters are a match \\
    @param mismatch:   The score gained when two characters are a mismatch \\
    @param gap:        The score gained when two characters need to be gapped \\
    @param just_score: When True, ignore creating a gapped alignment, return the score asap \\
    @param printing:   When True, print the score/point matrices, alignments, and sequences \\
    @return: The max score from performing a gapped alignment of sequence 1 and 2
    """

    # create the matrices to fill with alignment scores
    m: int = len(seq1)
    n: int = len(seq2)
    score_mat: np.array[np.array[np.int32]] = np.zeros((m + 1, n + 1), dtype=np.int32)
    point_mat: np.array[np.array[np.int32]] = np.zeros((m + 1, n + 1))
    
    # initialize vars for filling the matrices
    max_score: int = 0
    max_i: int = 0
    max_j: int = 0
    i: int = 0
    j: int = 0

    # fill score matrix
    while i < m:
        i += 1; j = 0
        while j < n:
            j += 1
            scores = [
                # min acceptable score
                0,
                # left score
                score_mat[i - 1][j] + gap,
                # up score
                score_mat[i][j - 1] + gap,
                # diagnal score
                score_mat[i - 1][j - 1] + score_alignment(seq1[i - 1], seq2[j - 1], match, mismatch, gap)
            ]

            # index (point mat) corresponds to dir
            # value (score mat) corresponds to greatest score @ that index
            # https://stackoverflow.com/questions/6193498/pythonic-way-to-find-maximum-value-and-its-index-in-a-list
            point_mat[i][j], score_mat[i][j] = max(
                # scores -> enumerate where (index, scores[index]) per item
                # sort by item, but you also get the index it's at
                enumerate(scores), key=lambda e: e[1]
            )
            # record high score
            if score_mat[i][j] >= max_score:
                max_i, max_j = i, j
                max_score = score_mat[i][j]            

    # ignore alignment if only the score is the goal
    if just_score:
        return max_score

    # back track variables
    aligned1: str = ''
    aligned2: str = ''
    i = max_i
    j = max_j

    # back track to get gapped alignment
    while point_mat[i][j] != 0:
        # diagnal movement
        if point_mat[i][j] == 3:
            a1 = seq1[i - 1]
            a2 = seq2[j - 1]
            i -= 1; j -= 1
        # up movement
        elif point_mat[i][j] == 2:
            a1 = GAP_CHAR
            a2 = seq2[j - 1]
            j -= 1
        # left movement
        elif point_mat[i][j] == 1:
            a1 = seq1[i - 1]
            a2 = GAP_CHAR
            i -= 1

        # append the chars to the aligned build str
        aligned1 += a1
        aligned2 += a2

    # reverse the strings to forward order
    aligned1 = aligned1[::-1]
    aligned2 = aligned2[::-1]

    # build strings for the output
    output_alignment: str = ''
    similarity_percent: float = 0

    # build gap aligned string
    for a1, a2 in zip(aligned1, aligned2):
        # char in both, append to output str
        if a1 == a2:
            output_alignment += a1
            similarity_percent += 1
        # no match, maybe gap append gap to output str
        elif a1 != a2 and a1 != GAP_CHAR and a2 != GAP_CHAR:
            output_alignment += GAP_CHAR
        # gap in both
        elif a1 == GAP_CHAR or a2 == GAP_CHAR:
            output_alignment += GAP_CHAR

    # similarity to percent
    similarity_percent = similarity_percent / len(aligned1) * 100

    # user outputs
    if printing:
        print()
        print(f'Score matrix:\n{score_mat}')
        print()
        print(f'Point matrix:\n{point_mat}')
        print()

        print(f'Sequence 1: {seq1}')
        print(f'Sequence 2: {seq2}')
        print()

        print(f'Similarity: {similarity_percent}')
        print(f'Max score: {max_score}')

        print()
        print('Aligned1: ', aligned1)
        print('Aligned2: ', aligned2)
        print()
        print('  Output: ', output_alignment)

    return similarity_percent

"""
External
"""

def smith_waterman_filter(data: Dict[str, Dict[str, List[int]]],
                          minscore: int,
                          match: int,
                          mismatch: int,
                          gap: int) -> Dict[str, Dict[str, List[int]]]:
    """
    @brief: Perform the Smith-Waterman algorithm on every sequence with itself, and delete it if it scores below the minscore \\
    @param data:     Data to use with format {'sequence name': {'split word': [indices], 'split word': [indices], ...}, ...} \\
    @param match:    The score gained when two characters are a match \\
    @param mismatch: The score gained when two characters are a mismatch \\
    @param gap:      The score gained when two characters need to be gapped \\
    Return the dict with low scoring words removed
    """
    # traverse each sequence
    for name, sequence in tqdm.tqdm(data.items()):
        # get the words from each element in each sequence
        for word in sequence.keys():
            # delete the sequence if it scored too low
            if smith_waterman(seq1=word, seq2=word, match=match, mismatch=mismatch, gap=gap, just_score=True) < minscore:
                del data[name][word]
    return data
