//=========================================================================
//  STRINGUTIL.H - part of
//                  OMNeT++/OMNEST
//           Discrete System Simulation in C++
//
//  Author: Andras Varga
//
//=========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 2006-2015 OpenSim Ltd.

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/

#ifndef __OMNETPP_COMMON_STRINGUTIL_H
#define __OMNETPP_COMMON_STRINGUTIL_H

#include <cstdarg>
#include <cstring>
#include <string>
#include <map>
#include "commondefs.h"
#include "commonutil.h"

namespace omnetpp {
namespace common {

/**
 * Returns true if the string is nullptr or has zero length.
 */
inline bool opp_isempty(const char *s)  {return !s || !s[0];}

/**
 * Returns the pointer passed as argument unchanged, except that if it was nullptr,
 * it returns a pointer to a null string ("").
 */
inline const char *opp_nulltoempty(const char *s)  {return s ? s : "";}

/**
 * Returns the pointer passed as argument unchanged, except that if it was empty,
 * it returns the second argument.
 */
inline const char *opp_emptytodefault(const char *s, const char *defaultString)  {return opp_isempty(s) ? defaultString : s;}

/**
 * Returns true if the string only contains whitespace.
 */
COMMON_API bool opp_isblank(const char *txt);

/**
 * Same as the standard strlen() function, except that does not crash
 * on nullptr but returns 0.
 */
inline int opp_strlen(const char *s)
{
    return s ? strlen(s) : 0;
}

/**
 * Duplicates the string, using <tt>new char[]</tt>. For nullptr and empty
 * strings it returns nullptr.
 */
inline char *opp_strdup(const char *s)
{
    if (!s || !s[0]) return nullptr;
    char *p = new char[strlen(s)+1];
    strcpy(p,s);
    return p;
}

/**
 * Same as the standard strcpy() function, except that nullptr
 * in the second argument is treated as a pointer to an empty string ("").
 */
inline char *opp_strcpy(char *s1, const char *s2)
{
    return strcpy(s1, s2 ? s2 : "");
}

/**
 * Same as the standard strcmp() function, except that nullptr
 * is treated exactly as an empty string ("").
 */
inline int opp_strcmp(const char *s1, const char *s2)
{
    if (s1)
        return s2 ? strcmp(s1,s2) : (*s1 ? 1 : 0);
    else
        return (s2 && *s2) ? -1 : 0;
}

/**
 * Removes any leading and trailing whitespace.
 */
COMMON_API std::string opp_trim(const char *txt);

/**
 * Reverse of opp_quotestr(): remove quotes and resolve backslashed escapes.
 *
 * Throws an exception if there is a parse error. If there is anything
 * (non-whitespace) in the input after the string literal, endp is set to
 * that character; otherwise endp is set to point to the terminating zero
 * of the string.
 */
COMMON_API std::string opp_parsequotedstr(const char *txt, const char *&endp);

/**
 * Reverse of opp_quotestr(): remove quotes and resolve backslashed escapes.
 *
 * Throws an exception if there is a parse error.
 */
COMMON_API std::string opp_parsequotedstr(const char *txt);

/**
 * Surround the given string with "quotes", also escape with backslash
 * where needed.
 */
COMMON_API std::string opp_quotestr(const char *txt);

/**
 * Returns true if the string contains space, backslash, quote, or anything
 * else that would make quoting (opp_quotestr()) necessary before writing
 * it into a data file.
 */
COMMON_API bool opp_needsquotes(const char *txt);

/**
 * Combines opp_needsquotes() and opp_quotestr().
 */
inline std::string opp_quotestr_ifneeded(const char *txt)
{
    return opp_needsquotes(txt) ? opp_quotestr(txt) : std::string(txt);
}

/**
 * A macro version of opp_quotestr_ifneeded(). This is more efficient,
 * because it avoids conversion to std::string when no quoting is needed.
 */
#define QUOTE(txt)   (opp_needsquotes(txt) ? opp_quotestr(txt).c_str() : (txt))

/**
 * Create a string using printf-like formatting. Limit: 1023 chars.
 */
COMMON_API std::string opp_stringf(const char *fmt, ...);

/**
 * Create a string using printf-like formatting. Limit: 1023 chars.
 */
COMMON_API std::string opp_vstringf(const char *fmt, va_list& args);

/**
 * A limited vsscanf implementation, used by cStatistic::freadvarsf()
 */
COMMON_API int opp_vsscanf(const char *s, const char *fmt, va_list va);

/**
 * Performs find/replace within a string.
 */
COMMON_API std::string opp_replacesubstring(const char *s, const char *substring, const char *replacement, bool replaceAll);

/**
 * For opp_substitutevariables().
 */
struct opp_substitutevariables_resolver {
    virtual bool isVariableNameChar(char c) = 0;
    virtual std::string operator()(const std::string&) = 0;
};

/**
 * Substitutes dollar variables ($name or ${name}) into the given string.
 * The second arg is a function that should return the value of a variable
 * from its name. (It may also throw an exception if there is no such variable.)
 */
COMMON_API std::string opp_substitutevariables(const std::string& raw, opp_substitutevariables_resolver& resolver);

/**
 * Substitutes dollar variables ($name or ${name}) into the given string, using the
 * dictionary passed as second arg. Throws an exception if a variable is not found.
 */
COMMON_API std::string opp_substitutevariables(const std::string& raw, const std::map<std::string,std::string>& vars);

/**
 * Inserts newlines into the string, performing rudimentary line breaking.
 */
COMMON_API std::string opp_breaklines(const char *text, int maxLineLength);

/**
 * Indent each line of the input text.
 */
COMMON_API std::string opp_indentlines(const char *text, const char *indent);

/**
 * Returns true if the first string begins with the second string.
 */
COMMON_API bool opp_stringbeginswith(const char *s, const char *prefix);

/**
 * Returns true if the first string ends in the second string.
 */
COMMON_API bool opp_stringendswith(const char *s, const char *ending);

/**
 * Returns the substring up to the first occurrence of the given substring, or "".
 */
COMMON_API std::string opp_substringbefore(const std::string& str, const std::string& substr);

/**
 * Returns the substring after the first occurrence of the given substring, or "".
 */
COMMON_API std::string opp_substringafter(const std::string& str, const std::string& substr);

/**
 * Returns the substring up to the last occurrence of the given substring, or "".
 */
COMMON_API std::string opp_substringbeforelast(const std::string& str, const std::string& substr);

/**
 * Returns the substring after the last occurrence of the given substring, or "".
 */
COMMON_API std::string opp_substringafterlast(const std::string& str, const std::string& substr);

/**
 * Concatenates up to four strings. Returns a pointer to a static buffer
 * of length 256. If the result length would exceed 256, it is truncated.
 */
COMMON_API char *opp_concat(const char *s1, const char *s2, const char *s3=nullptr, const char *s4=nullptr);

/**
 * Converts the string to uppercase. Returns a pointer to  the argument.
 */
COMMON_API char *opp_strupr(char *s);

/**
 * Converts the string to lowercase. Returns a pointer to  the argument.
 */
COMMON_API char *opp_strlwr(char *s);

/**
 * If either s1 or s2 is empty, returns the other one, otherwise returns
 * s1 + separator + s2.
 */
COMMON_API std::string opp_join(const char *separator, const char *s1, const char *s2);

/**
 * Concatenate the strings passed in the nullptr-terminated const char * array, using
 * the given separator.
 */
COMMON_API std::string opp_join(const char **strings, const char *separator);

/**
 * Dictionary-compare two strings, the main difference from strcasecmp()
 * being that integers embedded in the strings are compared in
 * numerical order.
 */
COMMON_API int strdictcmp(const char *s1, const char *s2);

/**
 * Prints the d integer into the given buffer, then returns the buffer pointer.
 */
COMMON_API char *opp_itoa(char *buf, int d);

/**
 * Prints the d integer into the given buffer, then returns the buffer pointer.
 */
COMMON_API char *opp_ltoa(char *buf, long d);

/**
 * Prints the d integer into the given buffer, then returns the buffer pointer.
 */
COMMON_API char *opp_i64toa(char *buf, int64_t d);

/**
 * Prints the d double into the given buffer with the given printf format (e.g. "%g"),
 * then returns the buffer pointer.
 */
COMMON_API char *opp_dtoa(char *buf, const char *format, double d);

/**
 * Like the standard strtol(), but throws opp_runtime_error if an overflow
 * occurs during conversion. Accepts decimal and C-style hexadecimal
 * notation, but not octal (leading zeroes are simply discarded and the number
 * is interpreted as decimal).
 */
COMMON_API long opp_strtol(const char *s, char **endptr);

/**
 * Like the standard atol(), but throws opp_runtime_error if an overflow
 * occurs during conversion, or if there is (non-whitespace) trailing garbage
 * after the number. Accepts decimal and C-style hexadecimal notation, but not
 * octal (leading zeroes are simply discarded and the number is interpreted as
 * decimal).
 */
COMMON_API long opp_atol(const char *s);

/**
 * Like the standard strtoul(), but throws opp_runtime_error if an
 * overflow occurs during conversion. Accepts decimal and C-style hexadecimal
 * notation, but not octal (leading zeroes are simply discarded and the number
 * is interpreted as decimal).
 */
COMMON_API unsigned long opp_strtoul(const char *s, char **endptr);

/**
 * Like the standard atol(), but for unsigned long, and throws opp_runtime_error
 * if an overflow occurs during conversion, or if there is (non-whitespace)
 * trailing garbage after the number. Accepts decimal and C-style hexadecimal
 * notation, but not octal (leading zeroes are simply discarded and the number
 * is interpreted as decimal).
 */
COMMON_API unsigned long opp_atoul(const char *s);

/**
 * Like the standard strtod(), but throws opp_runtime_error if an overflow
 * occurs during conversion.
 */
COMMON_API double opp_strtod(const char *s, char **endptr);

/**
 * Like the standard atof(), but throws opp_runtime_error if an overflow
 * occurs during conversion, or if there is (non-whitespace) trailing garbage
 * after the number.
 */
COMMON_API double opp_atof(const char *s);

/**
 * Returns the current date/time as a string in the "yyyymmdd-hh:mm:ss" format.
 */
COMMON_API std::string opp_makedatetimestring();

/**
 * s should point to a double quote '"'. The function returns a pointer to
 * the matching quote (i.e. the end of the string literal), or nullptr if
 * not found. It recognizes escaping embedded quotes with backslashes
 * (C-style string literals).
 */
COMMON_API const char *opp_findmatchingquote(const char *s);

/**
 * s should point to an open parenthesis. The function returns the matching
 * paren, or nullptr if not found. It does not search inside string constants
 * delimited by double quotes ('"'); it uses opp_findmatchingquote() to
 * parse them. Note: a nullptr return value (unmatched left paren) may also
 * be caused by an unterminated string constant.
 */
COMMON_API const char *opp_findmatchingparen(const char *s);

/**
 * Decode an URL-encoded string.
 */
COMMON_API std::string opp_urldecode(const std::string& src);

/**
 * Locates the first occurrence of the nul-terminated string needle in the
 * string haystack, where not more than n characters are searched. Characters
 * that appear after a '\0' character are not searched.
 */
COMMON_API const char *opp_strnistr(const char *haystack, const char *needle, int n, bool caseSensitive);

/**
 * Rudimentary quoting for LaTeX. Escape underscores, backslashes, dollar signs, etc.
 */
COMMON_API std::string opp_latexQuote(const char *s);

/**
 * Add break opportunities.
 */
COMMON_API std::string opp_latexInsertBreaks(const char *s);

/**
 * Convert opp markup to LaTeX.
 */
COMMON_API std::string opp_markup2Latex(const char *s);

} // namespace common
}  // namespace omnetpp

#endif


