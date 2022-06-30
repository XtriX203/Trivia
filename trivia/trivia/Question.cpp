#include "Question.h"

#include "Question.h"

Question::Question() { }
/*constractor that create question
Params:
	-puestion
	-possible answers
	*/
Question::Question(const string question, const vector<string> possibleAnswers) : m_question(question), m_possibleAnswers(possibleAnswers) { }

/// <summary>
/// method that set given question
/// </summary>
/// <param name="question">string that represents the new questipn</param>
void Question::setQuestion(const string question) { this->m_question = question; }

/// <summary>
/// method to add new possible answer
/// </summary>
void Question::addAnswer(const string ans) { this->m_possibleAnswers.push_back(ans); }

/// <summary>
/// getter to the question
/// </summary>
string Question::getQuestion() const { return this->m_question; }

/// <summary>
/// getter to the answers
/// </summary>
vector<string> Question::getPossibleAnswers() const { return this->m_possibleAnswers; }

/// <summary>
/// getter to the correct answer
/// </summary>
/// <returns></returns>
string Question::getCorrectAnswer() const { return this->m_possibleAnswers[CORRECT_ANSWER]; }
