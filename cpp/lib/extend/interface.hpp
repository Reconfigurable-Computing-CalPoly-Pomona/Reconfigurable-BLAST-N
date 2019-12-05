#pragma once

#include "../../util/types.hpp"

namespace Blastn {

struct PackedFmt {
    PackedFmt(const char *uart_path);
    ~PackedFmt();

    /**
     * @brief Fill PackedFmt's data with each character of query and subject into 4 letters per byte
     * @param query        The query sequence which is being extended and needs scoring
     * @param subject      The subject sequence which the query is being extended to
     * @param query_size   The length of the query
     * @param subject_size The length of the subject
     */
    void pack(const char *query, const char *subject, u32 size);
    void write();
    void read();

    u32 usize;
    s32 result;
    byte size[4];
    byte gap_index[4];
    byte gap_count[4];
    byte query[SW_MAX_BYTES];
    byte subject[SW_MAX_BYTES];
    byte buf[4 + 4 + 4 + 2* SW_MAX_BYTES];
};

}