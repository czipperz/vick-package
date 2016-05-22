/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <fstream>
#include <iostream>

#include "parse_packages.hh"

static std::vector<std::string> words_in_line(const std::string& line)
{
    using namespace std;
    vector<string> words;
    string::const_iterator b = begin(line), it, e = end(line);
    if ((it = find(b, e, '#')) != e and (it == b or it[-1] != '\\'))
        e = it;
    locale loc;
    auto pred = [&loc](char c) { return isspace(c, loc); };
    if (all_of(b, e, pred))
        return words;
    for (;; b = it) {
        b = find_if_not(b, e, pred);
        if (b == e)
            return words;
        it = find_if(b, e, pred);
        words.push_back(string(b, it));
        if (it == e)
            return words;
    }
    return words;
}

std::map<std::string, std::pair<std::string, std::vector<std::string> > >
parse_packages(std::string filename)
{
    std::map<std::string, std::pair<std::string, std::vector<std::string> > >
        lines;

    using namespace std;
    ifstream file(filename);
    if (not file) {
        cout << "File does not exist: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    for (size_t linenum = 1; file; linenum++) {
        string line;
        getline(file, line);
        auto words = words_in_line(line);
        if (words.empty())
            continue;
        if (words.size() < 2) {
            cout << "Parse error: Less than 2 arguments on line " << linenum
                 << " of " << filename << "." << endl;
            exit(EXIT_FAILURE);
        }
        string a = words[0], b = words[1];
        words.erase(words.begin(), words.begin() + 2);
        lines[move(a)] = pair<string, vector<string> >(move(b), move(words));
    }

    return lines;
}
