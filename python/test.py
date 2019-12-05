from lib import *

def tdust():
    """
    thisfilepath = os.path.dirname(os.path.abspath(__file__))

    test_data_path: str = '../data/dusttestdata.json'
    filtered_data_path: str = '../data/filtereddictionary.json'

    # opening the file
    with open(test_data_path) as json_file:
        data = json.load(json_file)

    # {name : {word : [indices], word : [indices], ...}, ...}
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
    results = [dust(word) for word in words]
    for result in results:
        print(result)

def textend():
    # inputs
    query = 'AAAAAAAATCCT'
    data  = 'AAATCAAAAAAAAAAACT'
    word1 = 'TC'
    word2 = 'CT'
    qindex1 = query.find(word1)
    dindex1 = data.find(word1)
    qindex2 = query.find(word2, qindex1 + len(word1))
    dindex2 = data.find(word2, dindex1 + len(word1))
    pair = AdjacentPair(word1=word1,
                        word2=word2,
                        qindex1=qindex1,
                        dindex1=dindex1,
                        qindex2=qindex2,
                        dindex2=dindex2)
    print(qindex1, dindex1, qindex2, dindex2)
    print(f"Query:\t\t{query}")
    print(f"Data:\t\t{data}")
    extended = extend_and_score(pair,
                                query,
                                data,
                                match=2,
                                mismatch=-1,
                                gap=-1,
                                minscore=6,
                                score=True,
                                printing=True)
    print(f"Extended: {extended}")

def tmatch():
    
    dpath = '../data/data_small.fasta'
    qpath = '../data/query_small.fa'
    length = 3

    data =  prepare_sequence(path=dpath, length=length)
    query = prepare_sequence(path=qpath, length=length)

    matches = get_exact_matches(query, data)

    # print the matches
    if matches is None:
        print("Match error")
        return

    builder = ''
    for data_name, queries in matches.items():
        for query_name, match_structs in queries.items():
            for match_struct in match_structs:
                builder += f"'{match_struct.word}'\t" \
                        + f"{data_name}{match_struct.data_indices}\t" \
                        + f"{query_name}{match_struct.query_indices}\n"
    print(builder)

def tpair():
    dpath = '../data/data_small.fasta'
    qpath = '../data/query_small.fa'
    length = 3

    data =  prepare_sequence(path=dpath, length=length)
    query = prepare_sequence(path=qpath, length=length)

    matches = get_exact_matches(query, data)

    # print the matches
    if matches is None:
        print("Match error")
        return

    builder = ''
    for data_name, queries in matches.items():
        for query_name, match_structs in queries.items():
            for match_struct in match_structs:
                builder += f"'{match_struct.word}'\t" \
                        + f"{data_name}{match_struct.data_indices}\t" \
                        + f"{query_name}{match_struct.query_indices}\n"
    print(builder)
    print(pair_filter(matches, query = build_sequence(path = qpath)))

    

def tprepare():
    path = '../data/data_small.fasta'
    length = 11
    sep = '>'
    formatted = False

    prepare_sequence(path=path, length=length, sep=sep, formatted=formatted, write=True)

def tsmith_waterman():
    s1 = 'ATCGAC'
    s2 = 'ACCGAC'
    printing = False
    match = 2
    mismatch = -1
    gap = -1

    smith_waterman(s1, s2, match=match, mismatch=mismatch, gap=gap, just_score=False, printing=True)

def tsort():
    pass

def tsplit_to_words():
    
    sequence = 'ACGTACGTACGTACGTACGTACGT'
    length = 5

    print(sequence)
    output = split_to_words(iterable=sequence, length=length)

    if output is not None:
        # pretty print the kmers
        for i, letter in enumerate(output):
            print(' ' * i, end='')
            print(letter, end='')
            print()

if __name__ == '__main__':
    #tsplit_to_words()
    tsmith_waterman()
    #tsequence()
    #tmatch()
    #tpair()
    #tdust()
    #textend()
