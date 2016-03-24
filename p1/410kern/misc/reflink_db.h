/** @file reflink_db.h
 *
 *  @brief Definitions for reflink level types
 *
 *  @author Matt Bryant (mbryant)
 */
#ifndef _REFLINK_DB_H_
#define _REFLINK_DB_H_

/** Maximum number of rows in a layout */
#define REFLINK_MAX_ROWS 7

/** Maximum number of columns in a layout */
#define REFLINK_MAX_COLS 7

/** @brief A representation of a reflink layout
 *
 *  @var width The width of each row (<= REFLINK_MAX_COLS)
 *  @var height The height of each column (<= REFLINK_MAX_ROWS)
 *  @var cells A string representing each board position, where the board is
 *       in a 1-D form.  ' ' corresponds to a blank square, and '[0-4]'
 *       correspond to cells with the respective values.
 */
typedef struct reflink_layout {
    unsigned width;
    unsigned height;
    const char *squares;
} reflink_layout_t;

extern const reflink_layout_t reflink_layouts[];
extern const int reflink_layout_count;

#endif /*_REFLINK_DB_H_*/
