#include <stdio.h>

/* -----------------------------------------------------------------------
 * Custom assertion messages to prevent exit interfering with
 * coverage calculations
 * 
 * Functions return 1 if the assertion is bad and 0 if they are true
 * This is so the user can keep a sum of all assertions that are false
 * -----------------------------------------------------------------------
 */

