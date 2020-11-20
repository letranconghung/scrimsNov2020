/** Robot's setup variables: Motor and sensor ports */
#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_
/** base motor ports */
#define FLPort 11
#define BLPort 12
#define FRPort 19
#define BRPort 18
/** other mechanical motor ports */
#define bouncerPort 5
#define ejectorPort 6
#define shooterPort 7
#define frollerPort 8
/** sensor ports */
#define encdL_port 2
#define encdR_port 4
#define colorPort 1
#define limitPort 3
#define visBallport 1
#define visFlagport 2
extern bool useVision;
#endif
