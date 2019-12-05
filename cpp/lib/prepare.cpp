#include <algorithm>
#include <fstream>

#include "prepare.hpp"
#include "split.hpp"
#include "../util/display.hpp"

namespace Blastn {

SequenceMap build_sequence(string path, char sep)
{
    SequenceMap result;
    string name, line;
    std::ifstream sequence_file{ path };

    if (!sequence_file.is_open()) {
        std::cerr << "Failure: Could not open: " << path << std::endl;
        std::exit(-1);
    }

    while (std::getline(sequence_file, line)) {
        // a new sequence name is found
        if (line[0] == sep) {
            // set the new name (sep char is length 1)
            name = line.substr(1, line.size());
            // no newlines in names
            name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());
            name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
            
            // pair the sequence name with an empty build string
            result[name] = "";
        }
        // the if statement MUST have been entered first
        else {
            // no newlines in entries
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            // append the next line of sequence data to the build string
            result[name].append(line);
        }
    }

    return result;
}

IndexedSequenceMap split_sequence(SequenceMap& data, u32 length)
{
    IndexedSequenceMap result;
    Progress progress{ data.size() };

    // traverse the sequence
    for (auto& name_seqmap : data) {
        // get all words and find their indices in that data set
        vector<string> words = split_to_words(name_seqmap.second, length);
        IndexedWordMap indexed_words;

        // map each word to all of their indices each time the word appears
        for (u32 i = 0; i < words.size(); i++) {

            // no newlines in sequences
            u64 back = words[i].size();
            if (words[i][back - 1] == '\r')
                back--;
            if (words[i][back - 1] == '\n')
                back--;

            string temp = words[i].substr(0, back);

            // insert the index if the item doesn't exist
            if (indexed_words.find(temp) == indexed_words.end())
                indexed_words[temp] = vector<u32>{};

            // append the index
            indexed_words[temp].push_back(i);
        }
        result[name_seqmap.first] = indexed_words;
        
        progress.update();
    }

    return result;
}

IndexedSequenceMap prepare_sequence(string path, u32 length, char sep)
{
    SequenceMap built_data = build_sequence(path, sep);
    IndexedSequenceMap indexed_data = split_sequence(built_data, length);

    return indexed_data;
}

u64 sequence_length(SequenceMap& data)
{
    u64 result = 0;
    for (auto& id_seq : data) {
        result += id_seq.second.size();
    }
    return result;
}

} // Blastn