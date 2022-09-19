#include "json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c == ',') continue;
      input.putback(c);
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadIntOrDouble(istream& input) {
    int32_t result = 0;
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }
    if (input.peek() == '.') {
      input.ignore(1);
      int32_t fractional = 0, cnt = 1;
      while (isdigit(input.peek())) {
        cnt *= 10;
        fractional *= 10;
        fractional += input.get() - '0';
      }
      return Node(double(result) + fractional / double(cnt));
    }
    return Node(result);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') continue;
      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadBool(istream& input) {
    char c;
    input >> c;
    bool res = true;
    input.ignore(3);
    if (c == 'f'){
      res = false;
      input.ignore(1);
    }
    return Node(res);
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else {
      input.putback(c);
      if (isdigit(c)) {
        return LoadIntOrDouble(input);
      }
      return LoadBool(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
