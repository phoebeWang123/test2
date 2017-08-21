#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#define MYASSERT(cond, msg) {if (!(cond)) {std::cout <<"File name: "<< __FILE__ << "\n"<<"Line: "<<__LINE__<<"\n"<<"Error message: " << msg << "\n"; throw std::invalid_argument(msg); }}

#define THROW(msg) {std::cout <<"File name: "<< __FILE__ << "\n"<<"Line: "<<__LINE__<<"\n"<<"Error message: "<< msg << "\n"; throw std::logic_error(msg);}

#define NOT_IMPLEMENTED throw std::logic_error("NOT IMPLEMENTED")
