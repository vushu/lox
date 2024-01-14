#include "lox/scanner.hpp"

#include <iostream>
#include <string_view>
#include <vector>

#include <lox/tokens.hpp>

namespace lox {

auto Scanner::scan_tokens() -> std::vector<Token> {
	while (!is_at_end()) {
		start_ = current_;
		scan_token();
	}
	add_token(TokenType::eof_tok, "", {});
	return tokens_;
}

auto Scanner::scan_token() -> void {
	auto c = advance();
	switch (c) {
		case '(':
			add_token(TokenType::left_paren_tok);
			break;
		case ')':
			add_token(TokenType::right_paren_tok);
			break;
		case '{':
			add_token(TokenType::left_brace_tok);
			break;
		case '}':
			add_token(TokenType::right_brace_tok);
			break;
		case ',':
			add_token(TokenType::comma_tok);
			break;
		case '.':
			add_token(TokenType::dot_tok);
			break;
		case '-':
			add_token(TokenType::minus_tok);
			break;
		case '+':
			add_token(TokenType::plus_tok);
			break;
		case ';':
			add_token(TokenType::semicolon_tok);
			break;
		case '*':
			add_token(TokenType::star_tok);
			break;
		case '!':
			add_token((match('=') ? TokenType::bang_equal_tok : TokenType::bang_tok));
			break;
		case '=':
			add_token((match('=') ? TokenType::equal_equal_tok : TokenType::equal_tok));
			break;
		case '<':
			add_token((match('=') ? TokenType::less_equal_tok : TokenType::less_tok));
			break;
		case '>':
			add_token((match('=') ? TokenType::greater_equal_tok : TokenType::greater_tok));
			break;
		case '/':
			if (match('/')) {
				// comment until the tne of line
				while (peek() != '\n' && !is_at_end()) {
					advance();
				}
			} else {
				add_token(TokenType::slash_tok);
			}
			break;
		case ' ':
			[[fallthrough]];
		case '\r':
			[[fallthrough]];
		case '\t':
			break;
		case '\n':
			line_++;
			break;
		case '"':
			handle_string();
			break;
		default:
			if (is_digit(c)) {
				handle_number();
				break;
			} else if (is_alpha_numeric(c)) {
				handle_identifier();
				break;
			}
			throw "Unexpected character";
			break;
	}
}

auto Scanner::handle_identifier() -> void {
	while (is_alpha_numeric(peek())) {
		advance();
	}

	auto text = source_.substr(start_, current_ - start_);
	// see if any keywords
	auto iter = keywords.find(text);
	auto type = TokenType::identifier_tok;
	if (iter != keywords.end()) {
		type = iter->second;
	}
	add_token(type);
}

auto Scanner::handle_number() -> void {
	while (is_digit(peek())) {
		advance();
	}
	if (peek() == '.' && is_digit(peek_next())) {
		advance();
		while (is_digit(peek())) {
			advance();
		}
	}

	auto text = source_.substr(start_, current_ - start_);
	// We just do all numbers to double which is simpler
	add_token(TokenType::number_tok, text, std::stod(text.data()));
}

auto Scanner::handle_string() -> void {
	while (peek() != '"' && !is_at_end()) {
		if (peek() == '\n') {
			line_++;
		}
		advance();
	}

	// Unterminated string.
	if (is_at_end()) {
		std::cout << "Unterminated string" << std::endl;
		return;
	}

	// The closing ".
	advance();

	// Trim the surrounding quotes.
	std::string_view value        = source_.substr(start_ + 1, current_ - start_ - 2);
	std::string_view with_quoutes = source_.substr(start_, current_ - start_);
	std::cout << " the string " << value << std::endl;
	add_token(TokenType::string_tok, with_quoutes, value);
}

auto Scanner::match(char expected) -> bool {
	if (is_at_end()) {
		return false;
	}

	if (source_[current_] != expected) {
		return false;
	}
	current_++;
	return true;
}

auto Scanner::peek_next() -> char {
	if (current_ + 1 >= source_.size()) {
		return '\0';
	}
	return source_.at(current_ + 1);
}

auto Scanner::peek() -> char {
	if (is_at_end()) {
		return '\0';
	}
	return source_[current_];
}

auto Scanner::add_token(TokenType type, LiteralType literal) -> void {
	std::string_view text = source_.substr(start_, current_);
	add_token(type, text, literal);
}

auto Scanner::add_token(TokenType type) -> void {
	auto lexeme = source_.substr(start_, current_ - start_);
	add_token(type, lexeme, lexeme);
}

auto Scanner::add_token(TokenType type, std::string_view lexeme, LiteralType literal) -> void {
	tokens_.push_back(Token(type, lexeme, literal, line_));
}

auto Scanner::is_digit(char c) -> bool { return c >= '0' && c <= '9'; }

auto Scanner::is_alpha(char c) -> bool { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

auto Scanner::is_alpha_numeric(char c) -> bool { return is_alpha(c) || is_digit(c); }

}  // namespace mirscript
