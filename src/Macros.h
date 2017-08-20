#pragma once

#include <exception>
#include <sstream>

#define MYASSERT(cond, msg) { if (!(cond)) { std::ostringstream os; os << __FILE__ << ":" << __LINE__ << "\n" << msg; throw std::invalid_argument(msg.str()); } }

#define THROW(msg) { { std::ostringstream os; os << __FILE__ << ":" << __LINE__ << "\n" << msg; throw std::logic_error(msg.str()); } }


#define NOT_IMPLEMENTED throw std::logic_error("NOT IMPLEMENTED")
