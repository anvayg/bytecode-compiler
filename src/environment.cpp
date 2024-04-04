#include "../include/environment.hpp"
#include <stdexcept>
#include <string>

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

std::ostream &operator<<(std::ostream &os, const Environment &env) {
  os << "TODO\n";

  return os;
}
