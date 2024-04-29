#include "../inc/Location.hpp"
// Default constructor
Location::Location() : name(""), root(""), return_(""), index(""), allow_methods(""), autoindex(""), alias("") {}

// Copy constructor
Location::Location(const Location& other)
    : name(other.name), root(other.root), return_(other.return_), index(other.index), allow_methods(other.allow_methods), autoindex(other.autoindex), alias(other.alias) {}

// Destructor
Location::~Location() {}

// Getter methods
const std::string& Location::getName() const { return name; }
const std::string& Location::getRoot() const { return root; }
const std::string& Location::getReturn() const { return return_; }
const std::string& Location::getIndex() const { return index; }
const std::string& Location::getAllowMethods() const { return allow_methods; }
const std::string& Location::getAutoindex() const { return autoindex; }
const std::string& Location::getAlias() const { return alias; }

// Setter methods
void Location::setName(const std::string& n) { name = n; }
void Location::setRoot(const std::string& r) { root = r; }
void Location::setReturn(const std::string& ret) { return_ = ret; }
void Location::setIndex(const std::string& idx) { index = idx; }
void Location::setAllowMethods(const std::string& allow) { allow_methods = allow; }
void Location::setAutoindex(const std::string& autoidx) { autoindex = autoidx; }
void Location::setAlias(const std::string& als) { alias = als; }