#pragma once

#include <exception>
#include <sstream>

//#define MYASSERT(cond, msg) { if (!(cond)) { std::ostringstream os; os << __FILE__ << ":" << __LINE__ << "\n" << msg; throw std::invalid_argument(os.str()); }}
#define MYASSERT(cond, msg) {if (!(cond)) {std::cout <<"File name: "<< __FILE__ << "\n"<<"Line: "<<__LINE__<<"\n"<<"Error message: " << msg << "\n"; throw std::invalid_argument(msg); }}

//#define THROW(msg) { { std::ostringstream os; os << __FILE__ << ":" << __LINE__ << "\n" << msg; std::cout<<msg; throw std::logic_error(os.str()); } }
#define THROW(msg) {std::cout <<"File name: "<< __FILE__ << "\n"<<"Line: "<<__LINE__<<"\n"<<"Error message: "<< msg << "\n"; throw std::logic_error(msg);}

#define NOT_IMPLEMENTED throw std::logic_error("NOT IMPLEMENTED")
