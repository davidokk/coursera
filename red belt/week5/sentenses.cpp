#include "test_runner.h"

#include <vector>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  bool last_is_end = false;

  vector<Sentence<Token>> sentences;
  Sentence<Token> sentence;
  for (Token& token : tokens) {
    if (!token.IsEndSentencePunctuation() && last_is_end) {
      last_is_end = token.IsEndSentencePunctuation();
      sentences.push_back(move(sentence));
      sentence.push_back(move(token));
    } else {
      last_is_end = token.IsEndSentencePunctuation();
      sentence.push_back(move(token));
    }
  }

  sentences.push_back(move(sentence));

  return sentences;
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

// РўРµСЃС‚ СЃРѕРґРµСЂР¶РёС‚ РєРѕРїРёСЂРѕРІР°РЅРёСЏ РѕР±СЉРµРєС‚РѕРІ РєР»Р°СЃСЃР° TestToken.
// Р”Р»СЏ РїСЂРѕРІРµСЂРєРё РѕС‚СЃСѓС‚СЃС‚РІРёСЏ РєРѕРїРёСЂРѕРІР°РЅРёР№ РІ С„СѓРЅРєС†РёРё SplitIntoSentences
// РЅРµРѕР±С…РѕРґРёРјРѕ РЅР°РїРёСЃР°С‚СЊ РѕС‚РґРµР»СЊРЅС‹Р№ С‚РµСЃС‚.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}