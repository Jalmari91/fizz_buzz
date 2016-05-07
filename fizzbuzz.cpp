#include <stdio.h>
#include <stdint.h>

enum {
	FIZZ = 0,
	BUZZ = 1,
	FIZZ_BUZZ = 2,
	NUM = 3,
	LAST = 4,
};

constexpr char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			    'a', 'b', 'c', 'd', 'e', 'f' };

/*
  if (start > end)
    return LAST;
  else if (start % 3 == 0 && start % 5 == 0)
    return FIZZ_BUZZ;
  else if (start % 3 == 0)
    return FIZZ;
  else if (start % 5 == 0)
    return BUZZ;
  else
    return NUM
 */
#define FIZZ_BUZZ_TYPE(start, end) (((start) > (end)) ? LAST :		\
		((start) % 3 == 0 && (start) % 5 == 0) ? FIZZ_BUZZ :	\
		((start) % 3 == 0) ? FIZZ :				\
		((start) % 5 == 0) ? BUZZ : NUM)

#define GET_DIGIT(num, digit) (((num) >> (digit) * 4) & 0x0F)

/* call this */
#define FIZZ_BUZZ(start, end) fizz_buzz_impl<-1, -1, (start), (end)>::value

#define DIGITS_IN_X(x) (sizeof((x)) * 2)

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

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

#define FIZZ_BUZZ_IMPL(type, str...)							\
	template<int digit, uint16_t start, uint16_t end, char... chars> 		\
	struct fizz_buzz_impl<type, digit, start, end, chars...> :			\
		fizz_buzz_impl<FIZZ_BUZZ_TYPE(start + 1, end), DIGITS_IN_X(start) - 1, 	\
			       start + 1, end, chars..., str, '\n'> {			\
											\
	}


/* Starting point */
template<uint16_t type, int digit, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl :
	fizz_buzz_impl<FIZZ_BUZZ_TYPE(start, end), DIGITS_IN_X(start) - 1,
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
template<int digit, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<NUM, digit, start, end, chars...> :
	fizz_buzz_impl<NUM, digit - 1, start, end, chars...,
		       digits[GET_DIGIT(start, digit)]> {
	static_assert(GET_DIGIT(start, digit) < ARRAY_SIZE(digits),
		"GET_DIGIT(start, digit) >= ARRAY_SIZE(digits)");
};

/* All digits parsed */
template<uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<NUM, -1, start, end, chars...> :
	fizz_buzz_impl<FIZZ_BUZZ_TYPE(start + 1, end), DIGITS_IN_X(start) - 1,
	start + 1, end, chars..., '\n'> {

};

/* Construct the string */
template<int digit, uint16_t start, uint16_t end, char... chars>
struct fizz_buzz_impl<LAST, digit, start, end, chars...> :
	string<chars..., '\0'> {

};

int main()
{
	static const char *r = FIZZ_BUZZ(1, 100);
	printf("%s", r);
}
