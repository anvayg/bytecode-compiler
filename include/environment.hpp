#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <ostream>
#include <string>
#include <unordered_map>

class Environment {
// Go with ints as the only values for now
using Table = std::unordered_map<std::string, int>;

private:
    Table table;
    Environment* parent;

public:
    Environment(Table env, Environment* parent);

    void define(std::string name, int value);

    void assign(std::string name, int value);

    int lookup(std::string name);

    Table& resolve(std::string name);

    bool isDefined(std::string name);

    friend std::ostream& operator<<(std::ostream& os, const Environment& env);
};

#endif