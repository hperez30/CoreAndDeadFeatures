// adapted from: https://www.cs.colostate.edu/~fsieker/misc/debug/C++/Debug.h

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <string>
#include <sstream>

/** @file Debug.h
 *  @brief header for <tt>Debug</tt> class
 */

class Debug {

public:
  static int debugLevel;

  static std::ostream* dbOS;

  static void init(int& argc, const char* argv[]);

  static void toFile(const char* fileName);

  static void close(void);

  Debug(const char* fileName, int lineNumber, const char* func);

  ~Debug();

  /** Overload the &lt;&lt; operator, so class acts as an output stream */
  template<class T>
  Debug& operator << (const T &msg) {
    (*Debug::dbOS) << msg;
    return *this;
  }
};

// based on: Shadow2531 https://stackoverflow.com/questions/9277906/stdvector-to-string-with-custom-delimiter

template <typename T>
std::string join(const char* name, const T& list) {
    std::ostringstream str;
    std::string delim = " {";
    str << name << "[" << list.size() << "]";
    for (const auto& elem : list) {
        str << delim;
        str << elem;
	delim = ", ";
    }

    str << "}";
    return str.str();
}

#ifdef DEBUG
#define DEBUG_ENABLED 1 // debug code available at runtime
#else
/** This macro controls whether all debugging code is optimized out of the
 *  executable, or is compiled and controlled at runtime by the
 *  <tt>Debug::debugLevel</tt> variable. The value (0/1) depends on whether
 *  the macro <tt>DEBUG</tt> is defined during the compile.
 */
#define DEBUG_ENABLED 0 // all debug code optimized out
#endif

/** Print this message if the variable <tt>Debug::debugLevel</tt> is greater
 *  than or equal to the parameter.
 *  @param level the level at which this information should be printed
 */
#define lDebug(level) \
if ((! DEBUG_ENABLED) || ((level) > Debug::debugLevel)) \
  ; \
else \
  Debug(__FILE__, __LINE__, __func__) 

/** Simple alias for <tt>lDebug()</tt> */
#define debug lDebug(1)

/** Print the file name, line number, function name and "HERE" */
#define HERE debug << "HERE"

/** Expand a name into a string and a value
 *  @param name name of variable
 */
#define debugVar(name)  #name "=(" << (name) << ") "

/** Output the name and value of a single variable
 *  @param name name of the variable to print
 */
#define varDebug(name) lDebug(1) << debugVar(name)

#define debugList(name) join(#name, name)

#define listDebug(name) lDebug(1) << debugList (name)

#endif  /* __DEBUG_H__ */