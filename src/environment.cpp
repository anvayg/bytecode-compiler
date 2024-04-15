#include "../include/ast.hpp"
#include <stdexcept>
#include <string>

Environment::Environment() { Environment(Table(), nullptr); }

Environment::Environment(Table env, Environment *parent = nullptr)
    : table(env), parent(parent) {}

void Environment::define(std::string name, ValueType value) {
  this->table.insert_or_assign(name, value);
}

void Environment::assign(std::string name, ValueType value) {
  Table &env = resolve(name);
  env.insert_or_assign(name, value);
}

ValueType Environment::lookup(std::string name) {
  auto val = resolve(name).find(name);
  return val->second;
}

Table &Environment::resolve(std::string name) {
  auto val = this->table.find(name);
  if (val != this->table.end()) {
    return this->table;
  }

  if (this->parent == nullptr) {
    throw std::runtime_error("Unable to resolve name");
  }

  return (*this->parent).resolve(name);
}

bool Environment::isDefined(std::string name) {
  try {
    resolve(name);
    return true;
  } catch (std::runtime_error e) {
    return false;
  }
}

// Define << operator for ValueType
std::ostream &operator<<(std::ostream &os, const ValueType &value) {
  struct Printer : public boost::static_visitor<> {
    std::ostream &os;

    explicit Printer(std::ostream &_os) : os(_os) {}

    void operator()(int i) const { os << i; }

    void operator()(const std::string &s) const { os << s; }

    void operator()(const std::vector<std::string> &vec) const {
      os << "[ ";
      for (const auto &str : vec) {
        os << str << " ";
      }
      os << "]";
    }

    void operator()(const std::shared_ptr<Function> &func) const {
      os << *func;
    }

    void operator()(const boost::recursive_wrapper<std::vector<Instruction>>
                        &wrapper) const {
      os << "Vector of Instructions: ";
      for (const auto &inst : wrapper.get()) {
        os << inst << " ";
      }
    }
  };

  boost::apply_visitor(Printer(os), value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Environment &env) {
  os << "{ ";
  for (auto it = env.table.begin(); it != env.table.end(); ++it) {
    os << it->first << ": " << it->second << ", ";
  }
  os << "}";

  return os;
}
