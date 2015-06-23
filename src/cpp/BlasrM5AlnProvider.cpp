// Copyright (c) 2011-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials provided
// with the distribution.
//
// * Neither the name of Pacific Biosciences nor the names of its
// contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.


#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <boost/format.hpp>
#include "Alignment.hpp"
#include "BlasrM5AlnProvider.hpp"


BlasrM5AlnProvider::BlasrM5AlnProvider(const std::string& fpath) :
    fpath_(fpath),
    currId_(""),
    firstAln_(true),
    fs_() {

    //checkFormat();
    fs_.open(fpath_);
    is_ = &fs_;
}

BlasrM5AlnProvider::BlasrM5AlnProvider(std::istream* stream) :
    fpath_(""),
    currId_(""),
    firstAln_(true),
    fs_(),
    is_(stream) {
}

BlasrM5AlnProvider::~BlasrM5AlnProvider() {
    delete is_;
}

bool BlasrM5AlnProvider::nextTarget(std::vector<dagcon::Alignment>& dest) {
    // first clear any previous alignments
    dest.clear();

    // process up to EOF or next target
    // need to maintain state in between calls
    if (! firstAln_)
        dest.push_back(prevAln_); 

    dagcon::Alignment aln;
    while (*is_ >> aln) {
        if (aln.id != currId_) {
            firstAln_ = false;
            prevAln_ = aln;
            currId_ = aln.id;
            break;
        }
        dest.push_back(aln);
    }

    return bool(*is_);
}

bool BlasrM5AlnProvider::nextTarget(std::string& targetSeq, std::vector<dagcon::Alignment>& dest) {
    // NOOP
    return false;
}

void BlasrM5AlnProvider::checkFormat() {
    std::ifstream ifs(fpath_);
    if (! ifs.is_open() || ifs.fail()) {
        throw M5Exception::FileOpenError();
    }
    // parse the first line and run some field checks
    std::string line;
    std::getline(ifs, line);
    std::stringstream row(line);
    std::string col;
    std::vector<std::string> fields;

    while(std::getline(row, col, ' ')) {
        if (col == "") continue;
        fields.push_back(col);
    }

    if (fields.size() < 19) {
        boost::format msg("Expected 19 fields, found %d");
        msg % fields.size();
        throw M5Exception::FormatError(msg.str());
    }

    // check how the alignments are grouped
    dagcon::Alignment aln;
    std::vector<std::string> raw, sorted;
    int max = 50, count = 0;
    while(ifs >> aln && count++ < max) 
        raw.push_back(aln.id);

    sorted = raw;
    std::sort(sorted.begin(), sorted.end());

    std::string logl = "dagcon::Alignments appear to be grouped by %s";
    ifs.close();
}
