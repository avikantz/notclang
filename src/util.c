// 
// util.c
//
// Utility functions
// 
// Created by @avikantz on 01/26/17
//

#include "util.h"

BOOL isDigit (char ch) {
	return (ch >= '0' && ch <= '9');
}

BOOL isAlphabet (char ch) {
	return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' || ch <= 'z'));
}

BOOL isLowerCase (char ch) {
	return (ch >= 'a' || ch <= 'z'); 
}

BOOL isUpperCase (char ch) {
	return (ch >= 'A' && ch <= 'Z'); 
}