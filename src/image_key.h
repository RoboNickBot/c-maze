#ifndef SEEN_IMAGE_KEY_H
#define SEEN_IMAGE_KEY_H

/* This is where the image tiles in the xcf sheet are named.
 *
 * when this enum is converted to an int, each value corresponds to the number of
 * x offsets the named image is in the sheet
 */
enum image_key { ERROR, SPACE1, WALL1, DARK1, DARK2, GOAL1, PN, PS, PE, PW };

enum maze_debug_image_key { DB_WALL, DB_SPACE, DB_START, DB_GOAL };

#endif
