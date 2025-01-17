/*
 Copyright 2016-2020 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "common/comm/atl_tag.hpp"
#include "exec/exec.hpp"

void ccl_atl_tag::print() {
    LOG_INFO("\n",
             "\ntag_bits:      ",
             tag_bits,
             "\nmax_tag:       ",
             max_tag,
             "\npof2(max_tag): ",
             ccl_pof2(max_tag),
             "\nmax_tag_mask:  ",
             max_tag_mask,
             "\n");
}

uint64_t ccl_atl_tag::create(ccl_comm_id_t comm_id,
                             int rank,
                             ccl_sched_id_t sched_id,
                             ccl_op_id_t op_id) {
    uint64_t tag = 0;

    if (tag_bits == 32) {
        tag |= (((uint64_t)op_id) << op_id_shift) & op_id_mask;
        tag |= (((uint64_t)sched_id) << sched_id_shift) & sched_id_mask;
    }
    else if (tag_bits == 64) {
        tag |= (((uint64_t)op_id) << op_id_shift) & op_id_mask;
        tag |= (((uint64_t)sched_id) << sched_id_shift) & sched_id_mask;
        tag |= (((uint64_t)rank) << rank_shift) & rank_mask;
        tag |= (((uint64_t)comm_id) << comm_id_shift) & comm_id_mask;
    }
    else {
        CCL_ASSERT(0);
    }

    if (tag > max_tag)
        tag &= max_tag_mask;

    LOG_DEBUG("tag ",
              tag,
              " (comm_id: ",
              comm_id,
              ", rank ",
              rank,
              ", sched_id: ",
              sched_id,
              ", op_id: ",
              (int)op_id,
              ")");

    CCL_THROW_IF_NOT(tag <= max_tag,
                     "unexpected tag value ",
                     tag,
                     ", max_tag ",
                     max_tag,
                     " (comm_id: ",
                     comm_id,
                     ", rank ",
                     rank,
                     ", sched_id: ",
                     sched_id,
                     ", op_id: ",
                     (int)op_id,
                     ")");

    return tag;
}
