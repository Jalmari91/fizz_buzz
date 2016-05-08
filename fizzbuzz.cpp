#include <stdio.h>
#include <stdint.h>

enum {
	FIZZ = 0,
	BUZZ = 1,
	FIZZ_BUZZ = 2,
	NUM = 3,
	PARSE_NUM = 4,
	LAST = 5,
	COUNT = 6,
};

/*
  if (start > end)
    return LAST;
  else if (start % 15 == 0)
    return FIZZ_BUZZ;
  else if (start % 3 == 0)
    return FIZZ;
  else if (start % 5 == 0)
    return BUZZ;
  else
    return NUM
 */
#define FIZZ_BUZZ_TYPE(start, end) (((start) > (end)) ? LAST :		\
		((start) % 15 == 0) ? FIZZ_BUZZ :			\
		((start) % 3 == 0) ? FIZZ :				\
		((start) % 5 == 0) ? BUZZ : NUM)

/* call this */
#define FIZZ_BUZZ(start, end) fizz_buzz_impl<COUNT, 0, (start), (end)>::value

#define FIZZ_STR 'F', 'i', 'z', 'z'
#define BUZZ_STR 'B', 'u', 'z', 'z'

template<char... chars>
struct string {
	static const char value[];
};

template<char... chars>
const char string<chars...>::value[] = {
	chars...
};

/* 423532 -> 235324 */
template<uint64_t integer, uint64_t reversed>
struct reverse_integer : reverse_integer<integer / 10,
					 reversed * 10 + integer % 10>
{

};

template<uint64_t reversed>
struct reverse_integer<0, reversed>
{
	static constexpr uint64_t value = reversed;
};

#define FIZZ_BUZZ_IMPL(type, type_str...)					\
	template<uint16_t num, uint16_t start, uint16_t end, char... chars> 	\
	struct fizz_buzz_impl<type, num, start, end, chars...> :		\
		fizz_buzz_impl<FIZZ_BUZZ_TYPE(start + 1, end), num, 		\
			       start + 1, end, chars..., type_str, '\n'> {	\
										\
	}


/* Starting point */
template<uint16_t type, uint16_t num, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl :
	fizz_buzz_impl<FIZZ_BUZZ_TYPE(start, end), num,
		       start, end, chars...> {
	static_assert(start <= end, "End cannot be smaller than start!");
};

/* Handle "Fizz" situation */
FIZZ_BUZZ_IMPL(FIZZ, FIZZ_STR);
/* Handle "Buzz" situation */
FIZZ_BUZZ_IMPL(BUZZ, BUZZ_STR);
/* Handle "FizzBuzz" situation */
FIZZ_BUZZ_IMPL(FIZZ_BUZZ, FIZZ_STR, BUZZ_STR);

/* Start parsing digits */
template<uint16_t num, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<NUM, num, start, end, chars...> :
	fizz_buzz_impl<PARSE_NUM, reverse_integer<start, 0>::value, start, end,
		       chars...> {

};

/* Parse one digit */
template<uint16_t num, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<PARSE_NUM, num, start, end, chars...> :
	fizz_buzz_impl<PARSE_NUM, num / 10, start, end, chars...,
		       '0' + num % 10> {

};

/* All digits parsed */
template<uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<PARSE_NUM, 0, start, end, chars...> :
	fizz_buzz_impl<FIZZ_BUZZ_TYPE(start+1, end), 0, start + 1, end,
		       chars..., '\n'> {

};

/* Construct the string */
template<uint16_t num, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<LAST, num, start, end, chars...> :
	string<chars..., '\0'> {

};

int main()
{
	static const char *r = FIZZ_BUZZ(1, 100);
	printf("%s", r);
}
