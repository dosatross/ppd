/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system.
 **/
#ifndef PPD_COIN
#define PPD_COIN
#define COIN_DELIM ","
struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS = 5, 
    TEN_CENTS = 10, 
    TWENTY_CENTS = 20, 
    FIFTY_CENTS = 50, 
    ONE_DOLLAR = 100, 
    TWO_DOLLARS = 200, 
    FIVE_DOLLARS = 500, 
    TEN_DOLLARS = 1000
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

#endif
