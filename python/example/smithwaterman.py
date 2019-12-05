"""
Original code:
http://biorpy.blogspot.com/2015/04/bpy24-smith-waterman-algorithm-in-python.html

Video explaining how to understand the Smith-Waterman local alignment algorithm.
https://www.youtube.com/watch?v=YSK0eEoxn9I

Wikipedia article about Smith-Waterman algorithm.
https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
"""

# imports for python version stability of built in functions
from __future__ import division, print_function
import numpy as np

# scoring dict, adjust values for varying results
pt ={'match': 2, 'mismatch': -1, 'gap': -1}

"""
@param alpha: value from s1 to compare with s2
@param beta: value from s2 to compare with s1
@return: A point based on a match, gap, or mismatch

Determine if alpha and beta are the same, if they are return a Match Value.
Return a Gap Value if there is a gap (denoted by '-').
Return a Mismatch Value if there is no gap and alpha/beta are not the same.
"""
def mch(alpha, beta):
    if alpha == beta:
        return pt['match']
    elif alpha == '-' or beta == '-':
        return pt['gap']
    else:
        return pt['mismatch']

"""
@param s1: A given DNA sequence to align with s2.
@param s2: A given DNA sequence to align with s1.
@return: None

The Smith-Waterman method for locally aligning two DNA sequences.
Will print out the following:
    Score Matrix,
    Point Matrix,
    Traceback from
    Percent similarity of sequence s1 and s2,
    Highest scoring index in the Score Matrix,
    Aligned s1 with gaps denoted by '-' characters,
    Aligned s1 and s2 with gaps denoted by ' ' characters,
    Aligned s2 with gaps denoted by '-' characters

"""
def water(s1, s2):
    # length of sequence 1 and 2
    m, n = len(s1), len(s2)
    # scoring matrix H: matrix with all points added up
    H = np.zeros((m+1, n+1))
    # point matrix T: matrix with the point score for each index
    T = np.zeros((m+1, n+1))
    # greatest value scored in the scoring matrix H
    max_score = 0

    # traverse the Score and Pointer Matrix and s1 (traversed with i) / s2 (traversed with j)
    # traversal will start at (1, 1) because row/column 0 are all zeros as described by the SW algorithm
    for i in range(1, m + 1):
        for j in range(1, n + 1):

            # calculate the scores for the current index from the diagnal up left, up, and left indices
            sc_diag = H[i-1][j-1] + mch(s1[i-1], s2[j-1])
            sc_up = H[i][j-1] + pt['gap']
            sc_left = H[i-1][j] + pt['gap']

            # assign the current index to the highest scoring index from diagnal up left, up, or left
            # if the score for diag up left, up, or left was negative, insert 0 instead
            H[i][j] = max(0, sc_left, sc_up, sc_diag)

            # record scored values in Point Matrix T
            if H[i][j] == 0: T[i][j] = 0            # no direction was good (negative)
            if H[i][j] == sc_left: T[i][j] = 1      # left was best
            if H[i][j] == sc_up: T[i][j] = 2        # up was best
            if H[i][j] == sc_diag: T[i][j] = 3      # diag up left was best

            # check if there is a new highest value, if so, record it and its x,y index
            if H[i][j] >= max_score:
                max_i = i
                max_j = j
                max_score = H[i][j];

    # print the scoring matrix H
    print('H=\n',H,'\n')
    # print the point matrix T
    print('T=\n',T,'\n')
    # aligned 1/2 is a build string for the aligned characters with '-' characters to represent gaps
    align1, align2 = '', ''
    # use i, j instead of max_i, max_j
    i, j = max_i, max_j

    # Traceback from index where the greatest value is in the score matrix H
    while T[i][j] != 0:
        # matched char, so move diagnally up/left to the next character
        if T[i][j] == 3:
            a1 = s1[i-1]
            a2 = s2[j-1]
            i -= 1
            j -= 1
        # upwards gap char, so move up to next character
        elif T[i][j] == 2:
            a1 = '-'
            a2 = s2[j-1]
            j -= 1
        # horizontal gap char, so move left to next character
        elif T[i][j] == 1:
            a1 = s1[i-1]
            a2 = '-'
            i -= 1
        # print the traceback as it is occurring, showing a1 and a2 for each step
        print(f'Add ---> a1 = {a1}\t a2 = {a2}\n')
        align1 += a1
        align2 += a2

    # reverse the order of the strings align1 and align2
    align1 = align1[::-1]
    align2 = align2[::-1]
    # the string which will contain only aligned characters with spaces
    sym = ''
    # iden is used as a way of tracking the number of aligned characters
    iden = 0
    # Create a string of the aligned characters, replacing '-' characters
    # with ' ' (space) characters for displaying to console.
    for i in range(len(align1)):
        a1 = align1[i]
        a2 = align2[i]
        # if the char is in both, add it to build string and update the match count
        if a1 == a2:
            sym += a1
            iden += 1
        # no match, so put a space ' ' in the build string
        elif a1 != a2 and a1 != '-' and a2 != '-':
            sym += ' '
        # there was a gap in both, so put a space ' ' in the build string
        elif a1 == '-' or a2 == '-':
            sym += ' '

    # convert the num of aligned characters to a percent and print
    identity = iden / len(align1) * 100
    print('Identity = %f percent' % identity)
    # print the highest scoring value from the scoring matrix
    print('Score =', max_score)
    # print aligned1 with '-' for gaps
    print(align1)
    # print the combination of aligned 1/2 with '-' replaced with ' ' chars
    print(sym)
    # print aligned2 with '-' for gaps
    print(align2)

if __name__ == '__main__':
    # example with two sequences
    water('AGCACACA','ACACACTA')
    #water('AGCA', 'ACAC')
