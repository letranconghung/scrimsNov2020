/** Robot's setup variables: Motor and sensor ports */
#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_
/** base motor ports */
#define BLPort 15
#define BRPort 16
/** other mechanical motor ports */
#define lRollerPort 14
#define rRollerPort 19
#define bouncerPort 13
#define ejectorPort 11
#define shooterPort 12
#define frollerPort 20
/** sensor ports */
#define encdL_port 1
#define encdR_port 7
#define colorPort 6
#define limitPort 1
#define visBallport 21
#define visFlagport 2 //bad
extern bool useVision;
#endif
