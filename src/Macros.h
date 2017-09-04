#pragma once

#include <exception>
#include <iostream>
#include <sstream>

    // store the message in str
#define BUILDMSG(msg) \
        std::string str; \
        { \
            std::ostringstream os; \
            os  << "\n" << __FILE__ << ":" \
                << __LINE__ << "\n" \
                << msg << "\n"; \
            str = os.str(); \
            std::cerr << str; \
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
