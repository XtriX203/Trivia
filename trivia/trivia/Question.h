#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

#define COMMA ','
#define CORRECT_ANSWER 0

class Question
{
private:
    string m_question;
    std::vector<std::string> m_possibleAnswers;

public:
    Question();
    Question(const string a, const vector<string> possibleAnswers);
    void setQuestion(const string question);
    void addAnswer(const string ans);
    string getQuestion() const;
    vector<string> getPossibleAnswers() const;
    string getCorrectAnswer() const;
};