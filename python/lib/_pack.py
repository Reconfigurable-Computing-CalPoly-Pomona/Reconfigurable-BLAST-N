
from itertools import product
import os, json

lookup = {'A':'00', 'C':'01', 'G':'10', 'T':'11'}
pad_char = 'A'
# load/save the lookup tables in the same directory as this file
thisfilepath = os.path.dirname(os.path.abspath(__file__))
packing_file = thisfilepath + os.sep + 'data' + os.sep + 'packing_table.json'
unpacking_file = thisfilepath + os.sep + 'data' + os.sep + 'unpacking_table.json'

"""Create a lookup table to create a byte at a time"""
def create_lookup():

    # get all permutations with repeated elements of four letters ACGT
    letters = list(lookup.keys())
    perms = list(product(letters, repeat=len(letters)))
    # result lookup table
    table = {}

    # for each permutation
    for perm in perms:
        # convert the tuple of chars to a string
        p = ''.join(perm)
        builder = ''

        # build the 1s and 0s from each letter in the permutation
        for letter in p:
            builder += lookup[letter]

        # the string of 4 chars packed into 1 byte
        table[p] = hex(int(builder, 2))

    return table

# lookup for: keys -> vals and vals -> keys
p_exists, u_exists = os.path.isfile(packing_file), os.path.isfile(unpacking_file)

# save the lookup tables to .json format if they don't exist, else load them into memory
if not p_exists:
    # save to json
    packing_table = create_lookup()
    with open(packing_file, 'w') as p_json:
        json.dump(packing_table, p_json, indent=4, separators=(',', ': '))
else:
    # load from json
    with open(packing_file, 'r') as p_json:
        packing_table = json.load(p_json)

if not u_exists:
    # save to json
    unpacking_table = {value: key for key, value in packing_table.items()}
    with open(unpacking_file, 'w') as u_json:
        json.dump(unpacking_table, u_json, indent=4, separators=(',', ': '))
else:
    # load from json
    with open(unpacking_file, 'r') as u_json:
        unpacking_table = json.load(u_json)

"""Pack the given sequence into 1 byte
@param seq: MUST be a string of len 4 and only A, C, G, or T
"""
def pack_byte(seq_in):
    return packing_table[seq_in]

"""Unpacks the 8 bit hex to string of len 4 with A, C, G, or T"""
def unpack_byte(hex_in):
    return unpacking_table[hex_in]

"""Pack an entire sequence into a grouping of bytes"""
def pack_sequence(seq_in):
    # determine how many zeros to pad the packed sequence with
    length = len(seq_in)
    padding = 4 - length % 4
    pad_index = length - (4 - padding)
    packed_bytes = []

    # traverse the sequence without string iteration
    i = 0
    while i < length:
        # padding needs to take place: seq_in len not divisible by 4
        if i == pad_index:
            # put an 'A's until the value will be a byte
            extended = seq_in[i:i + 4 - padding] + pad_char * padding
            packed_bytes.append(pack_byte(extended))
        else:
            packed_bytes.append(pack_byte(seq_in[i:i + 4]))
        # increment
        i += 4

    return packed_bytes

"""Unpack an entire sequence into a string
@param packed_bytes: MUST be a list of 8-bit hex values
"""
def unpack_sequence(packed_bytes):
    builder = []

    for packed_byte in packed_bytes:
        builder.append(unpack_byte(packed_byte))

    return ''.join(builder)

"""

Test

"""

def test_seq_packing():
    seq = 'ATATATATGTCACGTCA'
    print('Sequence:\n', seq, sep='')
    packed = pack_sequence(seq)
    print('Packed:\n', packed, sep='')
    unpacked = unpack_sequence(packed)
    print('Unpacked:\n', unpacked, sep='')

def test_byte_packing():
    seq = 'AAAA'
    packed = pack_byte(seq)
    unpacked = unpack_byte(packed)

    print(seq)
    print(packed)
    print(unpacked)

if __name__ == '__main__':
    test_seq_packing()
    #test_byte_packing()
