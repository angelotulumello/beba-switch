/* Copyright (c) 2011, TrafficLab, Ericsson Research, Hungary
 * Copyright (c) 2012, CPqD, Brazil
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Ericsson Research nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef PACKET_HANDLE_STD_H
#define PACKET_HANDLE_STD_H 1

#include <stdbool.h>
#include <stdio.h>
#include "packets.h"
#include "match_std.h"
#include "oflib/ofl-structs.h"
#include "oflib/oxm-match.h"


/****************************************************************************
 * A handler processing a datapath packet for standard matches.
 ****************************************************************************/


/* The data associated with the handler */



struct packet_handle_std {

   struct packet              *pkt;

   // struct ofl_match  pkt_match;  /* Match fields extracted from the packet
   //      			    are also stored in a match structure
   //      			    for convenience */

   struct protocols_std proto;	   /* Match fields extracted from the packet */

   struct oxm_packet_info info;

   bool  valid;			/* Set to true if the handler data is valid.
			     	   if false, it is revalidated before
                             	   executing any methods. */

   bool	 table_miss;		/* Packet was matched
   			     	   against table miss flow*/
};



/* Revalidates the handler data */
void
packet_handle_std_validate(struct packet_handle_std *handle);

/* Creates a handler */

static inline
void packet_handle_std_init(struct packet_handle_std *handle, struct packet *pkt)
{
	// FLAT
	// handle->proto = xmalloc(sizeof(struct protocols_std));

	handle->pkt = pkt;

	// hmap_init(&handle->pkt_match.match_fields);

	handle->valid = false;
	packet_handle_std_validate(handle);
}


static inline
struct packet_handle_std *
packet_handle_std_create(struct packet *pkt)
{
	struct packet_handle_std *handle = xmalloc(sizeof(struct packet_handle_std));
	// FLAT
	// handle->proto = xmalloc(sizeof(struct protocols_std));

	handle->pkt = pkt;

	// hmap_init(&handle->pkt_match.match_fields);

	handle->valid = false;
	packet_handle_std_validate(handle);
	return handle;
}


static inline
struct packet_handle_std *
packet_handle_std_clone(struct packet *pkt, struct packet_handle_std *handle UNUSED)
{
    struct packet_handle_std *clone = xmalloc(sizeof(struct packet_handle_std));

    // FLAT
    // clone->proto = xmalloc(sizeof(struct protocols_std));
    //

    clone->pkt = pkt;

    // hmap_init(&clone->pkt_match.match_fields);

    clone->valid = false;
    packet_handle_std_validate(clone);
    return clone;
}

static inline
void
packet_handle_std_destroy(struct packet_handle_std *handle) {

    (void)handle;

    // struct ofl_match_tlv * iter, *next;

    // if (unlikely(handle->pkt_match.dirty))
    // {
    // HMAP_FOR_EACH_SAFE(iter, next, struct ofl_match_tlv, hmap_node, &handle->pkt_match.match_fields){
    //     if (iter->ownership) {
    //     	free(iter->value);
    //     	free(iter);
    //     }
    // }
    //     handle->pkt_match.dirty = false;
    // }

    // FLAT
    // free(handle->proto);

    // hmap_destroy(&handle->pkt_match.match_fields);

    // FLAT
    // free(handle);
}


/* Returns true if the TTL fields of the supported protocols are valid. */
bool
packet_handle_std_is_ttl_valid(struct packet_handle_std *handle);

/* Returns true if the packet is a fragment (IPv4). */

static inline bool
packet_handle_std_is_fragment(struct packet_handle_std *handle)
{
    packet_handle_std_validate(handle);
    return ((handle->proto.ipv4 != NULL) && IP_IS_FRAGMENT(handle->proto.ipv4->ip_frag_off));
}


/* Returns true if the packet matches the given standard match structure. */
static inline
bool packet_handle_std_match(struct packet_handle_std *handle, struct ofl_match *match, struct ofl_exp *exp) {

    if (unlikely(!handle->valid)) {
        packet_handle_std_validate(handle);
        if (unlikely(!handle->valid)) {
            return false;
        }
    }
    return packet_match_pkt(match, &handle->info, exp);
}

/* Converts the packet to a string representation */
char *
packet_handle_std_to_string(struct packet_handle_std *handle);

void
packet_handle_std_print(FILE *stream, struct packet_handle_std *handle);



#endif /* PACKET_HANDLE_STD_H */
