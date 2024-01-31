#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set<string> words;
    set<string> minus_words;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        if (!text.empty()) {
            for (const string& word : SplitIntoWords(text)) {
                stop_words_.insert(word);
            }    
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        int N = words.size();

        if (N > 0) {
            for (const string& s : words) {
                word_to_document_freqs_[s].insert(make_pair(document_id, 0.));
                word_to_document_freqs_[s][document_id] += 1./N;
            }
            cout << endl;
            ++document_count_;
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    
    int document_count_ = 0;
    map<string, map<int, double>> word_to_document_freqs_;  
    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        if (!stop_words_.empty()) {
            return stop_words_.count(word) > 0;
        }
        return false;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (word[0] == '-') {
                string minus_word = word.substr(1);
                if (!IsStopWord(minus_word)) {
                    query.minus_words.insert(minus_word);
                }    
            }    
            else {
                query.words.insert(word); 
            }
        }
        return query;
    }

    vector<Document> FindAllDocuments(const Query& query) const {
        vector<Document> matched_documents; //это не надо? как обойтись без него?
        map<int, double> match_document;

        if (query.words.size() > 0) {
            
            for (const string& s : query.words) {
                if((s.size() > 0) && (word_to_document_freqs_.count(s) > 0)) { 
                    double idf = log(static_cast<double>(document_count_) / static_cast<double>(word_to_document_freqs_.at(s).size()));

                    for(const auto& i : word_to_document_freqs_.at(s)) { 
                        match_document[i.first] += idf * i.second;
                    }
                }    
            }   

            if (query.minus_words.size() > 0) {
                for (const string& s : query.minus_words) {
                    if ((s.size() > 0) && (word_to_document_freqs_.count(s) > 0)) {
                        for(const auto& i : word_to_document_freqs_.at(s)) {
                            match_document[i.first] = 0;
                        }
                    }
                }
            }

            for (pair<int, double> p : match_document) {
                if (p.second > 0) { 
                    matched_documents.push_back({p.first, p.second});
                }
            }
            
        }
            
        return matched_documents;
    }

};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}