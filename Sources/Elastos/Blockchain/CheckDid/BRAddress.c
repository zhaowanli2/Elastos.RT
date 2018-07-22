//
//  BRAddress.c
//
//  Created by Aaron Voisine on 9/18/15.
//  Copyright (c) 2015 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "BRAddress.h"
#include "BRInt.h"
#include <inttypes.h>
#include <assert.h>

#define VAR_INT16_HEADER  0xfd
#define VAR_INT32_HEADER  0xfe
#define VAR_INT64_HEADER  0xff
#define MAX_SCRIPT_LENGTH 0x100 // scripts over this size will not be parsed for an address

// reads a varint from buf and stores its length in intLen if intLen is non-NULL
// returns the varint value
uint64_t BRVarInt(const uint8_t *buf, size_t bufLen, size_t *intLen)
{
    uint64_t r = 0;
    uint8_t h = (buf && sizeof(uint8_t) <= bufLen) ? *buf : 0;

    switch (h) {
        case VAR_INT16_HEADER:
            if (intLen) *intLen = sizeof(h) + sizeof(uint16_t);
            r = (buf && sizeof(h) + sizeof(uint16_t) <= bufLen) ? UInt16GetLE(&buf[sizeof(h)]) : 0;
            break;

        case VAR_INT32_HEADER:
            if (intLen) *intLen = sizeof(h) + sizeof(uint32_t);
            r = (buf && sizeof(h) + sizeof(uint32_t) <= bufLen) ? UInt32GetLE(&buf[sizeof(h)]) : 0;
            break;

        case VAR_INT64_HEADER:
            if (intLen) *intLen = sizeof(h) + sizeof(uint64_t);
            r = (buf && sizeof(h) + sizeof(uint64_t) <= bufLen) ? UInt64GetLE(&buf[sizeof(h)]) : 0;
            break;

        default:
            if (intLen) *intLen = sizeof(h);
            r = h;
            break;
    }

    return r;
}

// writes i to buf as a varint and returns the number of bytes written, or bufLen needed if buf is NULL
size_t BRVarIntSet(uint8_t *buf, size_t bufLen, uint64_t i)
{
    size_t r = 0;

    if (i < VAR_INT16_HEADER) {
        if (buf && sizeof(uint8_t) <= bufLen) *buf = (uint8_t)i;
        r = (! buf || sizeof(uint8_t) <= bufLen) ? sizeof(uint8_t) : 0;
    }
    else if (i <= UINT16_MAX) {
        if (buf && sizeof(uint8_t) + sizeof(uint16_t) <= bufLen) {
            *buf = VAR_INT16_HEADER;
            UInt16SetLE(&buf[sizeof(uint8_t)], (uint16_t)i);
        }

        r = (! buf || sizeof(uint8_t) + sizeof(uint16_t) <= bufLen) ? sizeof(uint8_t) + sizeof(uint16_t) : 0;
    }
    else if (i <= UINT32_MAX) {
        if (buf && sizeof(uint8_t) + sizeof(uint32_t) <= bufLen) {
            *buf = VAR_INT32_HEADER;
            UInt32SetLE(&buf[sizeof(uint8_t)], (uint32_t)i);
        }

        r = (! buf || sizeof(uint8_t) + sizeof(uint32_t) <= bufLen) ? sizeof(uint8_t) + sizeof(uint32_t) : 0;
    }
    else {
        if (buf && sizeof(uint8_t) + sizeof(uint64_t) <= bufLen) {
            *buf = VAR_INT64_HEADER;
            UInt64SetLE(&buf[sizeof(uint8_t)], i);
        }

        r = (! buf || sizeof(uint8_t) + sizeof(uint64_t) <= bufLen) ? sizeof(uint8_t) + sizeof(uint64_t) : 0;
    }

    return r;
}

// returns the number of bytes needed to encode i as a varint
size_t BRVarIntSize(uint64_t i)
{
    return BRVarIntSet(NULL, 0, i);
}

// parses script and writes an array of pointers to the script elements (opcodes and data pushes) to elems
// returns the number of elements written, or elemsCount needed if elems is NULL
size_t BRScriptElements(const uint8_t *elems[], size_t elemsCount, const uint8_t *script, size_t scriptLen)
{
    size_t off = 0, i = 0, len = 0;

    assert(script != NULL || scriptLen == 0);

    while (script && off < scriptLen) {
        if (elems && i < elemsCount) elems[i] = &script[off];

        switch (script[off]) {
            case OP_PUSHDATA1:
                off++;
                if (off + sizeof(uint8_t) <= scriptLen) len = script[off];
                off += sizeof(uint8_t);
                break;

            case OP_PUSHDATA2:
                off++;
                if (off + sizeof(uint16_t) <= scriptLen) len = UInt16GetLE(&script[off]);
                off += sizeof(uint16_t);
                break;

            case OP_PUSHDATA4:
                off++;
                if (off + sizeof(uint32_t) <= scriptLen) len = UInt32GetLE(&script[off]);
                off += sizeof(uint32_t);
                break;

            default:
                len = (script[off] > OP_PUSHDATA4) ? 0 : script[off];
                off++;
                break;
        }

        off += len;
        i++;
    }

    return ((! elems || i <= elemsCount) && off == scriptLen) ? i : 0;
}

// given a data push script element, returns a pointer to the start of the data and writes its length to dataLen
const uint8_t *BRScriptData(const uint8_t *elem, size_t *dataLen)
{
    assert(elem != NULL);
    assert(dataLen != NULL);
    if (! elem || ! dataLen) return NULL;

    switch (*elem) {
        case OP_PUSHDATA1:
            elem++;
            *dataLen = *elem;
            elem += sizeof(uint8_t);
            break;

        case OP_PUSHDATA2:
            elem++;
            *dataLen = UInt16GetLE(elem);
            elem += sizeof(uint16_t);
            break;

        case OP_PUSHDATA4:
            elem++;
            *dataLen = UInt32GetLE(elem);
            elem += sizeof(uint32_t);
            break;

        default:
            *dataLen = (*elem > OP_PUSHDATA4) ? 0 : *elem;
            elem++;
            break;
    }

    return (*dataLen > 0) ? elem : NULL;
}

// writes a data push script element to script
// returns the number of bytes written, or scriptLen needed if script is NULL
size_t BRScriptPushData(uint8_t *script, size_t scriptLen, const uint8_t *data, size_t dataLen)
{
    size_t len = dataLen;

    assert(data != NULL || dataLen == 0);
    if (data == NULL && dataLen != 0) return 0;

    if (dataLen < OP_PUSHDATA1) {
        len += 1;
        if (script && len <= scriptLen) script[0] = dataLen;
    }
    else if (dataLen < UINT8_MAX) {
        len += 1 + sizeof(uint8_t);

        if (script && len <= scriptLen) {
            script[0] = OP_PUSHDATA1;
            script[1] = dataLen;
        }
    }
    else if (dataLen < UINT16_MAX) {
        len += 1 + sizeof(uint16_t);

        if (script && len <= scriptLen) {
            script[0] = OP_PUSHDATA2;
            UInt16SetLE(&script[1], dataLen);
        }
    }
    else {
        len += 1 + sizeof(uint32_t);

        if (script && len <= scriptLen) {
            script[0] = OP_PUSHDATA4;
            UInt32SetLE(&script[1], (uint32_t)dataLen);
        }
    }

    if (script && len <= scriptLen) memcpy(script + len - dataLen, data, dataLen);
    return (! script || len <= scriptLen) ? len : 0;
}