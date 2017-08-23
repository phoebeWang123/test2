#pragma once

#include <exception>
#include <iostream>
#include <sstream>

// store the message in str
#define BUILDMSG(msg) \
        std::string str; \
        { \
            std::ostringstream os; \
            os  << "File name: " << __FILE__ << "\n" \
                << "Line: "<< __LINE__ << "\n" \
                << "Error message: " << msg << "\n"; \
            str = os.str(); \
            std::cerr << msg; \
        }

#define MYASSERT(cond, msg) \
    { \
        if (!(cond)) \
        { \
            BUILDMSG(msg); \
            throw std::invalid_argument(str); \
        } \
    }

#define THROW(msg) \
    { \
        BUILDMSG(msg); \
        throw std::logic_error(str); \
    }



#define NOT_IMPLEMENTED throw std::logic_error("NOT IMPLEMENTED")
