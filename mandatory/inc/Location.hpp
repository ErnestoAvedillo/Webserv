#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

class Location 
{
	private:
		std::string name;
		std::string root;
		std::string return_;
		std::string index;
		std::string allow_methods;
		std::string autoindex;
		std::string alias;
	public:
		// Default constructor
		Location() ;

		// Copy constructor
		Location(const Location& other);

		// Destructor
		~Location();

		// Getter methods
		const std::string& getName() const ;
		const std::string& getRoot() const ;
		const std::string& getReturn() const ;
		const std::string& getIndex() const;
		const std::string& getAllowMethods() const;
		const std::string& getAutoindex() const ;
		const std::string& getAlias() const ;

		// Setter methods
		void setName(const std::string&);
		void setRoot(const std::string&);
		void setReturn(const std::string&);
		void setIndex(const std::string&);
		void setAllowMethods(const std::string& );
		void setAutoindex(const std::string&);
		void setAlias(const std::string&);
};

#endif // LOCATION_HPP