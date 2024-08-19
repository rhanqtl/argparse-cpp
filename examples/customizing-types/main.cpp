#include <ostream>
#include <string>

#include "argparse/argparse.h"

namespace argparse {

struct Rational {
  int n;
  int d{1};
};

std::ostream &operator<<(std::ostream &out, const Rational &r) {
  out << r.n;
  if (r.d != 1) out << '/' << r.d;
  return out;
}

ParsingError parse_value(Inputs &inputs, Rational &value) {
  if (inputs.empty()) {
    value = {.n = 0, .d = 1};
    return {"no input"};
  }

  auto s = inputs[0];
  auto pos = s.find_first_of('/');
  if (pos == std::string_view::npos) {
    value = {.n = std::stoi(std::string{s})};
  } else {
    value = {.n = std::stoi(std::string{s.substr(0, pos)}),
             .d = std::stoi(std::string{s.substr(pos + 1)})};
  }
  return {};
}

}  // namespace argparse

int main(int argc, char *argv[]) {
  using namespace argparse;

  auto parser = ArgumentParser{};
  parser.add_argument<Rational>({.name = "arg", .required = true});

  parser.parse(argc, argv);

  std::cout << parser.get<Rational>("arg") << std::endl;
}
