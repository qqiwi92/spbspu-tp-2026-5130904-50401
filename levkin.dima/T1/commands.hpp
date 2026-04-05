#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace levkin {
class Note {
public:
    Note(std::string s);
    void addContent(std::string c);

private:
    std::string id;
    std::vector<std::string> content;
};

using Database = std::unordered_map<std::string, std::shared_ptr<Note>>;
using cmd_t = void (*)(std::istream&, std::ostream&, Database&);
using Cmds = std::unordered_map<std::string, cmd_t>;

void note(std::istream& in, std::ostream& out, Database& db);
void line(std::istream& in, std::ostream& out, Database& db);
void show(std::istream& in, std::ostream& out, Database& db);
void drop(std::istream& in, std::ostream& out, Database& db);
void link(std::istream& in, std::ostream& out, Database& db);
void halt(std::istream& in, std::ostream& out, Database& db);
void mind(std::istream& in, std::ostream& out, Database& db);
void expired(std::istream& in, std::ostream& out, Database& db);
void refresh(std::istream& in, std::ostream& out, Database& db);

Cmds getCmds();
}

#endif
