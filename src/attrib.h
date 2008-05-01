/*!
 * \file attrib.h
 * \author Copyright (C) 2008 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief Definition of a DXF attribute entity (\c ATTRIB).
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.\n\n
 * This program is distributed in the hope that it will be useful, but
 * <b>WITHOUT ANY WARRANTY</b>; without even the implied warranty of
 * <b>MERCHANTABILITY</b> or <b>FITNESS FOR A PARTICULAR PURPOSE</b>.\n
 * See the GNU General Public License for more details.\n\n
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to:\n
 * Free Software Foundation, Inc.,\n
 * 59 Temple Place,\n
 * Suite 330,\n
 * Boston,\n
 * MA 02111 USA.\n
 * \n
 * Drawing eXchange Format (DXF) is a defacto industry standard for the
 * exchange of drawing files between various Computer Aided Drafting
 * programs.\n
 * DXF is an industry standard designed by Autodesk(TM).\n
 * For more details see http://www.autodesk.com.
 * <hr>
 */

#include "global.h"

/*!
 * \brief DXF definition of an AutoCAD attribute entity.
 */
typedef struct
dxf_attrib
{
        int id_code;
                /*!< group code = 5. */
        char *value;
                /*!< group code = 1. */
        char *tag_value;
                /*!< group code = 2. */
        char *linetype;
                /*!< group code = 6\n
                 * optional, defaults to BYLAYER. */
        char *text_style;
                /*!< group code = 7\n
                 * optional, defaults to STANDARD. */
        char *layer;
                /*!< group code = 8. */
        double x0;
                /*!< group code = 10\n
                 * start point. */
        double y0;
                /*!< group code = 20\n
                 * start point. */
        double z0;
                /*!< group code = 30\n
                 * start point. */
        double x1;
                /*!< group code = 11\n
                 * alignment point. */
        double y1;
                /*!< group code = 21\n
                 * alignment point. */
        double z1;
                /*!< group code = 31\n
                 * alignment point. */
        double thickness;
                /*!< group code = 39\n
                 * optional, defaults to 0.0. */
        double height;
                /*!< group code = 40. */
        double rel_x_scale;
                /*!< group code = 41\n
                 * optional, defaults to 1.0. */
        double rot_angle;
                /*!< group code = 50\n
                 * optional, defaults to 0.0. */
        double obl_angle;
                /*!< group code = 51\n
                 * optional, defaults to 0.0. */
        int color;
                /*!< group code = 62\n
                 * optional, defaults to BYLAYER. */
        int paperspace;
                /*!< group code = 67\n
                 * optional, defaults to 0 (modelspace). */
        int attr_flags;
                /*!< group code = 70\n
                 * bit coded:\n
                 * 1 = attribute is invisible (does not display)\n
                 * 2 = this is a constant attribute\n
                 * 4 = verification is required on input of this attribute\n
                 * 8 = attribute is preset (no prompt during insertion). */
        int text_flags;
                /*!< group code = 71\n
                 * optional, defaults to 0\n
                 * bit coded:\n
                 * 2 = text is backward (mirrored in X)\n
                 * 4 = text is upside down (mirrored in Y). */
        int hor_align;
                /*!< group code = 72\n
                 * optional, defaults to 0\n
                 * not bit coded:\n
                 * 0 = left\n
                 * 1 = center\n
                 * 2 = right\n
                 * 3 = aligned, only when vert_align = 0\n
                 * 4 = middle, only when vert_align = 0\n
                 * 5 = fit, only when vert_align = 0. */
        int field_length;
                /*!< group code = 73\n
                 * optional, defaults to 0. */
        int vert_align;
                /*!< group code = 74\n
                 * optional, defaults to 0\n
                 * not bit coded:\n
                 * 0 = baseline\n
                 * 1 = bottom\n
                 * 2 = middle\n
                 * 3 = top. */
} DxfAttrib, * DxfAttribPtr;

/* EOF */
