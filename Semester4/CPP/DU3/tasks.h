#include <functional>
#include <cstring>
#include <regex>
#include <vector>
#include <string_view>
// REMOVE AFTER DEBUGGING
#include <iostream>

using Parser = std::function<const char *(const char *)>;

/**
 * @brief Function to create parser, which parses the char in string
 * @param c char to be parsed
 * @return Parser, that tries to parse char in string, return nullptr if fail
 */
Parser create_char_parser(const char c);

/**
 * @brief Function to create parser, that parses string by inputed regex
 * @param regexString regex, that parses the string
 * @return Parser, that parses the string by regex, return nullptr if fail
 */
Parser regex_match(const char *regexString);

/**
 * @brief Function to create parser, that removes whitespaces at the start of the string
 * @return Parser, that removes whitespaces at the start of the string
 */
Parser skip_ws();

/**
 * @brief Function, that creates parser from vector of parsers, and tries to match first parser
 * @param parsers vector of parsers
 * @return Parser, that return first match of parser from parsers, return nullptr if fail
 */
Parser any_of(const std::vector<Parser> &parsers);

/**
 * @brief Function that creates parser from vector of parsers and tries to parse each of parser to input sequentialy
 * @param parsers vector of parser
 * @return Parser, that returns result from parsers applicated sequentialy on input, return nullptr if fails
 */
Parser sequence(const std::vector<Parser> &parsers);

/**
 * @brief Function that create parser, that applies parser repetitions times to the input
 * @param parser input parser
 * @param repetitions number of repetitons of parser
 * @return Parser, that repeats repetitions times parser to input, return nullptr if fails
 */
Parser repeat(const Parser &parser, const size_t repetitions);

/**
 * @brief Function that creates parser, that matches string from input
 * @param string string to be matched
 * @return Parser, that matches string in input string, return nullptr if fails
 */
Parser create_word_parser(const std::string_view &string);
