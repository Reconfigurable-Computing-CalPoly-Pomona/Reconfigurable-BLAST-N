from typing import Dict

def filter_short(query: Dict[str, str], data: Dict[str, str]) -> Dict[str, str]:
    """
    If no single query sequence is shorter than a particular data sequence, remove the data sequence.
    Don't record data sequences where the query is longer than the data

    Possibly don't need this function.
    """
    result: Dict[str, str] = {}
    for dname, dseq in data:
        found = False
        for _, qseq in query:
            # a the query is shorter than a data sequence
            if len(qseq) <= len(dseq):
                found = True
                break
        if found:
            result[dname] = dseq
    return result
