//======================================
// Definitions files
//======================================
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <list>
#include <algorithm>

using string_t = std::string;
using bool_t = bool;
using stringvec_t = std::vector<std::string>;
using ifstream_t = std::ifstream;

#define pairListKeyString(T) std::list<std::pair<T, string_t>>


#ifdef DEBUG
// COUT Logs
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define log_err( T ) std::cerr << CL_RED    << "~[err] [" << __FILENAME__ << "->" << __func__ << ":" << __LINE__ << "] " << T << CL_RESET << std::endl;
#define log_war( T ) std::cout << CL_YELLOW << "~[war] [" << __FILENAME__ << "->" << __func__ << ":" << __LINE__ << "] " << T << CL_RESET << std::endl;
#define log_inf( T ) std::cout << CL_GREEN  << "~[inf] [" << __FILENAME__ << "->" << __func__ << ":" << __LINE__ << "] " << T << CL_RESET << std::endl;
#define log_dbg( T ) std::cout << CL_BLUE   << "~[dbg] [" << __FILENAME__ << "->" << __func__ << ":" << __LINE__ << "] " << T << CL_RESET << std::endl;
#else
#define log_err( T ) std::cerr << CL_RED    << " " << T << CL_RESET << std::endl;
#define log_war( T ) std::cout << CL_YELLOW << " " << T << CL_RESET << std::endl;
#define log_inf( T ) std::cout << CL_GREEN  << " " << T << CL_RESET << std::endl;
#define log_dbg( T ) std::cout << CL_BLUE   << " " << T << CL_RESET << std::endl;
#endif

// Colors
// -------------------------------------

#define CL_RESET   "\033[0m"
#define CL_BLACK   "\033[30m"      /* Black */
#define CL_RED     "\033[31m"      /* Red */
#define CL_GREEN   "\033[32m"      /* Green */
#define CL_YELLOW  "\033[33m"      /* Yellow */
#define CL_BLUE    "\033[34m"      /* Blue */
#define CL_MAGENTA "\033[35m"      /* Magenta */
#define CL_CYAN    "\033[36m"      /* Cyan */
#define CL_WHITE   "\033[37m"      /* White */
#define CL_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define CL_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define CL_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define CL_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define CL_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define CL_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define CL_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define CL_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
